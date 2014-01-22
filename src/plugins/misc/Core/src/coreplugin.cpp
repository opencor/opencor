/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Core plugin
//==============================================================================

#include "cliutils.h"
#include "centralwidget.h"
#include "coreplugin.h"
#include "fileinterface.h"
#include "filemanager.h"
#include "guiutils.h"
#include "organisationwidget.h"
#include "plugin.h"

//==============================================================================

#include <QAction>
#include <QApplication>
#include <QEvent>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QPalette>
#include <QSettings>
#include <QStackedWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

PLUGININFO_FUNC CorePluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the core plugin."));
    descriptions.insert("fr", QString::fromUtf8("l'extension de base."));

    return new PluginInfo(PluginInfo::Miscellaneous, false,
                          QStringList(),
                          descriptions);
}

//==============================================================================
// Core interface
//==============================================================================

void CorePlugin::initialize()
{
    // Create our central widget

    mCentralWidget = new CentralWidget(mMainWindow);

    // Create our different File actions

    mFileOpenAction = newAction(mMainWindow, false,
                                ":/oxygen/actions/document-open.png",
                                QKeySequence::Open);

    mFileReloadAction = newAction(mMainWindow);

    mFileDuplicateAction = newAction(mMainWindow, false, QString(),
                                     QKeySequence(Qt::CTRL|Qt::Key_D));

    mFileLockedAction = newAction(mMainWindow, true, QString(),
                                  QKeySequence(Qt::CTRL|Qt::Key_L));

    mFileSaveAction    = newAction(mMainWindow, false,
                                   ":/oxygen/actions/document-save.png",
                                   QKeySequence::Save);
    mFileSaveAsAction  = newAction(mMainWindow, false,
                                   ":/oxygen/actions/document-save-as.png",
                                   QKeySequence::SaveAs);
    mFileSaveAllAction = newAction(mMainWindow, false,
                                   ":/oxygen/actions/document-save-all.png",
                                   QKeySequence::UnknownKey);

    // Note: for mFilePreviousAction and mFileNextAction, we would normally use
    //       QKeySequence::PreviousChild and QKeySequence::NextChild,
    //       respectively, but for Qt this means using Ctrl+Shift+BackTab and
    //       Ctrl+Tab, respectively, on Windows/Linux, and Ctrl+{ and Ctrl+},
    //       respectively, on OS X. On Windows, Ctrl+Shift+BackTab just doesn't
    //       work, on OS X those key sequences are not the most natural ones.
    //       So, instead, it would be more natural to use Ctrl+Shift+Tab and
    //       Ctr+Tab, respectively, on Windows/Linux, and Meta+Shift+Tab and
    //       Meta+Tab, respectively, on OS X. The original plan was therefore to
    //       use QKeySequence::PreviousChild and QKeySequence::NextChild, as
    //       well as our preferred key sequences, but Qt ended up 'allowing'
    //       only using one of them. So, in the end, we only use our preferred
    //       key sequences...

    mFilePreviousAction = newAction(mMainWindow, false,
                                    ":/oxygen/actions/go-previous.png",
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
                                    QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Tab)
#elif defined(Q_OS_MAC)
                                    QKeySequence(Qt::META | Qt::SHIFT | Qt::Key_Tab)
#else
    #error Unsupported platform
#endif
                                   );
    mFileNextAction     = newAction(mMainWindow, false,
                                    ":/oxygen/actions/go-next.png",
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
                                    QKeySequence(Qt::CTRL | Qt::Key_Tab)
#elif defined(Q_OS_MAC)
                                    QKeySequence(Qt::META | Qt::Key_Tab)
#else
    #error Unsupported platform
#endif
                                   );

    mFileCloseAction    = newAction(mMainWindow, false,
                                    ":/oxygen/actions/document-close.png",
                                    QKeySequence::Close);
    mFileCloseAllAction = newAction(mMainWindow);

    // Create the separator before which we will insert our Reopen sub-menu

    QAction *openReloadSeparator = newAction(mMainWindow);

    openReloadSeparator->setSeparator(true);

    // Create our Reopen sub-menu

    mFileReopenSubMenu = newMenu(mMainWindow,
                                 ":/oxygen/actions/document-open-recent.png",
                                 false);

    mFileReopenSubMenuSeparator = newAction(mMainWindow);
    mFileClearReopenSubMenuAction = newAction(mMainWindow);

    mFileReopenSubMenuSeparator->setSeparator(true);

    mFileReopenSubMenu->addAction(mFileReopenSubMenuSeparator);
    mFileReopenSubMenu->addAction(mFileClearReopenSubMenuAction);

    // Some connections to handle our different File actions

    connect(mFileOpenAction, SIGNAL(triggered()),
            mCentralWidget, SLOT(openFile()));

    connect(mFileReloadAction, SIGNAL(triggered()),
            mCentralWidget, SLOT(reloadFile()));

    connect(mFileDuplicateAction, SIGNAL(triggered()),
            mCentralWidget, SLOT(duplicateFile()));

    connect(mFileLockedAction, SIGNAL(triggered()),
            mCentralWidget, SLOT(toggleLockedFile()));

    connect(mFileSaveAction, SIGNAL(triggered()),
            mCentralWidget, SLOT(saveFile()));
    connect(mFileSaveAsAction, SIGNAL(triggered()),
            mCentralWidget, SLOT(saveFileAs()));
    connect(mFileSaveAllAction, SIGNAL(triggered()),
            mCentralWidget, SLOT(saveAllFiles()));

    connect(mFilePreviousAction, SIGNAL(triggered()),
            mCentralWidget, SLOT(previousFile()));
    connect(mFileNextAction, SIGNAL(triggered()),
            mCentralWidget, SLOT(nextFile()));

    connect(mFileCloseAction, SIGNAL(triggered()),
            mCentralWidget, SLOT(closeFile()));
    connect(mFileCloseAllAction, SIGNAL(triggered()),
            mCentralWidget, SLOT(closeAllFiles()));

    // Some connections to update the enabled state of our various actions

    connect(mCentralWidget, SIGNAL(atLeastOneFile(const bool &)),
            mFileReloadAction, SLOT(setEnabled(bool)));

    connect(mCentralWidget, SIGNAL(atLeastOneFile(const bool &)),
            mFileDuplicateAction, SLOT(setEnabled(bool)));

    connect(mCentralWidget, SIGNAL(atLeastOneFile(const bool &)),
            mFileLockedAction, SLOT(setEnabled(bool)));

    connect(mCentralWidget, SIGNAL(canSave(const bool &)),
            mFileSaveAction, SLOT(setEnabled(bool)));
    connect(mCentralWidget, SIGNAL(canSaveAs(const bool &)),
            mFileSaveAsAction, SLOT(setEnabled(bool)));
    connect(mCentralWidget, SIGNAL(canSaveAll(const bool &)),
            mFileSaveAllAction, SLOT(setEnabled(bool)));

    connect(mCentralWidget, SIGNAL(atLeastTwoFiles(const bool &)),
            mFilePreviousAction, SLOT(setEnabled(bool)));
    connect(mCentralWidget, SIGNAL(atLeastTwoFiles(const bool &)),
            mFileNextAction, SLOT(setEnabled(bool)));

    connect(mCentralWidget, SIGNAL(atLeastOneFile(const bool &)),
            mFileCloseAction, SLOT(setEnabled(bool)));
    connect(mCentralWidget, SIGNAL(atLeastOneFile(const bool &)),
            mFileCloseAllAction, SLOT(setEnabled(bool)));

    // A connection related to our Reopen sub-menu

    connect(mFileClearReopenSubMenuAction, SIGNAL(triggered()),
            this, SLOT(clearReopenSubMenu()));

    // Set our settings

    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileOpenAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, openReloadSeparator);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileReloadAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileDuplicateAction);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File);
    mGuiSettings->addMenuAction(GuiMenuActionSettings::File, mFileLockedAction);
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

    mGuiSettings->addMenu(GuiMenuSettings::File, openReloadSeparator, mFileReopenSubMenu);

    mGuiSettings->setCentralWidget(mCentralWidget);

    // Miscellaneous

    mRecentFileNames = QStringList();

    retrieveColors();
}

