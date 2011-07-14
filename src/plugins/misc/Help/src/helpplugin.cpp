#include "helpplugin.h"
#include "helpwindow.h"

namespace OpenCOR {
namespace Help {

PLUGININFO_FUNC HelpPluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "A plugin to provide help");
    descriptions.insert("fr", "Une extension pour fournir de l'aide");

    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       descriptions);
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

HelpPlugin::HelpPlugin() :
    GuiInterface("Help")
{
}

void HelpPlugin::initialize(MainWindow *pMainWindow)
{
    // Add our help window to OpenCOR

    mHelpWindow = new HelpWindow(pMainWindow);

    // Create some actions that are to be registered by OpenCOR

/*---GRY--- THE BELOW IS NOT RIGHT, NOT LEAST BECAUSE THE PLUGIN PROBABLY SHOULD
            CREATE THE ACTIONS, SO THAT IT CAN CONNECT TO THEM...
    mActions << GuiAction(Help, true, ":/oxygen/apps/help-browser.png");
*/
}

void HelpPlugin::finalize()
{
    // Delete our help window

    delete mHelpWindow;
}

void HelpPlugin::retranslateUi()
{
    // Retranslate our help window

    mHelpWindow->retranslateUi();
}

} }
