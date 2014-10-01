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
// CoreEditing plugin
//==============================================================================

#include "coreeditingplugin.h"
#include "editinginterface.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "guiutils.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QMainWindow>
#include <QMenu>

//==============================================================================

namespace OpenCOR {
namespace CoreEditing {

//==============================================================================

PLUGININFO_FUNC CoreEditingPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("the core editing plugin."));
    descriptions.insert("fr", QString::fromUtf8("l'extension d'édition de base."));

    return new PluginInfo(PluginInfo::Editing, false, false,
                          QStringList() << "Editor",
                          descriptions);
}

//==============================================================================

CoreEditingPlugin::CoreEditingPlugin() :
    mEditingInterface(0),
    mEditor(0),
    mFileName(QString())
{
}

//==============================================================================
// File handling interface
//==============================================================================

bool CoreEditingPlugin::saveFile(const QString &pOldFileName,
                                 const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void CoreEditingPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CoreEditingPlugin::filePermissionsChanged(const QString &pFileName)
{
    // Update some actions and make our editor read-only or writable, if needed

    if (mEditingInterface && !pFileName.compare(mFileName)) {
        // Update some actions

        updateUndoAndRedoActions();
        updateEditingActions();
        updateFindPreviousNextActions();
        updateSelectAllAction();

        // Make our editor read-only or writable

        if (mEditor)
            mEditor->setReadOnly(!Core::FileManager::instance()->isReadableAndWritable(pFileName));
    }
}

//==============================================================================

void CoreEditingPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void CoreEditingPlugin::fileReloaded(const QString &pFileName)
{
    // A file has been reloaded, so update our internals, if needed
    // Note: we clearly still have an editor for the given file, but when
    //       reloading it, fileReloaded() will first be called for this plugin
    //       and then for the 'proper' editing plugins. In other words, we don't
    //       know at this stage whether a new editor will have been assigned to
    //       the given file. Indeed, upon reloading a file, a 'proper' editing
    //       plugin may decide to 'close' and 'reopen' the file, meaning that
    //       its original editor will be discarded and a new one assigned to
    //       it, hence we need to reset things...

    if (!pFileName.compare(mFileName)) {
        mEditor = 0;
        mFileName = QString();
    }
}

//==============================================================================

void CoreEditingPlugin::fileRenamed(const QString &pOldFileName,
                                    const QString &pNewFileName)
{
    // A file has been renamed, so update our internals, if needed

    if (!pOldFileName.compare(mFileName))
        mFileName = pNewFileName;
}

//==============================================================================

void CoreEditingPlugin::fileClosed(const QString &pFileName)
{
    // A file has been closed, so update our internals, if needed

    if (!pFileName.compare(mFileName)) {
        mEditor = 0;
        mFileName = QString();
    }
}

//==============================================================================
// GUI interface
//==============================================================================

void CoreEditingPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    // Reset our previous editor and set up our new one, should the current
    // plugin handle the editing interface

    mEditingInterface = pViewPlugin?qobject_cast<EditingInterface *>(pViewPlugin->instance()):0;

    if (mEditingInterface) {
        // Reset our previous editor's connections

        if (mEditor) {
            disconnect(mEditor, SIGNAL(textChanged()),
                       this, SLOT(updateUndoAndRedoActions()));
            disconnect(mEditor, SIGNAL(copyAvailable(const bool &)),
                       this, SLOT(updateEditingActions()));
            disconnect(mEditor, SIGNAL(canFindReplace(const bool &)),
                       this, SLOT(updateFindPreviousNextActions()));
            disconnect(mEditor, SIGNAL(canSelectAll(const bool &)),
                       this, SLOT(updateSelectAllAction()));
        }

        // Retrieve our new editor

        mEditor = mEditingInterface->editor(pFileName);
        mFileName = pFileName;

        // Set our new editor's context menu, connections and background

        if (mEditor) {
            mEditor->setContextMenu(mEditMenu->actions());

            connect(mEditor, SIGNAL(textChanged()),
                    this, SLOT(updateUndoAndRedoActions()));
            connect(mEditor, SIGNAL(copyAvailable(const bool &)),
                    this, SLOT(updateEditingActions()));
            connect(mEditor, SIGNAL(canFindReplace(const bool &)),
                    this, SLOT(updateFindPreviousNextActions()));
            connect(mEditor, SIGNAL(canSelectAll(const bool &)),
                    this, SLOT(updateSelectAllAction()));
        }
    }

    // Show/enable or hide/disable various actions, depending on whether the
    // view plugin handles the editing interface

    Core::showEnableAction(mFileNewFileAction, mEditingInterface);

    Core::showEnableAction(mEditUndoAction, mEditingInterface, mEditor);
    Core::showEnableAction(mEditRedoAction, mEditingInterface, mEditor);

    Core::showEnableAction(mEditCutAction, mEditingInterface, mEditor);
    Core::showEnableAction(mEditCopyAction, mEditingInterface, mEditor);
    Core::showEnableAction(mEditPasteAction, mEditingInterface, mEditor);
    Core::showEnableAction(mEditDeleteAction, mEditingInterface, mEditor);

    Core::showEnableAction(mEditFindReplaceAction, mEditingInterface, mEditor);
    Core::showEnableAction(mEditFindNextAction, mEditingInterface, mEditor);
    Core::showEnableAction(mEditFindPreviousAction, mEditingInterface, mEditor);

    Core::showEnableAction(mEditSelectAllAction, mEditingInterface, mEditor);

    // Enable/disable some of our actions, if we have both an editing interface
    // and an editor

    if (mEditingInterface && mEditor) {
        updateUndoAndRedoActions();
        updateEditingActions();
        updateFindPreviousNextActions();
        updateSelectAllAction();
    }
}

