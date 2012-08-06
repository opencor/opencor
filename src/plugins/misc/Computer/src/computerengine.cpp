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
    mError(ComputerError())
{
    // Create a module

    static int counter = 0;

    mModule = new llvm::Module(qPrintable(QString("Module #%1").arg(QString::number(++counter))),
                               llvm::getGlobalContext());

    // Initialise the native target, so not only can we then create a JIT
    // execution engine, but more importantly its data layout will match that of
    // our target platform...

    llvm::InitializeNativeTarget();

    // Create a JIT execution engine

    mExecutionEngine = llvm::ExecutionEngine::createJIT(mModule);
}

//==============================================================================

ComputerEngine::~ComputerEngine()
{
    // Delete some internal objects

    delete mExecutionEngine;
    // Note: we must NOT delete mModule, since it gets deleted when deleting
    //       mExecutionEngine...
}

//==============================================================================

llvm::Module * ComputerEngine::module()
{
    // Return the computer engine's module

    return mModule;
}

//==============================================================================

llvm::ExecutionEngine * ComputerEngine::executionEngine()
{
    // Return the computer engine's execution engine

    return mExecutionEngine;
}

//==============================================================================

ComputerError ComputerEngine::error()
{
    // Return the computer engine's error

    return mError;
}

//==============================================================================

llvm::sys::Path getExecutablePath(const char *pArg) {
    return llvm::sys::Path::GetMainExecutable(pArg,
                                              (void *) (intptr_t) getExecutablePath);
}

//==============================================================================

llvm::Function * ComputerEngine::addFunction(const QString &pFunctionName,
                                             const QString &pFunctionBody,
                                             const bool &pOutputErrors)
{
    // Check whether we want to compute a definite integral

    if (pFunctionBody.contains("defint(func")) {
        mError = ComputerError(tr("definite integrals are not yet supported"));

        return 0;
    }

    // Save the function in a temporary file, after having prepended it with
    // all possible external functions which it may, or not, need
    // Note: indeed, we cannot include header files since we don't (and don't
    //       want to avoid complications) deploy them with OpenCOR. So, instead,
    //       we must declare as external functions all the functions which we
    //       would normally use through header files...

    QByteArray appByteArray = qApp->applicationFilePath().toLatin1();
    const char *appFileName = appByteArray.constData();

    QByteArray tempFileByteArray = QString(QDir::tempPath()+QDir::separator()+QFileInfo(appFileName).baseName()+".c").toLatin1();
    const char *tempFileName = tempFileByteArray.constData();
    QFile tempFile(tempFileName);

    if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // The temporary file can't be opened, so...

        mError = ComputerError(tr("'%1' could not be created").arg(tempFileName));

        tempFile.remove();

        return 0;
    }

    QTextStream out(&tempFile);

    out << "extern double fabs(double);\n"
        << "\n"
        << "extern double exp(double);\n"
        << "extern double log(double);\n"
        << "\n"
        << "extern double ceil(double);\n"
        << "extern double floor(double);\n"
        << "\n"
        << "extern double factorial(double);\n"
        << "\n"
        << "extern double sin(double);\n"
        << "extern double cos(double);\n"
        << "extern double tan(double);\n"
        << "extern double sinh(double);\n"
        << "extern double cosh(double);\n"
        << "extern double tanh(double);\n"
        << "extern double asin(double);\n"
        << "extern double acos(double);\n"
        << "extern double atan(double);\n"
        << "extern double asinh(double);\n"
        << "extern double acosh(double);\n"
        << "extern double atanh(double);\n"
        << "\n"
        << "extern double arbitrary_log(double, double);\n"
        << "\n"
        << "extern double pow(double, double);\n"
        << "\n"
        << "extern double gcd_multi(int, ...);\n"
        << "extern double lcm_multi(int, ...);\n"
        << "extern double multi_max(int, ...);\n"
        << "extern double multi_min(int, ...);\n"
        << "\n"
        << pFunctionBody;

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

        mError = ComputerError(tr("the compilation object could not be created"));

        tempFile.remove();

        return 0;
    }

    // The compilation object should have only one command, so if it doesn't
    // then something went wrong

    const clang::driver::JobList &jobList = compilation->getJobs();

    if (   (jobList.size() != 1)
        || !llvm::isa<clang::driver::Command>(*jobList.begin())) {
        mError = ComputerError(tr("the compilation object must contain only one command"));

        tempFile.remove();

        return 0;
    }

    // Retrieve the command job

    const clang::driver::Command *command = llvm::cast<clang::driver::Command>(*jobList.begin());
    QString commandName = command->getCreator().getName();

    if (commandName.compare("clang")) {
        mError = ComputerError(tr("a 'clang' command was expected, but a '%1' command was found instead").arg(commandName));

        tempFile.remove();

        return 0;
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
        mError = ComputerError(tr("the diagnostics engine could not be created"));

        tempFile.remove();

        return 0;
    }

    // Create and execute the frontend to generate the LLVM assembly code,
    // making sure that all added functions end up in the same module

    llvm::OwningPtr<clang::CodeGenAction> codeGenerationAction(new clang::EmitLLVMOnlyAction(&mModule->getContext()));

    codeGenerationAction->setLinkModule(mModule);

    if (!compilerInstance.ExecuteAction(*codeGenerationAction, outputStream)) {
        mError = ComputerError(tr("the '%1' function could not be compiled").arg(pFunctionName));

        tempFile.remove();

        return 0;
    }

    // The LLVM assembly code was generated, so we can update our module and
    // delete our temporary file

    mModule = codeGenerationAction->takeModule();

    tempFile.remove();

    // Try to retrieve the function we have just added

    llvm::Function *res = mModule->getFunction(qPrintable(pFunctionName));

    if (!res) {
        mError = ComputerError(tr("the '%1' function could not be found").arg(pFunctionName));

        return 0;
    }

    // Everything went fine, so reset the engine's error and return the function
    // we have just added

    mError = ComputerError();

    return res;
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
