//==============================================================================
// Computer function class
//==============================================================================

#ifndef COMPUTERFUNCTION_H
#define COMPUTERFUNCTION_H

//==============================================================================

#include "computerequation.h"
#include "computerexternalfunction.h"

//==============================================================================

#include <QStringList>

//==============================================================================

#include "llvm/Function.h"

//==============================================================================

namespace OpenCOR {
namespace Computer {

//==============================================================================

class ComputerFunction
{
public:
    enum Type {
        Void,
        Double
    };

    explicit ComputerFunction();
    ~ComputerFunction();

    Type type() const;
    void setType(const Type &pType);

    QString name() const;
    void setName(const QString &pName);

    QStringList parameters() const;
    bool addParameter(const QString &pParameter);

    ComputerEquations equations() const;
    void addEquation(ComputerEquation *pEquation);

    QString returnValue() const;
    void setReturnValue(const QString &pReturnValue);

    ComputerExternalFunctions externalFunctions() const;
    void addExternalFunction(const ComputerExternalFunction &pExternalFunction);

private:
    Type mType;
    QString mName;
    QStringList mParameters;
    ComputerEquations mEquations;
    QString mReturnValue;

    ComputerExternalFunctions mExternalFunctions;
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
