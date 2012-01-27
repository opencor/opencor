//==============================================================================
// Computer engine class
//==============================================================================

#ifndef COMPUTERENGINE_H
#define COMPUTERENGINE_H

//==============================================================================

#include "computerexternalfunction.h"
#include "computerfunction.h"
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
}

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

class ComputerParser;

//==============================================================================

typedef QMap<QString, int> ComputerEngineIndirectParameterAssemblyCodeIndexes;
typedef QMap<ComputerEquation *, int> ComputerEngineEquationAssemblyCodeIndexes;

//==============================================================================

struct ComputerEngineData
{
public:
    explicit ComputerEngineData();

    QString assemblyCode() const;
    void appendAssemblyCode(const QString &pAssemblyCode);

    int assemblyCodeIndex() const;
    int nextAssemblyCodeIndex();

    ComputerExternalFunctions externalFunctions() const;
    bool addExternalFunction(const QString &pExternalFunctionName,
                             const int &pNbOfArguments);

    bool needTbaaInformation() const;
    void setNeedTbaaInformation(const bool &pNeedTbaaInformation);

    ComputerEngineIndirectParameterAssemblyCodeIndexes indirectParameterPointerAssemblyCodeIndexes() const;
    void addIndirectParameterPointerAssemblyCodeIndex(const QString &pKey,
                                                      const int &pValue);

    ComputerEngineIndirectParameterAssemblyCodeIndexes indirectParameterLoadAssemblyCodeIndexes() const;
    void addIndirectParameterLoadAssemblyCodeIndex(const QString &pKey,
                                                   const int &pValue);

    ComputerEngineEquationAssemblyCodeIndexes equationAssemblyCodeIndexes() const;
    void addEquationAssemblyCodeIndex(ComputerEquation *pKey,
                                      const int &pValue);

private:
    QString mAssemblyCode;
    int mAssemblyCodeIndex;

    ComputerExternalFunctions mExternalFunctions;

    bool mNeedTbaaInformation;

    ComputerEngineIndirectParameterAssemblyCodeIndexes mIndirectParameterPointerAssemblyCodeIndexes;
    ComputerEngineIndirectParameterAssemblyCodeIndexes mIndirectParameterLoadAssemblyCodeIndexes;

    ComputerEngineEquationAssemblyCodeIndexes mEquationAssemblyCodeIndexes;
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

    ComputerError error();
    ComputerErrors parserErrors();

    llvm::Function * addFunction(const QString &pFunction);

private:
    llvm::Module *mModule;
    llvm::ExecutionEngine *mExecutionEngine;

    ComputerError mError;

    ComputerParser *mParser;

    ComputerExternalFunctions mExternalFunctions;

    QString numberAsString(const double &pNumber);

    llvm::Function * compileFunction(ComputerFunction *pFunction);

    int indirectParameterAssemblyCodeIndex(ComputerEquation *pIndirectParameter,
                                           ComputerEngineData &pData,
                                           const bool &pOperand);
    QString compileOperand(ComputerEquation *pOperand,
                           ComputerEngineData &pData);
    void compileAssignmentEquation(ComputerEquation *pIndirectParameter,
                                   ComputerEquation *pRhsEquation,
                                   ComputerEngineData &pData);
    void compileMathematicalOperator(ComputerEquation *pOperandOne,
                                     ComputerEquation *pOperandTwo,
                                     const QString &pOperator,
                                     ComputerEngineData &pData);
    void compileOneArgumentFunction(ComputerEquation *pOperand,
                                     const QString &pFunctionName,
                                     ComputerEngineData &pData);
    void compileTwoArgumentFunction(ComputerEquation *pOperandOne,
                                    ComputerEquation *pOperandTwo,
                                    const QString &pFunctionName,
                                    ComputerEngineData &pData);
    void compileEquationNode(ComputerEquation *pEquationNode,
                             ComputerEngineData &pData);
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
