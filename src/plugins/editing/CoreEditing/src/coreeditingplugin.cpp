//==============================================================================
// CoreEditing plugin
//==============================================================================

#include "coreeditingplugin.h"

//==============================================================================

#include <QMenu>

//==============================================================================

namespace OpenCOR {
namespace CoreEditing {

//==============================================================================

PLUGININFO_FUNC CoreEditingPluginInfo()
{
    Descriptions descriptions;

    descriptions.insert("en", "The core editing plugin for OpenCOR");
    descriptions.insert("fr", "L'extension d'édition de base pour OpenCOR");

    return PluginInfo(PluginInfo::V001,
                      PluginInfo::Gui,
                      PluginInfo::Editing,
                      false,
                      QStringList() << "Core",
                      descriptions);
}

//==============================================================================

Q_EXPORT_PLUGIN2(CoreEditing, CoreEditingPlugin)

//==============================================================================

void CoreEditingPlugin::initialize()
{
    // Create our Edit menu

    mEditMenu = newMenu(mMainWindow, EditGroup);

    // Create our different Edit actions, and add them to our Edit menu

    mEditUndoAction = newAction(mMainWindow, false,
                                ":/oxygen/actions/edit-undo.png",
                                QKeySequence::Undo);
    mEditRedoAction = newAction(mMainWindow, false,
                                ":/oxygen/actions/edit-redo.png",
                                QKeySequence::Redo);

    mEditCutAction    = newAction(mMainWindow, false,
                                  ":/oxygen/actions/edit-cut.png",
                                  QKeySequence::Cut);
    mEditCopyAction   = newAction(mMainWindow, false,
                                  ":/oxygen/actions/edit-copy.png",
                                  QKeySequence::Copy);
    mEditPasteAction  = newAction(mMainWindow, false,
                                  ":/oxygen/actions/edit-paste.png",
                                  QKeySequence::Paste);
    mEditDeleteAction = newAction(mMainWindow, false,
                                  ":/oxygen/actions/edit-delete.png",
                                  QKeySequence::Delete);

    mEditFindAction     = newAction(mMainWindow, false,
                                    ":/oxygen/actions/edit-find.png",
                                    QKeySequence::Find);
    mEditFindNextAction = newAction(mMainWindow, false, "",
                                    QKeySequence::FindNext);
    mEditPreviousAction = newAction(mMainWindow, false, "",
                                    QKeySequence::FindPrevious);
    mEditReplaceAction  = newAction(mMainWindow, false, "",
                                    QKeySequence::Replace);

    mEditSelectAllAction = newAction(mMainWindow, false, "",
                                     QKeySequence::SelectAll);

    mEditMenu->addAction(mEditUndoAction);
    mEditMenu->addAction(mEditRedoAction);
    mEditMenu->addSeparator();
    mEditMenu->addAction(mEditCutAction);
    mEditMenu->addAction(mEditCopyAction);
    mEditMenu->addAction(mEditPasteAction);
    mEditMenu->addAction(mEditDeleteAction);
    mEditMenu->addSeparator();
    mEditMenu->addAction(mEditFindAction);
    mEditMenu->addAction(mEditFindNextAction);
    mEditMenu->addAction(mEditPreviousAction);
    mEditMenu->addAction(mEditReplaceAction);
    mEditMenu->addSeparator();
    mEditMenu->addAction(mEditSelectAllAction);

    // Set our settings

    mGuiSettings->addMenu(GuiMenuSettings::View, mEditMenu);

    // Initialise the enabled state of our various actions

    updateActions();
}

//==============================================================================

void CoreEditingPlugin::retranslateUi()
{
    // Retranslate our Edit menu

    retranslateMenu(mEditMenu, tr("&Edit"));

    // Retranslate our different Edit actions

    retranslateAction(mEditUndoAction, tr("&Undo"),
                      tr("Undo the last action"));
    retranslateAction(mEditRedoAction, tr("&Redo"),
                      tr("Redo the last action"));

    retranslateAction(mEditCutAction, tr("Cu&t"),
                      tr("Cut the selected object"));
    retranslateAction(mEditCopyAction, tr("&Copy"),
                      tr("Copy the selected object"));
    retranslateAction(mEditPasteAction, tr("&Paste"),
                      tr("Paste the contents of the clipboard"));
    retranslateAction(mEditDeleteAction, tr("&Delete"),
                      tr("Delete the selected object"));

    retranslateAction(mEditFindAction, tr("&Find..."),
                      tr("Search for a specific object"));
    retranslateAction(mEditFindNextAction, tr("Find &Next"),
                      tr("Search forwards for the same object"));
    retranslateAction(mEditPreviousAction, tr("Find Pre&vious"),
                      tr("Search backwards for the same object"));
    retranslateAction(mEditReplaceAction, tr("Re&place"),
                      tr("Search for a specific object and replace it with another"));

    retranslateAction(mEditSelectAllAction, tr("Select &All"),
                      tr("Select all the objects"));
}

//==============================================================================

void CoreEditingPlugin::updateActions()
{
    // Make sure that the various actions are properly enabled/disabled

    mEditUndoAction->setEnabled(false);
    mEditRedoAction->setEnabled(false);

    mEditCutAction->setEnabled(false);
    mEditCopyAction->setEnabled(false);
    mEditPasteAction->setEnabled(false);
    mEditDeleteAction->setEnabled(false);

    mEditFindAction->setEnabled(false);
    mEditFindNextAction->setEnabled(false);
    mEditPreviousAction->setEnabled(false);
    mEditReplaceAction->setEnabled(false);

    mEditSelectAllAction->setEnabled(false);
}

//==============================================================================

}   // namespace CoreEditing
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
