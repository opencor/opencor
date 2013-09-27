/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Compiler engine class
//==============================================================================

#include "compilerengine.h"
#include "compilermath.h"

//==============================================================================

#include <QApplication>
#include <QDir>
#include <QTemporaryFile>
#include <QTextStream>

//==============================================================================

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/Host.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Tool.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    #pragma GCC diagnostic warning "-Wunused-parameter"
#endif

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

CompilerEngine::CompilerEngine() :
    mExecutionEngine(0),
    mModule(0),
    mError(QString())
{
}

//==============================================================================

CompilerEngine::~CompilerEngine()
{
    // Reset ourselves

    reset();
}

//==============================================================================

void CompilerEngine::reset(const bool &pResetError)
{
    // Delete some internal objects

    delete mExecutionEngine;
    // Note: we must NOT delete mModule, since it gets deleted when deleting
    //       mExecutionEngine...

    mExecutionEngine = 0;
    mModule = 0;

    if (pResetError)
        mError = QString();
}

//==============================================================================

QString CompilerEngine::error() const
{
    // Return the compiler engine's error

    return mError;
}

//==============================================================================

bool CompilerEngine::hasError() const
{
    // Return whether an error occurred

    return !mError.isEmpty();
}

//==============================================================================

bool CompilerEngine::compileCode(const QString &pCode)
{
    // Reset our compiler engine

    reset();

    // Retrieve the application file name and determine the name of the
    // temporary file which will contain our code

    QTemporaryFile tempFile(QDir::tempPath()+QDir::separator()+QFileInfo(qApp->applicationFilePath()).baseName()+"_XXXXXX.c");

    if (!tempFile.open()) {
        mError = tr("<strong>%1</strong> could not be created").arg(tempFile.fileName());

        return false;
    }

    QByteArray tempFileByteArray = tempFile.fileName().toUtf8();
    const char *tempFileName = tempFileByteArray.constData();

    // Save the code in our temporary file

    QTextStream out(&tempFile);

    out << pCode;

    tempFile.close();

    // Get a driver to compile our code

#ifdef QT_DEBUG
    llvm::raw_ostream &outputStream = llvm::outs();
#else
    llvm::raw_ostream &outputStream = llvm::nulls();
#endif
    llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagnosticOptions = new clang::DiagnosticOptions();
    clang::TextDiagnosticPrinter *diagnosticClient = new clang::TextDiagnosticPrinter(outputStream, &*diagnosticOptions);

    clang::DiagnosticsEngine diagnosticsEngine(llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs()),
                                               &*diagnosticOptions,
                                               diagnosticClient);
    clang::driver::Driver driver("clang", llvm::sys::getProcessTriple(), "",
                                 diagnosticsEngine);

    // Get a compilation object to which we pass some arguments
    // Note: in gcc, the -O3 option comes with a warning: "Under some
    //       circumstances where these optimizations are not favorable, this
    //       option might actually make a program slower." This is the reason
    //       we use -O2 to build OpenCOR. In Clang, however, there is no such
    //       warning, hence we use -O3 to compile our code...

    llvm::SmallVector<const char *, 16> compilationArguments;

    compilationArguments.push_back("clang");
    compilationArguments.push_back("-fsyntax-only");
    compilationArguments.push_back("-O3");
    compilationArguments.push_back("-Werror");
    compilationArguments.push_back(tempFileName);

    llvm::OwningPtr<clang::driver::Compilation> compilation(driver.BuildCompilation(compilationArguments));

    if (!compilation) {
        // We couldn't get a compilation object, so...

        mError = tr("the compilation object could not be created");

        return false;
    }

    // The compilation object should have only one command, so if it doesn't
    // then something went wrong

    const clang::driver::JobList &jobList = compilation->getJobs();

    if (   (jobList.size() != 1)
        || !llvm::isa<clang::driver::Command>(*jobList.begin())) {
        mError = tr("the compilation object must contain only one command");

        return false;
    }

    // Retrieve the command job

    const clang::driver::Command *command = llvm::cast<clang::driver::Command>(*jobList.begin());
    QString commandName = command->getCreator().getName();

    if (commandName.compare("clang")) {
        mError = tr("a <strong>clang</strong> command was expected, but a <strong>%1</strong> command was found instead").arg(commandName);

        return false;
    }

    // Create a compiler invocation using our command's arguments

    const clang::driver::ArgStringList &commandArguments = command->getArguments();
    llvm::OwningPtr<clang::CompilerInvocation> compilerInvocation(new clang::CompilerInvocation());

    clang::CompilerInvocation::CreateFromArgs(*compilerInvocation,
                                              const_cast<const char **>(commandArguments.data()),
                                              const_cast<const char **>(commandArguments.data())+commandArguments.size(),
                                              diagnosticsEngine);

    // Create a compiler instance to handle the actual work

    clang::CompilerInstance compilerInstance;

    compilerInstance.setInvocation(compilerInvocation.take());

    // Create the compiler instance's diagnostics engine

    compilerInstance.createDiagnostics(diagnosticClient, false);

    if (!compilerInstance.hasDiagnostics()) {
        mError = tr("the diagnostics engine could not be created");

        return false;
    }

    // Create an LLVM module

    mModule = new llvm::Module(tempFileName, llvm::getGlobalContext());

    // Initialise the native target, so not only can we then create a JIT
    // execution engine, but more importantly its data layout will match that of
    // our target platform...

    llvm::InitializeNativeTarget();

    // Create a JIT execution engine

    mExecutionEngine = llvm::ExecutionEngine::createJIT(mModule);

    if (!mExecutionEngine) {
        mError = tr("the JIT execution engine could not be created");

        delete mModule;

        return false;
    }

    // Create and execute the frontend to generate the LLVM assembly code,
    // making sure that all added functions end up in the same module
    // Note: the LLVM team has been meaning to modify
    //       CompilerInstance::ExecuteAction() so that we could specify the
    //       output stream we want to use (rather than always use llvm::errs()),
    //       but they have yet to actually do it, so we modified it ourselves...

    llvm::OwningPtr<clang::CodeGenAction> codeGenerationAction(new clang::EmitLLVMOnlyAction(&mModule->getContext()));

    codeGenerationAction->setLinkModule(mModule);

    if (!compilerInstance.ExecuteAction(*codeGenerationAction, outputStream)) {
        mError = tr("the code could not be compiled");

        reset(false);

        return false;
    }

    // The LLVM assembly code was generated, so we can update our module

    mModule = codeGenerationAction->takeModule();

    // Everything went fine, so...

    return true;
}

//==============================================================================

void * CompilerEngine::getFunction(const QString &pFunctionName)
{
    // Return the requested function

    if (mExecutionEngine) {
        llvm::Function *function = mModule->getFunction(qPrintable(pFunctionName));

        if (function)
            return mExecutionEngine->getPointerToFunction(function);
        else
            return 0;
    } else {
        return 0;
    }
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
