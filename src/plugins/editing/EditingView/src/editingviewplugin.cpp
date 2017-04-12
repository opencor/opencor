/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Editing view plugin
//==============================================================================

#include "coreguiutils.h"
#include "editingviewinterface.h"
#include "editingviewplugin.h"
#include "editorwidget.h"
#include "filemanager.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QMainWindow>
#include <QMenu>

//==============================================================================

namespace OpenCOR {
namespace EditingView {

//==============================================================================

PLUGININFO_FUNC EditingViewPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", QString::fromUtf8("a plugin that provides core editing view facilities."));
    descriptions.insert("fr", QString::fromUtf8("une extension qui fournit les fonctionalités de base d'une vue d'édition."));

    return new PluginInfo(PluginInfo::Editing, false, false,
                          QStringList() << "EditorWidget",
                          descriptions);
}

//==============================================================================

EditingViewPlugin::EditingViewPlugin() :
    mEditingViewInterface(0),
    mEditor(0),
    mFileName(QString())
{
}

//==============================================================================
// File handling interface
//==============================================================================

bool EditingViewPlugin::saveFile(const QString &pOldFileName,
                                 const QString &pNewFileName,
                                 bool &pNeedFeedback)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);
    Q_UNUSED(pNeedFeedback);

    // We don't handle this interface...

    return false;
}

//==============================================================================

void EditingViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::filePermissionsChanged(const QString &pFileName)
{
    // The given file has been un/locked, so update our GUI accordingly

    updateGui(pFileName);
}

//==============================================================================

void EditingViewPlugin::fileModified(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::fileReloaded(const QString &pFileName,
                                     const bool &pFileChanged)
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

    if (pFileChanged && !pFileName.compare(mFileName)) {
        mEditor = 0;
        mFileName = QString();
    }
}

//==============================================================================

void EditingViewPlugin::fileRenamed(const QString &pOldFileName,
                                    const QString &pNewFileName)
{
    // A file has been renamed, so update our internals, if needed

    if (!pOldFileName.compare(mFileName))
        mFileName = pNewFileName;
}

//==============================================================================

void EditingViewPlugin::fileClosed(const QString &pFileName)
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

void EditingViewPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    // Reset our previous editor and set up our new one, should the current
    // plugin handle the editing view interface

    mEditingViewInterface = pViewPlugin?qobject_cast<EditingViewInterface *>(pViewPlugin->instance()):0;

    if (mEditingViewInterface) {
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

        // Retrieve our new editor widget

        mEditor = mEditingViewInterface->editorWidget(pFileName);
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
    // view plugin handles the editing view interface

    Core::showEnableAction(mEditUndoAction, mEditingViewInterface, mEditor);
    Core::showEnableAction(mEditRedoAction, mEditingViewInterface, mEditor);

    Core::showEnableAction(mEditCutAction, mEditingViewInterface, mEditor);
    Core::showEnableAction(mEditCopyAction, mEditingViewInterface, mEditor);
    Core::showEnableAction(mEditPasteAction, mEditingViewInterface, mEditor);
    Core::showEnableAction(mEditDeleteAction, mEditingViewInterface, mEditor);

    Core::showEnableAction(mEditFindReplaceAction, mEditingViewInterface, mEditor);
    Core::showEnableAction(mEditFindNextAction, mEditingViewInterface, mEditor);
    Core::showEnableAction(mEditFindPreviousAction, mEditingViewInterface, mEditor);

    Core::showEnableAction(mEditSelectAllAction, mEditingViewInterface, mEditor);

    // Finish updating our GUI

    updateGui(pFileName);
}

//==============================================================================

Gui::Menus EditingViewPlugin::guiMenus() const
{
    // Return our menus

    return Gui::Menus() << Gui::Menu(Gui::Menu::View, mEditMenu);
}

//==============================================================================

Gui::MenuActions EditingViewPlugin::guiMenuActions() const
{
    // We don't handle this interface...

    return Gui::MenuActions();
}

//==============================================================================
// I18n interface
//==============================================================================

