/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// PMR Workspaces window
//==============================================================================

#include "borderedwidget.h"
#include "coreguiutils.h"
#include "filemanager.h"
#include "i18ninterface.h"
#include "toolbarwidget.h"
#include "pmrsupportpreferenceswidget.h"
#include "pmrwebservice.h"
#include "pmrworkspacemanager.h"
#include "pmrworkspaceswindownewworkspacedialog.h"
#include "pmrworkspaceswindowwidget.h"
#include "pmrworkspaceswindowwindow.h"
#include "preferencesinterface.h"

//==============================================================================

#include "ui_pmrworkspaceswindowwindow.h"

//==============================================================================

#include <QDesktopServices>
#include <QDir>
#include <QGraphicsColorizeEffect>
#include <QLabel>
#include <QMainWindow>
#include <QSettings>
#include <QTimer>
#include <QUrl>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PmrWorkspacesWindowWindow::PmrWorkspacesWindowWindow(QWidget *pParent) :
    Core::OrganisationWidget(pParent),
    mGui(new Ui::PmrWorkspacesWindowWindow),
    mInitialized(false),
    mSettingsGroup(QString()),
    mAuthenticated(false),
    mWaitingForPmrWebService(false)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a tool bar widget with different actions
    // Note #1: normally, we would retrieve the folder icon through a call to
    //          QFileIconProvider().icon(QFileIconProvider::Folder), but on
    //          Windows it will, in this case, return the QStyle::SP_DirIcon
    //          icon while we really want the QStyle::SP_DirClosedIcon icon...
    // Note #2: we right-align our PMR-related action and make it ready for
    //          colorisation effect...

    static const QIcon PlusIcon = QIcon(":/oxygen/actions/list-add.png");
    static const QIcon UserIcon = QIcon(":/oxygen/apps/preferences-desktop-user-password.png");

    static const int UserIconWidth = UserIcon.availableSizes().first().width();
    static const int UserIconHeight = UserIcon.availableSizes().first().height();

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);
    QIcon folderIcon = QApplication::style()->standardIcon(QStyle::SP_DirClosedIcon);
    int folderIconSize = folderIcon.availableSizes().first().width();
    int plusIconSize = 0.57*folderIconSize;

    mGui->actionNew->setIcon(Core::overlayedIcon(folderIcon, PlusIcon,
                                                 folderIconSize, folderIconSize,
                                                 folderIconSize-plusIconSize, 0,
                                                 plusIconSize, plusIconSize));

    toolBarWidget->addAction(mGui->actionNew);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionReload);
    toolBarWidget->addSeparator();
    toolBarWidget->addAction(mGui->actionPreferences);

    QWidget *spacer = new QWidget(toolBarWidget);

    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    toolBarWidget->addWidget(spacer);
    toolBarWidget->addAction(mGui->actionPmr);

    mLoggedOnIcon = Core::tintedIcon(UserIcon, UserIconWidth, UserIconHeight, Qt::darkGreen);
    mLoggedOffIcon = Core::tintedIcon(UserIcon, UserIconWidth, UserIconHeight, Qt::darkRed);

    mGui->layout->addWidget(toolBarWidget);

    // Create and add a label to highlight the repository we are using

    mPmrInstanceLabel = new QLabel(this);

    QFont newFont = mPmrInstanceLabel->font();

    newFont.setPointSize(0.75*newFont.pointSize());

    mPmrInstanceLabel->setAlignment(Qt::AlignCenter);
    mPmrInstanceLabel->setEnabled(false);
    mPmrInstanceLabel->setFont(newFont);

    mGui->layout->addWidget(mPmrInstanceLabel);

    // Create an instance of our PMR web service

    mPmrWebService = new PMRSupport::PmrWebService(this);

    // Create and add our workspaces widget

    mPmrWorkspacesWindowWidget = new PmrWorkspacesWindowWidget(mPmrUrl, mPmrWebService, this);

    mPmrWorkspacesWindowWidget->setObjectName("PmrWorkspacesWindowWidget");

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mPmrWorkspacesWindowWidget,
                                                     true, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->layout->addWidget(new Core::BorderedWidget(mPmrWorkspacesWindowWidget,
                                                     true, false, false, false));
#else
    #error Unsupported platform
