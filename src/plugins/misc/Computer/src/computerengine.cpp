#include <iostream>
#include <QDir>
#include <QTextStream>
//==============================================================================
// Computer engine class
//==============================================================================

#include "computerengine.h"
#include "computermath.h"

//==============================================================================

#include <QApplication>

//==============================================================================

#include "llvm/LLVMContext.h"

#ifdef Q_WS_WIN
    // To include llvm/Module.h results in some indirect warnings from MSVC, but
    // it's LLVM's task to address them not ours, so...

    #pragma warning(disable: 4146)
#endif

#include "llvm/Module.h"

#ifdef Q_WS_WIN
    #pragma warning(default: 4146)
#endif

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"

#include "clang/Basic/Diagnostic.h"
#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Job.h"
#include "clang/Driver/Tool.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerEngine::ComputerEngine() :
    mModule(0),
    mExecutionEngine(0),
    mError(QString())
{
}

//==============================================================================

ComputerEngine::~ComputerEngine()
{
    // Reset ourselves

    reset();
}

//==============================================================================

void ComputerEngine::reset()
{
    // Delete some internal objects

    delete mExecutionEngine;
    // Note: we must NOT delete mModule, since it gets deleted when deleting
    //       mExecutionEngine...

    mModule = 0;
    mExecutionEngine = 0;

    mError = QString();
}

//==============================================================================

QString ComputerEngine::error() const
{
    // Return the computer engine's error

    return mError;
}

//==============================================================================

bool ComputerEngine::hasError() const
{
    // Return whether an error occurred

    return !mError.isEmpty();
}

//==============================================================================

llvm::sys::Path getExecutablePath(const char *pArg) {
    return llvm::sys::Path::GetMainExecutable(pArg,
                                              (void *) (intptr_t) getExecutablePath);
}

//==============================================================================

bool ComputerEngine::compileCode(const QString &pCode,
                                 const bool &pOutputErrors)
{
    // Reset our computer engine

    reset();

    // Check whether we want to compute a definite integral

    if (pCode.contains("defint(func")) {
        mError = tr("definite integrals are not yet supported");

        return false;
    }

    // Retrieve the application file name and determine the name of the
    // temporary file which will contain our model code

    QByteArray appByteArray = qApp->applicationFilePath().toLatin1();
    const char *appFileName = appByteArray.constData();

    QByteArray tempFileByteArray = QString(QDir::tempPath()+QDir::separator()+QFileInfo(appFileName).baseName()+".c").toLatin1();
    const char *tempFileName = tempFileByteArray.constData();

    // Save the model code in our temporary file

    QFile tempFile(tempFileName);

    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        mError = tr("<strong>%1</strong> could not be created").arg(tempFileName);

        tempFile.remove();

        return false;
    }

    QTextStream out(&tempFile);

    out << pCode;

    tempFile.close();

    // Get a driver to compile our function

    llvm::raw_ostream &outputStream = pOutputErrors?llvm::errs():llvm::nulls();

    clang::DiagnosticsEngine diagnosticsEngine(llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs()),
                                               new clang::TextDiagnosticPrinter(outputStream,
                                                                                clang::DiagnosticOptions()));
    clang::driver::Driver driver(getExecutablePath(appFileName).str(),
                                 llvm::sys::getDefaultTargetTriple(),
                                 "a.out", true, diagnosticsEngine);

    // Get a compilation object to which we pass some arguments
    // Note: in gcc, the -O3 option comes with a warning: "Under some
    //       circumstances where these optimizations are not favorable, this
    //       option might actually make a program slower." This is the reason
    //       we use -O2 to build OpenCOR. In Clang, however, there is no such
    //       warning, hence we use -O3 to compile a model...

    llvm::SmallVector<const char *, 16> compilationArguments;

    compilationArguments.push_back(appFileName);
    compilationArguments.push_back("-fsyntax-only");
    compilationArguments.push_back("-O3");
    compilationArguments.push_back("-Werror");
    compilationArguments.push_back(tempFileName);

    llvm::OwningPtr<clang::driver::Compilation> compilation(driver.BuildCompilation(compilationArguments));

    if (!compilation) {
        // We couldn't get a compilation object, so...

        mError = tr("the compilation object could not be created");

        tempFile.remove();

        return false;
    }

    // The compilation object should have only one command, so if it doesn't
    // then something went wrong

    const clang::driver::JobList &jobList = compilation->getJobs();

    if (   (jobList.size() != 1)
        || !llvm::isa<clang::driver::Command>(*jobList.begin())) {
        mError = tr("the compilation object must contain only one command");

        tempFile.remove();

        return false;
    }

    // Retrieve the command job

    const clang::driver::Command *command = llvm::cast<clang::driver::Command>(*jobList.begin());
    QString commandName = command->getCreator().getName();

    if (commandName.compare("clang")) {
        mError = tr("a <strong>clang</strong> command was expected, but a <strong>%1</strong> command was found instead").arg(commandName);

        tempFile.remove();

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

    compilerInstance.createDiagnostics(int(commandArguments.size()),
                                       const_cast<char **>(commandArguments.data()),
                                       new clang::TextDiagnosticPrinter(outputStream,
                                                                        compilerInstance.getDiagnosticOpts()));

    if (!compilerInstance.hasDiagnostics()) {
        mError = tr("the diagnostics engine could not be created");

        tempFile.remove();

        return false;
    }

    // Create an LLVM module

    static int counter = 0;

    mModule = new llvm::Module(qPrintable(QString("Module #%1").arg(QString::number(++counter))),
                               llvm::getGlobalContext());

    // Initialise the native target, so not only can we then create a JIT
    // execution engine, but more importantly its data layout will match that of
    // our target platform...

    llvm::InitializeNativeTarget();

    // Create a JIT execution engine

    mExecutionEngine = llvm::ExecutionEngine::createJIT(mModule);

    if (!mExecutionEngine) {
        mError = tr("the JIT execution engine could not be created");

        tempFile.remove();

        delete mModule;

        return false;
    }

    // Create and execute the frontend to generate the LLVM assembly code,
    // making sure that all added functions end up in the same module

    llvm::OwningPtr<clang::CodeGenAction> codeGenerationAction(new clang::EmitLLVMOnlyAction(&mModule->getContext()));

    codeGenerationAction->setLinkModule(mModule);

    if (!compilerInstance.ExecuteAction(*codeGenerationAction, outputStream)) {
        mError = tr("the model could not be compiled");

        tempFile.remove();

        reset();

        return false;
    }

    // The LLVM assembly code was generated, so we can update our module and
    // delete our temporary file

    mModule = codeGenerationAction->takeModule();

    tempFile.remove();

    // Everything went fine, so...

    return true;
}

//==============================================================================

void * ComputerEngine::getFunction(const QString &pFunctionName)
{
    // Return the requested function

    return mExecutionEngine?mExecutionEngine->getPointerToFunction(mModule->getFunction(qPrintable(pFunctionName))):0;
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
