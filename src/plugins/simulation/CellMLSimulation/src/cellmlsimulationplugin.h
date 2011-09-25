#ifndef CELLMLSIMULATIONPLUGIN_H
#define CELLMLSIMULATIONPLUGIN_H

#include "guiinterface.h"
#include "plugininfo.h"

namespace OpenCOR {
namespace CellMLSimulation {

PLUGININFO_FUNC CellMLSimulationPluginInfo();

class CellMLSimulationPlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::CoreInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit CellMLSimulationPlugin();

protected:
    virtual void retranslateUi();
};

} }

#endif