#endif

    // Initialise (update) our PMR URL

    update(PreferencesInterface::preference(PMRSupport::PluginName,
                                            PMRSupport::SettingsPreferencesPmrUrl,
                                            PMRSupport::SettingsPreferencesPmrUrlDefault).toString());

    // Keep track of the window's visibility, so that we can request the list of
    // workspaces, if necessary

    connect(this, SIGNAL(visibilityChanged(bool)),
            this, SLOT(retrieveWorkspaces(const bool &)));

    // Some connections to process responses from our PMR web service

    connect(mPmrWebService, SIGNAL(busy(const bool &)),
            this, SLOT(busy(const bool &)));

    connect(mPmrWebService, SIGNAL(information(const QString &)),
            this, SLOT(showInformation(const QString &)));
    connect(mPmrWebService, SIGNAL(warning(const QString &)),
            this, SLOT(showWarning(const QString &)));
    connect(mPmrWebService, SIGNAL(error(const QString &)),
            this, SLOT(showError(const QString &)));

    connect(mPmrWebService, SIGNAL(authenticated(const bool &)),
            this, SLOT(updateGui()));
    connect(mPmrWebService, SIGNAL(authenticationCancelled()),
            this, SLOT(updateGui()));

    connect(mPmrWebService, SIGNAL(workspaces(const PMRSupport::PmrWorkspaces &)),
            mPmrWorkspacesWindowWidget, SLOT(initialize(const PMRSupport::PmrWorkspaces &)));

    // Connections to process requests from our PMR workspaces widget

    connect(mPmrWorkspacesWindowWidget, SIGNAL(information(const QString &)),
            this, SLOT(showInformation(const QString &)));
    connect(mPmrWorkspacesWindowWidget, SIGNAL(warning(const QString &)),
            this, SLOT(showWarning(const QString &)));

    connect(mPmrWorkspacesWindowWidget, SIGNAL(openFileRequested(const QString &)),
            this, SLOT(openFile(const QString &)));
    connect(mPmrWorkspacesWindowWidget, SIGNAL(openFilesRequested(const QStringList &)),
            this, SLOT(openFiles(const QStringList &)));

    // Retranslate our GUI

    retranslateUi();

    mInitialized = true;
}

//==============================================================================

PmrWorkspacesWindowWindow::~PmrWorkspacesWindowWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PmrWorkspacesWindowWindow::retranslateUi()
{
    // Retranslate our whole window

    mGui->retranslateUi(this);

    retranslateActionPmr();

    mPmrInstanceLabel->setText(mPmrWebService->siteName());

    // Retranslate our workspaces widget

    mPmrWorkspacesWindowWidget->retranslateUi();
}

//==============================================================================

