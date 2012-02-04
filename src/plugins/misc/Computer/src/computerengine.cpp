//==============================================================================
// Computer engine class
//==============================================================================

#include "computerengine.h"
#include "computermath.h"
#include "computerparser.h"

//==============================================================================

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Assembly/Parser.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/DynamicLibrary.h"

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

        mExternalFunctions.append(externalFunction);

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
    mError(QString()),
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

ComputerErrors ComputerEngine::parserErrors()
{
    // Return the computer engine's parser's errors

    return mParser->errors();
}

//==============================================================================

llvm::Function * ComputerEngine::addFunction(const QString &pFunction)
{
    qDebug("---------------------------------------");
    qDebug("Compilation of...");
    qDebug("");
    qDebug("%s", qPrintable(pFunction));

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
        // compile the function

        llvm::Function *res = compileFunction(function);

        delete function;

        return res;
        // Note: it's fine if the compilation failed, since compileFunction()
        //       will then return 0...
    } else {
        // The function wasn't properly parsed, so...

        return 0;
    }
}

//==============================================================================

static QString Indent = QString("  ");

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

        parameters += " %%"+parameter.name();
    }

    data.appendAssemblyCode("("+parameters+") nounwind uwtable");

    // Additional information for the function definition

    data.appendAssemblyCode(" {\n");

    // Mathematical statements

    foreach (ComputerEquation *equation, pFunction->equations()) {
        // Simplify the RHS of the equation, if possible

        equation->right()->simplify();

        // Compile the RHS of the equation

        compileEquationNode(equation->right(), data);

        // Assign the result of the RHS of the equation to the indirect parameter
        // which information can be found the LHS of the equation

        compileAssignmentEquation(equation->left(), equation->right(), data);
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

        compileEquationNode(pFunction->returnEquation(), data);

        // Return the value of the return equation

        QString returnEquationValue = compileOperand(pFunction->returnEquation(), data);

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

            mExternalFunctions.append(externalFunction);
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

    qDebug("");
    qDebug("LLVM assembly:");
    qDebug("");
    qDebug("%s", qPrintable(data.assemblyCode()));

    llvm::SMDiagnostic parseError;
    llvm::ParseAssemblyString(qPrintable(data.assemblyCode().replace("%%", "\%")),
                              mModule, parseError, llvm::getGlobalContext());
    // Note: for LLVM to be able to parse the assembly code properly, we must
    //       replace '%' with '\%', so...

    if (parseError.getMessage().size())
        mError = ComputerError(tr("the LLVM assembly code could not be parsed: %1").arg(QString::fromStdString(parseError.getMessage()).remove("error: ")),
                               parseError.getLineNo(), parseError.getColumnNo(),
                               data.assemblyCode());
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
        // error occurred during the parsing of the assembly code

        if (mError.isEmpty())
            mError = ComputerError(tr("the function '%1' could not be found").arg(pFunction->name()));

        return 0;
    }
}

//==============================================================================

