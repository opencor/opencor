/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Core plugin
//==============================================================================

#include "centralwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "coreplugin.h"
#include "datastoreinterface.h"
#include "filemanager.h"
#include "filetypeinterface.h"
#include "interfaces.h"
#include "organisationwidget.h"
#include "plugin.h"
#include "solverinterface.h"

//==============================================================================

#include <QAction>
#include <QApplication>
#include <QEvent>
#include <QFile>
#include <QMainWindow>
#include <QMenu>
#include <QPalette>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

PLUGININFO_FUNC CorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the core plugin."));
    descriptions.insert("fr", QString::fromUtf8("l'extension de base."));

    return new PluginInfo(PluginInfo::Category::Miscellaneous, false, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================
// Core interface
//==============================================================================

QWidget * CorePlugin::centralWidget()
{
    // Return our central widget

    return mCentralWidget;
}

//==============================================================================

void CorePlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    // Let our central widget know that all the other plugins have now loaded
    // their settings

    mCentralWidget->settingsLoaded(pLoadedPlugins);
}

//==============================================================================

void CorePlugin::openFile(const QString &pFileNameOrUrl)
{
    // Open the given file as if it was a remote file
    // Note: if it is not then CentralWidget::openRemoteFile() will open it as
    //       a normal file...

    mCentralWidget->openRemoteFile(pFileNameOrUrl);
}

//==============================================================================

bool CorePlugin::canClose()
{
    // To determine whether we can close, we must ask our central widget

    return mCentralWidget->canClose();
}

//==============================================================================
// File handling interface
//==============================================================================

bool CorePlugin::importFile(const QString &pFileName)
{
    // We rely on this interface not to import the given file as such, but to
    // get the current view to import it, if it can, or open it as any normal
    // file, if the current view cannot import it
    // Note: we call CentralWidget::importRemoteFile() because pFileName can
    //       actually be a remote file. If it isn't then the file will be
    //       imported as a normal file...

    mCentralWidget->importRemoteFile(pFileName);

    return false;
}

//==============================================================================

bool CorePlugin::saveFile(const QString &pOldFileName,
                          const QString &pNewFileName,
                          bool &pNeedFeedback)
{
    Q_UNUSED(pOldFileName)
    Q_UNUSED(pNewFileName)
    Q_UNUSED(pNeedFeedback)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CorePlugin::fileOpened(const QString &pFileName)
{
    // Remove the file from our list of recent files

    FileManager *fileManagerInstance = FileManager::instance();

    if (mRecentFiles.removeOne(fileManagerInstance->isRemote(pFileName)?
                                   fileManagerInstance->url(pFileName):
                                   pFileName)) {
        updateFileReopenMenu();
    }
}

//==============================================================================

void CorePlugin::filePermissionsChanged(const QString &pFileName)
{
    // Update the checked state of our Locked menu, if needed

    if (pFileName == mCentralWidget->currentFileName()) {
        updateNewModifiedSensitiveActions();
    }
}

//==============================================================================

void CorePlugin::fileModified(const QString &pFileName)
{
    // Update our new/modified sensitive actions, if needed

    if (pFileName == mCentralWidget->currentFileName()) {
        updateNewModifiedSensitiveActions();
    }
}

//==============================================================================

void CorePlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CorePlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void CorePlugin::fileRenamed(const QString &pOldFileName,
                             const QString &pNewFileName)
{
    // Remove the new file from our list of recent files, should it be there
    // Note: it's fine if the new file isn't in our list since nothing will be
    //       done in that case (thus avoiding us from having to test for its
    //       presence)...

    if (mRecentFiles.removeOne(pNewFileName)) {
        updateFileReopenMenu();
    }

    // A file has been created or saved under a new name, so we want the old
    // file name to be added to our list of recent files, i.e. as if it had been
    // closed

    fileClosed(pOldFileName);
}

//==============================================================================

void CorePlugin::fileClosed(const QString &pFileName)
{
    // Add, if isn't new and it still exists (i.e. we are not here because the
    // file has been deleted), the file to our list of recent files (making sure
    // that we don't end up with more than 10 recent files)
    // Note: the most recent file is to be shown first...

    FileManager *fileManagerInstance = FileManager::instance();

    if (   !fileManagerInstance->isNew(pFileName)
        &&  QFile::exists(pFileName)) {
        mRecentFiles.prepend(fileManagerInstance->isRemote(pFileName)?
                                 fileManagerInstance->url(pFileName):
                                 pFileName);

        while (mRecentFiles.count() > 10) {
            mRecentFiles.removeLast();
        }

        updateFileReopenMenu();
    }
}

//==============================================================================
// GUI interface
//==============================================================================

void CorePlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    Q_UNUSED(pViewPlugin)
    Q_UNUSED(pFileName)

    // Update our new/modified sensitive actions

    updateNewModifiedSensitiveActions();
}

