#include "apiinterface.h"
#include "centralwidget.h"
#include "coreplugin.h"
#include "plugin.h"

#include <QAction>
#include <QFileDialog>
#include <QMainWindow>
#include <QSettings>
#include <QToolBar>

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo()
{
    Descriptions descriptions;

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

    // Create our File toolbar (and its show/hide action)

    mFileToolbar = newToolBar(mMainWindow, FileGroup);
    mFileToolbarAction = newAction(mMainWindow, true);

    // Create our different File actions, and add some to our File toolbar
    // Note: all the save-related actions are to be invisible unless the Editing
    //       mode is active

    mFileOpenAction = newAction(mMainWindow, false,
                                ":/oxygen/actions/document-open.png");

    mFileSaveAction    = newAction(mMainWindow, false,
                                   ":/oxygen/actions/document-save.png",
                                   mCentralWidget->isModeEnabled(GuiViewSettings::Editing));
    mFileSaveAsAction  = newAction(mMainWindow, false,
                                   ":/oxygen/actions/document-save-as.png",
                                   mCentralWidget->isModeEnabled(GuiViewSettings::Editing));
    mFileSaveAllAction = newAction(mMainWindow, false,
                                   ":/oxygen/actions/document-save-all.png",
                                   mCentralWidget->isModeEnabled(GuiViewSettings::Editing));

    mFileCloseAction    = newAction(mMainWindow, false,
                                    ":/oxygen/actions/document-close.png");
    mFileCloseAllAction = newAction(mMainWindow);

    mFilePrintAction = newAction(mMainWindow, false,
                                 ":/oxygen/actions/document-print.png");

    mFileToolbar->addAction(mFileOpenAction);
    mFileToolbar->addSeparator();
    mFileToolbar->addAction(mFileSaveAction);
    mFileToolbar->addAction(mFileSaveAsAction);
    mFileToolbar->addAction(mFileSaveAllAction);
    mFileToolbar->addSeparator();
    mFileToolbar->addAction(mFileCloseAction);
    mFileToolbar->addSeparator();
    mFileToolbar->addAction(mFilePrintAction);

    // Some connections to handle our various actions

    connect(mFileOpenAction, SIGNAL(triggered(bool)),
            this, SLOT(openFile()));

    // Set our settings

    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileOpenAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileSaveAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileSaveAsAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileSaveAllAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileCloseAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileCloseAllAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFilePrintAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);

    mGuiSettings->addToolBar(Qt::TopToolBarArea, mFileToolbar,
                             mFileToolbarAction);
}

void CorePlugin::finalize()
{
    // Delete our data

    delete (GuiCoreSettings *) mData;
}

static const QString SettingsFileDialogDirectory = "FileDialogDirectory";

void CorePlugin::loadSettings(QSettings *pSettings,
                              const bool &pNeedDefaultSettings)
{
    // First, call the parent implementation

    GuiInterface::loadSettings(pSettings, pNeedDefaultSettings);

    // Retrieve the active directory

    mActiveDir.setPath(pSettings->value(SettingsFileDialogDirectory,
                                        QDir::currentPath()).toString());
}

void CorePlugin::saveSettings(QSettings *pSettings) const
{
    // First, call the parent implementation

    GuiInterface::saveSettings(pSettings);

    // Keep track of the active directory

    pSettings->setValue(SettingsFileDialogDirectory, mActiveDir.path());
}

void CorePlugin::retranslateUi()
{
    // Retranslate our central widget

    mCentralWidget->retranslateUi();

    // Retranslate our different File actions

    retranslateAction(mFileOpenAction, tr("&Open..."),
                      tr("Open a file"),
                      tr("Ctrl+O"));

    retranslateAction(mFileSaveAction, tr("&Save"),
                      tr("Save the current file"),
                      tr("Ctrl+S"));
    retranslateAction(mFileSaveAsAction, tr("Save &As..."),
                      tr("Save the current file under a different name"));
    retranslateAction(mFileSaveAllAction, tr("Save All"),
                      tr("Save all the files"));

    retranslateAction(mFileCloseAction, tr("Clos&e"),
                      tr("Close the current file"),
                      tr("Ctrl+W"));
    retranslateAction(mFileCloseAllAction, tr("Close All"),
                      tr("Close all the files"));

    retranslateAction(mFilePrintAction, tr("&Print..."),
                      tr("Print the current file"),
                      tr("Ctrl+P"));

    // Retranslate our show/hide actions

    retranslateAction(mFileToolbarAction, tr("&File"),
                      tr("Show/hide the File toolbar"));
}

void CorePlugin::openFile()
{
    // Ask for the file(s) to be opened

    QString supportedFileTypes;

    foreach (const FileType &supportedFileType, mSupportedFileTypes)
        supportedFileTypes +=  ";;"
                              +supportedFileType.description(mLocale)
                              +" (*."+supportedFileType.fileExtension()+")";

    QStringList files = QFileDialog::getOpenFileNames(mMainWindow,
                                                      tr("Open File"),
                                                      mActiveDir.path(),
                                                      tr("All Files")
                                                      +" (*"
#ifdef Q_WS_WIN
                                                      +".*"
#endif
                                                      +")"+supportedFileTypes);

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

    mCentralWidget->openFiles(files);
}

} }
