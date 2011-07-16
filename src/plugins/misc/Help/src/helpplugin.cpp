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

    return PluginInfo(PluginInfo::Gui, true,
                       QStringList() << "Core",
                       descriptions);
}

Q_EXPORT_PLUGIN2(Help, HelpPlugin)

HelpPlugin::HelpPlugin() :
    GuiInterface("Help")
{
}

void HelpPlugin::initialize(MainWindow *pMainWindow)
{
    // Create our help window

    mHelpWindow = new HelpWindow(pMainWindow);

    mSettings.addDockWidget(Qt::RightDockWidgetArea, mHelpWindow);

    // Create an action to show/hide our help window

    mHelpAction = newAction(pMainWindow, true,
                            ":/oxygen/apps/help-browser.png");

    mSettings.addAction(GuiSettingsAction::Help, mHelpAction);

    // Some connections to handle the visibility of our help window

    connect(mHelpAction, SIGNAL(triggered(bool)),
            mHelpWindow, SLOT(setVisible(bool)));
    connect(mHelpWindow, SIGNAL(visibilityChanged(bool)),
            mHelpAction, SLOT(setChecked(bool)));
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