//==============================================================================

Gui::Menus CorePlugin::guiMenus() const
{
    // Return our menus

    return Gui::Menus() << Gui::Menu(Gui::Menu::Type::File, mFileOpenReloadSeparator, mFileReopenSubMenu);
}

//==============================================================================

Gui::MenuActions CorePlugin::guiMenuActions() const
{
    // Return our menu actions

    return Gui::MenuActions() << Gui::MenuAction(Gui::MenuAction::Type::FileNew, mFileNewFileAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileOpenAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileOpenRemoteAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileOpenReloadSeparator)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileReloadAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, newSeparator(mainWindow()))
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileDuplicateAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, newSeparator(mainWindow()))
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileLockedAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, newSeparator(mainWindow()))
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileSaveAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileSaveAsAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileSaveAllAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, newSeparator(mainWindow()))
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFilePreviousAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileNextAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, newSeparator(mainWindow()))
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileCloseAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, mFileCloseAllAction)
                              << Gui::MenuAction(Gui::MenuAction::Type::File, newSeparator(mainWindow()));
}

//==============================================================================
// I18n interface
//==============================================================================

void CorePlugin::retranslateUi()
{
    // Retranslate our different File actions

    retranslateAction(mFileNewFileAction, tr("File"),
                      tr("Create a new file"));

    retranslateAction(mFileOpenAction, tr("Open..."),
                      tr("Open a file"));
    retranslateAction(mFileOpenRemoteAction, tr("Open Remote..."),
                      tr("Open a remote file"));

    retranslateAction(mFileReloadAction, tr("Reload"),
                      tr("Reload the current file"));

    retranslateAction(mFileDuplicateAction, tr("Duplicate"),
                      tr("Duplicate the current file"));

    retranslateAction(mFileLockedAction, tr("Locked"),
                      tr("Toggle the locked state of the current file"));

    retranslateAction(mFileSaveAction, tr("Save"),
                      tr("Save the current file"));
    retranslateAction(mFileSaveAsAction, tr("Save As..."),
                      tr("Save the current file under a different name"));
    retranslateAction(mFileSaveAllAction, tr("Save All"),
                      tr("Save all the files"));

    retranslateAction(mFilePreviousAction, tr("Previous"),
                      tr("Select the previous file"));
    retranslateAction(mFileNextAction, tr("Next"),
                      tr("Select the next file"));

    retranslateAction(mFileCloseAction, tr("Close"),
                      tr("Close the current file"));
    retranslateAction(mFileCloseAllAction, tr("Close All"),
                      tr("Close all the files"));

    // Retranslate our File sub-menu and its action

    retranslateMenu(mFileReopenSubMenu, tr("Reopen"));

    retranslateAction(mFileReopenMostRecentFileAction, tr("Most Recent"),
                      tr("Reopen the most recently closed file"));
    retranslateAction(mFileClearReopenSubMenuAction, tr("Clear Menu"),
                      tr("Clear the menu"));

    // Retranslate our central widget

    mCentralWidget->retranslateUi();
}

//==============================================================================
// Plugin interface
//==============================================================================

bool CorePlugin::definesPluginInterfaces()
{
    // We don't handle this interface...

    return false;
}

//==============================================================================

