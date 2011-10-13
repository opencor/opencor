//==============================================================================
// Viewer plugin
//==============================================================================

#include "viewerplugin.h"
#include "viewerwindow.h"

//==============================================================================

#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

PLUGININFO_FUNC ViewerPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "A plugin to graphically visualise various concepts (e.g. mathematical equations)");
    descriptions.insert("fr", "Une extension pour visualiser graphiquement différents concepts (par exemple des équations mathématiques)");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Editing,
                      true,
                      QStringList() << "CoreEditing" << "QtMmlWidget",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(Viewer, ViewerPlugin)

//==============================================================================

void ViewerPlugin::initialize()
{
    // Create an action to show/hide our viewer window

    mViewerAction = newAction(mMainWindow, true);

    // Create our viewer window

    mViewerWindow = new ViewerWindow(mMainWindow);

    // Set our settings

    mGuiSettings->addWindow(Qt::TopDockWidgetArea, mViewerWindow,
                            GuiWindowSettings::Editing, mViewerAction);
}

//==============================================================================

void ViewerPlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve our viewer window settings

    loadWindowSettings(pSettings, mViewerWindow);
}

//==============================================================================

void ViewerPlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of our viewer window settings

    saveWindowSettings(pSettings, mViewerWindow);
}

//==============================================================================

void ViewerPlugin::retranslateUi()
{
    // Retranslate our viewer action

    retranslateAction(mViewerAction, tr("Viewer"),
                      tr("Show/hide the Viewer window"));

    // Retranslate our viewer window

    mViewerWindow->retranslateUi();
}

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
