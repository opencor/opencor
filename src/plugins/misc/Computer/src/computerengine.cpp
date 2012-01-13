//==============================================================================
// Computer engine class
//==============================================================================
// The computer engine includes a reduced ANSI-C parser/scanner, e.g. see
//     http://www.lysator.liu.se/c/ANSI-C-grammar-y.html
// and http://www.lysator.liu.se/c/ANSI-C-grammar-l.html
//==============================================================================

#include "computerengine.h"

//==============================================================================

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Assembly/Parser.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"

//==============================================================================

#include <QDebug>

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

ComputerEngineIssue::ComputerEngineIssue(const QString &pMessage,
                                         const int &pLine, const int &pColumn,
                                         const QString &pExtraInformation) :
    mMessage(pMessage),
    mLine(pLine),
    mColumn(pColumn),
    mExtraInformation(pExtraInformation)
{
}

//==============================================================================

QString ComputerEngineIssue::message() const
{
    // Return the issue's message

    return mMessage;
}

//==============================================================================

QString ComputerEngineIssue::formattedMessage() const
{
    // Return the issue's message fully formatted (i.e. the beginning of the
    // message is capitalised and its end consists of a full stop)

    return mMessage.left(1).toUpper()+mMessage.right(mMessage.size()-1)+".";
}

//==============================================================================

int ComputerEngineIssue::line() const
{
    // Return the issue's line

    return mLine;
}

//==============================================================================

int ComputerEngineIssue::column() const
{
    // Return the issue's column

    return mColumn;
}

//==============================================================================

QString ComputerEngineIssue::extraInformation() const
{
    // Return the issue's extra information

    return mExtraInformation;
}

//==============================================================================

ComputerEngineFunction::ComputerEngineFunction() :
    mIrCode(0),
    mType(Void),
    mName(QString()),
    mEquations(ComputerEngineEquations()),
    mParameters(QStringList())
{
}

//==============================================================================

ComputerEngineFunction::~ComputerEngineFunction()
{
    // Delete the equations

    foreach (ComputerEngineEquation *equation, mEquations)
        delete equation;
}

//==============================================================================

llvm::Function * ComputerEngineFunction::irCode() const
{
    // Return the function's IR code

    return mIrCode;
}

//==============================================================================

void ComputerEngineFunction::setIrCode(llvm::Function *pIrCode)
{
    // Set the function's IR code

    mIrCode = pIrCode;
}

//==============================================================================

ComputerEngineFunction::Type ComputerEngineFunction::type() const
{
    // Return the function's type

    return mType;
}

//==============================================================================

void ComputerEngineFunction::setType(const Type &pType)
{
    // Set the function's type

    mType = pType;
}

//==============================================================================

QString ComputerEngineFunction::name() const
{
    // Return the function's name

    return mName;
}

//==============================================================================

void ComputerEngineFunction::setName(const QString &pName)
{
    // Set the function's name

    mName = pName;
}

//==============================================================================

QStringList ComputerEngineFunction::parameters() const
{
    // Return the function's parameters

    return mParameters;
}

//==============================================================================

bool ComputerEngineFunction::addParameter(const QString &pParameter)
{
    // Add a parameter to our list, but only if it isn't already there

    if (mParameters.contains(pParameter)) {
        // The parameter already exists, so...

        return false;
    } else {
        // The parameter is not yet in our list, so add it

        mParameters.append(pParameter);

        return true;
    }
}

//==============================================================================

ComputerEngineEquations ComputerEngineFunction::equations() const
{
    // Return the function's equations

    return mEquations;
}

//==============================================================================

void ComputerEngineFunction::addEquation(ComputerEngineEquation *pEquation)
{
    // Add an equation to the function

    mEquations.append(pEquation);
}

//==============================================================================

QString ComputerEngineFunction::returnValue() const
{
    // Return the function's return value

    return mReturnValue;
}

//==============================================================================

void ComputerEngineFunction::setReturnValue(const QString &pReturnValue)
{
    // Set the function's return value

    mReturnValue = pReturnValue;
}

//==============================================================================

ComputerEngine::ComputerEngine()
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

QList<ComputerEngineIssue> ComputerEngine::issues()
{
    // Return the computer's issue(s)

    return mIssues;
}

//==============================================================================

void ComputerEngine::addIssue(const ComputerScannerToken &pToken,
                              const QString &pMessage,
                              const bool &pExpectedMessage,
                              const QString &pExtraInformation)
{
    if (pExpectedMessage)
        mIssues.append(ComputerEngineIssue(tr("%1 is expected, but '%2' was found").arg(pMessage, pToken.string()),
                                           pToken.line(), pToken.column()));
    else
        mIssues.append(ComputerEngineIssue(pMessage,
                                           pToken.line(), pToken.column(),
                                           pExtraInformation));
}