bool CorePlugin::pluginInterfacesOk(const QString &pFileName,
                                    QObject *pInstance)
{
    Q_UNUSED(pFileName)
    Q_UNUSED(pInstance)

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CorePlugin::initializePlugin()
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (mainWindow() == nullptr) {
        return;
    }

    // Create our central widget

    mCentralWidget = new CentralWidget(mainWindow());

    mCentralWidget->setObjectName("CentralWidget");

    // Create our different File actions

    mFileNewFileAction = newAction(QIcon(":/oxygen/actions/document-new.png"),
                                   QKeySequence::New, mainWindow());

    mFileOpenAction = newAction(QIcon(":/oxygen/actions/document-open.png"),
                                QKeySequence::Open, mainWindow());
    mFileOpenRemoteAction = newAction(QIcon(":/oxygen/actions/document-open-remote.png"),
                                      QKeySequence(Qt::ControlModifier|Qt::ShiftModifier|Qt::Key_O),
                                      mainWindow());

    mFileReloadAction = newAction(mainWindow());

    mFileDuplicateAction = newAction(QKeySequence(Qt::ControlModifier|Qt::Key_D),
                                     mainWindow());

    mFileLockedAction = newAction(true, QKeySequence(Qt::ControlModifier|Qt::Key_L),
                                  mainWindow());

    mFileSaveAction = newAction(QIcon(":/oxygen/actions/document-save.png"),
                                QKeySequence::Save, mainWindow());
    mFileSaveAsAction = newAction(QIcon(":/oxygen/actions/document-save-as.png"),
                                  QKeySequence::SaveAs, mainWindow());
    mFileSaveAllAction = newAction(QIcon(":/oxygen/actions/document-save-all.png"),
                                   mainWindow());

    // Note: for mFilePreviousAction and mFileNextAction, we would normally use
    //       QKeySequence::PreviousChild and QKeySequence::NextChild,
    //       respectively, but for Qt this means using Ctrl+Shift+BackTab and
    //       Ctrl+Tab, respectively, on Windows/Linux, and Ctrl+{ and Ctrl+},
    //       respectively, on macOS. On Windows, Ctrl+Shift+BackTab just doesn't
    //       work, on macOS those key sequences are not the most natural ones.
    //       So, instead, it would be more natural to use Ctrl+Shift+Tab and
    //       Ctr+Tab, respectively, on Windows/Linux, and Meta+Shift+Tab and
    //       Meta+Tab, respectively, on macOS. The original plan was therefore
    //       to use QKeySequence::PreviousChild and QKeySequence::NextChild, as
    //       well as our preferred key sequences, but Qt ended up 'allowing'
    //       only using one of them. So, in the end, we only use our preferred
    //       key sequences...

    mFilePreviousAction = newAction(QIcon(":/oxygen/actions/go-previous.png"),
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
                                    QKeySequence(Qt::ControlModifier|Qt::ShiftModifier|Qt::Key_Tab),
#else
                                    QKeySequence(Qt::MetaModifier|Qt::ShiftModifier|Qt::Key_Tab),
#endif
                                    mainWindow());
    mFileNextAction = newAction(QIcon(":/oxygen/actions/go-next.png"),
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
                                QKeySequence(Qt::ControlModifier|Qt::Key_Tab),
#else
                                QKeySequence(Qt::MetaModifier|Qt::Key_Tab),
#endif
                                    mainWindow());

    mFileCloseAction = newAction(QIcon(":/oxygen/actions/document-close.png"),
#if defined(Q_OS_WIN)
                                 QList<QKeySequence>() << QKeySequence::Close << QKeySequence(Qt::ControlModifier|Qt::Key_W),
#else
                                 QKeySequence::Close,
#endif
                                 mainWindow());
    mFileCloseAllAction = newAction(mainWindow());

    // Create the separator before which we will insert in our Reopen sub-menu

    mFileOpenReloadSeparator = newSeparator(mainWindow());

    // Create our Reopen sub-menu

    mFileReopenSubMenu = newMenu(QIcon(":/oxygen/actions/document-open-recent.png"),
                                 mainWindow());

    mFileReopenMostRecentFileAction = newAction(QKeySequence(Qt::ControlModifier|Qt::ShiftModifier|Qt::Key_T),
                                                mainWindow());
    mFileReopenSubMenuSeparator1 = newSeparator(mainWindow());
    mFileReopenSubMenuSeparator2 = newSeparator(mainWindow());
    mFileClearReopenSubMenuAction = newAction(mainWindow());

    mFileReopenSubMenu->addAction(mFileReopenMostRecentFileAction);
    mFileReopenSubMenu->addAction(mFileReopenSubMenuSeparator1);
    mFileReopenSubMenu->addAction(mFileReopenSubMenuSeparator2);
    mFileReopenSubMenu->addAction(mFileClearReopenSubMenuAction);

    // Some connections to handle our different File actions

    connect(mFileNewFileAction, &QAction::triggered,
            this, &CorePlugin::newFile);

    connect(mFileOpenAction, &QAction::triggered,
            mCentralWidget, QOverload<>::of(&CentralWidget::openFile));
    connect(mFileOpenRemoteAction, &QAction::triggered,
            mCentralWidget, QOverload<>::of(&CentralWidget::openRemoteFile));

    connect(mFileReloadAction, &QAction::triggered,
            mCentralWidget, QOverload<>::of(&CentralWidget::reloadFile));

    connect(mFileDuplicateAction, &QAction::triggered,
            mCentralWidget, &CentralWidget::duplicateFile);

    connect(mFileLockedAction, &QAction::triggered,
            mCentralWidget, &CentralWidget::toggleLockedFile);

    connect(mFileSaveAction, &QAction::triggered,
            mCentralWidget, QOverload<>::of(&CentralWidget::saveFile));
    connect(mFileSaveAsAction, &QAction::triggered,
            mCentralWidget, &CentralWidget::saveFileAs);
    connect(mFileSaveAllAction, &QAction::triggered,
            mCentralWidget, &CentralWidget::saveAllFiles);

    connect(mFilePreviousAction, &QAction::triggered,
            mCentralWidget, &CentralWidget::previousFile);
    connect(mFileNextAction, &QAction::triggered,
            mCentralWidget, &CentralWidget::nextFile);

    connect(mFileCloseAction, &QAction::triggered,
            mCentralWidget, QOverload<>::of(&CentralWidget::closeFile));
    connect(mFileCloseAllAction, &QAction::triggered,
            mCentralWidget, QOverload<>::of(&CentralWidget::closeAllFiles));

    // Some connections to update the enabled state of our various actions

    connect(mCentralWidget, &CentralWidget::atLeastOneView,
            mFileOpenAction, &QAction::setEnabled);
    connect(mCentralWidget, &CentralWidget::atLeastOneView,
            mFileOpenRemoteAction, &QAction::setEnabled);

    connect(mCentralWidget, &CentralWidget::canSave,
            mFileSaveAction, &QAction::setEnabled);
    connect(mCentralWidget, &CentralWidget::canSaveAs,
            mFileSaveAsAction, &QAction::setEnabled);
    connect(mCentralWidget, &CentralWidget::canSaveAll,
            mFileSaveAllAction, &QAction::setEnabled);

    connect(mCentralWidget, &CentralWidget::atLeastTwoFiles,
            mFilePreviousAction, &QAction::setEnabled);
    connect(mCentralWidget, &CentralWidget::atLeastTwoFiles,
            mFileNextAction, &QAction::setEnabled);

    connect(mCentralWidget, &CentralWidget::atLeastOneFile,
            mFileCloseAction, &QAction::setEnabled);
    connect(mCentralWidget, &CentralWidget::atLeastOneFile,
            mFileCloseAllAction, &QAction::setEnabled);

    // Some connections related to our Reopen sub-menu

    connect(mFileReopenSubMenu, &QMenu::aboutToShow,
            this, &CorePlugin::updateFileReopenMenu);

    connect(mFileReopenMostRecentFileAction, &QAction::triggered,
            this, &CorePlugin::reopenMostRecentFile);
    connect(mFileClearReopenSubMenuAction, &QAction::triggered,
            this, &CorePlugin::clearReopenSubMenu);
}

