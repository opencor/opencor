//==============================================================================
// ForwardEulerSolver plugin
//==============================================================================

#ifndef FORWARDEULERSOLVERPLUGIN_H
#define FORWARDEULERSOLVERPLUGIN_H

//==============================================================================

#include "i18ninterface.h"
#include "solverinterface.h"

//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace ForwardEulerSolver {

//==============================================================================

PLUGININFO_FUNC ForwardEulerSolverPluginInfo();

//==============================================================================

class ForwardEulerSolverPlugin : public QObject, public I18nInterface,
                                 public SolverInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::SolverInterface)

public:
    virtual QString name() const;
    virtual SolverInterfaceType type() const;

    virtual void * newSolver() const;
};

//==============================================================================

}   // namespace ForwardEulerSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
