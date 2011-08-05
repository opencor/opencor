#ifndef CELLMLSIMULATIONPLUGIN_H
#define CELLMLSIMULATIONPLUGIN_H

#include "guiinterface.h"

namespace OpenCOR {
namespace CellMLSimulation {

PLUGININFO_FUNC CellMLSimulationPluginInfo();

class CellMLSimulationPlugin : public GuiInterface
{
    Q_OBJECT
    Q_INTERFACES(OpenCOR::PluginInterface)
    Q_INTERFACES(OpenCOR::GuiInterface)

public:
    explicit CellMLSimulationPlugin();

    virtual Mode requiredMode() const;
};

} }

#endif
