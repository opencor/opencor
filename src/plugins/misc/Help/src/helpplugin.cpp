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

void HelpPlugin::initialize(MainWindow *pMainWindow)
{
    // Add our help window to OpenCOR

    mHelpWindow = new HelpWindow(pMainWindow);
}

void HelpPlugin::finalize()
{
    // Delete our help window

    delete mHelpWindow;
}

} }
