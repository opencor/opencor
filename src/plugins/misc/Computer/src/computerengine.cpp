//==============================================================================
// Computer engine class
//==============================================================================

#include "computerengine.h"
#include "computermath.h"
#include "computerparser.h"

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

#include "llvm/Assembly/Parser.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/DynamicLibrary.h"



#include "llvm/Module.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/ADT/SmallString.h"
//#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/TargetSelect.h"

#include "clang/CodeGen/CodeGenAction.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Tool.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/DiagnosticOptions.h"
#include "clang/Frontend/FrontendDiagnostic.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerEngineData::ComputerEngineData() :
    mAssemblyCode(QString()),
    mAssemblyCodeIndex(0),
    mExternalFunctions(ComputerExternalFunctions()),
    mNeedTbaaInformation(false),
    mIndirectParameterPointerAssemblyCodeIndexes(ComputerEngineIndirectParameterAssemblyCodeIndexes()),
    mIndirectParameterLoadAssemblyCodeIndexes(ComputerEngineIndirectParameterAssemblyCodeIndexes()),
    mEquationAssemblyCodeIndexes(ComputerEngineEquationAssemblyCodeIndexes())
{
}

//==============================================================================

QString ComputerEngineData::assemblyCode() const
{
    // Return the data's assembly code

    return mAssemblyCode;
}

//==============================================================================

void ComputerEngineData::appendAssemblyCode(const QString &pAssemblyCode)
{
    // Append some assembly code

    mAssemblyCode += pAssemblyCode;
}

//==============================================================================

int ComputerEngineData::assemblyCodeIndex() const
{
    // Return the data's assembly code index

    return mAssemblyCodeIndex;
}

//==============================================================================

int ComputerEngineData::nextAssemblyCodeIndex()
{
    // Increment and return the data's assembly code index

    return ++mAssemblyCodeIndex;
}

//==============================================================================

ComputerExternalFunctions ComputerEngineData::externalFunctions() const
{
    // Return the function's external functions

    return mExternalFunctions;
}

//==============================================================================

bool ComputerEngineData::addExternalFunction(const QString &pExternalFunctionName,
                                             const int &pArgumentsCount,
                                             void *pFunction)
{
    // Add an external function to our list, but only if i isn't already there

    ComputerExternalFunction externalFunction = ComputerExternalFunction(pExternalFunctionName,
                                                                         pArgumentsCount,
                                                                         pFunction);

    if (mExternalFunctions.contains(externalFunction)) {
        // The external function already exists, so...

        return false;
    } else {
        // The external function is not yet in our list, so add it

        mExternalFunctions << externalFunction;

        return true;
    }
}

//==============================================================================

bool ComputerEngineData::needTbaaInformation() const
{
    // Return whether the data needs TBAA information

    return mNeedTbaaInformation;
}

//==============================================================================

void ComputerEngineData::setNeedTbaaInformation(const bool &pNeedTbaaInformation)
{
    // Set the data's need for TBAA information

    mNeedTbaaInformation = pNeedTbaaInformation;
}

//==============================================================================

ComputerEngineIndirectParameterAssemblyCodeIndexes ComputerEngineData::indirectParameterPointerAssemblyCodeIndexes() const
{
    // Return the data's list of indirect parameter pointer assembly code
    // indexes

    return mIndirectParameterPointerAssemblyCodeIndexes;
}

//==============================================================================

void ComputerEngineData::addIndirectParameterPointerAssemblyCodeIndex(const QString &pKey,
                                                                      const int &pValue)
{
    // Add the key/value pair to our list of indirect parameter pointer assembly
    // code indexes

    mIndirectParameterPointerAssemblyCodeIndexes.insert(pKey, pValue);
}

//==============================================================================

ComputerEngineIndirectParameterAssemblyCodeIndexes ComputerEngineData::indirectParameterLoadAssemblyCodeIndexes() const
{
    // Return the data's list of indirect parameter load assembly code indexes

    return mIndirectParameterLoadAssemblyCodeIndexes;
}

//==============================================================================

void ComputerEngineData::addIndirectParameterLoadAssemblyCodeIndex(const QString &pKey,
                                                                   const int &pValue)
{
    // Add the key/value pair to our list of indirect parameter load assembly
    // code indexes

    mIndirectParameterLoadAssemblyCodeIndexes.insert(pKey, pValue);
}

//==============================================================================

ComputerEngineEquationAssemblyCodeIndexes ComputerEngineData::equationAssemblyCodeIndexes() const
{
    // Return the data's list of equation assembly code indexes

    return mEquationAssemblyCodeIndexes;
}

