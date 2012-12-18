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

class Property
{
public:
    explicit Property(const PropertyType &pType, const QString &pName,
                      const QVariant &pDefaultValue,
                      const bool &pHasVoiUnit = false);

    PropertyType type() const;
    QString name() const;
    QVariant defaultValue() const;
    bool hasVoiUnit() const;

private:
    PropertyType mType;
    QString mName;
    QVariant mDefaultValue;
    bool mHasVoiUnit;
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
