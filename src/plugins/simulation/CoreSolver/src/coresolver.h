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

class CORESOLVER_EXPORT CoreSolver : public QObject
{
    Q_OBJECT

public:
    explicit CoreSolver();

    void setProperties(const Properties &pProperties);

    void emitError(const QString &pErrorMsg);

protected:
    Properties mProperties;

Q_SIGNALS:
    void error(const QString &pErrorMsg);
};

//==============================================================================

}   // namespace CoreSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
