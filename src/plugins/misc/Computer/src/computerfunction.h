//==============================================================================
// Computer function class
//==============================================================================

#ifndef COMPUTERFUNCTION_H
#define COMPUTERFUNCTION_H

//==============================================================================

#include "computerequation.h"
#include "computerparameter.h"

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

    ComputerParameters parameters() const;
    bool addParameter(const ComputerParameter &pParameter);

    ComputerEquations equations() const;
    void addEquation(ComputerEquation *pEquation);

    ComputerEquation * returnEquation();
    void setReturnEquation(ComputerEquation *pReturnEquation);

private:
    Type mType;
    QString mName;
    ComputerParameters mParameters;
    ComputerEquations mEquations;
    ComputerEquation * mReturnEquation;
};

//==============================================================================

}   // namespace Computer
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
