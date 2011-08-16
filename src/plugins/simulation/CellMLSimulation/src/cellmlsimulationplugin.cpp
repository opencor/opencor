#include "cellmlsimulationplugin.h"

#include <QTabBar>

namespace OpenCOR {
namespace CellMLSimulation {

PLUGININFO_FUNC CellMLSimulationPluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "A plugin to simulate <a href=\"http://www.cellml.org/\">CellML</a> files");
    descriptions.insert("fr", "Une extension pour simuler des fichiers <a href=\"http://www.cellml.org/\">CellML</a>");

    return PluginInfo(PluginInterface::V001,
                      PluginInfo::Gui,
                      PluginInfo::Simulation,
                      true,
                      QStringList() << "CoreSimulation" << "CellML",
                      descriptions);
}

Q_EXPORT_PLUGIN2(CellMLSimulation, CellMLSimulationPlugin)

CellMLSimulationPlugin::CellMLSimulationPlugin() :
    GuiInterface("CellMLXMLView")
{
    // Set our settings

    mSettings->addView(GuiViewSettings::Simulation);
}

void CellMLSimulationPlugin::retranslateUi()
{
    // Retranslate our view's tab

    GuiViewSettings *view = mSettings->views().at(0);

    view->tabBar()->setTabText(view->tabIndex(), tr("CellML"));
}

} }
