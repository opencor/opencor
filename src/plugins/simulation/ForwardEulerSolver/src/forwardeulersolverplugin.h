//==============================================================================
// ForwardEulerSolver plugin
//==============================================================================

#ifndef FORWARDEULERSOLVERPLUGIN_H
#define FORWARDEULERSOLVERPLUGIN_H

//==============================================================================

#include "i18ninterface.h"
#include "plugininfo.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {
namespace ForwardEulerSolver {

//==============================================================================

PLUGININFO_FUNC ForwardEulerSolverPluginInfo();

//==============================================================================

class ForwardEulerSolverPlugin : public QObject, public SolverInterface,
                                 public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.ForwardEulerSolverPlugin" FILE "forwardeulersolverplugin.json")

    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::SolverInterface)

public:
    virtual Solver::Type type() const;
    virtual QString name() const;
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
