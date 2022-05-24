/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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

//==============================================================================

#include "llvmclangbegin.h"
    #include "clang/CodeGen/CodeGenAction.h"
    #include "clang/Driver/Compilation.h"
    #include "clang/Driver/Driver.h"
    #include "clang/Driver/Tool.h"
    #include "clang/Frontend/CompilerInstance.h"
    #include "clang/Frontend/TextDiagnosticPrinter.h"
    #include "clang/Lex/PreprocessorOptions.h"

    #include "llvm/Support/Host.h"
    #include "llvm/Support/TargetSelect.h"

    #include "llvm-c/Core.h"
#include "llvmclangend.h"

//==============================================================================

namespace OpenCOR {
namespace Compiler {

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

bool CompilerEngine::addFunction(const QString &pName, void *pFunction)
{
    // Add the given function.

    if ((mLljit != nullptr) && !pName.isEmpty() && (pFunction != nullptr)) {
        auto &jitDylib = mLljit->getMainJITDylib();

        return !jitDylib.define(llvm::orc::absoluteSymbols({
                                                               { mLljit->mangleAndIntern(pName.toStdString()), llvm::JITEvaluatedSymbol(llvm::pointerToJITTargetAddress(pFunction), llvm::JITSymbolFlags::Exported) },
                                                           }));
    }

    return false;
}

//==============================================================================

bool CompilerEngine::compileCode(const QString &pCode)
{
    // Reset ourselves

    mError = QString();

    // Prepend all the external functions that may, or not, be needed by the
    // given code

    QString code =  R"(
extern double fabs(double);

extern double log(double);
extern double exp(double);

extern double floor(double);
extern double ceil(double);

extern double factorial(double);

extern double sin(double);
extern double sinh(double);
extern double asin(double);
extern double asinh(double);

extern double cos(double);
extern double cosh(double);
extern double acos(double);
extern double acosh(double);

extern double tan(double);
extern double tanh(double);
extern double atan(double);
extern double atanh(double);

extern double sec(double);
extern double sech(double);
extern double asec(double);
extern double asech(double);

extern double csc(double);
extern double csch(double);
extern double acsc(double);
extern double acsch(double);

extern double cot(double);
extern double coth(double);
extern double acot(double);
extern double acoth(double);

extern double arbitrary_log(double, double);

extern double pow(double, double);

extern double multi_min(int, ...);
extern double multi_max(int, ...);

extern double gcd_multi(int, ...);
extern double lcm_multi(int, ...);
)"+pCode;

    // Create a diagnostics engine

    auto diagnosticOptions = llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions>(new clang::DiagnosticOptions());
    auto diagnosticsEngine = llvm::IntrusiveRefCntPtr<clang::DiagnosticsEngine>(new clang::DiagnosticsEngine(llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs()),
                                                                                                             &*diagnosticOptions,
                                                                                                             new clang::TextDiagnosticPrinter(llvm::nulls(), &*diagnosticOptions)));

    diagnosticsEngine->setWarningsAsErrors(true);

    // Get a driver object and ask it not to check that input files exist

    clang::driver::Driver driver("clang", llvm::sys::getProcessTriple(), *diagnosticsEngine);

    driver.setCheckInputsExist(false);

    // Get a compilation object to which we pass some arguments

    constexpr char const *DummyFileName = "dummy.c";

    std::vector<const char *> compilationArguments = {"clang", "-fsyntax-only",
#ifdef QT_DEBUG
                                                      "-g",
                                                      "-O0",
#else
                                                      "-O3",
#endif
                                                      "-fno-math-errno",
                                                      DummyFileName};

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

    // Retrieve the command job and make sure that it is "clang"

    constexpr char const *Clang = "clang";

    auto &command = llvm::cast<clang::driver::Command>(*jobs.begin());
    auto commandName = command.getCreator().getName();

    if (strcmp(command.getCreator().getName(), Clang) != 0) {
        mError = tr("a <strong>clang</strong> command was expected, but a <strong>%1</strong> command was found instead").arg(commandName);

        return false;
    }

    // Prevent the Clang driver from asking CC1 to leak memory, this by removing
    // -disable-free from the command arguments

    auto commandArguments = command.getArguments();
    auto *commandArgument = find(commandArguments, llvm::StringRef("-disable-free"));

    if (commandArgument != commandArguments.end()) {
        commandArguments.erase(commandArgument);
    }

    // Create a compiler instance

    clang::CompilerInstance compilerInstance;

