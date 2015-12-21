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
#include "corecliutils.h"

//==============================================================================

#include "llvmdisablewarnings.h"
    #include "llvm/IR/LLVMContext.h"
    #include "llvm/Support/TargetSelect.h"

    #include "clang/Basic/DiagnosticOptions.h"
    #include "clang/CodeGen/CodeGenAction.h"
    #include "clang/Driver/Compilation.h"
    #include "clang/Driver/Driver.h"
    #include "clang/Driver/Tool.h"
    #include "clang/Frontend/CompilerInstance.h"
#include "llvmenablewarnings.h"

//==============================================================================

#include <string>

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

CompilerEngine::CompilerEngine() :
    mExecutionEngine(std::unique_ptr<llvm::ExecutionEngine>()),
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
    // Reset some internal objects

    delete mExecutionEngine.release();

    mExecutionEngine = std::unique_ptr<llvm::ExecutionEngine>();

    if (pResetError)
        mError = QString();
}

//==============================================================================

bool CompilerEngine::hasError() const
{
    // Return whether an error occurred

    return mError.size();
}

//==============================================================================

QString CompilerEngine::error() const
{
    // Return the compiler engine's error

    return mError;
}

//==============================================================================

bool CompilerEngine::compileCode(const QString &pCode)
{
    // Reset our compiler engine

    reset();

    // Determine our target triple
    // Note: normally, we would call llvm::sys::getProcessTriple(), but this
    //       returns the information about the system on which LLVM was built.
    //       In most cases it is fine, but on OS X it may be a problem. Indeed,
    //       with OS X 10.9, Apple decided to extend the C standard by adding
    //       some functions (e.g. __exp10()). So, if the given code needs one of
    //       those functions, then OpenCOR will crash if run on an 'old' version
    //       of OS X. So, to avoid this issue, we set the target triple
    //       ourselves, based on the system on which OpenCOR is being used...

    std::string targetTriple;

#if defined(Q_OS_WIN)
    targetTriple = (sizeof(void *) == 4)?"i686-pc-windows-msvc-elf":"x86_64-pc-windows-msvc-elf";
    // Note: MCJIT currently works only through the ELF object format, hence we
    //       are appending "-elf"...
#elif defined(Q_OS_LINUX)
    targetTriple = (sizeof(void *) == 4)?"i686-pc-linux-gnu":"x86_64-pc-linux-gnu";
#elif defined(Q_OS_MAC)
    targetTriple = "x86_64-apple-darwin"+std::to_string(QSysInfo::MacintoshVersion+2);
#else
    #error Unsupported platform
#endif

    // Get a driver to compile our code

    llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagnosticOptions = new clang::DiagnosticOptions();
    clang::DiagnosticsEngine diagnosticsEngine(llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs()),
                                               &*diagnosticOptions);
    clang::driver::Driver driver("clang", targetTriple, diagnosticsEngine);

    driver.setCheckInputsExist(false);

    // Get a compilation object to which we pass some arguments

    llvm::StringRef dummyFileName("dummyFile.c");
    llvm::SmallVector<const char *, 16> compilationArguments;

    compilationArguments.push_back("clang");
    compilationArguments.push_back("-fsyntax-only");
    compilationArguments.push_back("-O3");
    compilationArguments.push_back("-ffast-math");
    compilationArguments.push_back("-Werror");
    compilationArguments.push_back(dummyFileName.data());

    std::unique_ptr<clang::driver::Compilation> compilation(driver.BuildCompilation(compilationArguments));

    if (!compilation) {
        mError = tr("the compilation object could not be created");

        return false;
    }

    // The compilation object should have only one command, so if it doesn't
    // then something went wrong

    const clang::driver::JobList &jobList = compilation->getJobs();

    if (    (jobList.size() != 1)
        || !llvm::isa<clang::driver::Command>(*jobList.begin())) {
        mError = tr("the compilation object must contain only one command");

        return false;
    }

    // Retrieve the command job

    const clang::driver::Command &command = llvm::cast<clang::driver::Command>(*jobList.begin());
    QString commandName = command.getCreator().getName();

    if (commandName.compare("clang")) {
        mError = tr("a <strong>clang</strong> command was expected, but a <strong>%1</strong> command was found instead").arg(commandName);

        return false;
    }

    // Create a compiler invocation using our command's arguments

    const clang::driver::ArgStringList &commandArguments = command.getArguments();
    std::unique_ptr<clang::CompilerInvocation> compilerInvocation(new clang::CompilerInvocation());

    clang::CompilerInvocation::CreateFromArgs(*compilerInvocation,
                                              commandArguments.data(),
                                              commandArguments.data()+commandArguments.size(),
                                              diagnosticsEngine);

    // Map our dummy file to a memory buffer

    QByteArray codeByteArray = pCode.toUtf8();

    compilerInvocation->getPreprocessorOpts().addRemappedFile(dummyFileName, llvm::MemoryBuffer::getMemBuffer(codeByteArray.constData()).release());

    // Create a compiler instance to handle the actual work

    clang::CompilerInstance compilerInstance;

    compilerInstance.setInvocation(compilerInvocation.release());

    // Create the compiler instance's diagnostics engine

    compilerInstance.createDiagnostics();

    if (!compilerInstance.hasDiagnostics()) {
        mError = tr("the diagnostics engine could not be created");

        return false;
    }

    // Create and execute the frontend to generate an LLVM bitcode module

    std::unique_ptr<clang::CodeGenAction> codeGenerationAction(new clang::EmitLLVMOnlyAction(&llvm::getGlobalContext()));

    if (!compilerInstance.ExecuteAction(*codeGenerationAction)) {
        mError = tr("the code could not be compiled");

        reset(false);

        return false;
    }

    // Retrieve the LLVM bitcode module

    std::unique_ptr<llvm::Module> module = codeGenerationAction->takeModule();

    // Initialise the native target (and its ASM printer), so not only can we
    // then create an execution engine, but more importantly its data layout
    // will match that of our target platform

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    // Create and keep track of an execution engine

    mExecutionEngine = std::unique_ptr<llvm::ExecutionEngine>(llvm::EngineBuilder(std::move(module)).setEngineKind(llvm::EngineKind::JIT).create());

    if (!mExecutionEngine) {
        mError = tr("the execution engine could not be created");

        delete module.release();

        return false;
    }

    return true;
}

//==============================================================================

void * CompilerEngine::getFunction(const QString &pFunctionName)
{
    // Return the address of the requested function

    if (mExecutionEngine)
        return (void *) mExecutionEngine->getFunctionAddress(qPrintable(pFunctionName));
    else
        return 0;
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
