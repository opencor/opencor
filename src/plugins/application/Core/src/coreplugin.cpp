#include "centralwidget.h"
#include "coreplugin.h"
#include "plugin.h"

#include <QAction>
#include <QFileDialog>
#include <QMainWindow>

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo()
{
    PluginInfoDescriptions descriptions;

    descriptions.insert("en", "The core plugin for OpenCOR");
    descriptions.insert("fr", "L'extension de base pour OpenCOR");

    return PluginInfo(PluginInterface::V001,
                      PluginInfo::Gui,
                      PluginInfo::Application,
                      false,
                      QStringList(),
                      descriptions);
}

Q_EXPORT_PLUGIN2(Core, CorePlugin)

CorePlugin::CorePlugin() :
    GuiInterface("Core")
{
}

void CorePlugin::initialize()
{
    // Retrieve the file types supported by the plugins

    foreach (Plugin *loadedPlugin, mLoadedPlugins) {
        ApiInterface *apiInterface = qobject_cast<ApiInterface *>(loadedPlugin->instance());

        if (apiInterface)
            // The plugin implements our API interface, so...

            mSupportedFileTypes << apiInterface->fileTypes();
    }

    // Retrieve the active directory

//    mActiveDir.setPath(mMainWindow->setsettings.value(SettingsFileDialogDirectory,
//                                      QDir::currentPath()).toString());

    // Create our central widget

    mCentralWidget = new CentralWidget(mMainWindow);

    // Create and set our data

    mData = new GuiCoreSettings(mCentralWidget);

    // Check, based on the loaded plugins, which views, if any, our central
    // widget should support

    foreach (Plugin *loadedPlugin, mLoadedPlugins) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(loadedPlugin->instance());

        if (guiInterface)
            // The plugin implements our GUI interface, so go through each view
            // supported by the plugin and enable whatever mode is required

            foreach (GuiViewSettings *viewSettings,
                     guiInterface->guiSettings()->views())
                mCentralWidget->addView(loadedPlugin, viewSettings);
    }

    // Create our different File actions
    // Note: all the save-related actions are to be invisible unless the Editing
    //       mode is active

    mFileOpen = newAction(mMainWindow, false,
                          ":/oxygen/actions/document-open.png");

    mFileSave    = newAction(mMainWindow, false,
                             ":/oxygen/actions/document-save.png",
                             mCentralWidget->isModeEnabled(GuiViewSettings::Editing));
    mFileSaveAs  = newAction(mMainWindow, false,
                             ":/oxygen/actions/document-save-all.png",
                             mCentralWidget->isModeEnabled(GuiViewSettings::Editing));
    mFileSaveAll = newAction(mMainWindow, false,
                             ":/oxygen/actions/document-save-as.png",
                             mCentralWidget->isModeEnabled(GuiViewSettings::Editing));

    mFileClose    = newAction(mMainWindow, false,
                              ":/oxygen/actions/document-close.png");
    mFileCloseAll = newAction(mMainWindow);

    mFilePrint = newAction(mMainWindow, false,
                          ":/oxygen/actions/document-open.png");

    // Some connections to handle our various actions

    connect(mFileOpen, SIGNAL(triggered(bool)),
            this, SLOT(openFile()));

    // Set our settings

    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileOpen);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileSave);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileSaveAs);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileSaveAll);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileClose);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileCloseAll);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFilePrint);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
}

void CorePlugin::finalize()
{
    // Keep track of the active directory

//    settings.setValue(SettingsFileDialogDirectory, mActiveDir.path());

    // Delete our data

    delete (GuiCoreSettings *) mData;
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

void CorePlugin::openFile()
{
    // Ask for the file(s) to be opened

    QStringList files = QFileDialog::getOpenFileNames(mMainWindow,
                                                      tr("Open File"),
                                                      mActiveDir.path(),
                                                      tr("All Files")
                                                      +" (*"
#ifdef Q_WS_WIN
                                                      +".*"
#endif
                                                      +");;"+tr("CellML File")+" (*.cellml)");

    if (files.count())
        // There is at least one file which is to be opened, so we can keep
        // track of the folder in which it is
        // Note #1: we use the last file to determine the folder that is to be
        //          remembered since on Windows 7, at least, it's possible to
        //          search for files from within the file dialog box, the last
        //          file should be the one we are 'interested' in...
        // Note #2: this doesn't, unfortunately, address the case where the user
        //          goes to a directory and then closes the file dialog box
        //          without selecting any file. There might be a way to get it
        //          to work, but that would involve using the exec method rather
        //          than the static getOpenFilesNames method, which would result
        //          in a non-native looking file dialog box (on Windows 7 at
        //          least), so it's not an option unfortunately...

        mActiveDir = QFileInfo(files.at(files.count()-1)).path();

    // Open the file(s)

//    mCentralWidget->openFiles(files);
}

} }
