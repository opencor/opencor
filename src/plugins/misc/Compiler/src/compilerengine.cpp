//==============================================================================
// Compiler engine class
//==============================================================================
// The compiler engine consists of a reduced ANSI-C parser/scanner, e.g. see
//     http://www.lysator.liu.se/c/ANSI-C-grammar-y.html
// and http://www.lysator.liu.se/c/ANSI-C-grammar-l.html
//==============================================================================

#include "compilerengine.h"

//==============================================================================

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"

//==============================================================================

#include <QDebug>

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

CompilerEngineIssue::CompilerEngineIssue(const QString &pMessage,
                                         const int &pLine, const int &pColumn) :
    mMessage(pMessage),
    mLine(pLine),
    mColumn(pColumn)
{
}

//==============================================================================

QString CompilerEngineIssue::message() const
{
    // Return the issue's message

    return mMessage;
}

//==============================================================================

QString CompilerEngineIssue::formattedMessage() const
{
    // Return the issue's message fully formatted (i.e. the beginning of the
    // message is capitalised and its end consists of a full stop)

    return mMessage.left(1).toUpper()+mMessage.right(mMessage.size()-1)+".";
}

//==============================================================================

int CompilerEngineIssue::line() const
{
    // Return the issue's line

    return mLine;
}

//==============================================================================

int CompilerEngineIssue::column() const
{
    // Return the issue's column

    return mColumn;
}

//==============================================================================

CompilerEngineFunction::CompilerEngineFunction() :
    mJitCode(0),
    mType(Void),
    mName(QString()),
    mParameters(QStringList())
{
}

//==============================================================================

llvm::Function * CompilerEngineFunction::jitCode() const
{
    // Return the function's JIT code

    return mJitCode;
}

//==============================================================================

CompilerEngineFunction::Type CompilerEngineFunction::type() const
{
    // Return the function's type

    return mType;
}

//==============================================================================

void CompilerEngineFunction::setType(const Type &pType)
{
    // Set the function's type

    mType = pType;
}

//==============================================================================

QString CompilerEngineFunction::name() const
{
    // Return the function's name

    return mName;
}

//==============================================================================

void CompilerEngineFunction::setName(const QString &pName)
{
    // Set the function's name

    mName = pName;
}

//==============================================================================

QStringList CompilerEngineFunction::parameters() const
{
    // Return the function's parameters

    return mParameters;
}

//==============================================================================

bool CompilerEngineFunction::addParameter(const QString &pParameter)
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

double CompilerEngineFunction::returnValue() const
{
    // Return the function's return value

    return mReturnValue;
}

//==============================================================================

void CompilerEngineFunction::setReturnValue(const double &pReturnValue)
{
    // Set the function's return value

    mReturnValue = pReturnValue;
}

//==============================================================================

CompilerEngine::CompilerEngine()
{
    static int counter = 0;

    mModule = new llvm::Module(QString("Module #%1").arg(QString::number(++counter)).toLatin1().constData(),
                               llvm::getGlobalContext());
}

//==============================================================================

CompilerEngine::~CompilerEngine()
{
    // Delete some internal objects

    delete mModule;
}

//==============================================================================

llvm::Module * CompilerEngine::module()
{
    // Return the compiler engine's module

    return mModule;
}

//==============================================================================

QList<CompilerEngineIssue> CompilerEngine::issues()
{
    // Return the compiler's issue(s)

    return mIssues;
}

//==============================================================================

void CompilerEngine::addIssue(const CompilerScannerToken &pToken,
                              const QString &pMessage,
                              const bool &pExpectedMessage)
{
    if (pExpectedMessage)
        mIssues.append(CompilerEngineIssue(tr("%1 is expected, but '%2' was found instead").arg(pMessage, pToken.string()),
                                           pToken.line(), pToken.column()));
    else
        mIssues.append(CompilerEngineIssue(pMessage,
                                           pToken.line(), pToken.column()));
}

//==============================================================================

llvm::Function * CompilerEngine::addFunction(const QString &pFunction)
{
    qDebug("---------------------------------------");
    qDebug("Compilation of...");
    qDebug();
    qDebug(pFunction.toLatin1().constData());

    // Reset any issues that we may have found before

    mIssues.clear();

    // Get a scanner for our function

    CompilerScanner scanner(pFunction);
    CompilerEngineFunction function;

    // Parse the function

    if (parseFunction(scanner, function)) {
        // The function was properly parsed, so we can compile it

        compileFunction(function);

        // Output the contents of the module so far

        qDebug("---------------------------------------");
        qDebug("All generated code so far:");
        mModule->dump();
        qDebug("---------------------------------------");

        // Return the compiled function

        return function.jitCode();
    } else {
        // The function wasn't properly parsed, so...

        return 0;
    }
}