void PmrWorkspacesWindowWindow::loadSettings(QSettings *pSettings)
{
    // Keep track of our settings' group

    mSettingsGroup = pSettings->group();

    // Retrieve the settings of the workspaces window widget

    pSettings->beginGroup(mPmrWorkspacesWindowWidget->objectName());
        mPmrWorkspacesWindowWidget->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PmrWorkspacesWindowWindow::saveSettings(QSettings *pSettings) const
{
    // Keep track of the settings of the workspaces window widget

    pSettings->beginGroup(mPmrWorkspacesWindowWidget->objectName());
        mPmrWorkspacesWindowWidget->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void PmrWorkspacesWindowWindow::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    Core::OrganisationWidget::resizeEvent(pEvent);

    // Resize our busy widget

    mPmrWorkspacesWindowWidget->resizeBusyWidget();
}

//==============================================================================

Ui::PmrWorkspacesWindowWindow * PmrWorkspacesWindowWindow::gui() const
{
    // Return our GUI

    return mGui;
}

//==============================================================================

void PmrWorkspacesWindowWindow::update(const QString &pPmrUrl)
{
    // Update both our PMR web service and workspaces widget, and then update
    // our GUI (which will, as a result, also update our workspaces widget), if
    // needed

    if (pPmrUrl.compare(mPmrUrl)) {
        if (PMRSupport::PmrWorkspaceManager::instance()->hasWorkspaces())
            mPmrWorkspacesWindowWidget->initialize(PMRSupport::PmrWorkspaces(), QString(), false);

        mPmrUrl = pPmrUrl;

        mPmrWebService->update(pPmrUrl);
        mPmrWorkspacesWindowWidget->update(pPmrUrl);

        mPmrInstanceLabel->setText(mPmrWebService->siteName());

        updateGui();
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::busy(const bool &pBusy)
{
    // Show ourselves as busy or not busy anymore

    static int counter = 0;

    if (!pBusy && !counter)
        return;

    counter += pBusy?1:-1;

    if (pBusy && (counter == 1)) {
        mGui->dockWidgetContents->setEnabled(false);

        mPmrWorkspacesWindowWidget->showBusyWidget();
    } else if (!pBusy && !counter) {
        // Re-enable the GUI side

        mGui->dockWidgetContents->setEnabled(true);

        mPmrWorkspacesWindowWidget->hideBusyWidget();
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::showInformation(const QString &pMessage)
{
    // Show the given message as an information message box, but only if our
    // workspace manager is keeping track of some workspaces
    // Note: indeed, the idea is not to break the user's workflow, should some
    //       information become available when trying to retrieve the list of
    //       workspaces at startup...

    if (PMRSupport::PmrWorkspaceManager::instance()->hasWorkspaces())
        Core::informationMessageBox(windowTitle(), pMessage);
}

//==============================================================================

void PmrWorkspacesWindowWindow::showWarning(const QString &pMessage)
{
    // Show the given message as a warning message box, but only if our
    // workspace manager is keeping track of some workspaces
    // Note: indeed, the idea is not to break the user's workflow, should a
    //       warning occur when trying to retrieve the list of workspaces at
    //       startup...

    if (PMRSupport::PmrWorkspaceManager::instance()->hasWorkspaces())
        Core::warningMessageBox(windowTitle(), pMessage);
}

//==============================================================================

void PmrWorkspacesWindowWindow::showError(const QString &pMessage)
{
    // Either show the given message as an error message box or tell our
    // workspaces widget that we have a problem, this based on whether our
    // workspace manager is keeping track of some workspaces
    // Note: indeed, the idea is not to break the user's workflow, should an
    //       error occur when trying to retrieve the list of workspaces at
    //       startup...

    if (!PMRSupport::PmrWorkspaceManager::instance()->hasWorkspaces())
        mPmrWorkspacesWindowWidget->initialize(PMRSupport::PmrWorkspaces(), pMessage);
    else
        Core::criticalMessageBox(windowTitle(), pMessage);
}

//==============================================================================

void PmrWorkspacesWindowWindow::retrieveWorkspaces(const bool &pVisible)
{
    // Update our GUI, if we are becoming visible and the list of workspaces has
    // never been requested before (through a single shot, this to allow other
    // events, such as the one asking OpenCOR's main window to resize itself, to
    // be handled properly)
    // Note: this will result in the workspace list being populated if we are
    //       authenticated with PMR...

    static bool firstTime = true;

    if (pVisible && firstTime) {
        firstTime = false;

        QTimer::singleShot(0, this, SLOT(updateGui()));
    }
}

//==============================================================================

void PmrWorkspacesWindowWindow::updateGui()
{
    // Update our GUI

    mAuthenticated = mPmrWebService->isAuthenticated();

    mWaitingForPmrWebService = false;

    Core::showEnableAction(mGui->actionNew, true, mAuthenticated);
    Core::showEnableAction(mGui->actionReload, true, mAuthenticated);

    mGui->actionPmr->setIcon(mAuthenticated?mLoggedOnIcon:mLoggedOffIcon);

    retranslateActionPmr();

    // Ask ourselves to reload the list of workspaces from PMR, if we are
    // authenticated, or simply show a message asking us to authenticate
    // ourselves

    if (mAuthenticated)
        on_actionReload_triggered();
    else if (mInitialized)
        mPmrWorkspacesWindowWidget->initialize(PMRSupport::PmrWorkspaces(), QString(), false);
}

//==============================================================================

void PmrWorkspacesWindowWindow::retranslateActionPmr()
{
    // Retranslate our PMR action

    I18nInterface::retranslateAction(mGui->actionPmr,
                                     mAuthenticated?
                                         tr("Log Off"):
                                         tr("Log On"),
                                     mAuthenticated?
                                         tr("Log off PMR"):
                                         tr("Log on to PMR"));
}

//==============================================================================

void PmrWorkspacesWindowWindow::on_actionNew_triggered()
{
    // Create a new (owned) workspace

    QSettings settings;

    settings.beginGroup(mSettingsGroup);
        settings.beginGroup("PmrWorkspacesWindowNewWorkspaceDialog");
            PmrWorkspacesWindowNewWorkspaceDialog newWorkspaceDialog(&settings, Core::mainWindow());

            if (newWorkspaceDialog.exec() == QDialog::Accepted) {
                // Ask the PMR web service to create a new workspace, resulting
                // in the (empty) workspace being cloned into its folder

                mPmrWebService->requestNewWorkspace(newWorkspaceDialog.title(),
                                                    newWorkspaceDialog.description(),
                                                    newWorkspaceDialog.path());
            }
        settings.endGroup();
    settings.endGroup();
}

//==============================================================================

void PmrWorkspacesWindowWindow::on_actionReload_triggered()
{
    // Get the list of workspaces from our PMR web service, after making sure
    // that we have cleared existing workspaces from our workspace manager

    PMRSupport::PmrWorkspaceManager::instance()->clearWorkspaces();

    mPmrWebService->requestWorkspaces();
}

//==============================================================================

void PmrWorkspacesWindowWindow::on_actionPreferences_triggered()
{
    // Show the preferences for PMR support

    QDesktopServices::openUrl(QUrl("opencor://openPreferencesDialog/PMRSupport"));
}

//==============================================================================

void PmrWorkspacesWindowWindow::on_actionPmr_triggered()
{
    // Log on/off to/ PMR

    if (mWaitingForPmrWebService)
        return;

    mWaitingForPmrWebService = true;

    if (mAuthenticated) {
        if (Core::questionMessageBox(windowTitle(),
                                     tr("You are about to log off PMR. Do you want to proceed?")) == QMessageBox::Yes ) {
            mPmrWebService->authenticate(false);
        }
    } else {
        mPmrWebService->authenticate();
    }
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
