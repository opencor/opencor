#include "filebrowserplugin.h"
#include "filebrowserwindow.h"

#include <QMainWindow>

namespace OpenCOR {
namespace FileBrowser {

PLUGININFO_FUNC FileBrowserPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to access your local files");
    descriptions.insert("fr", "Une extension pour accéder à vos fichiers locaux");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Organisation,
                      true,
                      QStringList() << "Core",
                      descriptions);
}

Q_EXPORT_PLUGIN2(FileBrowser, FileBrowserPlugin)

void FileBrowserPlugin::initialize()
{
    // Create our file browser window

    mFileBrowserWindow = new FileBrowserWindow(mMainWindow);
}

void FileBrowserPlugin::loadSettings(QSettings *pSettings,
                                     const bool &pNeedDefaultSettings)
{
    // Retrieve our file browser window settings

    loadWindowSettings(pSettings, pNeedDefaultSettings, Qt::LeftDockWidgetArea,
                       mFileBrowserWindow);
}

void FileBrowserPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our file browser window settings

    saveWindowSettings(pSettings, mFileBrowserWindow);
}

void FileBrowserPlugin::retranslateUi()
{
    // Retranslate our file browser window

    mFileBrowserWindow->retranslateUi();
}

} }
