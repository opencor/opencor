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

inline std::string functionName(const char *pFunctionName)
{
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    return pFunctionName;
#else
    return std::string("_")+pFunctionName;
#endif
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

    // Get a driver to compile our code

    auto diagnosticOptions = new clang::DiagnosticOptions();
    clang::DiagnosticsEngine diagnosticsEngine(llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs()),
                                               &*diagnosticOptions);
    clang::driver::Driver driver("clang", llvm::sys::getProcessTriple(), diagnosticsEngine);

    driver.setCheckInputsExist(false);

    // Get a compilation object to which we pass some arguments

    llvm::StringRef dummyFileName("dummyFile.c");
    llvm::SmallVector<const char *, 16> compilationArguments;

    compilationArguments.emplace_back("clang");
    compilationArguments.emplace_back("-fsyntax-only");
#ifdef QT_DEBUG
    compilationArguments.emplace_back("-g");
    compilationArguments.emplace_back("-O0");
#else
    compilationArguments.emplace_back("-O3");
    compilationArguments.emplace_back("-ffast-math");
#endif
    compilationArguments.emplace_back("-Werror");
    compilationArguments.emplace_back(dummyFileName.data());

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

    static const QString Clang = "clang";

    auto &command = llvm::cast<clang::driver::Command>(*jobs.begin());
    QString commandName = command.getCreator().getName();

    if (commandName != Clang) {
        mError = tr("a <strong>clang</strong> command was expected, but a <strong>%1</strong> command was found instead").arg(commandName);

        return false;
    }

    // Create a compiler invocation using our command's arguments

    const llvm::opt::ArgStringList &commandArguments = command.getArguments();
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

    if (!module) {
        mError = tr("the bitcode module could not be retrieved");

        return false;
    }

    // Initialise the native target (and its ASM printer), so not only can we
    // then create an execution engine, but more importantly its data layout
    // will match that of our target platform

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    // Create and keep track of an execution engine

    mExecutionEngine = llvm::EngineBuilder(std::move(module)).setEngineKind(llvm::EngineKind::JIT).create();

    if (mExecutionEngine == nullptr) {
        mError = tr("the execution engine could not be created");

        module.reset();

        return false;
    }

    // Map all the external functions that may, or not, be needed by the given
    // code

    mExecutionEngine->addGlobalMapping(functionName("fabs"), reinterpret_cast<quint64>(compiler_fabs));

    mExecutionEngine->addGlobalMapping(functionName("log"), reinterpret_cast<quint64>(compiler_log));
    mExecutionEngine->addGlobalMapping(functionName("exp"), reinterpret_cast<quint64>(compiler_exp));

    mExecutionEngine->addGlobalMapping(functionName("floor"), reinterpret_cast<quint64>(compiler_floor));
    mExecutionEngine->addGlobalMapping(functionName("ceil"), reinterpret_cast<quint64>(compiler_ceil));

    mExecutionEngine->addGlobalMapping(functionName("factorial"), reinterpret_cast<quint64>(compiler_factorial));

    mExecutionEngine->addGlobalMapping(functionName("sin"), reinterpret_cast<quint64>(compiler_sin));
    mExecutionEngine->addGlobalMapping(functionName("sinh"), reinterpret_cast<quint64>(compiler_sinh));
    mExecutionEngine->addGlobalMapping(functionName("asin"), reinterpret_cast<quint64>(compiler_asin));
    mExecutionEngine->addGlobalMapping(functionName("asinh"), reinterpret_cast<quint64>(compiler_asinh));

    mExecutionEngine->addGlobalMapping(functionName("cos"), reinterpret_cast<quint64>(compiler_cos));
    mExecutionEngine->addGlobalMapping(functionName("cosh"), reinterpret_cast<quint64>(compiler_cosh));
    mExecutionEngine->addGlobalMapping(functionName("acos"), reinterpret_cast<quint64>(compiler_acos));
    mExecutionEngine->addGlobalMapping(functionName("acosh"), reinterpret_cast<quint64>(compiler_acosh));

    mExecutionEngine->addGlobalMapping(functionName("tan"), reinterpret_cast<quint64>(compiler_tan));
    mExecutionEngine->addGlobalMapping(functionName("tanh"), reinterpret_cast<quint64>(compiler_tanh));
    mExecutionEngine->addGlobalMapping(functionName("atan"), reinterpret_cast<quint64>(compiler_atan));
    mExecutionEngine->addGlobalMapping(functionName("atanh"), reinterpret_cast<quint64>(compiler_atanh));

    mExecutionEngine->addGlobalMapping(functionName("sec"), reinterpret_cast<quint64>(compiler_sec));
    mExecutionEngine->addGlobalMapping(functionName("sech"), reinterpret_cast<quint64>(compiler_sech));
    mExecutionEngine->addGlobalMapping(functionName("asec"), reinterpret_cast<quint64>(compiler_asec));
    mExecutionEngine->addGlobalMapping(functionName("asech"), reinterpret_cast<quint64>(compiler_asech));

    mExecutionEngine->addGlobalMapping(functionName("csc"), reinterpret_cast<quint64>(compiler_csc));
    mExecutionEngine->addGlobalMapping(functionName("csch"), reinterpret_cast<quint64>(compiler_csch));
    mExecutionEngine->addGlobalMapping(functionName("acsc"), reinterpret_cast<quint64>(compiler_acsc));
    mExecutionEngine->addGlobalMapping(functionName("acsch"), reinterpret_cast<quint64>(compiler_acsch));

    mExecutionEngine->addGlobalMapping(functionName("cot"), reinterpret_cast<quint64>(compiler_cot));
    mExecutionEngine->addGlobalMapping(functionName("coth"), reinterpret_cast<quint64>(compiler_coth));
    mExecutionEngine->addGlobalMapping(functionName("acot"), reinterpret_cast<quint64>(compiler_acot));
    mExecutionEngine->addGlobalMapping(functionName("acoth"), reinterpret_cast<quint64>(compiler_acoth));

    mExecutionEngine->addGlobalMapping(functionName("arbitrary_log"), reinterpret_cast<quint64>(compiler_arbitrary_log));

    mExecutionEngine->addGlobalMapping(functionName("pow"), reinterpret_cast<quint64>(compiler_pow));

    mExecutionEngine->addGlobalMapping(functionName("multi_min"), reinterpret_cast<quint64>(compiler_multi_min));
    mExecutionEngine->addGlobalMapping(functionName("multi_max"), reinterpret_cast<quint64>(compiler_multi_max));

    mExecutionEngine->addGlobalMapping(functionName("gcd_multi"), reinterpret_cast<quint64>(compiler_gcd_multi));
    mExecutionEngine->addGlobalMapping(functionName("lcm_multi"), reinterpret_cast<quint64>(compiler_lcm_multi));

    return true;
}

//==============================================================================

void * CompilerEngine::getFunction(const QString &pFunctionName)
{
    // Return the address of the requested function

    if (mExecutionEngine != nullptr) {
        return reinterpret_cast<void *>(mExecutionEngine->getFunctionAddress(qPrintable(pFunctionName)));
    }

    return nullptr;
}

//==============================================================================

} // namespace Compiler
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
