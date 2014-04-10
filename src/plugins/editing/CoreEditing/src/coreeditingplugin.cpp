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

    return new PluginInfo(PluginInfo::Editing, false,
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
// Core interface
//==============================================================================

void CoreEditingPlugin::initialize()
{
    // Create our different File|New actions

    mFileNewFileAction = newAction(QIcon(":/oxygen/actions/document-new.png"),
                                   QKeySequence::New, mMainWindow);

    // Create our Edit menu

    mEditMenu = newMenu("Edit", mMainWindow);

    // Create our different Edit actions, and add them to our Edit menu

    mEditUndoAction = newAction(QIcon(":/oxygen/actions/edit-undo.png"),
                                QKeySequence::Undo, mMainWindow);
    mEditRedoAction = newAction(QIcon(":/oxygen/actions/edit-redo.png"),
                                QKeySequence::Redo, mMainWindow);

    mEditCutAction    = newAction(QIcon(":/oxygen/actions/edit-cut.png"),
                                  QKeySequence::Cut, mMainWindow);
    mEditCopyAction   = newAction(QIcon(":/oxygen/actions/edit-copy.png"),
                                  QKeySequence::Copy, mMainWindow);
    mEditPasteAction  = newAction(QIcon(":/oxygen/actions/edit-paste.png"),
                                  QKeySequence::Paste, mMainWindow);
    mEditDeleteAction = newAction(QIcon(":/oxygen/actions/edit-delete.png"),
                                  QKeySequence::Delete, mMainWindow);

    mEditFindReplaceAction  = newAction(QIcon(":/oxygen/actions/edit-find.png"),
                                        QKeySequence::Find, mMainWindow);
    mEditFindNextAction     = newAction(QKeySequence::FindNext, mMainWindow);
    mEditFindPreviousAction = newAction(QKeySequence::FindPrevious, mMainWindow);

    mEditSelectAllAction = newAction(QKeySequence::SelectAll, mMainWindow);

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

    // Set our settings

    mGuiSettings->addMenuAction(GuiMenuActionSettings::FileNew, mFileNewFileAction);

    mGuiSettings->addMenu(GuiMenuSettings::View, mEditMenu);

    // Keep track of changes to the clipboard

    connect(QApplication::clipboard(), SIGNAL(dataChanged()),
            this, SLOT(clipboardDataChanged()));

    // Some connections to handle our different editing actions

    connect(mFileNewFileAction, SIGNAL(triggered()),
            this, SLOT(newFile()));

    connect(mEditUndoAction, SIGNAL(triggered()),
            this, SLOT(doUndo()));
    connect(mEditRedoAction, SIGNAL(triggered()),
            this, SLOT(doRedo()));

    connect(mEditCutAction, SIGNAL(triggered()),
            this, SLOT(doCut()));
    connect(mEditCopyAction, SIGNAL(triggered()),
            this, SLOT(doCopy()));
    connect(mEditPasteAction, SIGNAL(triggered()),
            this, SLOT(doPaste()));
    connect(mEditDeleteAction, SIGNAL(triggered()),
            this, SLOT(doDelete()));

    connect(mEditFindReplaceAction, SIGNAL(triggered()),
            this, SLOT(doFindReplace()));
    connect(mEditFindNextAction, SIGNAL(triggered()),
            this, SLOT(doFindNext()));
    connect(mEditFindPreviousAction, SIGNAL(triggered()),
            this, SLOT(doFindPrevious()));

    connect(mEditSelectAllAction, SIGNAL(triggered()),
            this, SLOT(doSelectAll()));
}

//==============================================================================

void CoreEditingPlugin::finalize()
{
    // We don't handle this interface...
}

//==============================================================================

void CoreEditingPlugin::initialized(const Plugins &pLoadedPlugins)
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

void CoreEditingPlugin::settingsLoaded(const Plugins &pLoadedPlugins)
{
    Q_UNUSED(pLoadedPlugins);

    // We don't handle this interface...
}

//==============================================================================

void CoreEditingPlugin::handleArguments(const QStringList &pArguments)
{
    Q_UNUSED(pArguments);

    // We don't handle this interface...
}

//==============================================================================

void CoreEditingPlugin::handleAction(const QUrl &pUrl)
{
    Q_UNUSED(pUrl);

    // We don't handle this interface...
}

//==============================================================================
// GUI interface
//==============================================================================

void CoreEditingPlugin::changeEvent(QEvent *pEvent)
{
    Q_UNUSED(pEvent);

    // We don't handle this interface...
}

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

    bool hasEditingInterfaceAndEditor = mEditingInterface && mEditor;

    Core::showEnableAction(mFileNewFileAction, mEditingInterface);

    Core::showEnableAction(mEditUndoAction, hasEditingInterfaceAndEditor);
    Core::showEnableAction(mEditRedoAction, hasEditingInterfaceAndEditor);

    Core::showEnableAction(mEditCutAction, hasEditingInterfaceAndEditor);
    Core::showEnableAction(mEditCopyAction, hasEditingInterfaceAndEditor);
    Core::showEnableAction(mEditPasteAction, hasEditingInterfaceAndEditor);
    Core::showEnableAction(mEditDeleteAction, hasEditingInterfaceAndEditor);

    Core::showEnableAction(mEditFindReplaceAction, hasEditingInterfaceAndEditor);
    Core::showEnableAction(mEditFindNextAction, hasEditingInterfaceAndEditor);
    Core::showEnableAction(mEditFindPreviousAction, hasEditingInterfaceAndEditor);

    Core::showEnableAction(mEditSelectAllAction, hasEditingInterfaceAndEditor);

    // Enable/disable some of our actions, if we have both an editing interface
    // and an editor

    if (hasEditingInterfaceAndEditor) {
        updateUndoAndRedoActions();
        updateEditingActions();
        updateFindPreviousNextActions();
        updateSelectAllAction();
    }
}