//==============================================================================

void ComputerEngineData::addEquationAssemblyCodeIndex(ComputerEquation *pKey,
                                                      const int &pValue)
{
    // Add the key/value pair to our list of equation assembly code indexes

    mEquationAssemblyCodeIndexes.insert(pKey, pValue);
}

//==============================================================================

ComputerEngine::ComputerEngine() :
    mError(ComputerError()),
    mExternalFunctions(ComputerExternalFunctions())
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

    // Create a parser

    mParser = new ComputerParser();
}

//==============================================================================

ComputerEngine::~ComputerEngine()
{
    // Delete some internal objects

    delete mParser;

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

ComputerError ComputerEngine::parserError()
{
    // Return the computer engine's parser's error

    return mParser->error();
}

//==============================================================================

llvm::sys::Path GetExecutablePath(const char *Argv0) {
  // This just needs to be some symbol in the binary; C++ doesn't
  // allow taking the address of ::main however.
  void *MainAddr = (void*) (intptr_t) GetExecutablePath;
  return llvm::sys::Path::GetMainExecutable(Argv0, MainAddr);
}

llvm::Function * ComputerEngine::addFunction(const QString &pFunction)
{



    qDebug(">>> 01");
    void *MainAddr = (void*) (intptr_t) GetExecutablePath;
    qDebug(">>> 02");
    llvm::sys::Path path = GetExecutablePath(qPrintable(qApp->applicationFilePath()));
    qDebug(">>> 03");
    clang::TextDiagnosticPrinter *diagnosticClient = new clang::TextDiagnosticPrinter(llvm::errs(),
                                                                                      clang::DiagnosticOptions());
    qDebug(">>> 04");
    llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diagnosticId = llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs());
    qDebug(">>> 05");
    clang::DiagnosticsEngine diagnostics = clang::DiagnosticsEngine(diagnosticId, diagnosticClient);
    qDebug(">>> 06");
    clang::driver::Driver driver = clang::driver::Driver(path.str(),
                                                         llvm::sys::getDefaultTargetTriple(),
                                                         "a.out",
                                                         false,   //---GRY--- MIGHT WANT TO USE true?
                                                         diagnostics);
    qDebug(">>> 07");

//    driver.setTitle("clang interpreter");

    llvm::SmallVector<const char *, 16> args;
    qDebug(">>> 08");

    args.push_back("C:\\Users\\Alan\\Desktop\\test.c");
//    args.push_back("-fsyntax-only");   //---GRY--- DO WE REALLY NEED THIS FLAG?
    qDebug(">>> 09");

    llvm::OwningPtr<clang::driver::Compilation> compilation(driver.BuildCompilation(args));
//    llvm::OwningPtr<clang::driver::Compilation> compilation(driver.BuildCompilation(llvm::ArrayRef<const char *>()));
    qDebug(">>> 10");

    if (!compilation)
      return 0;

    // We expect to get back exactly one command job, so if we didn't then
    // something failed...

    const clang::driver::JobList &jobList = compilation->getJobs();
    qDebug(">>> 11");

    if (jobList.size() != 1 || !llvm::isa<clang::driver::Command>(*jobList.begin())) {
//      llvm::SmallString<256> message;
//      llvm::raw_svector_ostream output(message);
//      compilation->PrintJob(output, compilation->getJobs(), "; ", true);
//      diagnostics.Report(clang::diag::err_fe_expected_compiler_job) << output.str();
        qDebug(">>> Something failed...");
      return 0;
    }
    qDebug(">>> 12");

    const clang::driver::Command *command = llvm::cast<clang::driver::Command>(*jobList.begin());
    qDebug(">>> 13");

    if (llvm::StringRef(command->getCreator().getName()) != "clang") {
//      diagnostics.Report(clang::diag::err_fe_expected_clang_command);
        qDebug(">>> Not a Clang command...");
      return 0;
    }
    qDebug(">>> 14");

    // Initialize a compiler invocation object from the clang (-cc1) arguments.
    const clang::driver::ArgStringList &CCArgs = command->getArguments();
    qDebug(">>> 15");
    llvm::OwningPtr<clang::CompilerInvocation> CI(new clang::CompilerInvocation);
    qDebug(">>> 16");
    clang::CompilerInvocation::CreateFromArgs(*CI,
                                       const_cast<const char **>(CCArgs.data()),
                                       const_cast<const char **>(CCArgs.data()) +
                                         CCArgs.size(),
                                       diagnostics);
    qDebug(">>> 17");

    // Show the invocation, with -v.
    if (CI->getHeaderSearchOpts().Verbose) {
//      llvm::errs() << "clang invocation:\n";
//      compilation->PrintJob(llvm::errs(), compilation->getJobs(), "\n", true);
//      llvm::errs() << "\n";
    }
    qDebug(">>> 18");

    // FIXME: This is copied from cc1_main.cpp; simplify and eliminate.

    // Create a compiler instance to handle the actual work.
    clang::CompilerInstance Clang;
    qDebug(">>> 19");
    Clang.setInvocation(CI.take());
    qDebug(">>> 20");

    // Create the compilers actual diagnostics engine.
    Clang.createDiagnostics(int(CCArgs.size()),const_cast<char**>(CCArgs.data()));
    qDebug(">>> 21");
    if (!Clang.hasDiagnostics())
      return 0;
    qDebug(">>> 22");

    // Infer the builtin include path if unspecified.
    if (Clang.getHeaderSearchOpts().UseBuiltinIncludes &&
        Clang.getHeaderSearchOpts().ResourceDir.empty())
      Clang.getHeaderSearchOpts().ResourceDir =
        clang::CompilerInvocation::GetResourcesPath(qPrintable(qApp->applicationFilePath()),
                                                    MainAddr);
    qDebug(">>> 23");

    // Create and execute the frontend to generate an LLVM bitcode module.
    llvm::OwningPtr<clang::CodeGenAction> Act(new clang::EmitLLVMOnlyAction());
    qDebug(">>> 24");
    if (!Clang.ExecuteAction(*Act))
      return 0;
    qDebug(">>> 25");

//    int Res = 255;
//    if (llvm::Module *Module = Act->takeModule())
//      Res = Execute(Module, envp);

    // Shutdown.

    llvm::llvm_shutdown();
    qDebug(">>> 26");



//------------------------------------------------------------------------------

    // Reset the engine's error

    mError = ComputerError();

    // Parse the function

    ComputerFunction *function = mParser->parseFunction(pFunction);

    if (function) {
        // The function was properly parsed, so check that we don't already have
        // a function with the same name in our module

        if (mModule->getFunction(qPrintable(function->name()))) {
            // A function with the same name already exists, so...

            mError = ComputerError(tr("there is already a function called '%1'").arg(function->name()));

            return 0;
        }

        // No function with the same name already exists, so we can try to
        // compile it

        llvm::Function *res = compileFunction(function);

        delete function;

        return res;
        // Note: it's OK if the compilation failed, since compileFunction() will
        //       then return 0...
    } else {
        // The function wasn't properly parsed, so...

        return 0;
    }
}

