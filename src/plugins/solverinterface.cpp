//==============================================================================
// Solver interface
//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace Solver {

//==============================================================================

Property::Property(const PropertyType &pType, const QString &pName,
                   const bool &pHasVoiUnit) :
    type(pType),
    name(pName),
    hasVoiUnit(pHasVoiUnit)
{
}

//==============================================================================

}   // namespace Solver

//==============================================================================

QString SolverInterface::typeAsString() const
{
    // Return the type of the solver as a string

    switch (type()) {
    case Solver::Ode:
        return QObject::tr("ODE");
    case Solver::Dae:
        return QObject::tr("DAE");
    case Solver::Nla:
        return QObject::tr("NLA");
    default:
        return "???";
    }
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
