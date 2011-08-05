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

            mCentralWidget->enableMode(guiInterface->requiredMode());
    }

    // Create our different File actions
    // Note: all the save-related actions are to be invisible unless the Editing
    //       mode is active

    mFileOpen = newAction(pMainWindow, false,
                          ":/oxygen/actions/document-open.png");

    mFileSave    = newAction(pMainWindow, false,
                             ":/oxygen/actions/document-save.png",
                             mCentralWidget->isModeEnabled(GuiInterface::Editing));
    mFileSaveAs  = newAction(pMainWindow, false,
                             ":/oxygen/actions/document-save-all.png",
                             mCentralWidget->isModeEnabled(GuiInterface::Editing));
    mFileSaveAll = newAction(pMainWindow, false,
                             ":/oxygen/actions/document-save-as.png",
                             mCentralWidget->isModeEnabled(GuiInterface::Editing));

    mFileClose    = newAction(pMainWindow, false,
                              ":/oxygen/actions/document-close.png");
    mFileCloseAll = newAction(pMainWindow);

    mFilePrint = newAction(pMainWindow, false,
                          ":/oxygen/actions/document-open.png");

    // Set our settings

    mSettings.addAction(GuiSettingsAction::File, mFileOpen);
    mSettings.addAction(GuiSettingsAction::File);
    mSettings.addAction(GuiSettingsAction::File, mFileSave);
    mSettings.addAction(GuiSettingsAction::File, mFileSaveAs);
    mSettings.addAction(GuiSettingsAction::File, mFileSaveAll);
    mSettings.addAction(GuiSettingsAction::File);
    mSettings.addAction(GuiSettingsAction::File, mFileClose);
    mSettings.addAction(GuiSettingsAction::File, mFileCloseAll);
    mSettings.addAction(GuiSettingsAction::File);
    mSettings.addAction(GuiSettingsAction::File, mFilePrint);
    mSettings.addAction(GuiSettingsAction::File);
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

    // Retranslate our different File actions

    retranslateAction(mFileOpen, tr("&Open..."),
                      tr("Open a file"),
                      tr("Ctrl+O"));

    retranslateAction(mFileSave, tr("&Save"),
                      tr("Save the current file"),
                      tr("Ctrl+S"));
    retranslateAction(mFileSaveAs, tr("Save &As..."),
                      tr("Save the current file under a different name"));
    retranslateAction(mFileSaveAll, tr("Save All"),
                      tr("Save all the files"));

    retranslateAction(mFileClose, tr("Clos&e"),
                      tr("Close the current file"),
                      tr("Ctrl+W"));
    retranslateAction(mFileCloseAll, tr("Close All"),
                      tr("Close all the files"));

    retranslateAction(mFilePrint, tr("&Print..."),
                      tr("Print the current file"),
                      tr("Ctrl+P"));
}

} }
