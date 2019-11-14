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
// PMR window
//==============================================================================

#include "borderedwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrsupportplugin.h"
#include "pmrsupportpreferenceswidget.h"
#include "pmrwebservice.h"
#include "pmrwindowwidget.h"
#include "pmrwindowwindow.h"
#include "preferencesinterface.h"
#include "toolbarwidget.h"

//==============================================================================

#include "ui_pmrwindowwindow.h"

//==============================================================================

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace PMRWindow {

//==============================================================================

PmrWindowWindow::PmrWindowWindow(QWidget *pParent) :
    Core::OrganisationWidget(pParent),
    mGui(new Ui::PmrWindowWindow)
{
    // Set up the GUI

    mGui->setupUi(this);

    connect(mGui->actionReload, &QAction::triggered,
            this, &PmrWindowWindow::actionReloadTriggered);

    // Create a tool bar widget with a URL value and refresh button
    // Note: the spacers is a little trick to improve the rendering of our tool
    //       bar widget...

    auto toolBarWidget = new Core::ToolBarWidget();
    auto spacer = new QWidget(toolBarWidget);

    spacer->setMinimumSize(0, 0);
    spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    mFilterLabel = new QLabel(toolBarWidget);
    mFilterValue = new QLineEdit(toolBarWidget);

    QFont font = mFilterLabel->font();

    font.setBold(true);

    mFilterLabel->setFont(font);

#ifdef Q_OS_MAC
    mFilterValue->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    connect(mFilterValue, &QLineEdit::textChanged,
            this, &PmrWindowWindow::filterValueChanged);

    toolBarWidget->addWidget(spacer);
    toolBarWidget->addWidget(mFilterLabel);
    toolBarWidget->addWidget(mFilterValue);
    toolBarWidget->addAction(mGui->actionReload);

    mGui->layout->addWidget(toolBarWidget);

    // Make the filter value our focus proxy

    setFocusProxy(mFilterValue);

    // Create and add a label to highlight the repository we are using

    mPmrInstanceLabel = new QLabel(this);

    QFont newFont = mPmrInstanceLabel->font();

    newFont.setPointSizeF(0.7*newFont.pointSizeF());

    mPmrInstanceLabel->setAlignment(Qt::AlignCenter);
    mPmrInstanceLabel->setEnabled(false);
    mPmrInstanceLabel->setFont(newFont);

    mGui->layout->addWidget(mPmrInstanceLabel);

    // Create and add our PMR widget

    mPmrWindowWidget = new PmrWindowWidget(this);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mPmrWindowWidget,
                                                     true, true, true, true));
#else
    mGui->layout->addWidget(new Core::BorderedWidget(mPmrWindowWidget,
                                                     true, false, false, false));
#endif

    // Keep track of the window's visibility, so that we can request the list of
    // exposures, if necessary

    connect(this, &PmrWindowWindow::visibilityChanged,
            this, QOverload<bool>::of(&PmrWindowWindow::retrieveExposures));

    // Create an instance of our PMR Web service

    mPmrWebService = new PMRSupport::PmrWebService(this);

    // Initialise (update) our PMR URL

    update(PreferencesInterface::preference(PMRSupport::PluginName,
                                            PMRSupport::SettingsPreferencesPmrUrl,
                                            PMRSupport::SettingsPreferencesPmrUrlDefault).toString());

    // Some connections to process responses from our PMR Web service

    connect(mPmrWebService, &PMRSupport::PmrWebService::busy,
            this, QOverload<bool>::of(&PmrWindowWindow::busy));

    connect(mPmrWebService, &PMRSupport::PmrWebService::information,
            this, &PmrWindowWindow::showInformation);
    connect(mPmrWebService, &PMRSupport::PmrWebService::warning,
            this, &PmrWindowWindow::showWarning);
    connect(mPmrWebService, &PMRSupport::PmrWebService::error,
            this, &PmrWindowWindow::showError);

    connect(mPmrWebService, &PMRSupport::PmrWebService::exposures,
            this, &PmrWindowWindow::initializeWidget);

    connect(mPmrWebService, &PMRSupport::PmrWebService::exposureFiles,
            mPmrWindowWidget, &PmrWindowWidget::addAndShowExposureFiles);

    // Some connections to know what our PMR widget wants from us

    connect(mPmrWindowWidget, &PmrWindowWidget::cloneWorkspaceRequested,
            mPmrWebService, &PMRSupport::PmrWebService::requestExposureWorkspaceClone);
    connect(mPmrWindowWidget, &PmrWindowWidget::exposureFilesRequested,
            mPmrWebService, &PMRSupport::PmrWebService::requestExposureFiles);
    connect(mPmrWindowWidget, &PmrWindowWidget::openExposureFileRequested,
            this, &PmrWindowWindow::openFile);
    connect(mPmrWindowWidget, &PmrWindowWidget::openExposureFilesRequested,
            this, &PmrWindowWindow::openFiles);
    connect(mPmrWindowWidget, QOverload<>::of(&PmrWindowWidget::itemDoubleClicked),
            this, &PmrWindowWindow::itemDoubleClicked);

    // Some further initialisations that are done as part of retranslating the
    // GUI (so that they can be updated when changing languages)

    retranslateUi();
}

//==============================================================================

PmrWindowWindow::~PmrWindowWindow()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PmrWindowWindow::retranslateUi()
{
    // Retranslate our whole window and our information note message

    mGui->retranslateUi(this);

    mFilterLabel->setText(tr("Filter:"));

    mPmrInstanceLabel->setText(mPmrWebService->siteName());

    // Retranslate our PMR widget

    mPmrWindowWidget->retranslateUi();
}

