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

    QMap<QString, int> mIndirectParameterAssemblyCodeIndexes;
    QMap<ComputerEquation *, int> mEquationAssemblyCodeIndexes;

    QString numberAsString(const double &pNumber);

    llvm::Function * compileFunction(ComputerFunction *pFunction);

    int indirectParameterAssemblyCodeIndex(ComputerEquation *pIndirectParameter,
                                           QString &pAssemblyCode,
                                           int &pAssemblyCodeIndex,
                                           const bool &pOperand);
    QString compileOperand(ComputerEquation *pOperand,
                           QString &pAssemblyCode, int &pAssemblyCodeIndex);
    void compileAssignmentEquation(ComputerEquation *pIndirectParameter,
                                   ComputerEquation *pRhsEquation,
                                   QString &pAssemblyCode, int &pAssemblyCodeIndex);
    void compileEquation(ComputerEquation *pEquation,
                         QString &pAssemblyCode, int &pAssemblyCodeIndex);
    int compileRhsEquation(ComputerEquation *pRhsEquation,
                           QString &pAssemblyCode, int &pAssemblyCodeIndex);
    void compileMathematicalOperator(const QString &pOperator,
                                     ComputerEquation *pOperandOne,
                                     ComputerEquation *pOperandTwo,
                                     QString &pAssemblyCode,
                                     int &pAssemblyCodeIndex);
    void compileEquationNode(ComputerEquation *pEquationNode,
                             QString &pAssemblyCode, int &pAssemblyCodeIndex);
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
