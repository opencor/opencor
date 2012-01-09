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
    mParameterNames(QStringList())
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

QStringList CompilerEngineFunction::parameterNames() const
{
    // Return the function's parameter names

    return mParameterNames;
}

//==============================================================================

bool CompilerEngineFunction::addParameterName(const QString &pParameterName)
{
    // Add a parameter name to our list, but only if it isn't already there

    if (mParameterNames.contains(pParameterName)) {
        // The parameter name already exists, so...

        return false;
    } else {
        // The parameter is not yet in our list, so add it

        mParameterNames.append(pParameterName);

        return true;
    }
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
                              const QString &pExpected)
{
    mIssues.append(CompilerEngineIssue(tr("%1 is expected, but '%2' was found instead").arg(pExpected, pToken.string()),
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
    //   DoubleFunction = "double" Type Identifier "(" [ Parameters ] ")" "{" [ Equations ] Return "}" ;
    //   Indentifier    = ( Letter | "_" ) { Letter | Digit | "_" } ;
    //   Letter         = "a" | ... | "z" | "A" | ... "Z" ;
    //   Digit          = "0" | ... | "9" ;
    //   Parameters     = Parameter { "," Parameter } ;
    //   Parameter      = "double" "*" Identifier ;
    //   Equations      = Equation { Equation } ;
    //   Equation       = EquationLHS "=" EquationRHS ;
    //   EquationLHS    = Identifier "[" DigitSequence "]" ;
    //   DigitSequence  = Digit | ( DigitSequence Digit ) ;
    //   EquationRHS    = ...
    //   Return         = "return" ReturnValue ";" ;
    //   ReturnValue    = DoubleValue | EquationRHS ;
    //   DoubleValue    =   ( [Sign] FractionalPart [ ExponentPart ] )
    //                    | ( [Sign] DigitSequence ExponentPart ) ;
    //   Sign           = "+" | "-" ;
    //   FractionalPart =   ( [ DigitSequence ] "." DigitSequence )
    //                    | ( DigitSequence "." ) ;
    //   ExponentPart   =   ( "e" [ Sign ] DigitSequence )
    //                    | ( "E" [ Sign ] DigitSequence ) ;

    // Retrieve the type of function that we are dealing with, i.e. a void or a
    // double function

    CompilerScannerToken token = pScanner.getNextToken();

    if (token.symbol() == CompilerScannerToken::Void) {
        // We are dealing with a void function

        pFunction.setType(CompilerEngineFunction::Void);
    } else if (token.symbol() == CompilerScannerToken::Double) {
        // We are dealing with a double function

        pFunction.setType(CompilerEngineFunction::Double);
    } else {
        // We are neither dealing with a void or a double function, so...

        addIssue(token, tr("either 'void' or 'double'"));

        return false;
    }

    // Retrieve the name of the function

    token = pScanner.getNextToken();

    if (token.symbol() == CompilerScannerToken::Identifier) {
        // We got an identifier, so set the name of the function

        pFunction.setName(token.string());
    } else {
        // We got something else, so...

        addIssue(token, tr("an identifier"));

        return false;
    }

    // The next token must be an opening bracket

    token = pScanner.getNextToken();

    if (token.symbol() != CompilerScannerToken::OpeningBracket) {
        addIssue(token, tr("'('"));

        return false;
    }

    // Parse the different parameters

    if (!parseParameters(pScanner, pFunction))
        // Something went wrong with the parsing of the different parameters,
        // so...

        return false;

    // The next token must be a closing bracket

    token = pScanner.getCurrentToken();
    // Note: we get the current token (as opposed to the next one) since, coming
    //       back from parseParameters, we are already dealing with the 'next'
    //       token, so...

    if (token.symbol() != CompilerScannerToken::ClosingBracket) {
        addIssue(token, tr("')'"));

        return false;
    }

    // The next token must be an opening curly bracket

    token = pScanner.getNextToken();

    if (token.symbol() != CompilerScannerToken::OpeningCurlyBracket) {
        addIssue(token, tr("'{'"));

        return false;
    }

    // Parse the different equations
    // Note: in the case of a void function, we need 1+ equation while 0+ in the
    //       case of a double function...

    if (!parseEquations(pScanner, pFunction))
        // Something went wrong with the parsing of the different equations,
        // so...

        return false;

    // Parse the return statement, but only in the case of a double function

    if (pFunction.type() == CompilerEngineFunction::Double) {
        if (!parseReturn(pScanner, pFunction))
            // Something went wrong with the parsing of the return statement,
            // so...

            return false;
    }

    // The next token must be a closing curly bracket

    token = pScanner.getCurrentToken();
    // Note: no matter whether we are dealing with a void or double function, we
    //       back from some parsing that we have already got the 'next' token,
    //       so...

    if (token.symbol() != CompilerScannerToken::ClosingCurlyBracket) {
        addIssue(token, tr("'}'"));

        return false;
    }









    qDebug("---------------------------------------");
    qDebug("Function details:");

    if (pFunction.type() == CompilerEngineFunction::Void)
        qDebug("   Type: void");
    else
        qDebug("   Type: double");

    qDebug(QString("   Name: %1").arg(pFunction.name()).toLatin1().constData());









    // Scan anything that remains
    //---GRY--- THE BELOW CODE SHOULD DISAPPEAR ONCE OUR PARSER IS FULLY
    //          IMPLEMENTED...

    do {
        token = pScanner.getNextToken();

        qDebug("---------------------------------------");
        qDebug("Token:");
        qDebug(QString("   Line: %1").arg(QString::number(token.line())).toLatin1().constData());
        qDebug(QString("   Column: %1").arg(QString::number(token.column())).toLatin1().constData());
        qDebug(QString("   Symbol: %1 [%2]").arg(token.symbolAsString(), QString::number(token.symbol())).toLatin1().constData());
        qDebug(QString("   String: %1").arg(token.string()).toLatin1().constData());
    } while (token.symbol() != CompilerScannerToken::Eof);









    // Everything went fine, so...

    return true;
}

//==============================================================================

bool CompilerEngine::parseParameters(CompilerScanner &pScanner,
                                     CompilerEngineFunction &pFunction)
{
    //---GRY--- TO BE DONE...

    CompilerScannerToken token = pScanner.getNextToken();

    return true;
}

//==============================================================================

bool CompilerEngine::parseEquations(CompilerScanner &pScanner,
                                    CompilerEngineFunction &pFunction)
{
    //---GRY--- TO BE DONE...

    CompilerScannerToken token = pScanner.getNextToken();

    return true;
}

//==============================================================================

bool CompilerEngine::parseReturn(CompilerScanner &pScanner,
                                 CompilerEngineFunction &pFunction)
{
    //---GRY--- TO BE DONE...

    CompilerScannerToken token = pScanner.getCurrentToken();
    // Note: we come here after having parsed some equations, hence we have
    //       already got the 'next' token, so...

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
