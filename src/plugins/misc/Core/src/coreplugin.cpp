#include "centralwidget.h"
#include "coreplugin.h"

#include <QAction>

namespace OpenCOR {
namespace Core {

PLUGININFO_FUNC CorePluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "The core plugin for OpenCOR");
    descriptions.insert("fr", "L'extension de base pour OpenCOR");

    return PluginInfo(PluginInterface::V001, PluginInfo::General, true,
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

    // Create our help window

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

    // Retranslate our central widget

    mCentralWidget->retranslateUi();
}

} }
