//==============================================================================
// Core plugin
//==============================================================================

#include "centralwidget.h"
#include "coreplugin.h"
#include "organisationwidget.h"
#include "plugin.h"

//==============================================================================

#include <QAction>
#include <QFileDialog>
#include <QMainWindow>
#include <QSettings>
#include <QToolBar>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

PLUGININFO_FUNC CorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "The core plugin for OpenCOR");
    descriptions.insert("fr", "L'extension de base pour OpenCOR");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Application,
                      false,
                      QStringList(),
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(Core, CorePlugin)

//==============================================================================

void CorePlugin::initialize()
{
    // Create our central widget

    mCentralWidget = new CentralWidget(mMainWindow);

    // Create our File toolbar (and its show/hide action)

    QToolBar *fileToolbar = newToolBar(mMainWindow, FileGroup);

    mFileToolbarAction = newAction(mMainWindow, true);

    // Create our different File actions, and add some to our File toolbar
//---GRY--- ALL THE SAVE-RELATED ACTIONS SHOULD BE INVISIBLE UNLESS THE EDITING
//          MODE IS ACTIVE

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

    mFilePreviousAction = newAction(mMainWindow, false,
                                    ":/oxygen/actions/go-previous.png");
    mFileNextAction     = newAction(mMainWindow, false,
                                    ":/oxygen/actions/go-next.png");

    mFileCloseAction    = newAction(mMainWindow, false,
                                    ":/oxygen/actions/document-close.png");
    mFileCloseAllAction = newAction(mMainWindow);

    mFilePrintAction = newAction(mMainWindow, false,
                                 ":/oxygen/actions/document-print.png");

    fileToolbar->addAction(mFileOpenAction);
    fileToolbar->addSeparator();
    fileToolbar->addAction(mFileSaveAction);
    fileToolbar->addAction(mFileSaveAsAction);
    fileToolbar->addAction(mFileSaveAllAction);
    fileToolbar->addSeparator();
    fileToolbar->addAction(mFilePreviousAction);
    fileToolbar->addAction(mFileNextAction);
    fileToolbar->addSeparator();
    fileToolbar->addAction(mFileCloseAction);
    fileToolbar->addSeparator();
    fileToolbar->addAction(mFilePrintAction);

    // Some connections to handle our various actions

    connect(mFileOpenAction, SIGNAL(triggered(bool)),
            this, SLOT(openFile()));

    connect(mFilePreviousAction, SIGNAL(triggered(bool)),
            mCentralWidget, SLOT(previousFile()));
    connect(mFileNextAction, SIGNAL(triggered(bool)),
            mCentralWidget, SLOT(nextFile()));

    connect(mFileCloseAction, SIGNAL(triggered(bool)),
            mCentralWidget, SLOT(closeFile()));
    connect(mFileCloseAllAction, SIGNAL(triggered(bool)),
            mCentralWidget, SLOT(closeAllFiles()));

    // Some connections to handle the result of our various actions

    connect(mCentralWidget, SIGNAL(fileOpened(const QString &)),
            this, SLOT(needUpdateActions()));

    connect(mCentralWidget, SIGNAL(fileClosed(const QString &)),
            this, SLOT(needUpdateActions()));

    // Set our settings

    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileOpenAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileSaveAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileSaveAsAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileSaveAllAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFilePreviousAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileNextAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileCloseAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileCloseAllAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFilePrintAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);

    mGuiSettings->addToolBar(Qt::TopToolBarArea, fileToolbar,
                             mFileToolbarAction);

    mGuiSettings->addCentralWidget(mCentralWidget);

    // Initialise the enabled state of our various actions

    updateActions();
}

//==============================================================================