//==============================================================================

void CorePlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CorePlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    // Retrieve the different file type, solver and data store interfaces that
    // are available to us

    FileTypeInterfaces fileTypeInterfaces = FileTypeInterfaces();
    FileTypeInterfaces dataStoreFileTypeInterfaces = FileTypeInterfaces();
    SolverInterfaces solverInterfaces = SolverInterfaces();
    DataStoreInterfaces dataStoreInterfaces = DataStoreInterfaces();

    for (auto plugin : pLoadedPlugins) {
        FileTypeInterface *fileTypeInterface = qobject_cast<FileTypeInterface *>(plugin->instance());
        SolverInterface *solverInterface = qobject_cast<SolverInterface *>(plugin->instance());
        DataStoreInterface *dataStoreInterface = qobject_cast<DataStoreInterface *>(plugin->instance());

        // Keep track of file types, but disinguish between those that are also
        // a data store and those that are not

        if (fileTypeInterface != nullptr) {
            if (dataStoreInterface != nullptr) {
                dataStoreFileTypeInterfaces << fileTypeInterface;
            } else {
                fileTypeInterfaces << fileTypeInterface;
            }
        }

        // Keep track of solvers

        if (solverInterface != nullptr) {
            solverInterfaces << solverInterface;
        }

        // Keep track of data stores

        if (dataStoreInterface != nullptr) {
            dataStoreInterfaces << dataStoreInterface;
        }
    }

    // Keep track of our various interfaces

    static InterfacesData data(fileTypeInterfaces, dataStoreFileTypeInterfaces,
                               solverInterfaces, dataStoreInterfaces);

    globalInstance(InterfacesDataSignature, &data);

    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (mainWindow() == nullptr) {
        return;
    }

    // Check, based on the loaded plugins, which views, if any, our central
    // widget should support

    for (auto plugin : pLoadedPlugins) {
        ViewInterface *viewInterface = qobject_cast<ViewInterface *>(plugin->instance());

        if (viewInterface != nullptr) {
            // The plugin implements our View interface, so add it to our
            // central widget

            mCentralWidget->addView(plugin);
        }
    }
}

