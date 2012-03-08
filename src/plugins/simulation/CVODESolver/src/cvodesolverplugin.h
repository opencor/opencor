//==============================================================================
// CVODESolver plugin
//==============================================================================

#ifndef CVODESOLVERPLUGIN_H
#define CVODESOLVERPLUGIN_H

//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace CVODESolver {

//==============================================================================

PLUGININFO_FUNC CVODESolverPluginInfo();

//==============================================================================

class CVODESolverPlugin : public QObject, public SolverInterface
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

}   // namespace CVODESolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