void CorePlugin::setup(const QList<Plugin *> &pLoadedPlugins)
{
    // Retrieve the file types supported by the plugins

    foreach (Plugin *loadedPlugin, pLoadedPlugins) {
        FileInterface *fileInterface = qobject_cast<FileInterface *>(loadedPlugin->instance());

        if (fileInterface)
            // The plugin implements our file interface, so...

            mSupportedFileTypes << fileInterface->fileTypes();
    }

    // Check, based on the loaded plugins, which views, if any, our central
    // widget should support

    foreach (Plugin *loadedPlugin, pLoadedPlugins) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(loadedPlugin->instance());

        if (guiInterface)
            // The plugin implements our GUI interface, so go through each view
            // supported by the plugin and enable whatever mode is required

            foreach (GuiViewSettings *viewSettings,
                     guiInterface->guiSettings()->views())
                mCentralWidget->addView(loadedPlugin, viewSettings);
    }

    // Some connections to handle certain plugin's windows

    foreach (Plugin *loadedPlugin, pLoadedPlugins) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(loadedPlugin->instance());

        if (guiInterface) {
            foreach (GuiWindowSettings *windowSettings,
                     guiInterface->guiSettings()->windows())
                switch (windowSettings->type()) {
                case GuiWindowSettings::Organisation:
                    // The plugin's window is of organisation type, so something
                    // we want our central widget to connect to

                    connect((OrganisationWidget *) windowSettings->window(), SIGNAL(filesOpened(const QStringList &)),
                            mCentralWidget, SLOT(openFiles(const QStringList &)));

                    break;
                default:
                    // Uninteresting type, so...

                    ;
                }
        }
    }
}

//==============================================================================

static const QString SettingsFileDialogDirectory = "FileDialogDirectory";

//==============================================================================

void CorePlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve the central widget settings

    pSettings->beginGroup(mCentralWidget->objectName());
        mCentralWidget->loadSettings(pSettings);
    pSettings->endGroup();

    // Retrieve the active directory

    mActiveDir.setPath(pSettings->value(SettingsFileDialogDirectory,
                                        QDir::currentPath()).toString());
}

//==============================================================================

void CorePlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of the central widget settings

    pSettings->beginGroup(mCentralWidget->objectName());
        mCentralWidget->saveSettings(pSettings);
    pSettings->endGroup();

    // Keep track of the active directory

    pSettings->setValue(SettingsFileDialogDirectory, mActiveDir.path());
}

//==============================================================================

void CorePlugin::retranslateUi()
{
    // Retranslate our different File actions

    retranslateAction(mFileOpenAction, tr("&Open..."),
                      tr("Open a file"),
                      QKeySequence::Open);

    retranslateAction(mFileSaveAction, tr("&Save"),
                      tr("Save the current file"),
                      QKeySequence::Save);
    retranslateAction(mFileSaveAsAction, tr("Save &As..."),
                      tr("Save the current file under a different name"),
                      QKeySequence::SaveAs);
    retranslateAction(mFileSaveAllAction, tr("Save All"),
                      tr("Save all the files"));

    retranslateAction(mFilePreviousAction, tr("Previous"),
                      tr("Select the previous file"),
                      QKeySequence::PreviousChild);
    retranslateAction(mFileNextAction, tr("Next"),
                      tr("Select the next file"),
                      QKeySequence::NextChild);

    retranslateAction(mFileCloseAction, tr("Clos&e"),
                      tr("Close the current file"),
                      QKeySequence::Close);
    retranslateAction(mFileCloseAllAction, tr("Close All"),
                      tr("Close all the files"));

    retranslateAction(mFilePrintAction, tr("&Print..."),
                      tr("Print the current file"),
                      QKeySequence::Print);

    // Retranslate our show/hide actions

    retranslateAction(mFileToolbarAction, tr("&File"),
                      tr("Show/hide the File toolbar"));

    // Retranslate our central widget

    mCentralWidget->retranslateUi();
}

//==============================================================================

void CorePlugin::updateActions()
{
    // Make sure that the various actions are properly enabled/disabled

    mFileOpenAction->setEnabled(true);

    mFileSaveAction->setEnabled(false);
    mFileSaveAsAction->setEnabled(false);
    mFileSaveAllAction->setEnabled(false);

    mFilePreviousAction->setEnabled(mCentralWidget->nbOfFilesOpened() > 1);
    mFileNextAction->setEnabled(mCentralWidget->nbOfFilesOpened() > 1);

    mFileCloseAction->setEnabled(mCentralWidget->nbOfFilesOpened());
    mFileCloseAllAction->setEnabled(mCentralWidget->nbOfFilesOpened());

    mFilePrintAction->setEnabled(false);
}

//==============================================================================

void CorePlugin::openFile()
{
    // Ask for the file(s) to be opened

    QString supportedFileTypes;

    foreach (const FileType &supportedFileType, mSupportedFileTypes)
        supportedFileTypes +=  ";;"
                              +supportedFileType.description()
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

//==============================================================================

void CorePlugin::needUpdateActions()
{
    // Something requires the actions to be udpated

    updateActions();
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
