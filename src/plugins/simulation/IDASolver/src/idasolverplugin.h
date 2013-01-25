//==============================================================================
// IDASolver plugin
//==============================================================================

#ifndef IDASOLVERPLUGIN_H
#define IDASOLVERPLUGIN_H

//==============================================================================

#include "i18ninterface.h"
#include "plugininfo.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {
namespace IDASolver {

//==============================================================================

PLUGININFO_FUNC IDASolverPluginInfo();

//==============================================================================

class IDASolverPlugin : public QObject, public SolverInterface,
                        public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.IDASolverPlugin" FILE "idasolverplugin.json")

    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::SolverInterface)

public:
    virtual Solver::Type type() const;
    virtual QString name() const;
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
