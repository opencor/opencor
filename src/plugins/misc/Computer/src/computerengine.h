//==============================================================================
// Computer engine class
//==============================================================================

#ifndef COMPUTERENGINE_H
#define COMPUTERENGINE_H

//==============================================================================

#include "computerglobal.h"
#include "computererror.h"

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
namespace Computer {

//==============================================================================

class COMPUTER_EXPORT ComputerEngine : public QObject
{
    Q_OBJECT

public:
    explicit ComputerEngine();
    ~ComputerEngine();

    llvm::Module * module();
    llvm::ExecutionEngine * executionEngine();

    ComputerError error();

    llvm::Function * addFunction(const QString &pFunctionName,
                                 const QString &pFunctionBody,
                                 const bool &pOutputErrors = false);

private:
    llvm::Module *mModule;
    llvm::ExecutionEngine *mExecutionEngine;

    ComputerError mError;
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
