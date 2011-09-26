#include "coreeditingplugin.h"

#include <QMenu>
#include <QToolBar>

namespace OpenCOR {
namespace CoreEditing {

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

Q_EXPORT_PLUGIN2(CoreEditing, CoreEditingPlugin)

void CoreEditingPlugin::initialize()
{
    // Create our Edit menu and toolbar (and its show/hide action)

    mEditMenu = newMenu(mMainWindow, EditGroup);

    mEditToolbar = newToolBar(mMainWindow, EditGroup);
    mEditToolbarAction = newAction(mMainWindow, true);

    // Create our different Edit actions, and add them to our Edit menu and
    // some to our Edit toolbar

    mEditUndoAction = newAction(mMainWindow, false,
                                ":/oxygen/actions/edit-undo.png");
    mEditRedoAction = newAction(mMainWindow, false,
                                ":/oxygen/actions/edit-redo.png");

    mEditCutAction    = newAction(mMainWindow, false,
                                  ":/oxygen/actions/edit-cut.png");
    mEditCopyAction   = newAction(mMainWindow, false,
                                  ":/oxygen/actions/edit-copy.png");
    mEditPasteAction  = newAction(mMainWindow, false,
                                  ":/oxygen/actions/edit-paste.png");
    mEditDeleteAction = newAction(mMainWindow, false,
                                  ":/oxygen/actions/edit-delete.png");

    mEditFindAction     = newAction(mMainWindow, false,
                                    ":/oxygen/actions/edit-find.png");
    mEditFindNextAction = newAction(mMainWindow, false);
    mEditPreviousAction = newAction(mMainWindow, false);
    mEditReplaceAction  = newAction(mMainWindow, false);

    mEditSelectAllAction = newAction(mMainWindow, false);

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

    mEditToolbar->addAction(mEditUndoAction);
    mEditToolbar->addAction(mEditRedoAction);
    mEditToolbar->addSeparator();
    mEditToolbar->addAction(mEditCutAction);
    mEditToolbar->addAction(mEditCopyAction);
    mEditToolbar->addAction(mEditPasteAction);
    mEditToolbar->addAction(mEditDeleteAction);
    mEditToolbar->addSeparator();
    mEditToolbar->addAction(mEditFindAction);

    // Set our settings

    mGuiSettings->addMenu(GuiMenuSettings::View, mEditMenu);

    mGuiSettings->addToolBar(Qt::TopToolBarArea, mEditToolbar,
                             mEditToolbarAction);
}

void CoreEditingPlugin::retranslateUi()
{
    // Retranslate our Edit menu

    retranslateMenu(mEditMenu, tr("&Edit"));

    // Retranslate our different Edit actions

    retranslateAction(mEditUndoAction, tr("&Undo"),
                      tr("Undo the last action"),
                      tr("Ctrl+Z"));
    retranslateAction(mEditRedoAction, tr("&Redo"),
                      tr("Redo the last action"),
                      tr("Ctrl+Y"));

    retranslateAction(mEditCutAction, tr("Cu&t"),
                      tr("Cut the selected object"),
                      tr("Ctrl+X"));
    retranslateAction(mEditCopyAction, tr("&Copy"),
                      tr("Copy the selected object"),
                      tr("Ctrl+C"));
    retranslateAction(mEditPasteAction, tr("&Paste"),
                      tr("Paste the contents of the clipboard"),
                      tr("Ctrl+V"));
    retranslateAction(mEditDeleteAction, tr("&Delete"),
                      tr("Delete the selected object"),
                      tr("Del"));

    retranslateAction(mEditFindAction, tr("&Find..."),
                      tr("Search for a specific object"),
                      tr("Ctrl+F"));
    retranslateAction(mEditFindNextAction, tr("Find &Next"),
                      tr("Search forwards for the same object"),
                      tr("F3"));
    retranslateAction(mEditPreviousAction, tr("Find Pre&vious"),
                      tr("Search backwards for the same object"),
                      tr("Shift+F3"));
    retranslateAction(mEditReplaceAction, tr("Re&place"),
                      tr("Search for a specific object and replace it with another"),
                      tr("Ctrl+H"));

    retranslateAction(mEditSelectAllAction, tr("Select &All"),
                      tr("Select all the objects"),
                      tr("Ctrl+A"));

    // Retranslate our show/hide actions

    retranslateAction(mEditToolbarAction, tr("&Edit"),
                      tr("Show/hide the Edit toolbar"));
}

} }
