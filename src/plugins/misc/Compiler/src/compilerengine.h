//==============================================================================
// Compiler engine class
//==============================================================================

#ifndef COMPILERENGINE_H
#define COMPILERENGINE_H

//==============================================================================

#include "compilerglobal.h"
#include "compilerscanner.h"

//==============================================================================

#include "llvm/Function.h"

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

class COMPILER_EXPORT CompilerEngineIssue
{
public:
    explicit CompilerEngineIssue(const QString &pMessage,
                                 const int &pLine = 0, const int &pColumn = 0);

    QString message() const;
    QString formattedMessage() const;
    int line() const;
    int column() const;

private:
    QString mMessage;
    int mLine;
    int mColumn;
};

//==============================================================================

class COMPILER_EXPORT CompilerEngine : public QObject
{
    Q_OBJECT

public:
    explicit CompilerEngine();
    ~CompilerEngine();

    llvm::Module * module();

    QList<CompilerEngineIssue> issues();

    llvm::Function * addFunction(const QString &pFunction);

private:
    llvm::Module *mModule;

    QList<CompilerEngineIssue> mIssues;

    void addIssue(const CompilerScannerToken &pToken, const QString &pExpected);
};

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
