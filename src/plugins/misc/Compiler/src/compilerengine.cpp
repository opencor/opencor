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

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"

#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Job.h"
#include "clang/Driver/Tool.h"
#include "clang/Driver/Util.h"
#include "clang/Frontend/CompilerInvocation.h"
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
    mModule(0),
    mExecutionEngine(0),
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

    mModule = 0;
    mExecutionEngine = 0;

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

llvm::sys::Path getExecutablePath(const char *pArg) {
    return llvm::sys::Path::GetMainExecutable(pArg,
                                              (void *) (intptr_t) getExecutablePath);
}

//==============================================================================

bool CompilerEngine::compileCode(const QString &pCode)
{
    // Reset our compiler engine

    reset();

    // Check whether we want to compute a definite integral

    if (pCode.contains("defint(func")) {
        mError = tr("definite integrals are not yet supported");

        return false;
    }

    // Retrieve the application file name and determine the name of the
    // temporary file which will contain our model code
    // Note: the temporary file will automatically get deleted when going out of
    //       scope...

    QByteArray appByteArray = qApp->applicationFilePath().toLatin1();
    const char *appFileName = appByteArray.constData();

    QTemporaryFile tempFile(QDir::tempPath()+QDir::separator()+QFileInfo(appFileName).baseName()+"_XXXXXX.c");

    if (!tempFile.open()) {
        mError = tr("<strong>%1</strong> could not be created").arg(tempFile.fileName());

        return false;
    }

    QByteArray tempFileByteArray = tempFile.fileName().toLatin1();
    const char *tempFileName = tempFileByteArray.constData();

    // Save the model code in our temporary file

    QTextStream out(&tempFile);

    out << pCode;

    tempFile.close();

    // Get a driver to compile our function

#ifdef QT_DEBUG
    llvm::raw_ostream &outputStream = llvm::outs();
#else
    llvm::raw_ostream &outputStream = llvm::nulls();
#endif
    llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagnosticOptions = new clang::DiagnosticOptions();

    clang::DiagnosticsEngine diagnosticsEngine(llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs()),
                                               &*diagnosticOptions,
                                               new clang::TextDiagnosticPrinter(outputStream, &*diagnosticOptions));
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

    compilerInstance.createDiagnostics(int(commandArguments.size()),
                                       const_cast<char **>(commandArguments.data()),
                                       new clang::TextDiagnosticPrinter(outputStream, &*diagnosticOptions));

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
        mError = tr("the model could not be compiled");

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

    return mExecutionEngine?mExecutionEngine->getPointerToFunction(mModule->getFunction(qPrintable(pFunctionName))):0;
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