//==============================================================================

static const char *SettingsRecentFiles = "RecentFiles";

//==============================================================================

void CorePlugin::loadSettings(QSettings &pSettings)
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (mainWindow() == nullptr) {
        return;
    }

    // Retrieve our recent files
    // Note: it's important to retrieve our recent files before retrieving our
    //       central widget settings since mRecentFiles gets updated as a result
    //       of opening/closing a file...

    mRecentFiles = pSettings.value(SettingsRecentFiles).toStringList();

    updateFileReopenMenu();

    // Retrieve the central widget settings

    pSettings.beginGroup(mCentralWidget->objectName());
        mCentralWidget->loadSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void CorePlugin::saveSettings(QSettings &pSettings) const
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (mainWindow() == nullptr) {
        return;
    }

    // Keep track of our recent files

    pSettings.setValue(SettingsRecentFiles, mRecentFiles);

    // Keep track of the central widget settings

    pSettings.beginGroup(mCentralWidget->objectName());
        mCentralWidget->saveSettings(pSettings);
    pSettings.endGroup();
}

//==============================================================================

void CorePlugin::handleUrl(const QUrl &pUrl)
{
    // Handle the action that was passed to us

    QString actionName = pUrl.authority();

    if (actionName.compare("Core.selectMode", Qt::CaseInsensitive) == 0) {
        // We want to select a given mode

        mCentralWidget->selectMode(urlArguments(pUrl));
    } else if (actionName.compare("Core.selectView", Qt::CaseInsensitive) == 0) {
        // We want to select a given view

        mCentralWidget->selectView(urlArguments(pUrl));
    }
}

//==============================================================================
// Plugin specific
//==============================================================================

void CorePlugin::newFile()
{
    // Ask our file manager to create a new file

    FileManager *fileManagerInstance = FileManager::instance();
#ifdef QT_DEBUG
    FileManager::Status createStatus =
#endif
    fileManagerInstance->create();

#ifdef QT_DEBUG
    // Make sure that the file has indeed been created

    if (createStatus != FileManager::Status::Created) {
        qFatal("FATAL ERROR | %s:%d: the new file could not be created.", __FILE__, __LINE__);
    }
#endif
}

//==============================================================================

