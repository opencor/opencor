//==============================================================================
// Computer engine class
//==============================================================================

#include "computerengine.h"
#include "computerparser.h"

//==============================================================================

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Assembly/Parser.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerEngine::ComputerEngine() :
    mError(QString()),
    mExternalFunctions(ComputerExternalFunctions())
{
    // Create a module

    static int counter = 0;

    mModule = new llvm::Module(QString("Module #%1").arg(QString::number(++counter)).toLatin1().constData(),
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
    qDebug(pFunction.toLatin1().constData());

    // Reset our lists of assembly code indexes

    mIndirectParameterPointerAssemblyCodeIndexes.clear();
    mIndirectParameterLoadAssemblyCodeIndexes.clear();
    mEquationAssemblyCodeIndexes.clear();

    // Parse the function

    ComputerFunction *function = mParser->parseFunction(pFunction);

    if (function) {
        // The function was properly parsed, so check that we don't already have
        // a function with the same name in our module

        if (mModule->getFunction(function->name().toLatin1().constData())) {
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

    QString assemblyCode = QString();

    // Declare any external function which we need and which are not already
    // declared

    foreach (const ComputerExternalFunction &externalFunction,
             pFunction->externalFunctions())
        if (!mExternalFunctions.contains(externalFunction)) {
            // The function's external function hasn't already been declared, so
            // declare it

            assemblyCode += QString("declare double @%1").arg(externalFunction.name());

            QString parameters = QString();

            for (int i = 0, iMax = externalFunction.nbOfParameters(); i < iMax; ++i) {
                // Add a separator first if we already have 1+ parameters

                if (!parameters.isEmpty())
                    parameters += ", ";

                // Add the parameter definition

                parameters += "double";
            }

            assemblyCode += "("+parameters+") nounwind readnone\n";

            // Add the extenral function to the computer engine's list of
            // external functions

            mExternalFunctions.append(externalFunction);
        }

    if (!assemblyCode.isEmpty())
        assemblyCode += "\n";

    // Define the function

    assemblyCode += "define";

    // Type of function

    if (pFunction->type() == ComputerFunction::Void)
        assemblyCode += " void";
    else
        assemblyCode += " double";

    // Name of the function

    assemblyCode += " @"+pFunction->name();

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

    assemblyCode += "("+parameters+") nounwind uwtable";

    // Additional information for the function definition

    assemblyCode += " {\n";

    // Mathematical statements

    int assemblyCodeIndex = 0;

    foreach (ComputerEquation *equation, pFunction->equations())
        compileEquation(equation, assemblyCode, assemblyCodeIndex);

    // Return statement

    if (pFunction->type() == ComputerFunction::Void) {
        // We are dealing with a void function, so...

        assemblyCode += Indent+"ret void\n";
    } else {
        // We are dealing with a double function, so check whether the return
        // equation is, in fact, a number or a 'proper' equation. If it is
        // 'only' a number, then return it as such, otherwise compile the return
        // equation as if it was the RHS of an equation and return its result

        if (pFunction->returnEquation()->type() == ComputerEquation::Number) {
            // We are dealing with a number as a return equation, so...

            assemblyCode += Indent+"ret double "+numberAsString(pFunction->returnEquation()->number())+"\n";
        } else {
            // We are dealing with a 'proper' return equation, so compile it as
            // if it was the RHS of an equation

            int returnEquationIndex = compileRhsEquation(pFunction->returnEquation(), assemblyCode, assemblyCodeIndex);

            // Return the result of the return equation

            assemblyCode += Indent+"ret double %%"+QString::number(returnEquationIndex)+"\n";
        }
    }

    // End the function

    assemblyCode += "}\n";

    // TBAA information
    // Note: should LLVM deem the TBAA information unnecessary, then it won't
    //       get converted to IR code, so it's fine to add it everytime to our
    //       assembly code, not least because it saves us the trouble of having
    //       to determine when it would have been necessary to add it...

    assemblyCode += "\n";
    assemblyCode += "!0 = metadata !{metadata !\"double\", metadata !1}\n";
    assemblyCode += "!1 = metadata !{metadata !\"omnipotent char\", metadata !2}\n";
    assemblyCode += "!2 = metadata !{metadata !\"Simple C/C++ TBAA\", null}";

    // Now that we are done generating some LLVM assembly code for the function,
    // we can parse that code and have LLVM generate some IR code that will get
    // automatically added to our module

//assemblyCode  = "declare double @sin(double)\n";
//assemblyCode += "\n";
//assemblyCode += "define void @test(double* %%data) {\n";
//assemblyCode += "  %%1 = getelementptr inbounds double* %%data, i64 0\n";
//assemblyCode += "  store double 1.230000e+02, double* %%1, align 8\n";
//assemblyCode += "\n";
//assemblyCode += "  %%2 = getelementptr inbounds double* %%data, i64 2\n";
//assemblyCode += "  store double 1.230000e+02, double* %%2, align 8\n";
//assemblyCode += "\n";
//assemblyCode += "  %%3 = getelementptr inbounds double* %%data, i64 4\n";
//assemblyCode += "  store double 1.230000e+02, double* %%3, align 8\n";
//assemblyCode += "\n";
//assemblyCode += "  %%4 = load double* %%data, align 8\n";
//assemblyCode += "  %%5 = tail call double @sin(double %%4)\n";
//assemblyCode += "  %%6 = getelementptr inbounds double* %%data, i64 1\n";
//assemblyCode += "  store double %%5, double* %%6, align 8\n";
//assemblyCode += "\n";
//assemblyCode += "  %%7 = getelementptr inbounds double* %%data, i64 2\n";
//assemblyCode += "  %%8 = load double* %%7, align 8\n";
//assemblyCode += "  %%9 = tail call double @sin(double %%8) nounwind readnone\n";
//assemblyCode += "  %%10 = getelementptr inbounds double* %%data, i64 3\n";
//assemblyCode += "  store double %%9, double* %%10, align 8\n";
//assemblyCode += "\n";
//assemblyCode += "  %%11 = getelementptr inbounds double* %%data, i64 4\n";
//assemblyCode += "  %%12 = load double* %%11, align 8\n";
//assemblyCode += "  %%13 = tail call double @sin(double %%12) nounwind readnone\n";
//assemblyCode += "  %%14 = getelementptr inbounds double* %%data, i64 5\n";
//assemblyCode += "  store double %%13, double* %%14, align 8\n";
//assemblyCode += "\n";
//assemblyCode += "  ret void\n";
//assemblyCode += "}";

    qDebug("");
    qDebug("LLVM assembly:");
    qDebug("");
    qDebug(QString(assemblyCode).toLatin1().constData());

    QString originalAssemblyCode = QString(assemblyCode);
    // Note: the above is required since we must replace '%' with '\%' prior to
    //       having LLVM parse the assembly code, yet we need to keep a trace of
    //       the original assembly code (in case the parsing goes wrong), so...

    llvm::SMDiagnostic parseError;
    llvm::ParseAssemblyString(assemblyCode.replace("%%", "\%").toLatin1().constData(),
                              mModule, parseError, llvm::getGlobalContext());

    if (parseError.getMessage().size())
        mError = ComputerError(tr("the LLVM assembly code could not be parsed: %1").arg(QString::fromStdString(parseError.getMessage()).remove("error: ")),
                               parseError.getLineNo(), parseError.getColumnNo(),
                               originalAssemblyCode);
        // Note: we must not exit straightaway since LLVM may have generated
        //       some IR code, so we first need to check whether part of the
        //       function has already been generated and, if so, remove it...

    // Try to retrieve the function which assembly code we have just parsed

    llvm::Function *res = mModule->getFunction(pFunction->name().toLatin1().constData());

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
                                                       QString &pAssemblyCode,
                                                       int &pAssemblyCodeIndex,
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

        if (mIndirectParameterPointerAssemblyCodeIndexes.contains(key)) {
            // An assembly code index already exists, so retrieve its value

            pointerAssemblyCodeIndex = mIndirectParameterPointerAssemblyCodeIndexes.value(key);
        } else {
            // No assembly code index for the direct pointer to the indirect
            // parameter exists, so create one

            pAssemblyCode += Indent+"%%"+QString::number(++pAssemblyCodeIndex)+" = getelementptr inbounds double* %%"+pIndirectParameter->parameterName()+", i64 "+QString::number(pIndirectParameter->parameterIndex())+"\n";

            // Keep track of the assembly code index

            mIndirectParameterPointerAssemblyCodeIndexes.insert(key, pAssemblyCodeIndex);

            pointerAssemblyCodeIndex = pAssemblyCodeIndex;
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

        if (mIndirectParameterLoadAssemblyCodeIndexes.contains(key)) {
            // An assembly code index already exists, so return its value

            return mIndirectParameterLoadAssemblyCodeIndexes.value(key);
        } else {
            // No assembly code index for the loading of the indirect  parameter
            // exists, so create one

            pAssemblyCode += Indent+"%%"+QString::number(++pAssemblyCodeIndex)+" = load double* %%";

            if (pointerAssemblyCodeIndex)
                pAssemblyCode += QString::number(pointerAssemblyCodeIndex);
            else
                pAssemblyCode += pIndirectParameter->parameterName();

            pAssemblyCode += ", align 8, !tbaa !0\n";

            // Keep track of the assembly code index

            mIndirectParameterLoadAssemblyCodeIndexes.insert(key, pAssemblyCodeIndex);

            // Return the assembly code index for the loading of the indirect
            // parameter

            return pAssemblyCodeIndex;
        }
    } else {
        // We only want an assembly code index for the direct pointer to the
        // indirect parameter, so...

        return pointerAssemblyCodeIndex;
    }
}

//==============================================================================

QString ComputerEngine::compileOperand(ComputerEquation *pOperand,
                                       QString &pAssemblyCode, int &pAssemblyCodeIndex)
{
    switch (pOperand->type()) {
    case ComputerEquation::DirectParameter:
        // A direct parameter, so...

        return "%%"+pOperand->parameterName();
    case ComputerEquation::IndirectParameter:
        // An indirect parameter, so...

        return "%%"+QString::number(indirectParameterAssemblyCodeIndex(pOperand,
                                                                       pAssemblyCode,
                                                                       pAssemblyCodeIndex,
                                                                       true));

        break;
    case ComputerEquation::Number:
        // A number, so...

        return numberAsString(pOperand->number());
    default:
        // Part of a computed equation, so...

        return "%%"+QString::number(mEquationAssemblyCodeIndexes.value(pOperand));
    }
}

//==============================================================================

void ComputerEngine::compileAssignmentEquation(ComputerEquation *pIndirectParameter,
                                               ComputerEquation *pRhsEquation,
                                               QString &pAssemblyCode,
                                               int &pAssemblyCodeIndex)
{
    // Keep track of the RHS equation assembly code index

    int rhsEquationAssemblyCodeIndex = pAssemblyCodeIndex;

    // Retrieve, for the indirect parameter and if necessary, a pointer to the
    // correct entry in the array of doubles

    int assemblyCodeIndex = indirectParameterAssemblyCodeIndex(pIndirectParameter,
                                                               pAssemblyCode,
                                                               pAssemblyCodeIndex,
                                                               false);

    // Store the RHS of the equation...
    // Note: we try optimise this by checking whether the RHS of the equation is
    //       a number...

    pAssemblyCode += Indent+"store double ";

    if (pRhsEquation->type() == ComputerEquation::Number)
        // We are dealing with a number, so...

        pAssemblyCode += numberAsString(pRhsEquation->number());
    else
        // We are not dealing with a number, but a 'proper' RHS equation, so...
//---GRY--- THIS PART NEEDS TO BE TESTED, BUT FOR THIS WE NEED compileRhsEquation TO BE COMPLETED...

        pAssemblyCode += "%%"+QString::number(rhsEquationAssemblyCodeIndex);

    // ... to the LHS of the equation

    pAssemblyCode += ", double* %%";

    if (assemblyCodeIndex)
        // We have an assembly code index for the indirect parameter, so...

        pAssemblyCode += QString::number(assemblyCodeIndex);
    else
        // We don't have an assembly code index for the indirect parameter,
        // so...

        pAssemblyCode += pIndirectParameter->parameterName();

    pAssemblyCode += ", align 8, !tbaa !0\n";
}

//==============================================================================

void ComputerEngine::compileEquation(ComputerEquation *pEquation,
                                     QString &pAssemblyCode,
                                     int &pAssemblyCodeIndex)
{
    // An equation must consist of an assignment, so just make sure that it is
    // the case

    if (pEquation->type() != ComputerEquation::Assign)
        // Not an assignment, so...

        return;

    // Make a copy of the equation and simplify its RHS, if possible

    ComputerEquation *equation = new ComputerEquation(pEquation);

    equation->right()->simplify();

    // Compile the RHS of the equation

    compileRhsEquation(equation->right(), pAssemblyCode, pAssemblyCodeIndex);

    // Assign the result of the RHS of the equation to the indirect parameter
    // which information can be found the LHS of the equation

    compileAssignmentEquation(equation->left(), equation->right(),
                              pAssemblyCode, pAssemblyCodeIndex);

    // We are now done with the copy of the equation, so...

    delete equation;
}

//==============================================================================

int ComputerEngine::compileRhsEquation(ComputerEquation *pRhsEquation,
                                        QString &pAssemblyCode,
                                       int &pAssemblyCodeIndex)
{
    // Compile the RHS of the equation starting from its top node

    compileEquationNode(pRhsEquation, pAssemblyCode, pAssemblyCodeIndex);

    // We are done, so return the latest assembly code index

    return pAssemblyCodeIndex;
}

//==============================================================================

void ComputerEngine::compileMathematicalOperator(const QString &pOperator,
                                                 ComputerEquation *pOperandOne,
                                                 ComputerEquation *pOperandTwo,
                                                 QString &pAssemblyCode,
                                                 int &pAssemblyCodeIndex)
{
    QString operandOne = compileOperand(pOperandOne,
                                        pAssemblyCode, pAssemblyCodeIndex);
    QString operandTwo = compileOperand(pOperandTwo,
                                        pAssemblyCode, pAssemblyCodeIndex);

    pAssemblyCode += Indent+"%%"+QString::number(++pAssemblyCodeIndex)+" = "+pOperator+" double "+operandOne+", "+operandTwo+"\n";
}

//==============================================================================

void ComputerEngine::compileEquationNode(ComputerEquation *pEquationNode,
                                         QString &pAssemblyCode,
                                         int &pAssemblyCodeIndex)
{
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

    compileEquationNode(pEquationNode->left(), pAssemblyCode, pAssemblyCodeIndex);

    // Compile the right node

    compileEquationNode(pEquationNode->right(), pAssemblyCode, pAssemblyCodeIndex);

    // Compilation of the current node

    switch (pEquationNode->type()) {
    case ComputerEquation::Times:
        // Compilation of a multiplication

        compileMathematicalOperator("fmul",
                                    pEquationNode->left(),
                                    pEquationNode->right(),
                                    pAssemblyCode, pAssemblyCodeIndex);

        break;
    case ComputerEquation::Divide:
        // Compilation of a division

        compileMathematicalOperator("fdiv",
                                    pEquationNode->left(),
                                    pEquationNode->right(),
                                    pAssemblyCode, pAssemblyCodeIndex);

        break;
    case ComputerEquation::Plus:
        // Compilation of an addition

        compileMathematicalOperator("fadd",
                                    pEquationNode->left(),
                                    pEquationNode->right(),
                                    pAssemblyCode, pAssemblyCodeIndex);

        break;
    case ComputerEquation::Minus:
        // Compilation of a subtraction

        compileMathematicalOperator("fsub",
                                    pEquationNode->left(),
                                    pEquationNode->right(),
                                    pAssemblyCode, pAssemblyCodeIndex);

        break;
    }

    // Keep track of the assembly code index of the current node

    mEquationAssemblyCodeIndexes.insert(pEquationNode, pAssemblyCodeIndex);
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
