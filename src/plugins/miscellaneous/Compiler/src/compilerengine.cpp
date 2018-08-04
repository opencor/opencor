/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

#include "llvmclangbegin.h"
    #include "llvm/ADT/IntrusiveRefCntPtr.h"
    #include "llvm/ExecutionEngine/ExecutionEngine.h"
    #include "llvm/Support/TargetSelect.h"

    #include "llvm-c/Core.h"

    #include "clang/Basic/Diagnostic.h"
    #include "clang/CodeGen/CodeGenAction.h"
    #include "clang/Driver/Compilation.h"
    #include "clang/Driver/Driver.h"
    #include "clang/Driver/Tool.h"
    #include "clang/Frontend/CompilerInstance.h"
    #include "clang/Lex/PreprocessorOptions.h"
#include "llvmclangend.h"

//==============================================================================

#include <string>

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

CompilerEngine::CompilerEngine() :
    mExecutionEngine(nullptr),
    mError(QString())
{
}

//==============================================================================

CompilerEngine::~CompilerEngine()
{
    // Delete some internal objects

    delete mExecutionEngine;
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

    delete mExecutionEngine;

    mExecutionEngine = nullptr;

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

    llvm::StringRef dummyFileName("dummyFile.c");
    llvm::SmallVector<const char *, 16> compilationArguments;

    compilationArguments.push_back("clang");
    compilationArguments.push_back("-fsyntax-only");
#ifdef QT_DEBUG
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
        mError = tr("the compilation object could not be created");

        return false;
    }

    // The compilation object should have only one command, so if it doesn't
    // then something went wrong

    clang::driver::JobList &jobs = compilation->getJobs();

    if (    (jobs.size() != 1)
        || !llvm::isa<clang::driver::Command>(*jobs.begin())) {
        mError = tr("the compilation object must contain only one command");

        return false;
    }

    // Retrieve the command job

    clang::driver::Command &command = llvm::cast<clang::driver::Command>(*jobs.begin());
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

    QByteArray codeByteArray = code.toUtf8();

    compilerInvocation->getPreprocessorOpts().addRemappedFile(dummyFileName, llvm::MemoryBuffer::getMemBuffer(codeByteArray.constData()).release());

    // Create a compiler instance to handle the actual work

    clang::CompilerInstance compilerInstance;

    compilerInstance.setInvocation(std::move(compilerInvocation));

    // Create the compiler instance's diagnostics engine

    compilerInstance.createDiagnostics();

    if (!compilerInstance.hasDiagnostics()) {
        mError = tr("the diagnostics engine could not be created");

        return false;
    }

    // Create and execute the frontend to generate an LLVM bitcode module

    std::unique_ptr<clang::CodeGenAction> codeGenerationAction(new clang::EmitLLVMOnlyAction(llvm::unwrap(LLVMGetGlobalContext())));

    if (!compilerInstance.ExecuteAction(*codeGenerationAction)) {
        mError = tr("the code could not be compiled");

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

    mExecutionEngine = llvm::EngineBuilder(std::move(module)).setEngineKind(llvm::EngineKind::JIT).create();

    if (!mExecutionEngine) {
        mError = tr("the execution engine could not be created");

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

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("fabs"), reinterpret_cast<quint64>(compiler_fabs));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("log"), reinterpret_cast<quint64>(compiler_log));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("exp"), reinterpret_cast<quint64>(compiler_exp));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("floor"), reinterpret_cast<quint64>(compiler_floor));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("ceil"), reinterpret_cast<quint64>(compiler_ceil));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("factorial"), reinterpret_cast<quint64>(compiler_factorial));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("sin"), reinterpret_cast<quint64>(compiler_sin));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("sinh"), reinterpret_cast<quint64>(compiler_sinh));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("asin"), reinterpret_cast<quint64>(compiler_asin));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("asinh"), reinterpret_cast<quint64>(compiler_asinh));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("cos"), reinterpret_cast<quint64>(compiler_cos));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("cosh"), reinterpret_cast<quint64>(compiler_cosh));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acos"), reinterpret_cast<quint64>(compiler_acos));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acosh"), reinterpret_cast<quint64>(compiler_acosh));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("tan"), reinterpret_cast<quint64>(compiler_tan));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("tanh"), reinterpret_cast<quint64>(compiler_tanh));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("atan"), reinterpret_cast<quint64>(compiler_atan));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("atanh"), reinterpret_cast<quint64>(compiler_atanh));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("sec"), reinterpret_cast<quint64>(compiler_sec));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("sech"), reinterpret_cast<quint64>(compiler_sech));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("asec"), reinterpret_cast<quint64>(compiler_asec));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("asech"), reinterpret_cast<quint64>(compiler_asech));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("csc"), reinterpret_cast<quint64>(compiler_csc));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("csch"), reinterpret_cast<quint64>(compiler_csch));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acsc"), reinterpret_cast<quint64>(compiler_acsc));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acsch"), reinterpret_cast<quint64>(compiler_acsch));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("cot"), reinterpret_cast<quint64>(compiler_cot));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("coth"), reinterpret_cast<quint64>(compiler_coth));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acot"), reinterpret_cast<quint64>(compiler_acot));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("acoth"), reinterpret_cast<quint64>(compiler_acoth));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("arbitrary_log"), reinterpret_cast<quint64>(compiler_arbitrary_log));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("pow"), reinterpret_cast<quint64>(compiler_pow));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("multi_min"), reinterpret_cast<quint64>(compiler_multi_min));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("multi_max"), reinterpret_cast<quint64>(compiler_multi_max));

    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("gcd_multi"), reinterpret_cast<quint64>(compiler_gcd_multi));
    mExecutionEngine->addGlobalMapping(FUNCTION_NAME("lcm_multi"), reinterpret_cast<quint64>(compiler_lcm_multi));

    return true;
}

//==============================================================================

void * CompilerEngine::getFunction(const QString &pFunctionName)
{
    // Return the address of the requested function

    if (mExecutionEngine)
        return reinterpret_cast<void *>(mExecutionEngine->getFunctionAddress(qPrintable(pFunctionName)));
    else
        return nullptr;
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