    compilerInstance.setDiagnostics(diagnosticsEngine.get());
    compilerInstance.setVerboseOutputStream(llvm::nulls());

    // Create a compiler invocation object

    if (!clang::CompilerInvocation::CreateFromArgs(compilerInstance.getInvocation(),
                                                   commandArguments,
                                                   *diagnosticsEngine)) {
        mError = tr("the compiler invocation object could not be created");

        return false;
    }

    // Map our code to a memory buffer

    QByteArray codeByteArray = code.toUtf8();

    compilerInstance.getInvocation().getPreprocessorOpts().addRemappedFile(DummyFileName,
                                                                           llvm::MemoryBuffer::getMemBuffer(codeByteArray.constData()).release());

    // Compile the given code, resulting in an LLVM bitcode module

    std::unique_ptr<clang::CodeGenAction> codeGenAction(new clang::EmitLLVMOnlyAction(llvm::unwrap(LLVMGetGlobalContext())));

    if (!compilerInstance.ExecuteAction(*codeGenAction)) {
        mError = tr("the code could not be compiled");

        return false;
    }

    // Retrieve the LLVM bitcode module

    auto module = codeGenAction->takeModule();

    if (!module) {
        mError = tr("the bitcode module could not be retrieved");

        return false;
    }

    // Initialise the native target (and its ASM printer), so not only can we
    // then create an execution engine, but more importantly its data layout
    // will match that of our target platform

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();

    // Create an ORC-based JIT and keep track of it (so that we can use it in
    // function())

    auto lljit = llvm::orc::LLJITBuilder().create();

    if (!lljit) {
        mError = tr("the ORC-based JIT could not be created");

        return false;
    }

    mLljit = std::move(*lljit);

    // Make sure that we can find various mathematical functions in the standard
    // C library and the additional ones that we want to support (see
    // compilermath.[cpp|h])

    auto dynamicLibrarySearchGenerator = llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(mLljit->getDataLayout().getGlobalPrefix());

    if (!dynamicLibrarySearchGenerator) {
        mError = tr("the dynamic library search generator could not be created");

        return false;
    }

    mLljit->getMainJITDylib().addGenerator(std::move(*dynamicLibrarySearchGenerator));

    if (   !addFunction("factorial", reinterpret_cast<void *>(factorial))

        || !addFunction("sec", reinterpret_cast<void *>(sec))
        || !addFunction("sech", reinterpret_cast<void *>(sech))
        || !addFunction("asec", reinterpret_cast<void *>(asec))
        || !addFunction("asech", reinterpret_cast<void *>(asech))

        || !addFunction("csc", reinterpret_cast<void *>(csc))
        || !addFunction("csch", reinterpret_cast<void *>(csch))
        || !addFunction("acsc", reinterpret_cast<void *>(acsc))
        || !addFunction("acsch", reinterpret_cast<void *>(acsch))

        || !addFunction("cot", reinterpret_cast<void *>(cot))
        || !addFunction("coth", reinterpret_cast<void *>(coth))
        || !addFunction("acot", reinterpret_cast<void *>(acot))
        || !addFunction("acoth", reinterpret_cast<void *>(acoth))

        || !addFunction("arbitrary_log", reinterpret_cast<void *>(arbitrary_log))

        || !addFunction("multi_min", reinterpret_cast<void *>(multi_min))
        || !addFunction("multi_max", reinterpret_cast<void *>(multi_max))

        || !addFunction("gcd_multi", reinterpret_cast<void *>(gcd_multi))
        || !addFunction("lcm_multi", reinterpret_cast<void *>(lcm_multi))) {
        mError = tr("the additional mathematical methods could not be added");

        return false;
    }

    // Add our LLVM bitcode module to our ORC-based JIT

    auto llvmContext = std::make_unique<llvm::LLVMContext>();
    auto threadSafeModule = llvm::orc::ThreadSafeModule(std::move(module), std::move(llvmContext));

    if (mLljit->addIRModule(std::move(threadSafeModule))) {
        mError = tr("the IR module could not be added to the ORC-based JIT");

        return false;
    }

    return true;
}

//==============================================================================

void * CompilerEngine::function(const QString &pName)
{
    // Return the address of the requested function

    if ((mLljit != nullptr) && !pName.isEmpty()) {
        auto symbol = mLljit->lookup(qPrintable(pName));

        if (symbol) {
            return reinterpret_cast<void *>(symbol->getAddress());
        }
    }

    return nullptr;
}

//==============================================================================

} // namespace Compiler
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
