#include "centralwidget.h"
#include "coreplugin.h"

#include <QAction>
#include <QMenu>

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "The core plugin for OpenCOR");
    descriptions.insert("fr", "L'extension de base pour OpenCOR");

    return PluginInfo(PluginInterface::V001, PluginInfo::General, false,
                      QStringList(),
                      descriptions);
}

Q_EXPORT_PLUGIN2(Core, CorePlugin)

CorePlugin::CorePlugin() :
    GuiInterface("Core")
{
}

void CorePlugin::initialize(QMainWindow *pMainWindow)
{
    // Create our different File actions

    mFileOpen = newAction(pMainWindow, false,
                          ":/oxygen/actions/document-open.png");

    mFileSave    = newAction(pMainWindow, false,
                             ":/oxygen/actions/document-save.png");
    mFileSaveAs  = newAction(pMainWindow, false,
                             ":/oxygen/actions/document-save-all.png");
    mFileSaveAll = newAction(pMainWindow, false,
                             ":/oxygen/actions/document-save-as.png");

    mFileClose    = newAction(pMainWindow, false,
                              ":/oxygen/actions/document-close.png");
    mFileCloseAll = newAction(pMainWindow);

    mFilePrint = newAction(pMainWindow, false,
                          ":/oxygen/actions/document-open.png");

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

    // Create our central widget

    mCentralWidget = new CentralWidget(pMainWindow);

    // Create and set our data

    GuiSettingsCorePlugin *guiSettingsCorePlugin;

    mData = guiSettingsCorePlugin = new GuiSettingsCorePlugin(mCentralWidget);

    guiSettingsCorePlugin->addAction(GuiSettingsCoreAction::File, mFileOpen);
    guiSettingsCorePlugin->addAction(GuiSettingsCoreAction::File);
    guiSettingsCorePlugin->addAction(GuiSettingsCoreAction::File, mFileSave);
    guiSettingsCorePlugin->addAction(GuiSettingsCoreAction::File, mFileSaveAs);
    guiSettingsCorePlugin->addAction(GuiSettingsCoreAction::File, mFileSaveAll);
    guiSettingsCorePlugin->addAction(GuiSettingsCoreAction::File);
    guiSettingsCorePlugin->addAction(GuiSettingsCoreAction::File, mFileClose);
    guiSettingsCorePlugin->addAction(GuiSettingsCoreAction::File, mFileCloseAll);
    guiSettingsCorePlugin->addAction(GuiSettingsCoreAction::File);
    guiSettingsCorePlugin->addAction(GuiSettingsCoreAction::File, mFilePrint);
    guiSettingsCorePlugin->addAction(GuiSettingsCoreAction::File);

    guiSettingsCorePlugin->addMenu(GuiSettingsCoreMenu::View, mEdit);
}

void CorePlugin::finalize()
{
    // Delete our data

    delete (GuiSettingsCorePlugin *) mData;
}

void CorePlugin::retranslateUi()
{
    // Retranslate our different File actions

    retranslateAction(mFileOpen, tr("&Open"),
                      tr("Open a file"),
                      tr("Ctrl+O"));

    retranslateAction(mFileSave, tr("&Save"),
                      tr("Save the current file"),
                      tr("Ctrl+S"));
    retranslateAction(mFileSaveAs, tr("Save &As..."),
                      tr("Save the current file under a different name"));
    retranslateAction(mFileSaveAll, tr("Save All"),
                      tr("Save all the files"));

    retranslateAction(mFileClose, tr("Clos&e"),
                      tr("Close the current file"),
                      tr("Ctrl+W"));
    retranslateAction(mFileCloseAll, tr("Close All"),
                      tr("Close all the files"));

    retranslateAction(mFilePrint, tr("&Print..."),
                      tr("Print the current file"),
                      tr("Ctrl+P"));

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

    // Retranslate our central widget

    mCentralWidget->retranslateUi();
}

} }
