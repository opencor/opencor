//==============================================================================
// KINSOLSolver plugin
//==============================================================================

#ifndef KINSOLSOLVERPLUGIN_H
#define KINSOLSOLVERPLUGIN_H

//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace KINSOLSolver {

//==============================================================================

PLUGININFO_FUNC KINSOLSolverPluginInfo();

//==============================================================================

class KINSOLSolverPlugin : public QObject, public SolverInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::SolverInterface)

public:
    virtual QString name() const;
    virtual Solver::Type type() const;
    virtual Solver::Properties properties() const;

    virtual void * instance() const;
};

//==============================================================================

}   // namespace KINSOLSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
