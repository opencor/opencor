#include "coreeditingplugin.h"

#include <QMenu>

namespace OpenCOR {
namespace CoreEditing {

PLUGININFO_FUNC CoreEditingPluginInfo()
{
    QMap<QString, QString> descriptions;

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

void CoreEditingPlugin::initialize(const QList<Plugin *> &,
                                   QMainWindow *pMainWindow)
{
    // Create our Edit menu

    mEdit = newMenu(pMainWindow);

    // Create our different Edit actions and add them to our Edit menu

    mEditUndo = newAction(pMainWindow, false,
                          ":/oxygen/actions/edit-undo.png");
    mEditRedo = newAction(pMainWindow, false,
                          ":/oxygen/actions/edit-redo.png");

    mEditCut    = newAction(pMainWindow, false,
                          ":/oxygen/actions/edit-cut.png");
    mEditCopy   = newAction(pMainWindow, false,
                          ":/oxygen/actions/edit-copy.png");
    mEditPaste  = newAction(pMainWindow, false,
                          ":/oxygen/actions/edit-paste.png");
    mEditDelete = newAction(pMainWindow, false,
                          ":/oxygen/actions/edit-delete.png");

    mEditFind     = newAction(pMainWindow, false,
                          ":/oxygen/actions/edit-find.png");
    mEditFindNext = newAction(pMainWindow, false);
    mEditPrevious = newAction(pMainWindow, false);
    mEditReplace  = newAction(pMainWindow, false);

    mEditSelectAll = newAction(pMainWindow, false);

    mEdit->addAction(mEditUndo);
    mEdit->addAction(mEditRedo);
    mEdit->addSeparator();
    mEdit->addAction(mEditCut);
    mEdit->addAction(mEditCopy);
    mEdit->addAction(mEditPaste);
    mEdit->addAction(mEditDelete);
    mEdit->addSeparator();
    mEdit->addAction(mEditFind);
    mEdit->addAction(mEditFindNext);
    mEdit->addAction(mEditPrevious);
    mEdit->addAction(mEditReplace);
    mEdit->addSeparator();
    mEdit->addAction(mEditSelectAll);

    // Set our settings

    mSettings->addMenu(GuiMenuSettings::View, mEdit);
}

void CoreEditingPlugin::retranslateUi()
{
    // Retranslate our Edit menu

    retranslateMenu(mEdit, tr("&Edit"));

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