//==============================================================================

Gui::Menus CoreEditingPlugin::guiMenus() const
{
    // Return our menus

    return Gui::Menus() << Gui::Menu(Gui::Menu::View, mEditMenu);
}

//==============================================================================

Gui::MenuActions CoreEditingPlugin::guiMenuActions() const
{
    // Return our menu actions

    return Gui::MenuActions() << Gui::MenuAction(Gui::MenuAction::FileNew, mFileNewFileAction);
}

//==============================================================================
// I18n interface
//==============================================================================

void CoreEditingPlugin::retranslateUi()
{
    // Retranslate our different File|New action

    retranslateAction(mFileNewFileAction, tr("File"),
                      tr("Create a new file"));

    // Retranslate our Edit menu

    retranslateMenu(mEditMenu, tr("Edit"));

    // Unpopulate our Edit menu
    // Note #1: this is required on OS X otherwise we get several console
    //          messages that read
    //              void QCocoaMenu::insertNative(QCocoaMenuItem *, QCocoaMenuItem *) Menu item is already in a menu, remove it from the other menu first before inserting
    // Note #2: beforeAction is used to keep track of where our first action
    //          should really be. Indeed, on OS X, our Edit menu gets additional
    //          menu items (they are automatically added by OS X at the end of
    //          our Edit menu, should in the system locale), so we cannot just
    //          use mEditMenu->clear() to unpopulate our Edit menu since
    //          otherwise when repopulating it those extra menu items will end
    //          up before (rather than after) our menu items...

    QAction *beforeAction = mEditMenu->insertSeparator(mEditUndoAction);

    unpopulateEditMenu();

    // Retranslate our different Edit actions

    retranslateAction(mEditUndoAction, tr("Undo"),
                      tr("Undo the last action"));
    retranslateAction(mEditRedoAction, tr("Redo"),
                      tr("Redo the last action"));

    retranslateAction(mEditCutAction, tr("Cut"),
                      tr("Cut the selected text"));
    retranslateAction(mEditCopyAction, tr("Copy"),
                      tr("Copy the selected text to the clipboard"));
    retranslateAction(mEditPasteAction, tr("Paste"),
                      tr("Paste the contents of the clipboard"));
    retranslateAction(mEditDeleteAction, tr("Delete"),
                      tr("Delete the selected text"));

    retranslateAction(mEditFindReplaceAction, tr("Find/Replace..."),
                      tr("Search/replace a specific text"));
    retranslateAction(mEditFindNextAction, tr("Find Next"),
                      tr("Search forwards for the same text"));
    retranslateAction(mEditFindPreviousAction, tr("Find Previous"),
                      tr("Search backwards for the same text"));

    retranslateAction(mEditSelectAllAction, tr("Select All"),
                      tr("Select all the text"));

    // (Re)populate our Edit menu

    populateEditMenu(beforeAction);

    mEditMenu->removeAction(beforeAction);
}

//==============================================================================
// Plugin interface
//==============================================================================

