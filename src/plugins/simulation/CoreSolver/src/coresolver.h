//==============================================================================
// Core solver class
//==============================================================================

#ifndef CORESOLVER_H
#define CORESOLVER_H

//==============================================================================

#include "coresolverglobal.h"

//==============================================================================

#include <QMap>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace CoreSolver {

//==============================================================================

typedef QMap<QString, QVariant> Properties;

//==============================================================================

class CORESOLVER_EXPORT CoreSolver
{
public:
    explicit CoreSolver();

    void setProperty(const QString &pName, const QVariant &pValue);

protected:
    Properties mProperties;

    virtual bool isValidProperty(const QString &pName) const = 0;
};

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
