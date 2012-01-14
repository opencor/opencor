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

class ComputerEngineEquation
{
};

//==============================================================================

typedef QList<ComputerEngineEquation *> ComputerEngineEquations;

//==============================================================================

class ComputerEngineExternalFunction
{
public:
    explicit ComputerEngineExternalFunction(const QString &pName,
                                            const int &pNbOfParameters);

    QString name() const;
    int nbOfParameters() const;

private:
    QString mName;
    int mNbOfParameters;
};

//==============================================================================

class ComputerEngineExternalFunctions : public QList<ComputerEngineExternalFunction>
{
public:
    bool contains(const ComputerEngineExternalFunction &pExternalFunction) const;
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
    ~ComputerEngineFunction();

    llvm::Function * irCode() const;
    void setIrCode(llvm::Function *pIrCode);

    Type type() const;
    void setType(const Type &pType);

    QString name() const;
    void setName(const QString &pName);

    QStringList parameters() const;
    bool addParameter(const QString &pParameter);

    ComputerEngineEquations equations() const;
    void addEquation(ComputerEngineEquation *pEquation);

    QString returnValue() const;
    void setReturnValue(const QString &pReturnValue);

    ComputerEngineExternalFunctions externalFunctions() const;
    void addExternalFunction(const ComputerEngineExternalFunction &pExternalFunction);

private:
    llvm::Function * mIrCode;

    Type mType;
    QString mName;
    QStringList mParameters;
    ComputerEngineEquations mEquations;
    QString mReturnValue;

    ComputerEngineExternalFunctions mExternalFunctions;
};

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

private:
    llvm::Module *mModule;
    llvm::ExecutionEngine *mExecutionEngine;

    QList<ComputerEngineIssue> mIssues;

    ComputerEngineExternalFunctions mExternalFunctions;

    void addIssue(const ComputerScannerToken &pToken, const QString &pMessage,
                  const bool &pExpectedMessage = true,
                  const QString &pExtraInformation = QString());

    bool parseFunction(ComputerScanner &pScanner,
                       ComputerEngineFunction &pFunction);
    bool parseParameters(ComputerScanner &pScanner,
                         ComputerEngineFunction &pFunction);
    bool parseParameter(ComputerScanner &pScanner,
                        ComputerEngineFunction &pFunction,
                        const bool &pNeeded = true);
    bool parseEquations(ComputerScanner &pScanner,
                        ComputerEngineFunction &pFunction);
    bool parseEquationParameter(ComputerScanner &pScanner,
                                QString &pArrayName, int &pArrayIndex);
    bool parseEquationRhs(ComputerScanner &pScanner,
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
