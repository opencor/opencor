//==============================================================================
// SecondOrderRungeKuttaSolver plugin
//==============================================================================

#ifndef SECONDORDERRUNGEKUTTASOLVERPLUGIN_H
#define SECONDORDERRUNGEKUTTASOLVERPLUGIN_H

//==============================================================================

#include "i18ninterface.h"
#include "plugininfo.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {
namespace SecondOrderRungeKuttaSolver {

//==============================================================================

PLUGININFO_FUNC SecondOrderRungeKuttaSolverPluginInfo();

//==============================================================================

class SecondOrderRungeKuttaSolverPlugin : public QObject,
                                          public SolverInterface,
                                          public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.SecondOrderRungeKuttaSolverPlugin" FILE "secondorderrungekuttasolverplugin.json")

    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::SolverInterface)

public:
    virtual Solver::Type type() const;
    virtual QString name() const;
    virtual Solver::Properties properties() const;

    virtual void * instance() const;
};

//==============================================================================

}   // namespace SecondOrderRungeKuttaSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
