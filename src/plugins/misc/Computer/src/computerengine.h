//==============================================================================
// Computer engine class
//==============================================================================

#ifndef COMPUTERENGINE_H
#define COMPUTERENGINE_H

//==============================================================================

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

class COMPUTER_EXPORT ComputerEngineIssue
{
public:
    explicit ComputerEngineIssue(const QString &pMessage,
                                 const int &pLine = 0, const int &pColumn = 0,
                                 const QString &pExtraInformation = QString());

    QString message() const;
    QString formattedMessage() const;
    int line() const;
    int column() const;
    QString extraInformation() const;

private:
    QString mMessage;
    int mLine;
    int mColumn;
    QString mExtraInformation;
};

//==============================================================================

class ComputerEngineFunction
{
public:
    enum Type {
        Void,
        Double
    };

    explicit ComputerEngineFunction();

    llvm::Function * irCode() const;
    void setIrCode(llvm::Function *pIrCode);

    Type type() const;
    void setType(const Type &pType);

    QString name() const;
    void setName(const QString &pName);

    QStringList parameters() const;
    bool addParameter(const QString &pParameter);

    QString returnValue() const;
    void setReturnValue(const QString &pReturnValue);

private:
    llvm::Function * mIrCode;

    Type mType;
    QString mName;
    QStringList mParameters;
    QString mReturnValue;
};

//==============================================================================

typedef int (*VoidFunction)();
typedef double (*DoubleFunction)();

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

    VoidFunction voidFunctionStub(const QString &pFunctionName,
                                  const std::vector<llvm::GenericValue> &pArguments);
    DoubleFunction doubleFunctionStub(const QString &pFunctionName,
                                      const std::vector<llvm::GenericValue> &pArguments);

    llvm::Function * addFunction(const QString &pFunction);

private:
    llvm::Module *mModule;
    llvm::ExecutionEngine *mExecutionEngine;

    QList<ComputerEngineIssue> mIssues;

    void addIssue(const ComputerScannerToken &pToken, const QString &pMessage,
                  const bool &pExpectedMessage = true,
                  const QString &pExtraInformation = QString());

    bool parseFunction(ComputerScanner &pScanner,
                       ComputerEngineFunction &pFunction);
    bool parseParameter(ComputerScanner &pScanner,
                        ComputerEngineFunction &pFunction,
                        const bool &pNeeded = true);
    bool parseParameters(ComputerScanner &pScanner,
                         ComputerEngineFunction &pFunction);
    bool parseEquations(ComputerScanner &pScanner,
                        ComputerEngineFunction &pFunction);
    bool parseEquationRhs(ComputerScanner &pScanner,
                          ComputerEngineFunction &pFunction);
    bool parseReturn(ComputerScanner &pScanner,
                     ComputerEngineFunction &pFunction);

    bool compileFunction(ComputerEngineFunction &pFunction);

    void * functionStub(const QString &pFunctionName,
                        const std::vector<llvm::GenericValue> &pArguments);
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