int ComputerEngine::indirectParameterAssemblyCodeIndex(ComputerEquation *pIndirectParameter,
                                                       ComputerEngineData &pData,
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

    QString key = pIndirectParameter->parameterName()+"|"+pIndirectParameter->parameterIndex();

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

            pData.appendAssemblyCode(Indent+"%%"+QString::number(pData.nextAssemblyCodeIndex())+" = getelementptr inbounds double* %%"+pIndirectParameter->parameterName()+", i64 "+QString::number(pIndirectParameter->parameterIndex())+"\n");

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

            pData.appendAssemblyCode(Indent+"%%"+QString::number(pData.nextAssemblyCodeIndex())+" = load double* %%");

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

QString ComputerEngine::compileOperand(ComputerEquation *pOperand,
                                       ComputerEngineData &pData)
{
    switch (pOperand->type()) {
    case ComputerEquation::DirectParameter:
        // A direct parameter, so...

        return "%%"+pOperand->parameterName();
    case ComputerEquation::IndirectParameter:
        // An indirect parameter, so...

        return "%%"+QString::number(indirectParameterAssemblyCodeIndex(pOperand, pData, true));

        break;
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

        return "%%"+QString::number(pData.equationAssemblyCodeIndexes().value(pOperand));
    }
}

//==============================================================================

void ComputerEngine::compileAssignmentEquation(ComputerEquation *pIndirectParameter,
                                               ComputerEquation *pRhsEquation,
                                               ComputerEngineData &pData)
{
    // Get the RHS equation as an operand

    QString rhsEquation = compileOperand(pRhsEquation, pData);

    // Retrieve, for the indirect parameter and if necessary, a pointer to the
    // correct entry in the array of doubles

    int assemblyCodeIndex = indirectParameterAssemblyCodeIndex(pIndirectParameter,
                                                               pData, false);

    // Store the RHS of the equation to the LHS of the equation

    pData.appendAssemblyCode(Indent+"store double "+rhsEquation+", double* %%");

    if (assemblyCodeIndex)
        // We have an assembly code index for the indirect parameter, so...

        pData.appendAssemblyCode(QString::number(assemblyCodeIndex));
    else
        // We don't have an assembly code index for the indirect parameter,
        // so...

        pData.appendAssemblyCode(pIndirectParameter->parameterName());

    pData.appendAssemblyCode(", align 8, !tbaa !0\n");

    // Keep track of the fact that we need TBAA information

    pData.setNeedTbaaInformation(true);
}

//==============================================================================

void ComputerEngine::compileMathematicalOperator(ComputerEquation *pOperandOne,
                                                 ComputerEquation *pOperandTwo,
                                                 const QString &pOperator,
                                                 ComputerEngineData &pData)
{
    // Compile the two operands

    QString operandOne = compileOperand(pOperandOne, pData);
    QString operandTwo = compileOperand(pOperandTwo, pData);

    // Apply the mathematical operator

    pData.appendAssemblyCode(Indent+"%%"+QString::number(pData.nextAssemblyCodeIndex())+" = "+pOperator+" double "+operandOne+", "+operandTwo+"\n");
}

//==============================================================================

void ComputerEngine::compileOneArgumentFunction(ComputerEquation *pOperand,
                                                const QString &pFunctionName,
                                                ComputerEngineData &pData,
                                                void *pFunction)
{
    // Compile the operand

    QString operand = compileOperand(pOperand, pData);

    // Apply the one-argument function

    pData.appendAssemblyCode(Indent+"%%"+QString::number(pData.nextAssemblyCodeIndex())+" = tail call double @"+pFunctionName+"(double "+operand+") nounwind\n");

    // Keep track of the need for the one-argument function

    pData.addExternalFunction(pFunctionName, 1, pFunction);
}

//==============================================================================

void ComputerEngine::compileTwoArgumentFunction(ComputerEquation *pOperandOne,
                                                ComputerEquation *pOperandTwo,
                                                const QString &pFunctionName,
                                                ComputerEngineData &pData,
                                                void *pFunction)
{
    // Compile the two operands

    QString operandOne = compileOperand(pOperandOne, pData);
    QString operandTwo = compileOperand(pOperandTwo, pData);

    // Apply the two-argument function

    pData.appendAssemblyCode(Indent+"%%"+QString::number(pData.nextAssemblyCodeIndex())+" = tail call double @"+pFunctionName+"(double "+operandOne+", double "+operandTwo+") nounwind\n");

    // Keep track of the need for the two-argument function

    pData.addExternalFunction(pFunctionName, 2, pFunction);
}

//==============================================================================

void ComputerEngine::compileEquationNode(ComputerEquation *pEquationNode,
                                         ComputerEngineData &pData)
{
    static const double zeroNumber = 0;
    static ComputerEquation zeroNumberEquation = ComputerEquation(zeroNumber);

    // Make sure that the node is valid

    if (!pEquationNode)
        // It isn't, so...

        return;

    // Check whether the current node is a direct parameter or a number

    if (   (pEquationNode->type() == ComputerEquation::DirectParameter)
        || (pEquationNode->type() == ComputerEquation::IndirectParameter)
        || (pEquationNode->type() == ComputerEquation::Number))
        // It is either a direct parameter or a number, so nothing to compile
        // (since this will be done as part of the compilation of another node),
        // so...

        return;

    // Compile the left node

    compileEquationNode(pEquationNode->left(), pData);

    // Compile the right node

    compileEquationNode(pEquationNode->right(), pData);

    // Compilation of the current node

    switch (pEquationNode->type()) {
    // Mathematical operators

    case ComputerEquation::Times:
        compileMathematicalOperator(pEquationNode->left(), pEquationNode->right(), "fmul", pData);

        break;
    case ComputerEquation::Divide:
        compileMathematicalOperator(pEquationNode->left(), pEquationNode->right(), "fdiv", pData);

        break;
    case ComputerEquation::Plus:
        if (pEquationNode->right())
            // Normal addition, so...

            compileMathematicalOperator(pEquationNode->left(), pEquationNode->right(), "fadd", pData);
        else
            // Unary "+", so...
            // Note: we can only come here if the equation wasn't first
            //       simplified, hence we need to test for the unary "+"...

            compileOperand(pEquationNode->left(), pData);

        break;
    case ComputerEquation::Minus:
        if (pEquationNode->right())
            // Normal subtraction, so...

            compileMathematicalOperator(pEquationNode->left(), pEquationNode->right(), "fsub", pData);
        else
            // Unary "-", so...

            compileMathematicalOperator(&zeroNumberEquation, pEquationNode->left(), "fsub", pData);

        break;

    // Mathematical functions with 1 argument

    case ComputerEquation::Abs:
        compileOneArgumentFunction(pEquationNode->left(), "fabs", pData);

        break;
    case ComputerEquation::Exp:
        compileOneArgumentFunction(pEquationNode->left(), "exp", pData);

        break;
    case ComputerEquation::Log:
        compileOneArgumentFunction(pEquationNode->left(), "log", pData);

        break;
    case ComputerEquation::Ceil:
        compileOneArgumentFunction(pEquationNode->left(), "ceil", pData);

        break;
    case ComputerEquation::Floor:
        compileOneArgumentFunction(pEquationNode->left(), "floor", pData);

        break;
    case ComputerEquation::Factorial:
        compileOneArgumentFunction(pEquationNode->left(), "factorial", pData,
                                   (void *)(intptr_t) factorial);

        break;
    case ComputerEquation::Sin:
        compileOneArgumentFunction(pEquationNode->left(), "sin", pData);

        break;
    case ComputerEquation::Cos:
        compileOneArgumentFunction(pEquationNode->left(), "cos", pData);

        break;
    case ComputerEquation::Tan:
        compileOneArgumentFunction(pEquationNode->left(), "tan", pData);

        break;
    case ComputerEquation::SinH:
        compileOneArgumentFunction(pEquationNode->left(), "sinh", pData);

        break;
    case ComputerEquation::CosH:
        compileOneArgumentFunction(pEquationNode->left(), "cosh", pData);

        break;
    case ComputerEquation::TanH:
        compileOneArgumentFunction(pEquationNode->left(), "tanh", pData);

        break;
    case ComputerEquation::ASin:
        compileOneArgumentFunction(pEquationNode->left(), "asin", pData);

        break;
    case ComputerEquation::ACos:
        compileOneArgumentFunction(pEquationNode->left(), "acos", pData);

        break;
    case ComputerEquation::ATan:
        compileOneArgumentFunction(pEquationNode->left(), "atan", pData);

        break;
    case ComputerEquation::ASinH:
        compileOneArgumentFunction(pEquationNode->left(), "asinh", pData,
                                   (void *)(intptr_t) asinh);

        break;
    case ComputerEquation::ACosH:
        compileOneArgumentFunction(pEquationNode->left(), "acosh", pData,
                                   (void *)(intptr_t) acosh);

        break;
    case ComputerEquation::ATanH:
        compileOneArgumentFunction(pEquationNode->left(), "atanh", pData,
                                   (void *)(intptr_t) atanh);

        break;

    // Mathematical functions with 2 arguments

    case ComputerEquation::ArbitraryLog:
        compileTwoArgumentFunction(pEquationNode->left(), pEquationNode->right(), "arbitraryLog", pData,
                                   (void *)(intptr_t) arbitraryLog);

        break;
    case ComputerEquation::FactorOf:
        compileTwoArgumentFunction(pEquationNode->left(), pEquationNode->right(), "factorOf", pData,
                                   (void *)(intptr_t) factorOf);

        break;
    case ComputerEquation::Pow:
        compileTwoArgumentFunction(pEquationNode->left(), pEquationNode->right(), "pow", pData);

        break;
    case ComputerEquation::Quotient:
        compileTwoArgumentFunction(pEquationNode->left(), pEquationNode->right(), "quotient", pData,
                                   (void *)(intptr_t) quotient);

        break;
    case ComputerEquation::Rem:
        compileTwoArgumentFunction(pEquationNode->left(), pEquationNode->right(), "rem", pData,
                                   (void *)(intptr_t) rem);

        break;
    case ComputerEquation::XOr:
        compileTwoArgumentFunction(pEquationNode->left(), pEquationNode->right(), "xOr", pData,
                                   (void *)(intptr_t) xOr);

        break;

    // We should never reach this point...

    default:
        qDebug(">>> ERROR: untreated equation type '%s'...", qPrintable(pEquationNode->typeAsString()));
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