//==============================================================================

bool CompilerEngine::parseFunction(CompilerScanner &pScanner,
                                   CompilerEngineFunction &pFunction)
{
    // The EBNF grammar of a function is as follows:
    //
    //   Function       = VoidFunction | DoubleFunction ;
    //   VoidFunction   = "void" Identifier "(" Parameters ")" "{" Equations "}" ;
    //   DoubleFunction = "double" Identifier "(" [ Parameters ] ")" "{" [ Equations ] Return "}" ;

    // Note that after retrieving/parsing something, we must get ready for the
    // next task and this means getting the next token. Indeed, doing so means
    // that the next task doesn't have to worry about whether the current token
    // is the correct one or not...

    // Retrieve the type of function that we are dealing with, i.e. a void or a
    // double function

    if (pScanner.token().symbol() == CompilerScannerToken::Void) {
        // We are dealing with a void function

        pFunction.setType(CompilerEngineFunction::Void);
    } else if (pScanner.token().symbol() == CompilerScannerToken::Double) {
        // We are dealing with a double function

        pFunction.setType(CompilerEngineFunction::Double);
    } else {
        // We are dealing with neither a void nor a double function, so...

        addIssue(pScanner.token(), tr("either 'void' or 'double'"));

        return false;
    }

    pScanner.getNextToken();

    // Retrieve the name of the function

    if (pScanner.token().symbol() == CompilerScannerToken::Identifier) {
        // We got an identifier, so set the name of the function

        pFunction.setName(pScanner.token().string());
    } else {
        // We got something else, so...

        addIssue(pScanner.token(), tr("an identifier"));

        return false;
    }

    pScanner.getNextToken();

    // The current token must be an opening bracket

    if (pScanner.token().symbol() != CompilerScannerToken::OpeningBracket) {
        addIssue(pScanner.token(), tr("'('"));

        return false;
    }

    pScanner.getNextToken();

    // Parse the different parameters

    if (!parseParameters(pScanner, pFunction))
        // Something went wrong with the parsing of the different parameters,
        // so...

        return false;

    // The current token must be a closing bracket

    if (pScanner.token().symbol() != CompilerScannerToken::ClosingBracket) {
        addIssue(pScanner.token(), tr("')'"));

        return false;
    }

    pScanner.getNextToken();

    // The current token must be an opening curly bracket

    if (pScanner.token().symbol() != CompilerScannerToken::OpeningCurlyBracket) {
        addIssue(pScanner.token(), tr("'{'"));

        return false;
    }

    pScanner.getNextToken();

    // Parse the different equations

    if (!parseEquations(pScanner, pFunction))
        // Something went wrong with the parsing of the different equations,
        // so...

        return false;

    // Parse the return statement, but only in the case of a double function

    if (   (pFunction.type() == CompilerEngineFunction::Double)
        && !parseReturn(pScanner, pFunction))
        // Something went wrong with the parsing of the return statement, so...

        return false;

    // The current token must be a closing curly bracket

    if (pScanner.token().symbol() != CompilerScannerToken::ClosingCurlyBracket) {
        addIssue(pScanner.token(), tr("'}'"));

        return false;
    }

    pScanner.getNextToken();









    // Scan anything that remains
    //---GRY--- THE BELOW CODE SHOULD DISAPPEAR ONCE OUR PARSER IS FULLY
    //          IMPLEMENTED...

    while (pScanner.token().symbol() != CompilerScannerToken::Eof) {
        qDebug("---------------------------------------");
        qDebug("Token:");
        qDebug(QString("   Line: %1").arg(QString::number(pScanner.token().line())).toLatin1().constData());
        qDebug(QString("   Column: %1").arg(QString::number(pScanner.token().column())).toLatin1().constData());
        qDebug(QString("   Symbol: %1 [%2]").arg(pScanner.token().symbolAsString(), QString::number(pScanner.token().symbol())).toLatin1().constData());
        qDebug(QString("   String: %1").arg(pScanner.token().string()).toLatin1().constData());

        pScanner.getNextToken();
    }









    qDebug("---------------------------------------");
    qDebug("Function details:");

    if (pFunction.type() == CompilerEngineFunction::Void)
        qDebug("   Type: void");
    else
        qDebug("   Type: double");

    qDebug(QString("   Name: %1").arg(pFunction.name()).toLatin1().constData());

    qDebug(QString("   Nb of params: %1").arg(QString::number(pFunction.parameters().count())).toLatin1().constData());

    if (!pFunction.parameters().isEmpty())
        foreach (const QString &parameter, pFunction.parameters())
            qDebug(QString("    - %1").arg(parameter).toLatin1().constData());

    if (pFunction.type() == CompilerEngineFunction::Double)
        qDebug(QString("   Return value: %1").arg(QString::number(pFunction.returnValue())).toLatin1().constData());









    // Everything went fine, so...

    return true;
}

