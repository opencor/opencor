//==============================================================================
// FileOrganiser plugin
//==============================================================================

#include "fileorganiserplugin.h"
#include "fileorganiserwindow.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiser {

//==============================================================================

PLUGININFO_FUNC FileOrganiserPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to organise your files");
    descriptions.insert("fr", "Une extension pour organiser vos fichiers");

    return new PluginInfo(PluginInfo::V001,
                          PluginInfo::Gui,
                          PluginInfo::Organisation,
                          true,
                          QStringList() << "Core",
                          descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(FileOrganiser, FileOrganiserPlugin)

//==============================================================================

void FileOrganiserPlugin::initialize()
{
    // Create an action to show/hide our file organiser window

    mFileOrganiserAction = newAction(mMainWindow, true);

    // Create our file organiser window

    mFileOrganiserWindow = new FileOrganiserWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::LeftDockWidgetArea, mFileOrganiserWindow,
                            GuiWindowSettings::Organisation,
                            mFileOrganiserAction);
}

//==============================================================================

void FileOrganiserPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our file organiser window settings

    loadWindowSettings(pSettings, mFileOrganiserWindow);
}

//==============================================================================

void FileOrganiserPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our file organiser window settings

    saveWindowSettings(pSettings, mFileOrganiserWindow);
}

//==============================================================================

void FileOrganiserPlugin::retranslateUi()
{
    // Retranslate our file organiser action

    retranslateAction(mFileOrganiserAction, tr("File Organiser"),
                      tr("Show/hide the File Organiser window"));

    // Retranslate our file organiser window

    mFileOrganiserWindow->retranslateUi();
}

//==============================================================================

}   // namespace FileOrganiser
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
