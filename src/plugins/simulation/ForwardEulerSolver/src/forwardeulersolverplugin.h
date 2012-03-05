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

class ForwardEulerSolverPlugin : public QObject, public SolverInterface,
                                 public I18nInterface
{
    Q_OBJECT

public:
    virtual QString solverName() const;
};

//==============================================================================

}   // namespace ForwardEulerSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
