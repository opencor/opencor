//==============================================================================
// CellMLSimulation plugin
//==============================================================================

#ifndef CELLMLSIMULATIONPLUGIN_H
#define CELLMLSIMULATIONPLUGIN_H

//==============================================================================

#include "coreinterface.h"
#include "guiinterface.h"
#include "plugininfo.h"

//==============================================================================

class QwtPlot;

//==============================================================================

namespace OpenCOR {
namespace CellMLSimulation {

//==============================================================================

PLUGININFO_FUNC CellMLSimulationPluginInfo();

class CellMLSimulationPlugin : public QObject, public CoreInterface,
                               public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit CellMLSimulationPlugin();

    virtual void initialize();

    virtual QWidget * viewWidget(const QString & pFileName, const int &);
    virtual QString viewName(const int &pViewIndex);

private:
    QwtPlot *mSimulationView;
};

//==============================================================================

}   // namespace CellMLSimulation
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
