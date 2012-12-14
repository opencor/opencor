//==============================================================================
// Solver interface
//==============================================================================

#ifndef SOLVERINTERFACE_H
#define SOLVERINTERFACE_H

//==============================================================================

#include "interface.h"

//==============================================================================

#include <QList>
#include <QTranslator>

//==============================================================================

namespace OpenCOR {
namespace Solver {

//==============================================================================

enum Type {
    Dae,
    Nla,
    Ode
};

//==============================================================================

enum PropertyType {
    Integer,
    Double
};

//==============================================================================

struct Property
{
    QString name;
    PropertyType type;
    bool hasVoiUnit;

    explicit Property(const QString &pName, const PropertyType &pType,
                      const bool &pHasVoiUnit = false);
};

//==============================================================================

typedef QList<Property> Properties;

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

Q_DECLARE_INTERFACE(OpenCOR::SolverInterface, "OpenCOR::SolverInterface")

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