//==============================================================================

bool CompilerEngine::parseParameter(CompilerScanner &pScanner,
                                    CompilerEngineFunction &pFunction,
                                    const bool &pNeeded)
{
    // The EBNF grammar of a parameter is as follows:
    //
    //   Parameter  = "double" "*" Identifier ;

    // The current token must be "double"

    if (pScanner.token().symbol() != CompilerScannerToken::Double) {
        if (pNeeded)
            // We need a parameter definition, so...

            addIssue(pScanner.token(), tr("'double'"));

        return false;
    }

    pScanner.getNextToken();

    // The current token must be "*"

    if (pScanner.token().symbol() != CompilerScannerToken::Times) {
        addIssue(pScanner.token(), tr("'*'"));

        return false;
    }

    pScanner.getNextToken();

    // The current token must be an identifier

    if (pScanner.token().symbol() == CompilerScannerToken::Identifier) {
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

bool CompilerEngine::parseParameters(CompilerScanner &pScanner,
                                     CompilerEngineFunction &pFunction)
{
    // The EBNF grammar of a list of parameters is as follows:
    //
    //   Parameters = Parameter { "," Parameter } ;

    // We must have 1+/0+ parameters in the case of a void/double function

    if (parseParameter(pScanner, pFunction,
                       pFunction.type() == CompilerEngineFunction::Void))
        // The first parameter was properly parsed, so look for other parameters

        // The current token must be "," if we are to have another parameter
        // definition

        while (pScanner.token().symbol() == CompilerScannerToken::Comma) {
            pScanner.getNextToken();

            // We must then have the parameter definition itself

            if (!parseParameter(pScanner, pFunction))
                // Something went wrong with the parsing of the parameter
                // definition, so...

                return false;
        }
    else
        // Something went wrong with the parsing of the parameter definition,
        // so...

        return false;

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool CompilerEngine::parseEquations(CompilerScanner &pScanner,
                                    CompilerEngineFunction &pFunction)
{
    // The EBNF grammar of a series of equations is as follows:
    //
    //   Equations     = Equation { Equation } ;
    //   Equation      = Identifier "[" DigitSequence "]" "=" EquationRHS ;
    //   DigitSequence = Digit | ( DigitSequence Digit ) ;

    //---GRY--- TO BE DONE...

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool CompilerEngine::parseEquationRhs(CompilerScanner &pScanner,
                                      CompilerEngineFunction &pFunction)
{
    // The EBNF grammar of an equation's RHS is as follows:
    //
    //   EquationRHS = ...

    //---GRY--- TO BE DONE...

    // Everything went fine, so...

    return true;
}

//==============================================================================

bool CompilerEngine::parseReturn(CompilerScanner &pScanner,
                                 CompilerEngineFunction &pFunction)
{
    // The EBNF grammar of a return statement is as follows:
    //
    //   Return = "return" EquationRHS ";" ;

    // The current token must be "return"

    if (pScanner.token().symbol() != CompilerScannerToken::Return) {
        addIssue(pScanner.token(), tr("'return'"));

        return false;
    }

    pScanner.getNextToken();

    // Parse the equivalent of the RHS of an equation

//---GRY---    if (!parseEquationRhs(pScanner, pFunction))
//---GRY---        return false;

//---GRY--- THE BELOW IS JUST FOR TESTING PURPOSES...
if (   (pScanner.token().symbol() == CompilerScannerToken::IntegerValue)
    || (pScanner.token().symbol() == CompilerScannerToken::DoubleValue)) {
    pFunction.setReturnValue(pScanner.token().string().toDouble());
} else {
    addIssue(pScanner.token(), "a number");

    return false;
}

pScanner.getNextToken();

    // The current token must be ";"

    if (pScanner.token().symbol() != CompilerScannerToken::SemiColon) {
        addIssue(pScanner.token(), tr("';'"));

        return false;
    }

    pScanner.getNextToken();

    // Everything went fine, so...

    return true;
}

//==============================================================================

void CompilerEngine::compileFunction(CompilerEngineFunction &pFunction)
{
    //---GRY--- TO BE DONE...
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
