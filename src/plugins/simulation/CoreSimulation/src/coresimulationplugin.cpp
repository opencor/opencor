#include "coresimulationplugin.h"

namespace OpenCOR {
namespace CoreSimulation {

PLUGININFO_FUNC CoreSimulationPluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "The core simulation plugin for OpenCOR");
    descriptions.insert("fr", "L'extension de simulation de base pour OpenCOR");

    return PluginInfo(PluginInterface::V001, PluginInfo::Gui,
                      PluginInfo::Simulation, false,
                      QStringList(),
                      descriptions);
}

Q_EXPORT_PLUGIN2(CoreSimulation, CoreSimulationPlugin)

} }