//==============================================================================

llvm::Function * ComputerEngine::addFunction(const QString &pFunction)
{
    qDebug("---------------------------------------");
    qDebug("Compilation of...");
    qDebug();
    qDebug(pFunction.toLatin1().constData());

    // Reset any issues that we may have found before

    mIssues.clear();

    // Get a scanner for our function

    ComputerScanner scanner(pFunction);
    ComputerEngineFunction function;

    // Parse the function

    if (parseFunction(scanner, function)) {
        // Output the function's details

        qDebug("---------------------------------------");
        qDebug("Function details:");

        if (function.type() == ComputerEngineFunction::Void)
            qDebug("   Type: void");
        else
            qDebug("   Type: double");

        qDebug(QString("   Name: %1").arg(function.name()).toLatin1().constData());

        qDebug(QString("   Nb of params: %1").arg(QString::number(function.parameters().count())).toLatin1().constData());

        if (!function.parameters().isEmpty())
            foreach (const QString &parameter, function.parameters())
                qDebug(QString("    - %1").arg(parameter).toLatin1().constData());

        if (function.type() == ComputerEngineFunction::Double)
            qDebug(QString("   Return value: %1").arg(function.returnValue()).toLatin1().constData());

        // The function was properly parsed, so check that we don't already
        //  have a function with the same name in our module

        if (mModule->getFunction(function.name().toLatin1().constData())) {
            // A function with the same name already exists, so...

            addIssue(ComputerScannerToken(),
                     tr("there is already a function called '%1'").arg(function.name()),
                     false);

            return 0;
        }

        // No function with the same already exists, so we can try to compile
        // the function

        if (!compileFunction(function))
            // The function couldn't be compiled, so...

            return 0;

        // Return the function's IR code

        return function.irCode();
    } else {
        // The function wasn't properly parsed, so...

        return 0;
    }
}

//==============================================================================

