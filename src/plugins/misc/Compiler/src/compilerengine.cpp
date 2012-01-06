//==============================================================================
// Compiler engine class
//==============================================================================
// The compiler engine consists of a reduced ANSI-C parser/scanner, e.g. see
//     http://www.lysator.liu.se/c/ANSI-C-grammar-y.html
// and http://www.lysator.liu.se/c/ANSI-C-grammar-l.html
//==============================================================================

#include "compilerengine.h"
#include "compilerscanner.h"

//==============================================================================

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"

//==============================================================================

#include <QDebug>

//==============================================================================

namespace OpenCOR {
namespace Compiler {

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

llvm::Function * CompilerEngine::addFunction(const QString &pFunction)
{
    qDebug("---------------------------------------");
    qDebug("Compilation of...");
    qDebug();
    qDebug(pFunction.toLatin1().constData());

    // Get a scanner for our source code

    CompilerScanner scanner(pFunction);

    // Scan the whole function

    CompilerScannerToken token;
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