//==============================================================================

static const QString Indent = QString("  ");

//==============================================================================

QString ComputerEngine::numberAsString(const double &pNumber)
{
    // Return a number as a string
    // Note: the number will have the following formating: [-]9.9e[+|-]999

    return  QString::number(pNumber, 'e');
}

//==============================================================================

llvm::Function * ComputerEngine::compileFunction(ComputerFunction *pFunction)
{
    // Generate some LLVM assembly code based on the contents of the function
    // and then have LLVM generate some IR code out of it, before returning the
    // resulting LLVM function

    // Data that will be needed for the compilation of our function

    ComputerEngineData data;

    // Define the function

    data.appendAssemblyCode("define");

    // Type of function

    if (pFunction->type() == ComputerFunction::Void)
        data.appendAssemblyCode(" void");
    else
        data.appendAssemblyCode(" double");

    // Name of the function

    data.appendAssemblyCode(" @"+pFunction->name());

    // Parameters of the function

    QString parameters = QString();

    foreach (const ComputerParameter &parameter, pFunction->parameters()) {
        // Add a separator first if we already have 1+ parameters

        if (!parameters.isEmpty())
            parameters += ", ";

        // Add the parameter definition

        parameters += "double";

        if (parameter.pointer())
            // The parameter is a pointer, so...

            parameters += "* nocapture";

        parameters += " %"+parameter.name();
    }

    data.appendAssemblyCode("("+parameters+") nounwind uwtable");

    // Additional information for the function definition

    data.appendAssemblyCode(" {\n");

    // Mathematical statements

    foreach (ComputerEquation *equation, pFunction->equations()) {
        // Simplify the RHS of the equation, if possible

        equation->right()->simplify();

        // Compile the RHS of the equation

        compileEquationNode(data, equation->right());

        // Assign the result of the RHS of the equation to the indirect
        // parameter which information can be found the LHS of the equation

        compileAssignmentEquation(data, equation);
    }

    // Return statement

    if (pFunction->type() == ComputerFunction::Void) {
        // We are dealing with a void function, so...

        data.appendAssemblyCode(Indent+"ret void\n");
    } else {
        // We are dealing with a double function

        // Simplify the return equation, if possible

        pFunction->returnEquation()->simplify();

        // Compile the return equation

        compileEquationNode(data, pFunction->returnEquation());

        // Return the value of the return equation

        QString returnEquationValue = compileOperand(data, pFunction->returnEquation());

        data.appendAssemblyCode(Indent+"ret double "+returnEquationValue+"\n");
    }

    // End the function

    data.appendAssemblyCode("}");

    // Declare any external function which we need

    bool hasExternalFunctions = data.externalFunctions().count();
    int oldExternalFunctionsCount = mExternalFunctions.count();

    if (hasExternalFunctions)
        data.appendAssemblyCode("\n\n");

    foreach (const ComputerExternalFunction &externalFunction,
             data.externalFunctions())
        if (!mExternalFunctions.contains(externalFunction)) {
            // The external function hasn't already been declared, so declare it

            data.appendAssemblyCode(QString("declare double @%1").arg(externalFunction.name()));

            QString parameters = QString();

            if (externalFunction.parametersCount() == -1)
                // A function with X parameters

                parameters = "i32, ...";
            else
                // A function with a finite number of parameters

                for (int i = 0, iMax = externalFunction.parametersCount(); i < iMax; ++i) {
                    // Add a separator first if we already have 1+ parameters

                    if (!parameters.isEmpty())
                        parameters += ", ";

                    // Add the parameter definition

                    parameters += "double";
                }

            data.appendAssemblyCode("("+parameters+") nounwind\n");

            // Add a symbol for the function, if needed
            // Note: by default, LLVM adds a symbol for some known functions
            //       (e.g. sin()), but not for some other (math) functions (e.g.
            //       asinh()) and obviously not for our CellML-related math
            //       functions (e.g. factorial()), so in all of those cases we
            //       need to 'manually' add a symbol...

            if (externalFunction.function())
                llvm::sys::DynamicLibrary::AddSymbol(qPrintable(externalFunction.name()),
                                                     externalFunction.function());

            // Keep track of the fact that we have already defined the external
            // function

            mExternalFunctions << externalFunction;
        }

    // Add the TBAA information, if needed

    if (data.needTbaaInformation()) {
        if (hasExternalFunctions) {
            if (oldExternalFunctionsCount != mExternalFunctions.count())
                data.appendAssemblyCode("\n");
        } else {
            data.appendAssemblyCode("\n\n");
        }

        data.appendAssemblyCode("!0 = metadata !{metadata !\"double\", metadata !1}\n");
        data.appendAssemblyCode("!1 = metadata !{metadata !\"omnipotent char\", metadata !2}\n");
        data.appendAssemblyCode("!2 = metadata !{metadata !\"Simple C/C++ TBAA\", null}");
    }

    // Now that we are done generating some LLVM assembly code for the function,
    // we can parse that code and have LLVM generate some IR code that will get
    // automatically added to our module

    llvm::SMDiagnostic parseError;
    llvm::ParseAssemblyString(qPrintable(data.assemblyCode()),
                              mModule, parseError, llvm::getGlobalContext());

    if (parseError.getMessage().size())
        mError = ComputerError(tr("the LLVM assembly code for the '%1' function could not be parsed").arg(pFunction->name()));
        // Note: we must not exit straightaway since LLVM may have generated
        //       some IR code, so we first need to check whether part of the
        //       function has already been generated and, if so, remove it...

    // Try to retrieve the function which assembly code we have just parsed

    llvm::Function *res = mModule->getFunction(qPrintable(pFunction->name()));

    if (res) {
        // The function could be retrieved, but it should be removed in case an
        // error of sorts occurred during the parsing of the assembly code

        if (!mError.isEmpty()) {
            // An error occurred during the parsing of the assembly code, so
            // remove the function

            res->eraseFromParent();

            return 0;
        }

        // Everything went fine, so...

        return res;
    } else {
        // The function couldn't be retrieved, so add an error but only if no
        // error occurred during the parsing of the LLVM assembly code

        if (mError.isEmpty())
            mError = ComputerError(tr("the '%1' function could not be found").arg(pFunction->name()));

        return 0;
    }
}