void EditingViewPlugin::retranslateUi()
{
    // Retranslate our Edit menu

    retranslateMenu(mEditMenu, tr("Edit"));

    // Unpopulate our Edit menu
    // Note #1: this is required on macOS otherwise we get several console
    //          messages that read
    //              void QCocoaMenu::insertNative(QCocoaMenuItem *, QCocoaMenuItem *) Menu item is already in a menu, remove it from the other menu first before inserting
    // Note #2: beforeAction is used to keep track of where our first action
    //          should really be. Indeed, on macOS, our Edit menu gets
    //          additional menu items (they are automatically added by macOS at
    //          the end of our Edit menu, should in the system locale), so we
    //          cannot just use mEditMenu->clear() to unpopulate our Edit menu
    //          since otherwise when repopulating it those extra menu items will
    //          end up before (rather than after) our menu items...

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

bool EditingViewPlugin::definesPluginInterfaces()
{
    // We define the editing view interface

    return true;
}

//==============================================================================

bool EditingViewPlugin::pluginInterfacesOk(const QString &pFileName,
                                           QObject *pInstance)
{
    // Make sure that the given plugin instance uses the right version of the
    // editing view interface, if it supports it

    return !(   qobject_cast<EditingViewInterface *>(pInstance)
             && (Plugin::interfaceVersion(pFileName, "editingViewInterfaceVersion") != editingViewInterfaceVersion()));
}

//==============================================================================

void EditingViewPlugin::initializePlugin()
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (!Core::mainWindow())
        return;

    // Create our Edit menu

    mEditMenu = Core::newMenu("Edit", Core::mainWindow());

    // Create our different Edit actions, and add them to our Edit menu

    mEditUndoAction = Core::newAction(QIcon(":/oxygen/actions/edit-undo.png"),
                                      QKeySequence::Undo, Core::mainWindow());
    mEditRedoAction = Core::newAction(QIcon(":/oxygen/actions/edit-redo.png"),
                                      QKeySequence::Redo, Core::mainWindow());

    mEditCutAction = Core::newAction(QIcon(":/oxygen/actions/edit-cut.png"),
                                     QKeySequence::Cut, Core::mainWindow());
    mEditCopyAction = Core::newAction(QIcon(":/oxygen/actions/edit-copy.png"),
                                      QKeySequence::Copy, Core::mainWindow());
    mEditPasteAction = Core::newAction(QIcon(":/oxygen/actions/edit-paste.png"),
                                       QKeySequence::Paste, Core::mainWindow());
    mEditDeleteAction = Core::newAction(QIcon(":/oxygen/actions/edit-delete.png"),
                                        QKeySequence::Delete, Core::mainWindow());

    mEditFindReplaceAction = Core::newAction(QIcon(":/oxygen/actions/edit-find.png"),
                                             QKeySequence::Find, Core::mainWindow());
    mEditFindNextAction = Core::newAction(QKeySequence::FindNext, Core::mainWindow());
    mEditFindPreviousAction = Core::newAction(QKeySequence::FindPrevious, Core::mainWindow());

    mEditSelectAllAction = Core::newAction(QKeySequence::SelectAll, Core::mainWindow());

    populateEditMenu();

    // Keep track of changes to the clipboard

    connect(QApplication::clipboard(), SIGNAL(dataChanged()),
            this, SLOT(clipboardDataChanged()));

    // Some connections to handle our different editing actions

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

void EditingViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::handleUrl(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// Plugin specific
//==============================================================================

void EditingViewPlugin::updateGui(const QString &pFileName)
{
    // Update some actions and make our editor read-only or writable, if needed

    if (mEditingViewInterface && !pFileName.compare(mFileName)) {
        // Update some actions

        updateUndoAndRedoActions();
        updateEditingActions();
        updateFindPreviousNextActions();
        updateSelectAllAction();

        // Make our editor widget read-only or writable

        if (mEditor) {
            mEditor->setReadOnly(   !Core::FileManager::instance()->isReadableAndWritable(pFileName)
                                 || !mEditingViewInterface->isEditorWidgetUseable(pFileName));
        }
    }
}

//==============================================================================

void EditingViewPlugin::unpopulateEditMenu()
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

    mEditMenu->removeAction(mEditSelectAllAction);
}

//==============================================================================

void EditingViewPlugin::populateEditMenu(QAction *pBeforeAction)
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

void EditingViewPlugin::clipboardDataChanged()
{
    // Enable our paste action if the clipboard contains some text

    if (mEditingViewInterface) {
        mEditPasteAction->setEnabled(    mEditor
                                     &&  Core::FileManager::instance()->isReadableAndWritable(mFileName)
                                     && !QApplication::clipboard()->text().isEmpty());
    }
}

//==============================================================================

void EditingViewPlugin::updateUndoAndRedoActions()
{
    // Update our undo/redo actions, and update the modified state of the
    // current file

    if (mEditingViewInterface) {
        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        bool editorAndFileReadableAndWritable =    mEditor
                                                && fileManagerInstance->isReadableAndWritable(mFileName);

        mEditUndoAction->setEnabled(editorAndFileReadableAndWritable && mEditor->isUndoAvailable());
        mEditRedoAction->setEnabled(editorAndFileReadableAndWritable && mEditor->isRedoAvailable());

        fileManagerInstance->setModified(mFileName,
                                         mEditor && mEditingViewInterface->isEditorWidgetContentsModified(mFileName));
    }
}

//==============================================================================

void EditingViewPlugin::updateEditingActions()
{
    // Update our editing actions

    if (mEditingViewInterface) {
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

void EditingViewPlugin::updateFindPreviousNextActions()
{
    // Update our find previous and next actions

    if (mEditingViewInterface) {
        bool canFindReplace =    mEditor
                              && mEditor->isFindPreviousNextAvailable();

        mEditFindPreviousAction->setEnabled(canFindReplace);
        mEditFindNextAction->setEnabled(canFindReplace);
    }
}

//==============================================================================

void EditingViewPlugin::updateSelectAllAction()
{
    // Update our select all action

    if (mEditingViewInterface) {
        mEditSelectAllAction->setEnabled(   mEditor
                                         && mEditor->isSelectAllAvailable());
    }
}

//==============================================================================

void EditingViewPlugin::doUndo()
{
    // Undo the last action and update our undo/redo actions, should there be an
    // editor

    if (!mEditor)
        return;

    mEditor->undo();

    updateUndoAndRedoActions();
}

//==============================================================================

void EditingViewPlugin::doRedo()
{
    // Redo the last action and update our undo/redo actions, should there be an
    // editor

    if (!mEditor)
        return;

    mEditor->redo();

    updateUndoAndRedoActions();
}

//==============================================================================

void EditingViewPlugin::doCut()
{
    // Cut the text and update our undo/redo actions, should there be an editor

    if (!mEditor)
        return;

    mEditor->cut();

    updateUndoAndRedoActions();
}

//==============================================================================

void EditingViewPlugin::doCopy()
{
    // Copy the text and update our undo/redo actions, should there be an editor

    if (!mEditor)
        return;

    mEditor->copy();
}

//==============================================================================

void EditingViewPlugin::doPaste()
{
    // Paste the text and update our undo/redo actions, should there be an
    // editor

    if (!mEditor)
        return;

    mEditor->paste();

    updateUndoAndRedoActions();
}

//==============================================================================

void EditingViewPlugin::doDelete()
{
    // Delete the text and update our undo/redo actions, should there be an
    // editor

    if (!mEditor)
        return;

    mEditor->del();

    updateUndoAndRedoActions();
}

//==============================================================================

void EditingViewPlugin::doFindReplace()
{
    // Show/select the find/replace widget in our editor

    if (mEditor)
        mEditor->setFindReplaceVisible(true);
}

//==============================================================================

void EditingViewPlugin::doFindNext()
{
    // Find the next occurrence of the text in our editor

    if (mEditor)
        mEditor->findNext();
}

//==============================================================================

void EditingViewPlugin::doFindPrevious()
{
    // Find the previous occurrence of the text in our editor

    if (mEditor)
        mEditor->findPrevious();
}

//==============================================================================

void EditingViewPlugin::doSelectAll()
{
    // Select all the text and update our select all action, should there be an
    // editor

    if (!mEditor)
        return;

    mEditor->selectAll();

    updateSelectAllAction();
}

//==============================================================================

}   // namespace EditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
