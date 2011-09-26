#include "fileorganiserplugin.h"
#include "fileorganiserwindow.h"

#include <QMainWindow>

namespace OpenCOR {
namespace FileOrganiser {

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to organise your files");
    descriptions.insert("fr", "Une extension pour organiser vos fichiers");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Organisation,
                      true,
                      QStringList() << "Core",
                      descriptions);
}

Q_EXPORT_PLUGIN2(FileOrganiser, FileOrganiserPlugin)

void FileOrganiserPlugin::initialize()
{
    // Create our file organiser window

    mFileOrganiserWindow = new FileOrganiserWindow(mMainWindow);
}

void FileOrganiserPlugin::loadSettings(QSettings *pSettings,
                                       const bool &pNeedDefaultSettings)
{
    // Retrieve our file organiser window settings

    loadWindowSettings(pSettings, pNeedDefaultSettings, Qt::LeftDockWidgetArea,
                       mFileOrganiserWindow);
}

void FileOrganiserPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our file organiser window settings

    saveWindowSettings(pSettings, mFileOrganiserWindow);
}

void FileOrganiserPlugin::retranslateUi()
{
    // Retranslate our file organiser window

    mFileOrganiserWindow->retranslateUi();
}

} }