//==============================================================================

int ComputerEngine::indirectParameterAssemblyCodeIndex(ComputerEngineData &pData,
                                                       ComputerEquation *pIndirectParameter,
                                                       const bool &pOperand)
{
    // Retrieve the assembly code index associated with an indirect parameter or
    // create one, if needed. Two assembly code indexes exist for a given
    // indirect parameter depending on whether it is used as an operand (as on
    // the RHS of an equation) or not (as on the LHS of an equation, i.e. for an
    // assignment). In both cases, we first need to retrieve the assembly code
    // index which gives us a direct pointer to the indirect parameter we are
    // after. Then, depending on whether the indirect parameter is used as an
    // operand, we need to load the indirect parameter and return the
    // corresponding assembly code index

    // Key used to retrieve existing assembly code indexes for the independent
    // parameter

    QString key =  pIndirectParameter->parameterName()
                  +"|"
                  +QString::number(pIndirectParameter->parameterIndex());

    // Get the assembly code index for the direct pointer to the indirect
    // parameter

    int pointerAssemblyCodeIndex;

    if (pIndirectParameter->parameterIndex()) {
        // We are not dealing with the first entry in the array of doubles,
        // so we need to retrieve an assembly code index for the direct pointer
        // to the indirect parameter or create one, if needed

        if (pData.indirectParameterPointerAssemblyCodeIndexes().contains(key)) {
            // An assembly code index already exists, so retrieve its value

            pointerAssemblyCodeIndex = pData.indirectParameterPointerAssemblyCodeIndexes().value(key);
        } else {
            // No assembly code index for the direct pointer to the indirect
            // parameter exists, so create one

            pData.appendAssemblyCode(Indent+"%"+QString::number(pData.nextAssemblyCodeIndex())+" = getelementptr inbounds double* %"+pIndirectParameter->parameterName()+", i64 "+QString::number(pIndirectParameter->parameterIndex())+"\n");

            // Keep track of the assembly code index

            pointerAssemblyCodeIndex = pData.assemblyCodeIndex();

            pData.addIndirectParameterPointerAssemblyCodeIndex(key, pointerAssemblyCodeIndex);
        }
    } else {
        // We are dealing with the first entry in the array of doubles, so there
        // is no need for an assembly code index for the direct pointer to the
        // indirect parameter

        pointerAssemblyCodeIndex = 0;
    }

    // Check whether we want an assembly code index for an operand

    if (pOperand) {
        // The indirect parameter is to be used as an operand, so we need to
        // retrieve an assembly code index for the loading of the indirect
        // parameter or create one, if needed

        if (pData.indirectParameterLoadAssemblyCodeIndexes().contains(key)) {
            // An assembly code index already exists, so return its value

            return pData.indirectParameterLoadAssemblyCodeIndexes().value(key);
        } else {
            // No assembly code index for the loading of the indirect  parameter
            // exists, so create one

            pData.appendAssemblyCode(Indent+"%"+QString::number(pData.nextAssemblyCodeIndex())+" = load double* %");

            if (pointerAssemblyCodeIndex)
                pData.appendAssemblyCode(QString::number(pointerAssemblyCodeIndex));
            else
                pData.appendAssemblyCode(pIndirectParameter->parameterName());

            pData.appendAssemblyCode(", align 8, !tbaa !0\n");

            // Keep track of the fact that we need TBAA information

            pData.setNeedTbaaInformation(true);

            // Keep track of the assembly code index

            pData.addIndirectParameterLoadAssemblyCodeIndex(key, pData.assemblyCodeIndex());

            // Return the assembly code index for the loading of the indirect
            // parameter

            return pData.assemblyCodeIndex();
        }
    } else {
        // We only want an assembly code index for the direct pointer to the
        // indirect parameter, so...

        return pointerAssemblyCodeIndex;
    }
}

