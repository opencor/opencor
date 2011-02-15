#include "commandviewerwindow.h"
#include "commandviewerwidget.h"

#include "ui_commandviewerwindow.h"

#define SETTINGS_COMMANDVIEWERWINDOW "CommandViewerWindow"

CommandViewerWindow::CommandViewerWindow(QWidget *pParent) :
    QDockWidget(pParent),
    mUi(new Ui::CommandViewerWindow)
{
    // Set up the UI

    mUi->setupUi(this);

    // Create and add the command viewer widget

    mCommandViewerWidget = new CommandViewerWidget();

    mUi->verticalLayout->addWidget(mCommandViewerWidget);

    // Prevent objects from being dropped on the window

    setAcceptDrops(false);
}

CommandViewerWindow::~CommandViewerWindow()
{
    // Delete the UI

    delete mUi;
}

void CommandViewerWindow::retranslateUi()
{
    // Translate the whole window

    mUi->retranslateUi(this);

    // Retranslate the command viewer widget

    mCommandViewerWidget->retranslateUi();
}

void CommandViewerWindow::defaultSettings()
{
    // Set the default settings for the command viewer widget

    mCommandViewerWidget->defaultSettings();
}

void CommandViewerWindow::loadSettings(const QSettings &pSettings,
                                       const QString &)
{
    // Retrieve the settings of the command viewer widget

    mCommandViewerWidget->loadSettings(pSettings, SETTINGS_COMMANDVIEWERWINDOW);
}

void CommandViewerWindow::saveSettings(QSettings &pSettings, const QString &)
{
    // Keep track of the settings of the command viewer widget

    mCommandViewerWidget->saveSettings(pSettings, SETTINGS_COMMANDVIEWERWINDOW);
}
