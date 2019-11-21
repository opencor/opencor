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
// Editing view plugin
//==============================================================================

#include "corecliutils.h"
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

    return new PluginInfo(PluginInfo::Category::Editing, false, false,
                          QStringList() << "EditorWidget",
                          descriptions);
}

//==============================================================================
// File handling interface
//==============================================================================

bool EditingViewPlugin::importFile(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...

    return false;
}

//==============================================================================

bool EditingViewPlugin::saveFile(const QString &pOldFileName,
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

void EditingViewPlugin::fileOpened(const QString &pFileName)
{
    Q_UNUSED(pFileName)

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
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::fileSaved(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::fileReloaded(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::fileRenamed(const QString &pOldFileName,
                                    const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName)
    Q_UNUSED(pNewFileName)

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::fileClosed(const QString &pFileName)
{
    Q_UNUSED(pFileName)

    // We don't handle this interface...
}

//==============================================================================
// GUI interface
//==============================================================================

void EditingViewPlugin::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    // Reset our previous editor and set up our new one, should the current
    // plugin handle the editing view interface

    mEditingViewInterface = (pViewPlugin != nullptr)?qobject_cast<EditingViewInterface *>(pViewPlugin->instance()):nullptr;

    if (mEditingViewInterface != nullptr) {
        // Retrieve our new editor widget

        mEditor = mEditingViewInterface->editorWidget(pFileName);
        mFileName = pFileName;

        // Set our new editor's context menu, connections and background
        // Note: we pass Qt::UniqueConnection in our call to connect() so that
        //       we don't end up with several identical connections (something
        //       that might happen if we were to switch views and back)...

        if (mEditor != nullptr) {
            mEditor->setContextMenu(mEditMenu->actions());

            connect(mEditor, &EditorWidget::EditorWidget::textChanged,
                    this, &EditingViewPlugin::updateUndoAndRedoActions,
                    Qt::UniqueConnection);
            connect(mEditor, &EditorWidget::EditorWidget::copyAvailable,
                    this, &EditingViewPlugin::updateEditingActions,
                    Qt::UniqueConnection);
            connect(mEditor, &EditorWidget::EditorWidget::canFindReplace,
                    this, &EditingViewPlugin::updateFindPreviousNextActions,
                    Qt::UniqueConnection);
            connect(mEditor, &EditorWidget::EditorWidget::canSelectAll,
                    this, &EditingViewPlugin::updateSelectAllAction,
                    Qt::UniqueConnection);

            mEditWordWrapAction->setChecked(mEditor->wordWrap());
        }
    } else {
        mEditor = nullptr;
        mFileName = QString();
    }

    // Show/enable or hide/disable various actions, depending on whether the
    // view plugin handles the editing view interface

    Core::showEnableAction(mEditUndoAction, mEditingViewInterface != nullptr, mEditor != nullptr);
    Core::showEnableAction(mEditRedoAction, mEditingViewInterface != nullptr, mEditor != nullptr);

    Core::showEnableAction(mEditCutAction, mEditingViewInterface != nullptr, mEditor != nullptr);
    Core::showEnableAction(mEditCopyAction, mEditingViewInterface != nullptr, mEditor != nullptr);
    Core::showEnableAction(mEditPasteAction, mEditingViewInterface != nullptr, mEditor != nullptr);
    Core::showEnableAction(mEditDeleteAction, mEditingViewInterface != nullptr, mEditor != nullptr);

    Core::showEnableAction(mEditFindReplaceAction, mEditingViewInterface != nullptr, mEditor != nullptr);
    Core::showEnableAction(mEditFindNextAction, mEditingViewInterface != nullptr, mEditor != nullptr);
    Core::showEnableAction(mEditFindPreviousAction, mEditingViewInterface != nullptr, mEditor != nullptr);

    Core::showEnableAction(mEditSelectAllAction, mEditingViewInterface != nullptr, mEditor != nullptr);

    Core::showEnableAction(mEditWordWrapAction, mEditingViewInterface != nullptr, mEditor != nullptr);

    // Finish updating our GUI

    updateGui(pFileName);
}

//==============================================================================

Gui::Menus EditingViewPlugin::guiMenus() const
{
    // Return our menus

    return Gui::Menus() << Gui::Menu(Gui::Menu::Type::View, mEditMenu);
}

//==============================================================================

Gui::MenuActions EditingViewPlugin::guiMenuActions() const
{
    // We don't handle this interface...

    return {};
}

//==============================================================================
// I18n interface
//==============================================================================

void EditingViewPlugin::retranslateUi()
{
    // Retranslate our Edit menu

    retranslateMenu(mEditMenu, tr("Edit"));

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

    retranslateAction(mEditWordWrapAction, tr("Word Wrap"),
                      tr("Word wrap the text"));
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

    return !(   (qobject_cast<EditingViewInterface *>(pInstance) != nullptr)
             && (Plugin::interfaceVersion(pFileName, "editingViewInterfaceVersion") != editingViewInterfaceVersion()));
}

//==============================================================================

void EditingViewPlugin::initializePlugin()
{
    // What we are doing below requires to be in GUI mode, so leave if we are
    // not in that mode

    if (Core::mainWindow() == nullptr) {
        return;
    }

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

    mEditWordWrapAction = Core::newAction(true, Core::mainWindow());

    // Populate our Edit menu

    mEditMenu->addAction(mEditUndoAction);
    mEditMenu->addAction(mEditRedoAction);
    mEditMenu->addSeparator();
    mEditMenu->addAction(mEditCutAction);
    mEditMenu->addAction(mEditCopyAction);
    mEditMenu->addAction(mEditPasteAction);
    mEditMenu->addAction(mEditDeleteAction);
    mEditMenu->addSeparator();
    mEditMenu->addAction(mEditFindReplaceAction);
    mEditMenu->addAction(mEditFindNextAction);
    mEditMenu->addAction(mEditFindPreviousAction);
    mEditMenu->addSeparator();
    mEditMenu->addAction(mEditSelectAllAction);
    mEditMenu->addSeparator();
    mEditMenu->addAction(mEditWordWrapAction);

    // Keep track of changes to the clipboard

    connect(QApplication::clipboard(), &QClipboard::dataChanged,
            this, &EditingViewPlugin::clipboardDataChanged);

    // Some connections to handle our different editing actions

    connect(mEditUndoAction, &QAction::triggered,
            this, &EditingViewPlugin::doUndo);
    connect(mEditRedoAction, &QAction::triggered,
            this, &EditingViewPlugin::doRedo);

    connect(mEditCutAction, &QAction::triggered,
            this, &EditingViewPlugin::doCut);
    connect(mEditCopyAction, &QAction::triggered,
            this, &EditingViewPlugin::doCopy);
    connect(mEditPasteAction, &QAction::triggered,
            this, &EditingViewPlugin::doPaste);
    connect(mEditDeleteAction, &QAction::triggered,
            this, &EditingViewPlugin::doDelete);

    connect(mEditFindReplaceAction, &QAction::triggered,
            this, &EditingViewPlugin::doFindReplace);
    connect(mEditFindNextAction, &QAction::triggered,
            this, &EditingViewPlugin::doFindNext);
    connect(mEditFindPreviousAction, &QAction::triggered,
            this, &EditingViewPlugin::doFindPrevious);

    connect(mEditSelectAllAction, &QAction::triggered,
            this, &EditingViewPlugin::doSelectAll);

    connect(mEditWordWrapAction, &QAction::triggered,
            this, &EditingViewPlugin::doWordWrap);
}

//==============================================================================

void EditingViewPlugin::finalizePlugin()
{
    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::pluginsInitialized(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins)

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::loadSettings(QSettings &pSettings)
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)

    // We don't handle this interface...
}

//==============================================================================

void EditingViewPlugin::handleUrl(const QUrl &pUrl)
{
    // Handle the action that was passed to us

    QString actionName = pUrl.authority();

    if (actionName.compare("EditingView.setCursorPosition", Qt::CaseInsensitive) == 0) {
        // We want to set the cursor position of our editor, should there be one

        if (mEditor != nullptr) {
            QStringList cursorPosition = Core::urlArguments(pUrl).split("|");

            if (cursorPosition.count() == 2) {
                mEditor->setCursorPosition(cursorPosition[0].toInt()-1, cursorPosition[1].toInt()-1);
            }
        }
    }
}

//==============================================================================
// Plugin specific
//==============================================================================

void EditingViewPlugin::updateGui(const QString &pFileName)
{
    // Update some actions and make our editor read-only or writable, if needed

    if ((mEditingViewInterface != nullptr) && (pFileName == mFileName)) {
        // Update some actions

        updateUndoAndRedoActions();
        updateEditingActions();
        updateFindPreviousNextActions();
        updateSelectAllAction();

        // Make our editor widget read-only or writable

        if (mEditor != nullptr) {
            mEditor->setReadOnly(   !Core::FileManager::instance()->isReadableAndWritable(pFileName)
                                 || !mEditingViewInterface->isEditorWidgetUseable(pFileName));
        }
    }
}

//==============================================================================

void EditingViewPlugin::clipboardDataChanged()
{
    // Enable our paste action if the clipboard contains some text

    if (mEditingViewInterface != nullptr) {
        mEditPasteAction->setEnabled(    Core::FileManager::instance()->isReadableAndWritable(mFileName)
                                     && !QApplication::clipboard()->text().isEmpty());
    }
}

//==============================================================================

void EditingViewPlugin::updateUndoAndRedoActions()
{
    // Make sure that our editor allows us to handle connections

    if ((mEditor == nullptr) || !mEditor->handleEditorChanges()) {
        return;
    }

    // Update our undo/redo actions, and update the modified state of the
    // current file

    if (mEditingViewInterface != nullptr) {
        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        bool editorAndFileReadableAndWritable = (mEditor != nullptr) && fileManagerInstance->isReadableAndWritable(mFileName);

        mEditUndoAction->setEnabled(editorAndFileReadableAndWritable && mEditor->isUndoAvailable());
        mEditRedoAction->setEnabled(editorAndFileReadableAndWritable && mEditor->isRedoAvailable());

        fileManagerInstance->setModified(mFileName, mEditingViewInterface->isEditorWidgetContentsModified(mFileName));
    }
}

//==============================================================================

void EditingViewPlugin::updateEditingActions()
{
    // Make sure that our editor allows us to handle connections

    if ((mEditor == nullptr) || !mEditor->handleEditorChanges()) {
        return;
    }

    // Update our editing actions

    if (mEditingViewInterface != nullptr) {
        bool hasSelectedText = (mEditor != nullptr) && mEditor->hasSelectedText();
        bool fileReadableOrWritable = Core::FileManager::instance()->isReadableAndWritable(mFileName);

        mEditCutAction->setEnabled(hasSelectedText && fileReadableOrWritable);
        mEditCopyAction->setEnabled(hasSelectedText);
        clipboardDataChanged();
        mEditDeleteAction->setEnabled(hasSelectedText && fileReadableOrWritable);
    }
}

//==============================================================================

void EditingViewPlugin::updateFindPreviousNextActions()
{
    // Update our find previous and next actions

    if (mEditingViewInterface != nullptr) {
        bool findPreviousNextAvailable = (mEditor != nullptr) && mEditor->isFindPreviousNextAvailable();

        mEditFindPreviousAction->setEnabled(findPreviousNextAvailable);
        mEditFindNextAction->setEnabled(findPreviousNextAvailable);
    }
}

//==============================================================================

void EditingViewPlugin::updateSelectAllAction()
{
    // Make sure that our editor allows us to handle connections

    if ((mEditor == nullptr) || !mEditor->handleEditorChanges()) {
        return;
    }

    // Update our select all action

    if (mEditingViewInterface != nullptr) {
        mEditSelectAllAction->setEnabled((mEditor != nullptr) && mEditor->isSelectAllAvailable());
    }
}

//==============================================================================

void EditingViewPlugin::doUndo()
{
    // Undo the last action and update our undo/redo actions, should there be an
    // editor

    mEditor->undo();

    updateUndoAndRedoActions();
}

//==============================================================================

void EditingViewPlugin::doRedo()
{
    // Redo the last action and update our undo/redo actions, should there be an
    // editor

    mEditor->redo();

    updateUndoAndRedoActions();
}

//==============================================================================

void EditingViewPlugin::doCut()
{
    // Cut the text and update our undo/redo actions, should there be an editor

    mEditor->cut();

    updateUndoAndRedoActions();
}

//==============================================================================

void EditingViewPlugin::doCopy()
{
    // Copy the text and update our undo/redo actions, should there be an editor

    mEditor->copy();
}

//==============================================================================

void EditingViewPlugin::doPaste()
{
    // Paste the text and update our undo/redo actions, should there be an
    // editor

    mEditor->paste();

    updateUndoAndRedoActions();
}

//==============================================================================

void EditingViewPlugin::doDelete()
{
    // Delete the text and update our undo/redo actions, should there be an
    // editor

    mEditor->del();

    updateUndoAndRedoActions();
}

//==============================================================================

void EditingViewPlugin::doFindReplace()
{
    // Show/select the find/replace widget in our editor

    mEditor->setFindReplaceVisible(true);
}

//==============================================================================

void EditingViewPlugin::doFindNext()
{
    // Find the next occurrence of the text in our editor

    mEditor->findNext();
}

//==============================================================================

void EditingViewPlugin::doFindPrevious()
{
    // Find the previous occurrence of the text in our editor

    mEditor->findPrevious();
}

//==============================================================================

void EditingViewPlugin::doSelectAll()
{
    // Select all the text and update our select all action, should there be an
    // editor

    mEditor->selectAll();

    updateSelectAllAction();
}

//==============================================================================

void EditingViewPlugin::doWordWrap()
{
    // Word wrap (or not) the text

    mEditor->setWordWrap(mEditWordWrapAction->isChecked());
}

//==============================================================================

} // namespace EditingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
