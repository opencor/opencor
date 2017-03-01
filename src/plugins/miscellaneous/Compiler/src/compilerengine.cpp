/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Compiler engine
//==============================================================================

#ifdef _WIN32
    #define _SCL_SECURE_NO_WARNINGS
#endif

//==============================================================================

#include "compilerengine.h"
#include "compilermath.h"
#include "corecliutils.h"

//==============================================================================

#include "llvmbegin.h"
    #include "llvm/Support/TargetSelect.h"

    #include "llvm-c/Core.h"

    #include "clang/Basic/DiagnosticOptions.h"
    #include "clang/CodeGen/CodeGenAction.h"
    #include "clang/Driver/Compilation.h"
    #include "clang/Driver/Driver.h"
    #include "clang/Driver/Tool.h"
    #include "clang/Frontend/CompilerInstance.h"
#include "llvmend.h"

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

bool CompilerEngine::hasError() const
{
    // Return whether an error occurred

    return !mError.isEmpty();
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
    // Reset ourselves

    mExecutionEngine.reset();

    mError = QString();

    // Prepend all the external functions that may, or not, be needed by the
    // given code
    // Note: indeed, we cannot include header files since we don't (and don't
    //       want in order to avoid complications) deploy them with OpenCOR. So,
    //       instead, we must declare as external functions all the functions
    //       that we would normally use through header files...

    QString code =  "extern double fabs(double);\n"
                    "\n"
                    "extern double log(double);\n"
                    "extern double exp(double);\n"
                    "\n"
                    "extern double floor(double);\n"
                    "extern double ceil(double);\n"
                    "\n"
                    "extern double factorial(double);\n"
                    "\n"
                    "extern double sin(double);\n"
                    "extern double sinh(double);\n"
                    "extern double asin(double);\n"
                    "extern double asinh(double);\n"
                    "\n"
                    "extern double cos(double);\n"
                    "extern double cosh(double);\n"
                    "extern double acos(double);\n"
                    "extern double acosh(double);\n"
                    "\n"
                    "extern double tan(double);\n"
                    "extern double tanh(double);\n"
                    "extern double atan(double);\n"
                    "extern double atanh(double);\n"
                    "\n"
                    "extern double sec(double);\n"
                    "extern double sech(double);\n"
                    "extern double asec(double);\n"
                    "extern double asech(double);\n"
                    "\n"
                    "extern double csc(double);\n"
                    "extern double csch(double);\n"
                    "extern double acsc(double);\n"
                    "extern double acsch(double);\n"
                    "\n"
                    "extern double cot(double);\n"
                    "extern double coth(double);\n"
                    "extern double acot(double);\n"
                    "extern double acoth(double);\n"
                    "\n"
                    "extern double arbitrary_log(double, double);\n"
                    "\n"
                    "extern double pow(double, double);\n"
                    "\n"
                    "extern double multi_min(int, ...);\n"
                    "extern double multi_max(int, ...);\n"
                    "\n"
                    "extern double gcd_multi(int, ...);\n"
                    "extern double lcm_multi(int, ...);\n"
                    "\n"
                   +pCode;

    // Determine our target triple
    // Note: normally, we would call llvm::sys::getProcessTriple(), but this
    //       returns the information about the system on which LLVM was built.
    //       In most cases it is fine, but on macOS it may be a problem. Indeed,
    //       with OS X 10.9, Apple decided to extend the C standard by adding
    //       some functions (e.g. __exp10()). So, if the given code needs one of
    //       those functions, then OpenCOR will crash if run on an 'old' version
    //       of macOS. So, to avoid this issue, we set the target triple
    //       ourselves, based on the system on which OpenCOR is to be used...

    std::string targetTriple;

#if defined(Q_OS_WIN)
    targetTriple = "x86_64-pc-windows-msvc-elf";
    // Note: MCJIT currently works only through the ELF object format, hence we
    //       are appending "-elf"...
#elif defined(Q_OS_LINUX)
    targetTriple = "x86_64-pc-linux-gnu";
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
    // Note: on Windows, we have both a pre-built release and a pre-built debug
    //       version of LLVM, so we make sure that we use the right flags for
    //       the right version, not least because with LLVM 3.9.x, to use an
    //       optimisation flag with a pre-built debug version of LLVM results in
    //       some of the Compiler and CellMLSupport tests to fail...

    llvm::StringRef dummyFileName("dummyFile.c");
    llvm::SmallVector<const char *, 16> compilationArguments;

    compilationArguments.push_back("clang");
    compilationArguments.push_back("-fsyntax-only");
#if    defined(QT_DEBUG) \
    && (   (defined(USE_PREBUILT_LLVM_PLUGIN) && defined(Q_OS_WIN)) \
        || !defined(USE_PREBUILT_LLVM_PLUGIN))
    compilationArguments.push_back("-g");
    compilationArguments.push_back("-O0");
#else
    compilationArguments.push_back("-O3");
    compilationArguments.push_back("-ffast-math");
#endif
    compilationArguments.push_back("-Werror");
    compilationArguments.push_back(dummyFileName.data());

    std::unique_ptr<clang::driver::Compilation> compilation(driver.BuildCompilation(compilationArguments));

    if (!compilation) {
        mError = QObject::tr("the compilation object could not be created");

        return false;
    }

    // The compilation object should have only one command, so if it doesn't
    // then something went wrong

    clang::driver::JobList &jobs = compilation->getJobs();

    if (    (jobs.size() != 1)
        || !llvm::isa<clang::driver::Command>(*jobs.begin())) {
        mError = QObject::tr("the compilation object must contain only one command");

        return false;
    }

    // Retrieve the command job

    clang::driver::Command &command = llvm::cast<clang::driver::Command>(*jobs.begin());
    QString commandName = command.getCreator().getName();

    if (commandName.compare("clang")) {
        mError = QObject::tr("a <strong>clang</strong> command was expected, but a <strong>%1</strong> command was found instead").arg(commandName);

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

    QByteArray codeByteArray = code.toUtf8();

    compilerInvocation->getPreprocessorOpts().addRemappedFile(dummyFileName, llvm::MemoryBuffer::getMemBuffer(codeByteArray.constData()).release());

    // Create a compiler instance to handle the actual work

    clang::CompilerInstance compilerInstance;

    compilerInstance.setInvocation(compilerInvocation.release());

    // Create the compiler instance's diagnostics engine

    compilerInstance.createDiagnostics();

    if (!compilerInstance.hasDiagnostics()) {
        mError = QObject::tr("the diagnostics engine could not be created");

        return false;
    }

    // Create and execute the frontend to generate an LLVM bitcode module

    std::unique_ptr<clang::CodeGenAction> codeGenerationAction(new clang::EmitLLVMOnlyAction(llvm::unwrap(LLVMGetGlobalContext())));

    if (!compilerInstance.ExecuteAction(*codeGenerationAction)) {
        mError = QObject::tr("the code could not be compiled");

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

    mExecutionEngine.reset(llvm::EngineBuilder(std::move(module)).setEngineKind(llvm::EngineKind::JIT).create());

    if (!mExecutionEngine) {
        mError = QObject::tr("the execution engine could not be created");

        module.reset();

        return false;
    }

    // Map all the external functions that may, or not, be needed by the given
    // code

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    #define FUNCTION_NAME(x) (x)
#elif defined(Q_OS_MAC)
    #define FUNCTION_NAME(x) (std::string(std::string("_")+(x)).c_str())
#else
    #error Unsupported platform
#endif

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("fabs"), (uint64_t) compiler_fabs);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("log"), (uint64_t) compiler_log);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("exp"), (uint64_t) compiler_exp);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("floor"), (uint64_t) compiler_floor);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("ceil"), (uint64_t) compiler_ceil);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("factorial"), (uint64_t) compiler_factorial);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("sin"), (uint64_t) compiler_sin);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("sinh"), (uint64_t) compiler_sinh);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("asin"), (uint64_t) compiler_asin);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("asinh"), (uint64_t) compiler_asinh);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("cos"), (uint64_t) compiler_cos);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("cosh"), (uint64_t) compiler_cosh);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acos"), (uint64_t) compiler_acos);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acosh"), (uint64_t) compiler_acosh);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("tan"), (uint64_t) compiler_tan);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("tanh"), (uint64_t) compiler_tanh);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("atan"), (uint64_t) compiler_atan);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("atanh"), (uint64_t) compiler_atanh);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("sec"), (uint64_t) compiler_sec);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("sech"), (uint64_t) compiler_sech);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("asec"), (uint64_t) compiler_asec);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("asech"), (uint64_t) compiler_asech);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("csc"), (uint64_t) compiler_csc);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("csch"), (uint64_t) compiler_csch);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acsc"), (uint64_t) compiler_acsc);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acsch"), (uint64_t) compiler_acsch);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("cot"), (uint64_t) compiler_cot);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("coth"), (uint64_t) compiler_coth);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acot"), (uint64_t) compiler_acot);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acoth"), (uint64_t) compiler_acoth);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("arbitrary_log"), (uint64_t) compiler_arbitrary_log);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("pow"), (uint64_t) compiler_pow);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("multi_min"), (uint64_t) compiler_multi_min);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("multi_max"), (uint64_t) compiler_multi_max);

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("gcd_multi"), (uint64_t) compiler_gcd_multi);
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("lcm_multi"), (uint64_t) compiler_lcm_multi);

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