//==============================================================================

void PmrWindowWindow::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    Core::OrganisationWidget::resizeEvent(pEvent);

    // Resize our busy widget

    mPmrWindowWidget->resizeBusyWidget();
}

//==============================================================================

void PmrWindowWindow::update(const QString &pPmrUrl)
{
    // Update our PMR Web service and then reload ourselves, if needed
    // Note: we make sure that no busy widget is visible. Indeed, this is in
    //       case an instance wasn't working and we decided to switch to another
    //       that does (in which case the busy widget of the first instance
    //       would still have been visible)...

    if (pPmrUrl != mPmrUrl) {
        if (mPmrWindowWidget->hasExposures()) {
            mPmrWindowWidget->initialize(PMRSupport::PmrExposures(), QString(), QString());
        }

        busy(false, true);

        mFirstTimeRetrievingExposures = true;

        mPmrUrl = pPmrUrl;

        mPmrWebService->update(pPmrUrl);

        mPmrInstanceLabel->setText(mPmrWebService->siteName());

        retrieveExposures(isVisible(), true);
    }
}

//==============================================================================

void PmrWindowWindow::filterValueChanged(const QString &pText)
{
    // Ask our PMR widget to filter its output using the given regular
    // expression

    mPmrWindowWidget->filter(pText);
}

//==============================================================================

void PmrWindowWindow::busy(bool pBusy, bool pResetCounter)
{
    // Show ourselves as busy or not busy anymore

    static int counter = 0;

    if (!pBusy && (counter == 0)) {
        return;
    }

    if (pResetCounter) {
        counter = 0;
    } else {
        counter += pBusy?1:-1;
    }

    if (pBusy && (counter == 1)) {
        mGui->dockWidgetContents->setEnabled(false);

        mPmrWindowWidget->showBusyWidget();
    } else if (!pBusy && (counter == 0)) {
        // Re-enable the GUI side and give, within the current window, the focus
        // to mFilterValue, but only if the current window already has the
        // focus, or to mPmrWindowWidget if it was previously double clicked

        mGui->dockWidgetContents->setEnabled(true);

        if (mItemDoubleClicked) {
            mItemDoubleClicked = false;

            mPmrWindowWidget->setFocus();
        } else {
            Core::setFocusTo(mFilterValue);
        }

        mPmrWindowWidget->hideBusyWidget();
    }
}

//==============================================================================

void PmrWindowWindow::busy(bool pBusy)
{
    // Show ourselves as busy or not busy anymore

    busy(pBusy, false);
}

//==============================================================================

void PmrWindowWindow::showInformation(const QString &pMessage)
{
    // Show the given message as an information message box, but only if we
    // already have some exposures
    // Note: indeed, the idea is not to break the user's workflow, should some
    //       information become available when trying to retrieve the list of
    //       exposures at startup...

    if (mPmrWindowWidget->hasExposures()) {
        Core::informationMessageBox(windowTitle(), pMessage);
    }
}

//==============================================================================

void PmrWindowWindow::showWarning(const QString &pMessage)
{
    // Show the given message as a warning message box, but only if we already
    // have some exposures
    // Note: indeed, the idea is not to break the user's workflow, should a
    //       warning occur when trying to retrieve the list of exposures at
    //       startup...

    if (mPmrWindowWidget->hasExposures()) {
        Core::warningMessageBox(windowTitle(), pMessage);
    }
}

//==============================================================================

void PmrWindowWindow::showError(const QString &pMessage)
{
    // Either show the given message as an error message box or tell our PMR
    // widget that we have a problem, this based on whether we have already
    // retrieved the list of exposures
    // Note: indeed, the idea is not to break the user's workflow, should an
    //       error occur when trying to retrieve the list of exposures at
    //       startup...

    if (mPmrWindowWidget->hasExposures()) {
        Core::criticalMessageBox(windowTitle(), pMessage);
    } else {
        mPmrWindowWidget->initialize(PMRSupport::PmrExposures(), QString(), pMessage);
    }
}

//==============================================================================

void PmrWindowWindow::actionReloadTriggered()
{
    // Get the list of exposures from our PMR Web service

    mPmrWebService->requestExposures();
}

//==============================================================================

void PmrWindowWindow::initializeWidget(const PMRSupport::PmrExposures &pExposures)
{
    // Ask our PMR widget to initialise itself

    mPmrWindowWidget->initialize(pExposures, mFilterValue->text(), QString());
}

//==============================================================================

void PmrWindowWindow::itemDoubleClicked()
{
    // Keep track of the fact that an item got double clicked and that we will
    // want it to get the focus back once we are not busy anymore

    mItemDoubleClicked = true;
}

//==============================================================================

void PmrWindowWindow::retrieveExposures(bool pVisible, bool pForceRetrieval)
{
    // Retrieve the list of exposures, if we are becoming visible and the list
    // of exposures has never been requested before (through a single shot, this
    // to allow other events, such as the one asking OpenCOR's main window to
    // resize itself, to be handled properly)

    if (pVisible && (mFirstTimeRetrievingExposures || pForceRetrieval)) {
        mFirstTimeRetrievingExposures = false;

        QTimer::singleShot(0, this, &PmrWindowWindow::actionReloadTriggered);
    }
}

//==============================================================================

void PmrWindowWindow::retrieveExposures(bool pVisible)
{
    // Retrieve the exposures

    retrieveExposures(pVisible, false);
}

//==============================================================================

} // namespace PMRWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
