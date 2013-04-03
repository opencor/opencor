//==============================================================================
// FourthOrderRungeKuttaSolver plugin
//==============================================================================

#ifndef FOURTHORDERRUNGEKUTTASOLVERPLUGIN_H
#define FOURTHORDERRUNGEKUTTASOLVERPLUGIN_H

//==============================================================================

#include "i18ninterface.h"
#include "plugininfo.h"
#include "solverinterface.h"

//==============================================================================

namespace OpenCOR {
namespace FourthOrderRungeKuttaSolver {

//==============================================================================

PLUGININFO_FUNC FourthOrderRungeKuttaSolverPluginInfo();

//==============================================================================

class FourthOrderRungeKuttaSolverPlugin : public QObject,
                                          public SolverInterface,
                                          public I18nInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "OpenCOR.FourthOrderRungeKuttaSolverPlugin" FILE "fourthorderrungekuttasolverplugin.json")

    Q_INTERFACES(OpenCOR::I18nInterface)
    Q_INTERFACES(OpenCOR::SolverInterface)

public:
    virtual Solver::Type type() const;
    virtual QString name() const;
    virtual Solver::Properties properties() const;

    virtual void * instance() const;
};

//==============================================================================

}   // namespace FourthOrderRungeKuttaSolver
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