//==============================================================================

void CorePlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void CorePlugin::initialized(const Plugins &pLoadedPlugins)
{
    // Retrieve the different file types supported by our various plugins and
    // make our central widget aware of them

    FileTypes supportedFileTypes = FileTypes();

    foreach (Plugin *loadedPlugin, pLoadedPlugins) {
        FileInterface *fileInterface = qobject_cast<FileInterface *>(loadedPlugin->instance());

        if (fileInterface)
            // The plugin implements our file interface, so add the supported
            // file types, but only if they are not already in our list

            foreach (const FileType &fileType, fileInterface->fileTypes())
                if (!supportedFileTypes.contains(fileType))
                    supportedFileTypes << fileType;
    }

    mCentralWidget->setSupportedFileTypes(supportedFileTypes);

    // Check, based on the loaded plugins, which views, if any, our central
    // widget should support

    foreach (Plugin *loadedPlugin, pLoadedPlugins) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(loadedPlugin->instance());

        if (guiInterface && guiInterface->guiSettings()->view())
            // The plugin implements our GUI interface and it has a view, so add
            // it to our central widget

            mCentralWidget->addView(loadedPlugin,
                                    guiInterface->guiSettings()->view());
    }
}

//==============================================================================

static const auto SettingsRecentFiles = QStringLiteral("RecentFiles");

