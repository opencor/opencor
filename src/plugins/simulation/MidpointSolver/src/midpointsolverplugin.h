//==============================================================================
// MidpointSolver plugin
//==============================================================================

#ifndef MIDPOINTSOLVERPLUGIN_H
#define MIDPOINTSOLVERPLUGIN_H

//==============================================================================

#include "i18ninterface.h"
#include "plugininfo.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {
namespace MidpointSolver {

//==============================================================================

PLUGININFO_FUNC MidpointSolverPluginInfo();

//==============================================================================

class MidpointSolverPlugin : public QObject, public SolverInterface,
                             public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.MidpointSolverPlugin" FILE "midpointsolverplugin.json")

    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::SolverInterface)

public:
    virtual Solver::Type type() const;
    virtual QString name() const;
    virtual Solver::Properties properties() const;

    virtual void * instance() const;
};

//==============================================================================

}   // namespace MidpointSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