//==============================================================================

QString ComputerEngine::compileOperand(ComputerEngineData &pData,
                                       ComputerEquation *pOperand)
{
    switch (pOperand->type()) {
    case ComputerEquation::DirectParameter:
        // A direct parameter, so...

        return "%"+pOperand->parameterName();
    case ComputerEquation::IndirectParameter:
        // An indirect parameter, so...

        return "%"+QString::number(indirectParameterAssemblyCodeIndex(pData, pOperand, true));
    case ComputerEquation::Number: {
        // A number, so...

        double number = pOperand->number();

        if (number != number)
            // NaN

            return "0x7ff8000000000000";
        else if (number > std::numeric_limits<double>::max())
            // +Inf

            return "0x7ff0000000000000";
        else if (number < -std::numeric_limits<double>::max())
            // -Inf

            return "0xfff0000000000000";
        else
            return numberAsString(number);
    }
    default:
        // Part of a computed equation, so...

        return "%"+QString::number(pData.equationAssemblyCodeIndexes().value(pOperand));
    }
}

//==============================================================================

void ComputerEngine::compileAssignmentEquation(ComputerEngineData &pData,
                                               ComputerEquation *pAssignmentEquation)
{
    // Get the RHS equation as an operand

    QString rhsEquation = compileOperand(pData, pAssignmentEquation->right());

    // Retrieve, for the indirect parameter and if necessary, a pointer to the
    // correct entry in the array of doubles

    int assemblyCodeIndex = indirectParameterAssemblyCodeIndex(pData, pAssignmentEquation->left(), false);

    // Store the RHS of the equation to the LHS of the equation

    pData.appendAssemblyCode(Indent+"store double "+rhsEquation+", double* %");

    if (assemblyCodeIndex)
        // We have an assembly code index for the indirect parameter, so...

        pData.appendAssemblyCode(QString::number(assemblyCodeIndex));
    else
        // We don't have an assembly code index for the indirect parameter,
        // so...

        pData.appendAssemblyCode(pAssignmentEquation->left()->parameterName());

    pData.appendAssemblyCode(", align 8, !tbaa !0\n");

    // Keep track of the fact that we need TBAA information

    pData.setNeedTbaaInformation(true);
}