//==============================================================================

void CorePlugin::loadSettings(QSettings *pSettings)
{
    // Retrieve the recent files
    // Note: it's important to retrieve the recent files before retrieving our
    //       central widget settings since mRecentFileNames gets updated as a
    //       result of opening/closing a file...

    mRecentFileNames = pSettings->value(SettingsRecentFiles).toStringList();

    // Update our Reopen sub-menu

    updateFileReopenMenu();

    // Retrieve the central widget settings

    pSettings->beginGroup(mCentralWidget->objectName());
        mCentralWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CorePlugin::saveSettings(QSettings *pSettings) const
{
    // Keep track of the recent files

    pSettings->setValue(SettingsRecentFiles, mRecentFileNames);

    // Keep track of the central widget settings

    pSettings->beginGroup(mCentralWidget->objectName());
        mCentralWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void CorePlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    // Let our central widget know that all the other plugins have now loaded
    // their settings

    mCentralWidget->settingsLoaded(pLoadedPlugins);
}

//==============================================================================

void CorePlugin::handleArguments(const QStringList &pArguments)
{
    // All the arguments are currently assumed to be files to open, so...

    mCentralWidget->openFiles(pArguments);
}

//==============================================================================

void CorePlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// GUI interface
//==============================================================================

void CorePlugin::changeEvent(QEvent *pEvent)
{
    // Check whether the palette has changed and if so then retrieve some new
    // colours to be used

    if (pEvent->type() == QEvent::PaletteChange)
        retrieveColors();
}

//==============================================================================

void CorePlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    Q_UNUSED(pViewPlugin);
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CorePlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void CorePlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool CorePlugin::hasViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

QWidget * CorePlugin::viewWidget(const QString &pFileName,
                                 const bool &pCreate)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pCreate);

    // We don't handle this interface...

    return 0;
}

//==============================================================================

void CorePlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

QString CorePlugin::viewName() const
{
    // We don't handle this interface...

    return QString();
}

//==============================================================================

QIcon CorePlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

//==============================================================================

