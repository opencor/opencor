//==============================================================================
// Computer engine class
//==============================================================================

#ifndef COMPUTERENGINE_H
#define COMPUTERENGINE_H

//==============================================================================

#include "computerengineequation.h"
#include "computerengineexternalfunction.h"
#include "computerenginefunction.h"
#include "computerengineissue.h"
#include "computerglobal.h"
#include "computerscanner.h"

//==============================================================================

#include <QStringList>

//==============================================================================

#include "llvm/Function.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"

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

    QList<ComputerEngineIssue> issues();

    llvm::Function * addFunction(const QString &pFunction);

    void addIssue(const ComputerScannerToken &pToken, const QString &pMessage,
                  const bool &pExpectedMessage = true,
                  const QString &pExtraInformation = QString());

    bool parseEquationRhs(ComputerScanner &pScanner,
                          ComputerEngineFunction &pFunction);

private:
    llvm::Module *mModule;
    llvm::ExecutionEngine *mExecutionEngine;

    QList<ComputerEngineIssue> mIssues;

    ComputerEngineExternalFunctions mExternalFunctions;

    bool parseFunction(ComputerScanner &pScanner,
                       ComputerEngineFunction &pFunction);
    bool parseFunctionParameters(ComputerScanner &pScanner,
                                 ComputerEngineFunction &pFunction);
    bool parseFunctionParameter(ComputerScanner &pScanner,
                                ComputerEngineFunction &pFunction,
                        const bool &pNeeded = true);
    bool parseEquations(ComputerScanner &pScanner,
                        ComputerEngineFunction &pFunction);
    bool parseReturn(ComputerScanner &pScanner,
                     ComputerEngineFunction &pFunction);

    bool compileFunction(ComputerEngineFunction &pFunction);
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
