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

    // Get the first token which is going to tell us whether we are dealing with
    // a procedure or a 'proper' function

    CompilerScannerToken token = scanner.getToken();
    bool procedure = true;

    if (token.symbol() == CompilerScannerToken::Double) {
        // We are dealing with a 'proper' function

        procedure = false;
    } else if (token.symbol() != CompilerScannerToken::Void) {
        // The current symbol is neither Void nor Double, something is wrong

        addIssue(token, tr("either 'void' or 'double'"));

        return 0;
    }



    // Scan the whole function

    int tokenNb = 0;

    do {
        token = scanner.getToken();

        qDebug("---------------------------------------");
        qDebug(QString("Token #%1:").arg(QString::number(++tokenNb)).toLatin1().constData());
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