//==============================================================================

void ComputerEngine::compileMathematicalOperator(ComputerEngineData &pData,
                                                 const QString &pOperator,
                                                 ComputerEquation *pOperand1,
                                                 ComputerEquation *pOperand2)
{
    // Compile the two operands

    QString operandOne = compileOperand(pData, pOperand1);
    QString operandTwo = compileOperand(pData, pOperand2);

    // Apply the mathematical operator

    pData.appendAssemblyCode(Indent+"%"+QString::number(pData.nextAssemblyCodeIndex())+" = "+pOperator+" double "+operandOne+", "+operandTwo+"\n");
}

//==============================================================================

void ComputerEngine::compileMathematicalOperator(ComputerEngineData &pData,
                                                 const QString &pOperator,
                                                 ComputerEquation *pOperands)
{
    // Compile the mathematical operator

    compileMathematicalOperator(pData, pOperator,
                                pOperands->left(), pOperands->right());
}

//==============================================================================

QStringList ComputerEngine::compileMathematicalFunctionArguments(ComputerEngineData &pData,
                                                                 ComputerEquation *pArguments,
                                                                 const int &pLevel)
{
    if (!pLevel || (pArguments->type() == ComputerEquation::OtherArguments))
        return QStringList() << compileMathematicalFunctionArguments(pData, pArguments->left(), pLevel+1)
                             << compileMathematicalFunctionArguments(pData, pArguments->right(), pLevel+1);
    else
        return QStringList() << compileOperand(pData, pArguments);
}

//==============================================================================

void ComputerEngine::compileMathematicalFunction(ComputerEngineData &pData,
                                                 const QString &pFunctionName,
                                                 const int &pArgumentsCount,
                                                 ComputerEquation *pArguments,
                                                 void *pFunction)
{
    // Compile the different arguments

    QStringList arguments = QStringList();

    if (pArgumentsCount == 1) {
        arguments << compileOperand(pData, pArguments->left());
    } else if (pArgumentsCount == 2) {
        arguments << compileOperand(pData, pArguments->left());
        arguments << compileOperand(pData, pArguments->right());
    } else if (pArgumentsCount == 3) {
        arguments << compileOperand(pData, pArguments->left());
        arguments << compileOperand(pData, pArguments->right()->left());
        arguments << compileOperand(pData, pArguments->right()->right());
    } else {
        // X arguments

        arguments << compileMathematicalFunctionArguments(pData, pArguments);
    }

    // Apply the mathematical function

    pData.appendAssemblyCode(Indent+"%"+QString::number(pData.nextAssemblyCodeIndex())+" = tail call double ");

    if (pArgumentsCount == -1)
        pData.appendAssemblyCode("(i32, ...)* ");

    pData.appendAssemblyCode("@"+pFunctionName+"(");

    if (pArgumentsCount == -1)
        pData.appendAssemblyCode("i32 "+QString::number(arguments.count())+", ");

    for (int i = 0, iMax = arguments.count(); i < iMax; ++i) {
        if (i)
            pData.appendAssemblyCode(", ");

        pData.appendAssemblyCode("double "+arguments[i]);
    }

    pData.appendAssemblyCode(") nounwind\n");

    // Keep track of the need for the mathematical function

    pData.addExternalFunction(pFunctionName, pArgumentsCount, pFunction);
}

