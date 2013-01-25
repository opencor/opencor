//==============================================================================
// CVODESolver plugin
//==============================================================================

#ifndef CVODESOLVERPLUGIN_H
#define CVODESOLVERPLUGIN_H

//==============================================================================

#include "i18ninterface.h"
#include "plugininfo.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {
namespace CVODESolver {

//==============================================================================

PLUGININFO_FUNC CVODESolverPluginInfo();

//==============================================================================

class CVODESolverPlugin : public QObject, public SolverInterface,
                          public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.CVODESolverPlugin" FILE "cvodesolverplugin.json")

    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::SolverInterface)

public:
    virtual Solver::Type type() const;
    virtual QString name() const;
    virtual Solver::Properties properties() const;

    virtual void * instance() const;
};

//==============================================================================

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
