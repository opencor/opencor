//==============================================================================
// ForwardEulerSolver plugin
//==============================================================================

#ifndef FORWARDEULERSOLVERPLUGIN_H
#define FORWARDEULERSOLVERPLUGIN_H

//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace ForwardEulerSolver {

//==============================================================================

PLUGININFO_FUNC ForwardEulerSolverPluginInfo();

//==============================================================================

class ForwardEulerSolverPlugin : public QObject, public SolverInterface
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

}   // namespace ForwardEulerSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