void CoreEditingPlugin::initializePlugin(QMainWindow *pMainWindow)
{
    // Create our different File|New actions

    mFileNewFileAction = Core::newAction(QIcon(":/oxygen/actions/document-new.png"),
                                         QKeySequence::New, pMainWindow);

    // Create our Edit menu

    mEditMenu = Core::newMenu("Edit", pMainWindow);

    // Create our different Edit actions, and add them to our Edit menu

    mEditUndoAction = Core::newAction(QIcon(":/oxygen/actions/edit-undo.png"),
                                      QKeySequence::Undo, pMainWindow);
    mEditRedoAction = Core::newAction(QIcon(":/oxygen/actions/edit-redo.png"),
                                      QKeySequence::Redo, pMainWindow);

    mEditCutAction    = Core::newAction(QIcon(":/oxygen/actions/edit-cut.png"),
                                        QKeySequence::Cut, pMainWindow);
    mEditCopyAction   = Core::newAction(QIcon(":/oxygen/actions/edit-copy.png"),
                                        QKeySequence::Copy, pMainWindow);
    mEditPasteAction  = Core::newAction(QIcon(":/oxygen/actions/edit-paste.png"),
                                        QKeySequence::Paste, pMainWindow);
    mEditDeleteAction = Core::newAction(QIcon(":/oxygen/actions/edit-delete.png"),
                                        QKeySequence::Delete, pMainWindow);

    mEditFindReplaceAction  = Core::newAction(QIcon(":/oxygen/actions/edit-find.png"),
                                              QKeySequence::Find, pMainWindow);
    mEditFindNextAction     = Core::newAction(QKeySequence::FindNext, pMainWindow);
    mEditFindPreviousAction = Core::newAction(QKeySequence::FindPrevious, pMainWindow);

    mEditSelectAllAction = Core::newAction(QKeySequence::SelectAll, pMainWindow);

    populateEditMenu();

    // Keep track of changes to the clipboard

    connect(QApplication::clipboard(), SIGNAL(dataChanged()),
            this, SLOT(clipboardDataChanged()));

    // Some connections to handle our different editing actions

    connect(mFileNewFileAction, SIGNAL(triggered(bool)),
            this, SLOT(newFile()));

    connect(mEditUndoAction, SIGNAL(triggered(bool)),
            this, SLOT(doUndo()));
    connect(mEditRedoAction, SIGNAL(triggered(bool)),
            this, SLOT(doRedo()));

    connect(mEditCutAction, SIGNAL(triggered(bool)),
            this, SLOT(doCut()));
    connect(mEditCopyAction, SIGNAL(triggered(bool)),
            this, SLOT(doCopy()));
    connect(mEditPasteAction, SIGNAL(triggered(bool)),
            this, SLOT(doPaste()));
    connect(mEditDeleteAction, SIGNAL(triggered(bool)),
            this, SLOT(doDelete()));

    connect(mEditFindReplaceAction, SIGNAL(triggered(bool)),
            this, SLOT(doFindReplace()));
    connect(mEditFindNextAction, SIGNAL(triggered(bool)),
            this, SLOT(doFindNext()));
    connect(mEditFindPreviousAction, SIGNAL(triggered(bool)),
            this, SLOT(doFindPrevious()));

    connect(mEditSelectAllAction, SIGNAL(triggered(bool)),
            this, SLOT(doSelectAll()));
}

//==============================================================================

void CoreEditingPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void CoreEditingPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CoreEditingPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CoreEditingPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void CoreEditingPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void CoreEditingPlugin::unpopulateEditMenu()
{
    // Unpopulate our Edit menu
    // Note: we do not want to use mEditMenu->clear() since we want to keep our
    //       tracker, so we can repopulate our Edit menu without any problem...

    mEditMenu->removeAction(mEditUndoAction);
    mEditMenu->removeAction(mEditRedoAction);

    mEditMenu->removeAction(mEditCutAction);
    mEditMenu->removeAction(mEditCopyAction);
    mEditMenu->removeAction(mEditPasteAction);
    mEditMenu->removeAction(mEditDeleteAction);

    mEditMenu->removeAction(mEditFindReplaceAction);
    mEditMenu->removeAction(mEditFindNextAction);
    mEditMenu->removeAction(mEditFindPreviousAction);

    mEditMenu->addAction(mEditSelectAllAction);
}

//==============================================================================

void CoreEditingPlugin::populateEditMenu(QAction *pBeforeAction)
{
    // Populate our Edit menu

    mEditMenu->insertAction(pBeforeAction, mEditUndoAction);
    mEditMenu->insertAction(pBeforeAction, mEditRedoAction);
    mEditMenu->insertSeparator(pBeforeAction);
    mEditMenu->insertAction(pBeforeAction, mEditCutAction);
    mEditMenu->insertAction(pBeforeAction, mEditCopyAction);
    mEditMenu->insertAction(pBeforeAction, mEditPasteAction);
    mEditMenu->insertAction(pBeforeAction, mEditDeleteAction);
    mEditMenu->insertSeparator(pBeforeAction);
    mEditMenu->insertAction(pBeforeAction, mEditFindReplaceAction);
    mEditMenu->insertAction(pBeforeAction, mEditFindNextAction);
    mEditMenu->insertAction(pBeforeAction, mEditFindPreviousAction);
    mEditMenu->insertSeparator(pBeforeAction);
    mEditMenu->insertAction(pBeforeAction, mEditSelectAllAction);
}

//==============================================================================

void CoreEditingPlugin::clipboardDataChanged()
{
    // Enable our paste action if the clipboard contains some text

    if (mEditingInterface)
        mEditPasteAction->setEnabled(   mEditor
                                     && Core::FileManager::instance()->isReadableAndWritable(mFileName)
                                     && QApplication::clipboard()->text().size());
}

