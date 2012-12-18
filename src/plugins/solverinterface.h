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
#include <QVariant>

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
    PropertyType type;
    QString name;
    QVariant defaultValue;
    bool hasVoiUnit;

    explicit Property(const PropertyType &pType, const QString &pName,
                      const QVariant &pDefaultValue,
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
    virtual Solver::Type type() const = 0;
    virtual QString name() const = 0;
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
