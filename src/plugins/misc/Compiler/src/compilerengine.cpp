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

    // The EBNF grammar of a function is as follows:
    //
    //   Function       = VoidFunction | DoubleFunction ;
    //   VoidFunction   = "void" Identifier "(" Parameters ")" "{" Equations "}" ;
    //   DoubleFunction = "double" Type Identifier "(" Parameters ")" "{" [ Equations ] DoubleReturn "}" ;
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
    //   DoubleReturn   = "return" ReturnValue ";" ;
    //   ReturnValue    = DoubleValue | EquationRHS ;
    //   DoubleValue    =   ( FractionalPart [ ExponentPart ] )
    //                    | ( DigitSequence ExponentPart ) ;
    //   FractionalPart =   ( [ DigitSequence ] "." DigitSequence )
    //                    | ( DigitSequence "." ) ;
    //   ExponentPart   =   ( "e" [ Sign ] DigitSequence )
    //                    | ( "E" [ Sign ] DigitSequence ) ;

    // Retrieve the type of function that we are dealing with, i.e. a void or a
    // double function

    bool voidFunction = true;

    CompilerScannerToken token = scanner.getToken();

    if (token.symbol() == CompilerScannerToken::Double) {
        // We are dealing with a 'proper' function

        voidFunction = false;
    } else if (token.symbol() != CompilerScannerToken::Void) {
        // The current symbol is neither Void nor Double, so...

        addIssue(token, tr("either 'void' or 'double'"));

        return 0;
    }

    // Retrieve the name of the function

    QString functionName;

    token = scanner.getToken();

    if (token.symbol() == CompilerScannerToken::Identifier) {
        // We got an identifier

        functionName = token.string();
    } else {
        addIssue(token, tr("an identifier"));

        return 0;
    }

    // An opening bracket is now required

    token = scanner.getToken();

    if (token.symbol() != CompilerScannerToken::OpeningBracket) {
        addIssue(token, tr("'('"));

        return 0;
    }

    // Retrieve the different parameters (i.e. at least one array of doubles)

//---GRY--- TO BE DONE...

    // A closing bracket is now required

    token = scanner.getToken();

    if (token.symbol() != CompilerScannerToken::ClosingBracket) {
        addIssue(token, tr("')'"));

        return 0;
    }

    // An opening curly bracket is now required

    token = scanner.getToken();

    if (token.symbol() != CompilerScannerToken::OpeningCurlyBracket) {
        addIssue(token, tr("'{'"));

        return 0;
    }

    // Parse the different equations
    // Note: in the case of a void function, we need 1+ equation while 0+ in the
    //       case of a double function...

//---GRY--- TO BE DONE...

    // Parse the return statement, but only in the case of a double function

    if (!voidFunction) {
//---GRY--- TO BE DONE...
    }

    // A closing curly bracket is now required

    token = scanner.getToken();

    if (token.symbol() != CompilerScannerToken::ClosingCurlyBracket) {
        addIssue(token, tr("'}'"));

        return 0;
    }









    qDebug("---------------------------------------");
    qDebug("Function details:");

    if (voidFunction)
        qDebug("   Type: void");
    else
        qDebug("   Type: double");

    qDebug(QString("   Name: %1").arg(functionName).toLatin1().constData());









    // Scan anything that remains
    // Note: this should disappear once our parser is fully implemented...

    do {
        token = scanner.getToken();

        qDebug("---------------------------------------");
        qDebug("Token:");
        qDebug(QString("   Line: %1").arg(QString::number(token.line())).toLatin1().constData());
        qDebug(QString("   Column: %1").arg(QString::number(token.column())).toLatin1().constData());
        qDebug(QString("   Symbol: %1 [%2]").arg(token.symbolAsString(), QString::number(token.symbol())).toLatin1().constData());
        qDebug(QString("   String: %1").arg(token.string()).toLatin1().constData());
    } while (token.symbol() != CompilerScannerToken::Eof);

    // Output the contents of the module so far

    qDebug("---------------------------------------");
    qDebug("All generated code so far:");
    mModule->dump();
    qDebug("---------------------------------------");

    return 0;
}

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