//==============================================================================

void CoreEditingPlugin::updateUndoAndRedoActions()
{
    // Update our undo/redo actions, and update the modified state of the
    // current file

    if (mEditingInterface) {
        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        bool editorAndFileReadableAndWritable =    mEditor
                                                && fileManagerInstance->isReadableAndWritable(mFileName);

        mEditUndoAction->setEnabled(editorAndFileReadableAndWritable && mEditor->isUndoAvailable());
        mEditRedoAction->setEnabled(editorAndFileReadableAndWritable && mEditor->isRedoAvailable());

        fileManagerInstance->setModified(mFileName,
                                         mEditor && fileManagerInstance->isDifferent(mFileName, mEditor->contents()));
    }
}

//==============================================================================

void CoreEditingPlugin::updateEditingActions()
{
    // Update our editing actions

    if (mEditingInterface) {
        bool editorAndHasSelectedText =    mEditor
                                        && mEditor->hasSelectedText();
        bool fileReadableOrWritable = Core::FileManager::instance()->isReadableAndWritable(mFileName);

        mEditCutAction->setEnabled(editorAndHasSelectedText && fileReadableOrWritable);
        mEditCopyAction->setEnabled(editorAndHasSelectedText);
        clipboardDataChanged();
        mEditDeleteAction->setEnabled(editorAndHasSelectedText && fileReadableOrWritable);
    }
}

//==============================================================================

void CoreEditingPlugin::updateFindPreviousNextActions()
{
    // Update our find previous and next actions

    if (mEditingInterface) {
        bool canFindReplace =    mEditor
                              && mEditor->isFindPreviousNextAvailable();

        mEditFindPreviousAction->setEnabled(canFindReplace);
        mEditFindNextAction->setEnabled(canFindReplace);
    }
}

//==============================================================================

void CoreEditingPlugin::updateSelectAllAction()
{
    // Update our select all action

    if (mEditingInterface)
        mEditSelectAllAction->setEnabled(   mEditor
                                         && mEditor->isSelectAllAvailable());
}

//==============================================================================

void CoreEditingPlugin::newFile()
{
    // Ask our file manager to create a new file

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();
#ifdef QT_DEBUG
    Core::FileManager::Status createStatus =
#endif
    fileManagerInstance->create();

#ifdef QT_DEBUG
    // Make sure that the file has indeed been created

    if (createStatus != Core::FileManager::Created)
        qFatal("FATAL ERROR | %s:%d: the new file was not created", __FILE__, __LINE__);
#endif
}

//==============================================================================

void CoreEditingPlugin::doUndo()
{
    // Undo the last action and update our undo/redo actions, should there be an
    // editor

    if (!mEditor)
        return;

    mEditor->undo();

    updateUndoAndRedoActions();
}

//==============================================================================

void CoreEditingPlugin::doRedo()
{
    // Redo the last action and update our undo/redo actions, should there be an
    // editor

    if (!mEditor)
        return;

    mEditor->redo();

    updateUndoAndRedoActions();
}

//==============================================================================

void CoreEditingPlugin::doCut()
{
    // Cut the text and update our undo/redo actions, should there be an editor

    if (!mEditor)
        return;

    mEditor->cut();

    updateUndoAndRedoActions();
}

//==============================================================================

void CoreEditingPlugin::doCopy()
{
    // Copy the text and update our undo/redo actions, should there be an editor

    if (!mEditor)
        return;

    mEditor->copy();
}

//==============================================================================

void CoreEditingPlugin::doPaste()
{
    // Paste the text and update our undo/redo actions, should there be an
    // editor

    if (!mEditor)
        return;

    mEditor->paste();

    updateUndoAndRedoActions();
}

//==============================================================================

void CoreEditingPlugin::doDelete()
{
    // Delete the text and update our undo/redo actions, should there be an
    // editor

    if (!mEditor)
        return;

    mEditor->del();

    updateUndoAndRedoActions();
}

//==============================================================================

void CoreEditingPlugin::doFindReplace()
{
    // Show/select the find/replace widget in our editor

    if (mEditor)
        mEditor->setFindReplaceVisible(true);
}

//==============================================================================

void CoreEditingPlugin::doFindNext()
{
    // Find the next occurrence of the text in our editor

    if (mEditor)
        mEditor->findNext();
}

//==============================================================================

void CoreEditingPlugin::doFindPrevious()
{
    // Find the previous occurrence of the text in our editor

    if (mEditor)
        mEditor->findPrevious();
}

//==============================================================================

void CoreEditingPlugin::doSelectAll()
{
    // Select all the text and update our select all action, should there be an
    // editor

    if (!mEditor)
        return;

    mEditor->selectAll();

    updateSelectAllAction();
}

//==============================================================================

}   // namespace CoreEditing
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