bool ComputerEngine::parseFunction(ComputerScanner &pScanner,
                                   ComputerEngineFunction &pFunction)
{
    // The EBNF grammar of a function is as follows:
    //
    //   Function       = VoidFunction | DoubleFunction ;
    //   VoidFunction   = "void" Identifier "(" Parameters ")" "{" [ Equations ] "}" ;
    //   DoubleFunction = "double" Identifier "(" [ Parameters ] ")" "{" [ Equations ] Return "}" ;

    // Note that after retrieving/parsing something, we must get ready for the
    // next task and this means getting the next token. Indeed, doing so means
    // that the next task doesn't have to worry about whether the current token
    // is the correct one or not...

    // Retrieve the type of function that we are dealing with, i.e. a void or a
    // double function

    if (pScanner.token().symbol() == ComputerScannerToken::Void) {
        // We are dealing with a void function

        pFunction.setType(ComputerEngineFunction::Void);
    } else if (pScanner.token().symbol() == ComputerScannerToken::Double) {
        // We are dealing with a double function

        pFunction.setType(ComputerEngineFunction::Double);
    } else {
        // We are dealing with neither a void nor a double function, so...

        addIssue(pScanner.token(), tr("either 'void' or 'double'"));

        return false;
    }

    pScanner.getNextToken();

    // Retrieve the name of the function

    if (pScanner.token().symbol() == ComputerScannerToken::Identifier) {
        // We got an identifier, so set the name of the function

        pFunction.setName(pScanner.token().string());
    } else {
        // We got something else, so...

        addIssue(pScanner.token(), tr("an identifier"));

        return false;
    }

    pScanner.getNextToken();

    // The current token must be an opening bracket

    if (pScanner.token().symbol() != ComputerScannerToken::OpeningBracket) {
        addIssue(pScanner.token(), "'('");

        return false;
    }

    pScanner.getNextToken();

    // Parse the different parameters

    if (!parseParameters(pScanner, pFunction))
        // Something went wrong with the parsing of the different parameters,
        // so...

        return false;

    // The current token must be a closing bracket

    if (pScanner.token().symbol() != ComputerScannerToken::ClosingBracket) {
        addIssue(pScanner.token(), "')'");

        return false;
    }

    pScanner.getNextToken();

    // The current token must be an opening curly bracket

    if (pScanner.token().symbol() != ComputerScannerToken::OpeningCurlyBracket) {
        addIssue(pScanner.token(), "'{'");

        return false;
    }

    pScanner.getNextToken();

    // Parse the different equations

    if (!parseEquations(pScanner, pFunction))
        // Something went wrong with the parsing of the different equations,
        // so...

        return false;

    // Parse the return statement, but only in the case of a double function

    if (   (pFunction.type() == ComputerEngineFunction::Double)
        && !parseReturn(pScanner, pFunction))
        // Something went wrong with the parsing of the return statement, so...

        return false;

    // The current token must be a closing curly bracket

    if (pScanner.token().symbol() != ComputerScannerToken::ClosingCurlyBracket) {
        addIssue(pScanner.token(), "'}'");

        return false;
    }

    pScanner.getNextToken();

    // The current token must be EOF

    if (pScanner.token().symbol() != ComputerScannerToken::Eof) {
        addIssue(pScanner.token(), "EOF");

        return false;
    }

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerEngine::parseParameter(ComputerScanner &pScanner,
                                    ComputerEngineFunction &pFunction,
                                    const bool &pNeeded)
{
    // The EBNF grammar of a parameter is as follows:
    //
    //   Parameter  = "double" "*" Identifier ;

    // The current token must be "double"

    if (pScanner.token().symbol() != ComputerScannerToken::Double) {
        if (pNeeded)
            // We need a parameter definition, so...

            addIssue(pScanner.token(), "'double'");

        return false;
    }

    pScanner.getNextToken();

    // The current token must be "*"

    if (pScanner.token().symbol() != ComputerScannerToken::Times) {
        addIssue(pScanner.token(), "'*'");

        return false;
    }

    pScanner.getNextToken();

    // The current token must be an identifier

    if (pScanner.token().symbol() == ComputerScannerToken::Identifier) {
        // We got an identifier, so try to add it as the name of a new parameter

        if (!pFunction.addParameter(pScanner.token().string())) {
            // The parameter already exists, so...

            addIssue(pScanner.token(),
                     tr("there is already a parameter called '%1'").arg(pScanner.token().string()),
                     false);

            return false;
        }
    } else {
        // We got something else, so...

        addIssue(pScanner.token(), tr("an identifier"));

        return false;
    }

    pScanner.getNextToken();

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerEngine::parseParameters(ComputerScanner &pScanner,
                                     ComputerEngineFunction &pFunction)
{
    // The EBNF grammar of a list of parameters is as follows:
    //
    //   Parameters = Parameter { "," Parameter } ;

    // We must have 1+/0+ parameters in the case of a void/double function

    bool needAtLeastOneParameter = pFunction.type() == ComputerEngineFunction::Void;

    if (parseParameter(pScanner, pFunction, needAtLeastOneParameter))
        // The first parameter was properly parsed, so look for other parameters

        // The current token must be "," if we are to have another parameter
        // definition

        while (pScanner.token().symbol() == ComputerScannerToken::Comma) {
            pScanner.getNextToken();

            // We must then have the parameter definition itself

            if (!parseParameter(pScanner, pFunction))
                // Something went wrong with the parsing of the parameter
                // definition, so...

                return false;
        }
    else
        // Something went wrong with the parsing of the parameter definition,
        // but it should only be reported as an error if we expected a parameter

        return !needAtLeastOneParameter;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerEngine::parseEquations(ComputerScanner &pScanner,
                                    ComputerEngineFunction &pFunction)
{
    // The EBNF grammar of a series of equations is as follows:
    //
    //   Equations = { Equation } ;
    //   Equation  = Identifier "[" IntegerValue "]" "=" EquationRHS ";" ;

    while (pScanner.token().symbol() == ComputerScannerToken::Identifier) {
        // The current token is an identifier which is what we would expect for
        // the beginning of an equation, so we get ready for the parsing of an
        // equation

        ComputerEngineEquation *equation = new ComputerEngineEquation();

        pScanner.getNextToken();

        // The current token must be "["

        if (pScanner.token().symbol() != ComputerScannerToken::OpeningSquareBracket) {
            addIssue(pScanner.token(), "'['");

            delete equation;

            return false;
        }

        pScanner.getNextToken();

        // The current token must be an integer valud

        if (pScanner.token().symbol() != ComputerScannerToken::IntegerValue) {
            addIssue(pScanner.token(), tr("an integer"));

            delete equation;

            return false;
        }

        pScanner.getNextToken();

        // The current token must be "]"

        if (pScanner.token().symbol() != ComputerScannerToken::ClosingSquareBracket) {
            addIssue(pScanner.token(), "']'");

            delete equation;

            return false;
        }

        pScanner.getNextToken();

        // The current token must be "="

        if (pScanner.token().symbol() != ComputerScannerToken::Equal) {
            addIssue(pScanner.token(), "'='");

            delete equation;

            return false;
        }

        pScanner.getNextToken();

        // Parse the RHS of the equation

        if (!parseEquationRhs(pScanner, pFunction)) {
            delete equation;

            return false;
        }

        // The current token must be ";"

        if (pScanner.token().symbol() != ComputerScannerToken::SemiColon) {
            addIssue(pScanner.token(), "';'");

            return false;
        }

        pScanner.getNextToken();

        // The equation was successfully parsed, so add it to the list of
        // functions

        pFunction.addEquation(equation);
    }

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerEngine::parseEquationRhs(ComputerScanner &pScanner,
                                      ComputerEngineFunction &pFunction)
{
    // The EBNF grammar of an equation's RHS is as follows:
    //
    //   EquationRHS = ...

//---GRY--- TO BE DONE...

//---GRY--- THE BELOW CODE PARSES A NUMBER AND IS ONLY FOR TESTING PURPOSES...
if (   (pScanner.token().symbol() == ComputerScannerToken::IntegerValue)
    || (pScanner.token().symbol() == ComputerScannerToken::DoubleValue)) {
    pFunction.setReturnValue(pScanner.token().string());
} else {
    addIssue(pScanner.token(), "a number");

    return false;
}

pScanner.getNextToken();

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerEngine::parseReturn(ComputerScanner &pScanner,
                                 ComputerEngineFunction &pFunction)
{
    // The EBNF grammar of a return statement is as follows:
    //
    //   Return = "return" EquationRHS ";" ;

    // The current token must be "return"

    if (pScanner.token().symbol() != ComputerScannerToken::Return) {
        addIssue(pScanner.token(), "'return'");

        return false;
    }

    pScanner.getNextToken();

    // Parse the equivalent of the RHS of an equation

    if (!parseEquationRhs(pScanner, pFunction))
        return false;

    // The current token must be ";"

    if (pScanner.token().symbol() != ComputerScannerToken::SemiColon) {
        addIssue(pScanner.token(), "';'");

        return false;
    }

    pScanner.getNextToken();

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool ComputerEngine::compileFunction(ComputerEngineFunction &pFunction)
{
    // Generate some LLVM assembly code based on the contents of the function

    static QString indent = QString("  ");
    QString assemblyCode = QString();

    // Define the function

    assemblyCode += "define";

    // Type of function

    if (pFunction.type() == ComputerEngineFunction::Void)
        assemblyCode += " void";
    else
        assemblyCode += " double";

    // Name of the function

    assemblyCode += " @"+pFunction.name();

    // Parameters of the function

    QString parameters = QString();

    foreach (const QString &parameter, pFunction.parameters()) {
        // Add a separator first if we already have 1+ parameters

        if (!parameters.isEmpty())
            parameters += ", ";

        // Add the parameter definition

        parameters += "double* %%"+parameter;
    }

    assemblyCode += "("+parameters+")";

    // Additional information for the function definition

    assemblyCode += " {\n";

    // Mathematical statements

//---GRY--- TO BE DONE...

    // Return statement

    if (pFunction.type() == ComputerEngineFunction::Void)
        assemblyCode += indent+"ret void\n";
    else
        assemblyCode += indent+"ret double "+pFunction.returnValue()+"\n";

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

    qDebug(QString("   LLVM assembly:\n%1").arg(assemblyCode).toLatin1().constData());

    QString originalAssemblyCode = QString(assemblyCode);
    // Note: the above is required since we must replace '%' with '\%' prior to
    //       having LLVM parse the assembly code, yet we need to keep a trace of
    //       the original assembly code (in case the parsing goes wrong), so...

    llvm::SMDiagnostic parseError;
    llvm::ParseAssemblyString(assemblyCode.replace("%%", "\%").toLatin1().constData(),
                              mModule, parseError, llvm::getGlobalContext());

    if (parseError.getMessage().size())
        addIssue(ComputerScannerToken(parseError.getLineNo(), parseError.getColumnNo()),
                 tr("the LLVM assembly code could not be parsed: %1").arg(QString::fromStdString(parseError.getMessage()).remove("error: ")),
                 false, originalAssemblyCode);

    // Try to retrieve the function which assembly code we have just parsed

    llvm::Function *function = mModule->getFunction(pFunction.name().toLatin1().constData());

    if (function) {
        // The function could be retrieved, but it should be removed in case an
        // error of sorts occurred during the compilation

        if (!mIssues.isEmpty()) {
            // An error occurred during the compilation of the function, so...

            function->eraseFromParent();

            return false;
        }

        // Set the function's IR code

        pFunction.setIrCode(function);

        // Everything went fine, so...

        return true;
    } else {
        // The function couldn't be retrieved, so add an issue but only if no
        // error occurred during the compilation

        if (mIssues.isEmpty())
            addIssue(ComputerScannerToken(),
                     tr("the function '%1' could not be found").arg(pFunction.name()),
                     false);

        return false;
    }
}

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
