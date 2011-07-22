#include "centralwidget.h"
#include "coreplugin.h"

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "The core plugin for OpenCOR");
    descriptions.insert("fr", "L'extension de base pour OpenCOR");

    return PluginInfo(PluginInterface::V001, PluginInfo::General, true,
                      QStringList(),
                      descriptions);
}

Q_EXPORT_PLUGIN2(Core, CorePlugin)

CorePlugin::CorePlugin() :
    GuiInterface("Core")
{
}

void CorePlugin::initialize(QMainWindow *pMainWindow)
{
    // Create our help window

    mCentralWidget = new CentralWidget(pMainWindow);

    mSettings.addCentralWidget(mCentralWidget);
}

void CorePlugin::retranslateUi()
{
    // Retranslate our central widget

    mCentralWidget->retranslateUi();
}

} }
