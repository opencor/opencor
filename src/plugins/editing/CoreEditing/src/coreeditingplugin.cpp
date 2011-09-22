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

    return PluginInfo(PluginInterface::V001,
                      PluginInfo::Gui,
                      PluginInfo::Editing,
                      false,
                      QStringList() << "Core",
                      descriptions);
}

Q_EXPORT_PLUGIN2(CoreEditing, CoreEditingPlugin)

CoreEditingPlugin::CoreEditingPlugin() :
    GuiInterface("CoreEditing")
{
}

void CoreEditingPlugin::initialize()
{
    // Create our Edit menu and toolbar

    mEditMenu    = newMenu(mMainWindow, EditMenu);
    mEditToolbar = newToolBar(mMainWindow, EditMenu);

    // Create our different Edit actions, and add them to our Edit menu and
    // some to our Edit toolbar

    mEditUndo = newAction(mMainWindow, false,
                          ":/oxygen/actions/edit-undo.png");
    mEditRedo = newAction(mMainWindow, false,
                          ":/oxygen/actions/edit-redo.png");

    mEditCut    = newAction(mMainWindow, false,
                          ":/oxygen/actions/edit-cut.png");
    mEditCopy   = newAction(mMainWindow, false,
                          ":/oxygen/actions/edit-copy.png");
    mEditPaste  = newAction(mMainWindow, false,
                          ":/oxygen/actions/edit-paste.png");
    mEditDelete = newAction(mMainWindow, false,
                          ":/oxygen/actions/edit-delete.png");

    mEditFind     = newAction(mMainWindow, false,
                          ":/oxygen/actions/edit-find.png");
    mEditFindNext = newAction(mMainWindow, false);
    mEditPrevious = newAction(mMainWindow, false);
    mEditReplace  = newAction(mMainWindow, false);

    mEditSelectAll = newAction(mMainWindow, false);

    mEditMenu->addAction(mEditUndo);
    mEditMenu->addAction(mEditRedo);
    mEditMenu->addSeparator();
    mEditMenu->addAction(mEditCut);
    mEditMenu->addAction(mEditCopy);
    mEditMenu->addAction(mEditPaste);
    mEditMenu->addAction(mEditDelete);
    mEditMenu->addSeparator();
    mEditMenu->addAction(mEditFind);
    mEditMenu->addAction(mEditFindNext);
    mEditMenu->addAction(mEditPrevious);
    mEditMenu->addAction(mEditReplace);
    mEditMenu->addSeparator();
    mEditMenu->addAction(mEditSelectAll);

    mEditToolbar->addAction(mEditUndo);
    mEditToolbar->addAction(mEditRedo);
    mEditToolbar->addSeparator();
    mEditToolbar->addAction(mEditCut);
    mEditToolbar->addAction(mEditCopy);
    mEditToolbar->addAction(mEditPaste);
    mEditToolbar->addAction(mEditDelete);
    mEditToolbar->addSeparator();
    mEditToolbar->addAction(mEditFind);

    // Set our settings

    mGuiSettings->addMenu(GuiMenuSettings::View, mEditMenu);
    mGuiSettings->addToolBar(Qt::TopToolBarArea, mEditToolbar);
}

void CoreEditingPlugin::retranslateUi()
{
    // Retranslate our Edit menu

    retranslateMenu(mEditMenu, tr("&Edit"));

    // Retranslate our different Edit actions

    retranslateAction(mEditUndo, tr("&Undo"),
                      tr("Undo the last action"),
                      tr("Ctrl+Z"));
    retranslateAction(mEditRedo, tr("&Redo"),
                      tr("Redo the last action"),
                      tr("Ctrl+Y"));

    retranslateAction(mEditCut, tr("Cu&t"),
                      tr("Cut the selected object"),
                      tr("Ctrl+X"));
    retranslateAction(mEditCopy, tr("&Copy"),
                      tr("Copy the selected object"),
                      tr("Ctrl+C"));
    retranslateAction(mEditPaste, tr("&Paste"),
                      tr("Paste the contents of the clipboard"),
                      tr("Ctrl+V"));
    retranslateAction(mEditDelete, tr("&Delete"),
                      tr("Delete the selected object"),
                      tr("Del"));

    retranslateAction(mEditFind, tr("&Find..."),
                      tr("Search for a specific object"),
                      tr("Ctrl+F"));
    retranslateAction(mEditFindNext, tr("Find &Next"),
                      tr("Search forwards for the same object"),
                      tr("F3"));
    retranslateAction(mEditPrevious, tr("Find Pre&vious"),
                      tr("Search backwards for the same object"),
                      tr("Shift+F3"));
    retranslateAction(mEditReplace, tr("Re&place"),
                      tr("Search for a specific object and replace it with another"),
                      tr("Ctrl+H"));

    retranslateAction(mEditSelectAll, tr("Select &All"),
                      tr("Select all the objects"),
                      tr("Ctrl+A"));
}

} }
