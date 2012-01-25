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

llvm::Function * ComputerEngine::compileFunction(ComputerFunction *pFunction)
{
    // Generate some LLVM assembly code based on the contents of the function

    static QString indent = QString("  ");
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

            parameters += "*";

        parameters += " %%"+parameter.name();
    }

    assemblyCode += "("+parameters+")";

    // Additional information for the function definition

    assemblyCode += " {\n";

    // Mathematical statements

    foreach (ComputerEquation *equation, pFunction->equations())
        compileEquation(equation, assemblyCode);

    // Return statement

    if (pFunction->type() == ComputerFunction::Void) {
        // We are dealing with a void function, so...

        assemblyCode += indent+"ret void\n";
    } else {
        // We are dealing with a double function, so check whether the return
        // equation is, in fact, a number or a 'proper' equation. If it is
        // 'only' a number, then return it as such, otherwise compile the return
        // equation as if it was the RHS of an equation and return its result

        if (pFunction->returnEquation()->type() == ComputerEquation::Number) {
            // We are dealing with a number as a retun equation, so...

            double returnValue = pFunction->returnEquation()->number();
            QString returnValueAsString =  QString::number(returnValue)
                                          +QString((returnValue == qRound(returnValue))?".0":"");
            // Note: if the return value is an integer, then we must append ".0"
            //       otherwise LLVM won't be able to parse the assembly code,
            //       so...

            assemblyCode += indent+"ret double "+returnValueAsString+"\n";
        } else {
            // We are dealing with a 'proper' return equation, so compile it as
            // if it was the RHS of an equation

            int returnEquationIndex = compileRhsEquation(pFunction->returnEquation(), assemblyCode);

            // Return the result of the return equation

            assemblyCode += indent+"ret double %%"+QString::number(returnEquationIndex)+"\n";
        }
    }

    // End the function

    assemblyCode += "}";

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

void ComputerEngine::compileEquation(ComputerEquation *pEquation,
                                     QString &pAssemblyCode)
{
//---GRY--- TO BE DONE...

    // An equation must consist of an assignment, so just make sure that it is
    // the case

    if (pEquation->type() != ComputerEquation::Assign)
        // Not an assignment, so...

        return;

    // Make a copy of the equation and simplify its RHS, if possible

    ComputerEquation *equation = new ComputerEquation(pEquation);

    equation->right()->simplify();

    delete equation;
}

//==============================================================================

int ComputerEngine::compileRhsEquation(ComputerEquation *pRhsEquation,
                                        QString &pAssemblyCode)
{
//---GRY--- TO BE DONE...

    return 0;
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
