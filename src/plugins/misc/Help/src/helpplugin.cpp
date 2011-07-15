#include "helpplugin.h"
#include "helpwindow.h"

#include <QAction>

namespace OpenCOR {
namespace Help {

PLUGININFO_FUNC HelpPluginInfo()
{
    QMap<QString, QString> descriptions;

    descriptions.insert("en", "A plugin to provide help");
    descriptions.insert("fr", "Une extension pour fournir de l'aide");

    return PluginInfo(PluginInfo::Gui,
                       QStringList() << "Core",
                       descriptions);
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

HelpPlugin::HelpPlugin() :
    GuiInterface("Help")
{
}

void HelpPlugin::initialize(MainWindow *pMainWindow, GuiSettings *pSettings)
{
    // Add our help window to OpenCOR

    mHelpWindow = new HelpWindow(pMainWindow);

    // Create some actions that are to be registered by OpenCOR

    mHelpAction = newAction(pMainWindow, true,
                            ":/oxygen/apps/help-browser.png");

    pSettings->addAction(GuiSettingsAction::Help, mHelpAction);
}

void HelpPlugin::retranslateUi()
{
    // Retranslate our help window

    mHelpWindow->retranslateUi();

    // Retranslate our aciton

    mHelpAction->setText(tr("&Help"));
    mHelpAction->setStatusTip(tr("Show/hide the OpenCOR help"));
    mHelpAction->setShortcut(tr("F1"));
}

} }
