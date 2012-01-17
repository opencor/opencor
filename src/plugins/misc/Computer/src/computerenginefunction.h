//==============================================================================
// Computer engine function class
//==============================================================================

#ifndef COMPUTERENGINEFUNCTION_H
#define COMPUTERENGINEFUNCTION_H

//==============================================================================

#include "computerengineequation.h"
#include "computerengineexternalfunction.h"

//==============================================================================

#include <QStringList>

//==============================================================================

#include "llvm/Function.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

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

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
