//==============================================================================
// CellMLSimulation plugin
//==============================================================================

#include "cellmlsimulationplugin.h"

//==============================================================================

#include <QTabBar>

//==============================================================================

namespace OpenCOR {
namespace CellMLSimulation {

//==============================================================================

PLUGININFO_FUNC CellMLSimulationPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to simulate <a href=\"http://www.cellml.org/\">CellML</a> files");
    descriptions.insert("fr", "Une extension pour simuler des fichiers <a href=\"http://www.cellml.org/\">CellML</a>");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Simulation,
                      true,
                      QStringList() << "CoreSimulation" << "CellML",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CellMLSimulation, CellMLSimulationPlugin)

//==============================================================================

}   // namespace CellMLSimulation
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
