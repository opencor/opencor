//==============================================================================
// IDASolver plugin
//==============================================================================

#ifndef IDASOLVERPLUGIN_H
#define IDASOLVERPLUGIN_H

//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace IDASolver {

//==============================================================================

PLUGININFO_FUNC IDASolverPluginInfo();

//==============================================================================

class IDASolverPlugin : public QObject, public SolverInterface
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

}   // namespace IDASolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