//==============================================================================

void ComputerEngine::compileEquationNode(ComputerEngineData &pData,
                                         ComputerEquation *pEquationNode)
{
    static const double zeroNumber = 0;
    static ComputerEquation zeroNumberEquation = ComputerEquation(zeroNumber);

    // Make sure that the node is valid

    if (!pEquationNode)
        // It isn't, so...

        return;

    // Check whether the current node is a direct parameter, an indirect
    // parameter, a number or a set of other arguments

    if (   (pEquationNode->type() == ComputerEquation::DirectParameter)
        || (pEquationNode->type() == ComputerEquation::IndirectParameter)
        || (pEquationNode->type() == ComputerEquation::Number))
        // It is, so nothing to compile (since this will be done as part of the
        // compilation of another node), so...

        return;

    // Compile both the left and right nodes

    compileEquationNode(pData, pEquationNode->left());
    compileEquationNode(pData, pEquationNode->right());

    // Compilation of the current node

    switch (pEquationNode->type()) {
    // Logical operators

    case ComputerEquation::Not:
        compileMathematicalFunction(pData, "not", 1, pEquationNode,
                                    (void *)(intptr_t) _not);

        break;
    case ComputerEquation::Or:
        compileMathematicalFunction(pData, "_or", 2, pEquationNode,
                                    (void *)(intptr_t) _or);

        break;
    case ComputerEquation::Xor:
        compileMathematicalFunction(pData, "_xor", 2, pEquationNode,
                                    (void *)(intptr_t) _xor);

        break;
    case ComputerEquation::And:
        compileMathematicalFunction(pData, "_and", 2, pEquationNode,
                                    (void *)(intptr_t) _and);

        break;
    case ComputerEquation::EqualEqual:
        compileMathematicalFunction(pData, "eq", 2, pEquationNode,
                                    (void *)(intptr_t) eq);

        break;
    case ComputerEquation::NotEqual:
        compileMathematicalFunction(pData, "neq", 2, pEquationNode,
                                    (void *)(intptr_t) neq);

        break;
    case ComputerEquation::LowerThan:
        compileMathematicalFunction(pData, "lt", 2, pEquationNode,
                                    (void *)(intptr_t) lt);

        break;
    case ComputerEquation::GreaterThan:
        compileMathematicalFunction(pData, "gt", 2, pEquationNode,
                                    (void *)(intptr_t) gt);

        break;
    case ComputerEquation::LowerOrEqualThan:
        compileMathematicalFunction(pData, "le", 2, pEquationNode,
                                    (void *)(intptr_t) le);

        break;
    case ComputerEquation::GreaterOrEqualThan:
        compileMathematicalFunction(pData, "ge", 2, pEquationNode,
                                    (void *)(intptr_t) ge);

        break;

    // Mathematical operators

    case ComputerEquation::Times:
        compileMathematicalOperator(pData, "fmul", pEquationNode);

        break;
    case ComputerEquation::Divide:
        compileMathematicalOperator(pData, "fdiv", pEquationNode);

        break;
    case ComputerEquation::Modulo:
        compileMathematicalFunction(pData, "fmod", 2, pEquationNode);

        break;
    case ComputerEquation::Plus:
        if (pEquationNode->right())
            // Normal addition, so...

            compileMathematicalOperator(pData, "fadd", pEquationNode);
        else
            // Unary "+", so...
            // Note: we can only come here if the equation wasn't first
            //       simplified, hence we need to test for the unary "+"...

            compileOperand(pData, pEquationNode->left());

        break;
    case ComputerEquation::Minus:
        if (pEquationNode->right())
            // Normal subtraction, so...

            compileMathematicalOperator(pData, "fsub", pEquationNode);
        else
            // Unary "-", so...

            compileMathematicalOperator(pData, "fsub", &zeroNumberEquation, pEquationNode->left());

        break;

    // Mathematical functions with 1 argument

    case ComputerEquation::Abs:
        compileMathematicalFunction(pData, "fabs", 1, pEquationNode);

        break;
    case ComputerEquation::Exp:
        compileMathematicalFunction(pData, "exp", 1, pEquationNode);

        break;
    case ComputerEquation::Log:
        compileMathematicalFunction(pData, "log", 1, pEquationNode);

        break;
    case ComputerEquation::Ceil:
        compileMathematicalFunction(pData, "ceil", 1, pEquationNode);

        break;
    case ComputerEquation::Floor:
        compileMathematicalFunction(pData, "floor", 1, pEquationNode);

        break;
    case ComputerEquation::Factorial:
        compileMathematicalFunction(pData, "factorial", 1, pEquationNode,
                                    (void *)(intptr_t) factorial);

        break;
    case ComputerEquation::Sin:
        compileMathematicalFunction(pData, "sin", 1, pEquationNode);

        break;
    case ComputerEquation::Cos:
        compileMathematicalFunction(pData, "cos", 1, pEquationNode);

        break;
    case ComputerEquation::Tan:
        compileMathematicalFunction(pData, "tan", 1, pEquationNode);

        break;
    case ComputerEquation::Sinh:
        compileMathematicalFunction(pData, "sinh", 1, pEquationNode);

        break;
    case ComputerEquation::Cosh:
        compileMathematicalFunction(pData, "cosh", 1, pEquationNode);

        break;
    case ComputerEquation::Tanh:
        compileMathematicalFunction(pData, "tanh", 1, pEquationNode);

        break;
    case ComputerEquation::Asin:
        compileMathematicalFunction(pData, "asin", 1, pEquationNode);

        break;
    case ComputerEquation::Acos:
        compileMathematicalFunction(pData, "acos", 1, pEquationNode);

        break;
    case ComputerEquation::Atan:
        compileMathematicalFunction(pData, "atan", 1, pEquationNode);

        break;
    case ComputerEquation::Asinh:
        compileMathematicalFunction(pData, "asinh", 1, pEquationNode,
                                    (void *)(intptr_t) asinh);

        break;
    case ComputerEquation::Acosh:
        compileMathematicalFunction(pData, "acosh", 1, pEquationNode,
                                    (void *)(intptr_t) acosh);

        break;
    case ComputerEquation::Atanh:
        compileMathematicalFunction(pData, "atanh", 1, pEquationNode,
                                    (void *)(intptr_t) atanh);

        break;

    // Mathematical functions with 2 arguments

    case ComputerEquation::ArbitraryLog:
        compileMathematicalFunction(pData, "arbitraryLog", 2, pEquationNode,
                                    (void *)(intptr_t) arbitraryLog);

        break;
    case ComputerEquation::Pow:
        compileMathematicalFunction(pData, "pow", 2, pEquationNode);

        break;
    case ComputerEquation::Quot:
        compileMathematicalFunction(pData, "quot", 2, pEquationNode,
                                    (void *)(intptr_t) quot);

        break;
    case ComputerEquation::Rem:
        compileMathematicalFunction(pData, "fmod", 2, pEquationNode);

        break;

    // Mathematical functions with 2+ arguments

    case ComputerEquation::Gcd:
        compileMathematicalFunction(pData, "gcd", -1, pEquationNode,
                                    (void *)(intptr_t) gcd);

        break;
    case ComputerEquation::Lcm:
        compileMathematicalFunction(pData, "lcm", -1, pEquationNode,
                                    (void *)(intptr_t) lcm);

        break;
    case ComputerEquation::Max:
        compileMathematicalFunction(pData, "max", -1, pEquationNode,
                                    (void *)(intptr_t) max);

        break;
    case ComputerEquation::Min:
        compileMathematicalFunction(pData, "min", -1, pEquationNode,
                                    (void *)(intptr_t) min);

        break;

    // Definite integral function

    case ComputerEquation::DefInt:
        //---GRY--- DEFINITE INTEGRALS ARE NOT YET SUPPORTED BY OpenCOR, SO...

        break;

    // Miscellaneous

    case ComputerEquation::Piecewise:
        compileMathematicalFunction(pData, "piecewise", 3, pEquationNode,
                                    (void *)(intptr_t) piecewise);

        break;
    case ComputerEquation::PiecewiseCases:
    case ComputerEquation::OtherArguments:
    case ComputerEquation::TwoArguments:
        // Nothing to do...

        break;

    // We should never reach this point...

    default:
        qCritical("ComputerEngine::compileEquationNode: the equation node of type '%s' could not be compiled. This is a bug! Please fix it!", qPrintable(pEquationNode->typeAsString()));
    }

    // Keep track of the assembly code index of the current node

    pData.addEquationAssemblyCodeIndex(pEquationNode,
                                       pData.assemblyCodeIndex());
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
