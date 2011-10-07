#include "coresimulationplugin.h"

#include <QCalendarWidget>
#include <QMainWindow>

namespace OpenCOR {
namespace CoreSimulation {

PLUGININFO_FUNC CoreSimulationPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "The core simulation plugin for OpenCOR");
    descriptions.insert("fr", "L'extension de simulation de base pour OpenCOR");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Simulation,
                      false,
                      QStringList() << "Core" << "Qwt",
                      descriptions);
}

Q_EXPORT_PLUGIN2(CoreSimulation, CoreSimulationPlugin)

void CoreSimulationPlugin::initialize()
{
    // Create our simulation view widget

    mSimulationView = new QCalendarWidget(mMainWindow);

    mSimulationView->setVisible(false);

    // Set our settings

    mGuiSettings->addView(GuiViewSettings::Simulation);
    // Note: this won't so much set a new view (since the simulation mode should
    //       have only one view), but ensure that the simulation mode becomes
    //       available
}

QWidget * CoreSimulationPlugin::viewWidget(const QString &)
{
    // Always return our simulation view widget, no matter which file name is
    // being passed

    return mSimulationView;
}

} }
