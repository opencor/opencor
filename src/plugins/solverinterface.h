//==============================================================================
// Solver interface
//==============================================================================

#ifndef SOLVERINTERFACE_H
#define SOLVERINTERFACE_H

//==============================================================================

#include "interface.h"

//==============================================================================

#include <QMap>
#include <QTranslator>

//==============================================================================

namespace OpenCOR {
namespace Solver {

//==============================================================================

enum Type
{
    Dae,
    NonLinearAlgebraic,
    Ode
};

enum PropertyType {
    Double,
    Integer
};

typedef QMap<QString, PropertyType> Properties;

//==============================================================================

}   // namespace Solver

//==============================================================================

class SolverInterface : public Interface
{
public:
    virtual QString name() const = 0;
    virtual Solver::Type type() const = 0;
    virtual Solver::Properties properties() const = 0;

    QString typeAsString() const;

    virtual void * instance() const = 0;
};

//==============================================================================

typedef QList<SolverInterface *> SolverInterfaces;

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::SolverInterface, "OpenCOR.SolverInterface")

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
