//==============================================================================
// Compiler engine class
//==============================================================================

#ifndef COMPUTERENGINE_H
#define COMPUTERENGINE_H

//==============================================================================

#include "compilerglobal.h"

//==============================================================================

#include <QMap>
#include <QStringList>

//==============================================================================

namespace llvm {
    class ExecutionEngine;
    class Function;
    class Module;
}   // namespace llvm

//==============================================================================

namespace OpenCOR {
namespace Compiler {

//==============================================================================

class COMPUTER_EXPORT CompilerEngine : public QObject
{
    Q_OBJECT

public:
    explicit CompilerEngine();
    ~CompilerEngine();

    QString error() const;
    bool hasError() const;

    bool compileCode(const QString &pCode, const bool &pOutputErrors = false);

    void * getFunction(const QString &pFunctionName);

private:
    llvm::Module *mModule;
    llvm::ExecutionEngine *mExecutionEngine;

    QString mError;

    void reset();
};

//==============================================================================

}   // namespace Compiler
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
