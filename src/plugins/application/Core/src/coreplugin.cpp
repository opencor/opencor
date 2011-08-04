#include "centralwidget.h"
#include "coreplugin.h"
#include "plugin.h"

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "The core plugin for OpenCOR");
    descriptions.insert("fr", "L'extension de base pour OpenCOR");

    return PluginInfo(PluginInterface::V001, PluginInfo::Gui,
                      PluginInfo::Application, false,
                      QStringList(),
                      descriptions);
}

Q_EXPORT_PLUGIN2(Core, CorePlugin)

CorePlugin::CorePlugin() :
    GuiInterface("Core")
{
}

void CorePlugin::initialize(const QList<Plugin *> &pPlugins, QMainWindow *pMainWindow)
{
    // Create our central widget

    mCentralWidget = new CentralWidget(pMainWindow);

    // Create and set our data

    GuiSettingsCorePlugin *guiSettingsCorePlugin;

    mData = guiSettingsCorePlugin = new GuiSettingsCorePlugin(mCentralWidget);

    // Check, based on the loaded plugins, which modes, if any, our central
    // widget should support

    foreach (Plugin *plugin, pPlugins) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

        if (guiInterface)
            // The plugin implements our GUI interface, so...

            mCentralWidget->requireMode(guiInterface->requiredMode());
    }
}

void CorePlugin::finalize()
{
    // Delete our data

    delete (GuiSettingsCorePlugin *) mData;
}

void CorePlugin::retranslateUi()
{
    // Retranslate our central widget

    mCentralWidget->retranslateUi();
}

} }