//==============================================================================

void CoreEditingPlugin::initializeView()
{
    // We don't handle this interface...
}

//==============================================================================

void CoreEditingPlugin::finalizeView()
{
    // We don't handle this interface...
}

//==============================================================================

bool CoreEditingPlugin::hasViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return false;
}

//==============================================================================

QWidget * CoreEditingPlugin::viewWidget(const QString &pFileName,
                                        const bool &pCreate)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pCreate);

    // We don't handle this interface...

    return 0;
}

//==============================================================================

void CoreEditingPlugin::removeViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...
}

//==============================================================================

QString CoreEditingPlugin::viewName() const
{
    // We don't handle this interface...

    return QString();
}

//==============================================================================

QIcon CoreEditingPlugin::fileTabIcon(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // We don't handle this interface...

    return QIcon();
}

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

void CoreEditingPlugin::fileModified(const QString &pFileName,
                                     const bool &pModified)
{
    Q_UNUSED(pFileName);
    Q_UNUSED(pModified);

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
    //       its original editor will be discarded and a new one assigned to it,
    //       so...

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

bool CoreEditingPlugin::canClose()
{
    // We don't handle this interface...

    return true;
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
}

//==============================================================================
// Plugin specific
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
    // current file (since it can be determined by whether we can undo)

    if (mEditingInterface) {
        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        bool editorAndFileReadableAndWritable =    mEditor
                                                && fileManagerInstance->isReadableAndWritable(mFileName);

        mEditUndoAction->setEnabled(editorAndFileReadableAndWritable && mEditor->isUndoAvailable());
        mEditRedoAction->setEnabled(editorAndFileReadableAndWritable && mEditor->isRedoAvailable());

        fileManagerInstance->setModified(mFileName,
                                         mEditor && mEditor->isUndoAvailable());
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
        mEditor->showFindReplace();
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