void CorePlugin::updateFileReopenMenu()
{
    // Update the contents of our Reopen sub-menu by first cleaning it

    for (auto recentFileAction : mRecentFileActions) {
        mFileReopenSubMenu->removeAction(recentFileAction);

        delete recentFileAction;
    }

    mRecentFileActions.clear();

    // Add our recent files to our Reopen sub-menu

    bool enabled = mFileOpenAction->isEnabled();

    for (const auto &recentFile : mRecentFiles) {
        auto action = newAction(mainWindow());

        action->setEnabled(enabled);
        action->setText(recentFile);

        connect(action, &QAction::triggered,
                this, &CorePlugin::reopenRecentFile);

        mFileReopenSubMenu->insertAction(mFileReopenSubMenuSeparator2, action);

        mRecentFileActions << action;
    }

    // Enable/disable our Reopen sub-menu actions

    enabled = enabled && !mRecentFiles.isEmpty();

    mFileReopenMostRecentFileAction->setEnabled(enabled);
    mFileClearReopenSubMenuAction->setEnabled(enabled);

    showEnableAction(mFileReopenSubMenuSeparator2, enabled);
}

//==============================================================================

void CorePlugin::updateNewModifiedSensitiveActions()
{
    // Update our actions, keeping in mind the fact that a file may be modified
    // or even new

    QString fileName = mCentralWidget->currentFileName();

    if (!fileName.isEmpty()) {
        FileManager *fileManagerInstance = FileManager::instance();
        bool fileIsNew = fileManagerInstance->isNew(fileName);
        bool fileIsNewRemoteOrModified =    fileIsNew
                                         || fileManagerInstance->isRemote(fileName)
                                         || fileManagerInstance->isModified(fileName);

        mFileReloadAction->setEnabled(!fileIsNew);
        mFileDuplicateAction->setEnabled(!fileIsNewRemoteOrModified);
        mFileLockedAction->setEnabled(   !fileIsNewRemoteOrModified
                                      &&  fileManagerInstance->isReadable(fileName));
    } else {
        mFileReloadAction->setEnabled(false);
        mFileDuplicateAction->setEnabled(false);
        mFileLockedAction->setEnabled(false);
    }

    mFileLockedAction->setChecked(!FileManager::instance()->isReadableAndWritable(fileName));
    // Note: we really want to call isReadableAndWritable() rather than
    //       isLocked() since from a GUI perspective a file should only be
    //       considered unlocked if it can be both readable and writable
    //       (see CentralWidget::updateFileTab())...
}

//==============================================================================

void CorePlugin::reopenFile(const QString &pFileName)
{
    // Remove the file from our list of recent files

    if (mRecentFiles.removeOne(pFileName)) {
        updateFileReopenMenu();
    }

    // Open the recent file after checking that it still exists, if needed

    bool isLocalFile;
    QString fileNameOrUrl;

    checkFileNameOrUrl(pFileName, isLocalFile, fileNameOrUrl);

    if (isLocalFile) {
        if (QFile::exists(fileNameOrUrl)) {
            mCentralWidget->openFile(fileNameOrUrl);
        } else {
            warningMessageBox(tr("Reopen File"),
                              tr("<strong>%1</strong> does not exist anymore.").arg(QDir::toNativeSeparators(fileNameOrUrl)));
        }
    } else {
        mCentralWidget->openRemoteFile(fileNameOrUrl);
    }
}

//==============================================================================

void CorePlugin::reopenRecentFile()
{
    // Reopen a recent file

    reopenFile(qobject_cast<QAction *>(sender())->text());
}

//==============================================================================

void CorePlugin::reopenMostRecentFile()
{
    // Reopen the most recently closed file
    // Note: we don't want to get a reference to mRecentFiles' first item, hence
    //       we construct a new string from it. Indeed, reopenFile() is going to
    //       remove that item from mRecentFiles, so if we were to use a
    //       reference, it would eventually become an empty string...

    reopenFile(QString(mRecentFiles.first()));
}

//==============================================================================

void CorePlugin::clearReopenSubMenu()
{
    // Indirectly clear our Reopen sub-menu

    mRecentFiles.clear();

    updateFileReopenMenu();
}

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