bool CorePlugin::saveFile(const QString &pOldFileName,
                          const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CorePlugin::fileOpened(const QString &pFileName)
{
    // Remove the file from our list of recent files and update our Reopen
    // sub-menu

    mRecentFileNames.removeOne(pFileName);

    updateFileReopenMenu();
}

//==============================================================================

void CorePlugin::fileLocked(const QString &pFileName, const bool &pLocked)
{
    // Update the checked state of our Locked menu, if needed

    if (!pFileName.compare(mCentralWidget->currentFileName()))
        mFileLockedAction->setChecked(pLocked);
}

//==============================================================================

void CorePlugin::fileModified(const QString &pFileName, const bool &pModified)
{
    // Enable/disable our Duplicate and Locked menus, if needed

    if (!pFileName.compare(mCentralWidget->currentFileName())) {
        mFileDuplicateAction->setEnabled(!pModified);
        mFileLockedAction->setEnabled(!pModified);
    }
}

//==============================================================================

void CorePlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CorePlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CorePlugin::fileRenamed(const QString &pOldFileName,
                             const QString &pNewFileName)
{
    // Remove the new file from our list of recent files, should it be there
    // Note: it's fine if the new file isn't in our list since nothing will be
    //       done in that case (thus avoiding us having to test for its
    //       presence)...

    mRecentFileNames.removeOne(pNewFileName);

    // A file has been created or saved under a new name, so we want the old
    // file name to be added to our list of recent files, i.e. as if it had been
    // closed

    fileClosed(pOldFileName);
}

//==============================================================================

void CorePlugin::fileClosed(const QString &pFileName)
{
    // Add the file to our list of recent files (making sure that we don't end
    // up with more than 10 recent file names) and update our Reopen sub-menu
    // Note: the most recent file is to be shown first...

    mRecentFileNames.prepend(pFileName);

    while (mRecentFileNames.count() > 10)
        mRecentFileNames.removeLast();

    updateFileReopenMenu();
}

//==============================================================================

bool CorePlugin::canClose()
{
    // To determine whether we can close, we must ask our central widget

    return mCentralWidget->canClose();
}

//==============================================================================
// I18n interface
//==============================================================================

void CorePlugin::retranslateUi()
{
    // Retranslate our different File actions

    retranslateAction(mFileOpenAction, tr("Open..."), tr("Open a file"));

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

    retranslateAction(mFileClearReopenSubMenuAction, tr("Clear Menu"),
                      tr("Clear the menu"));

    // Retranslate our central widget

    mCentralWidget->retranslateUi();
}

//==============================================================================
// Plugin specific
//==============================================================================

void CorePlugin::updateFileReopenMenu()
{
    // Update the contents of our Reopen sub-menu by first cleaning it

    foreach (QAction *action, mFileReopenSubMenu->actions()) {
        if (action != mFileReopenSubMenuSeparator)
            disconnect(action, SIGNAL(triggered()),
                       this, SLOT(openRecentFile()));
        else
            // We have reached our Reopen sub-menu separator, so...

            break;

        mFileReopenSubMenu->removeAction(action);

        delete action;
    }

    // Add the recent files to our Reopen sub-menu

    foreach (const QString &recentFile, mRecentFileNames) {
        QAction *action = newAction(mMainWindow);

        action->setText(recentFile);

        connect(action, SIGNAL(triggered()),
                this, SLOT(openRecentFile()));

        mFileReopenSubMenu->insertAction(mFileReopenSubMenuSeparator, action);
    }

    // Enable/disable mFileClearReopenSubMenuAction depending on whether we have
    // recent file names

    mFileClearReopenSubMenuAction->setEnabled(!mRecentFileNames.isEmpty());
}

//==============================================================================

void CorePlugin::retrieveBorderColor()
{
    // Retrieve the colour used for a 'normal' border
    // Note #1: we use a QStackedWidget object and retrieve the colour of the
    //          pixel which is in the middle of the right border...
    // Note #2: we don't rely on the top border because it may be rendered in a
    //          special way. Also, we don't rely on a corner as such in case
    //          it's rendered as a rounded corner...
    // Note #3: on OS X, our widget must be shown otherwise, the retrieved
    //          border colour will be black. So we show it off screen, even on
    //          Windows and Linux, in case their behaviour was to change in the
    //          future...

    // Create our widget and show it off screen

    QStackedWidget stackedWidget;

    stackedWidget.setFrameShape(QFrame::StyledPanel);

    stackedWidget.move(-2*stackedWidget.width(), -2*stackedWidget.height());
    stackedWidget.show();

    // Render the widget to an image

    QImage image = QImage(stackedWidget.size(),
                          QImage::Format_ARGB32_Premultiplied);

    stackedWidget.render(&image);

    // Retrieve the colour we are after

    QColor borderColor = QColor(image.pixel(image.width()-1, 0.5*image.height()));

    // Use our settings to keep track of the colour

    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        settings.setValue(SettingsBorderColor, borderColor);
    settings.endGroup();
}

//==============================================================================

void CorePlugin::retrieveColors()
{
    // Retrieve our border colour

    retrieveBorderColor();

    // Retrieve some other colours

    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        settings.setValue(SettingsBaseColor, qApp->palette().color(QPalette::Base));
        settings.setValue(SettingsWindowColor, qApp->palette().color(QPalette::Window));
        settings.setValue(SettingsHighlightColor, qApp->palette().color(QPalette::Highlight));
    settings.endGroup();
}

//==============================================================================

void CorePlugin::openRecentFile()
{
    // Check that the recent file still exists

    QString fileName = qobject_cast<QAction *>(sender())->text();

    if (!QFileInfo(fileName).exists()) {
        // The file doesn't exist anymore, so let the user know about it

        QMessageBox::warning(mMainWindow, tr("Reopen File"),
                             tr("Sorry, but <strong>%1</strong> does not exist anymore.").arg(fileName));

        // Remove the file from our list of recent files and update our Reopen
        // sub-menu

        mRecentFileNames.removeOne(fileName);

        updateFileReopenMenu();

        // Leave since we couldn't open the recent file

        return;
    }

    // Open the recent file

    mCentralWidget->openFile(fileName);
}

//==============================================================================

void CorePlugin::clearReopenSubMenu()
{
    // Indirectly clear our Reopen sub-menu

    mRecentFileNames.clear();

    updateFileReopenMenu();
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
