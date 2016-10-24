/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// PMR window
//==============================================================================

#include "borderedwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrwebservice.h"
#include "pmrwindowwidget.h"
#include "pmrwindowwindow.h"

//==============================================================================

#include "ui_pmrwindowwindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

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

#ifdef Q_OS_MAC
    mGui->filterValue->setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above removes the focus border since it messes up the look of
    //       our filter value widget...
#endif

    // Make the name value our focus proxy

    setFocusProxy(mGui->filterValue);

    // Create and add the PMR widget

    mPmrWidget = new PmrWindowWidget(this);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->dockWidgetContents->layout()->addWidget(new Core::BorderedWidget(mPmrWidget,
                                                                           true, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->dockWidgetContents->layout()->addWidget(new Core::BorderedWidget(mPmrWidget,
                                                                           true, false, false, false));
#else
    #error Unsupported platform
#endif

    // Keep track of the window's visibility, so that we can request the list of
    // exposures, if necessary

    connect(this, SIGNAL(visibilityChanged(bool)),
            this, SLOT(retrieveExposures(const bool &)));

    // Get a PMR repository

    mPmrWebService = new PMRSupport::PmrWebService(this);

    // Some connections to process responses from the PMR web service

    connect(mPmrWebService, SIGNAL(busy(const bool &)),
            this, SLOT(busy(const bool &)));
    connect(mPmrWebService, SIGNAL(progress(double)),
            this, SLOT(showProgress(double)));

    connect(mPmrWebService, SIGNAL(error(const QString &, const bool &)),
            this, SLOT(repositoryError(const QString &, const bool &)));
    connect(mPmrWebService, SIGNAL(information(const QString &)),
            this, SLOT(showInformation(const QString &)));
    connect(mPmrWebService, SIGNAL(warning(const QString &)),
            this, SLOT(showWarning(const QString &)));

    connect(mPmrWebService, SIGNAL(exposures(const PMRSupport::PmrExposures &)),
            this, SLOT(gotExposures(const PMRSupport::PmrExposures &)));

    connect(mPmrWebService, SIGNAL(exposureFiles(const QString &, const QStringList &)),
            mPmrWidget, SLOT(addExposureFiles(const QString &, const QStringList &)));

    // Some connections to know what our PMR widget wants from us

    connect(mPmrWidget, SIGNAL(cloneWorkspaceRequested(const QString &)),
            this, SLOT(cloneWorkspace(const QString &)));
    connect(mPmrWidget, SIGNAL(requestExposureFiles(const QString &)),
            this, SLOT(requestExposureFiles(const QString &)));

    connect(mPmrWidget, SIGNAL(openExposureFileRequested(const QString &)),
            this, SLOT(openFile(const QString &)));

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
    // Retranslate the whole window and our information note message

    mGui->retranslateUi(this);

    // Retranslate our PMR widget

    mPmrWidget->retranslateUi();
}

//==============================================================================

void PmrWindowWindow::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    Core::OrganisationWidget::resizeEvent(pEvent);

    // Resize our busy widget

    mPmrWidget->resizeBusyWidget();
}

//==============================================================================

void PmrWindowWindow::busy(const bool &pBusy)
{
    // Show ourselves as busy or not busy anymore

    static int counter = 0;

    counter += pBusy?1:-1;

    if (pBusy && (counter == 1)) {
        mPmrWidget->showBusyWidget();

        mGui->dockWidgetContents->setEnabled(false);
    } else if (!pBusy && !counter) {
        // Re-enable the GUI side and give, within the current window, the focus
        // to mGui->filterValue, but only if the current window already has the
        // focus

        mPmrWidget->hideBusyWidget();

        mGui->dockWidgetContents->setEnabled(true);

        Core::setFocusTo(mGui->filterValue);
    }
}

//==============================================================================

void PmrWindowWindow::showProgress(const double &pProgress)
{
    setBusyWidgetProgress(pProgress);
}

//==============================================================================

void PmrWindowWindow::showError(const QString &pMessage)
{
    // Show the given message as an error

    Core::criticalMessageBox(Core::mainWindow(), windowTitle(), pMessage);
}

//==============================================================================

void PmrWindowWindow::showInformation(const QString &pMessage)
{
    // Show the given message as informative text

    Core::informationMessageBox(Core::mainWindow(), windowTitle(), pMessage);
}

//==============================================================================

void PmrWindowWindow::showWarning(const QString &pMessage)
{
    // Show the given message as a warning

    Core::warningMessageBox(Core::mainWindow(), windowTitle(), pMessage);
}

//==============================================================================

void PmrWindowWindow::on_filterValue_textChanged(const QString &pText)
{
    // Ask our PMR widget to filter its output using the given regular
    // expression

    mPmrWidget->filter(pText);
}

//==============================================================================

void PmrWindowWindow::on_refreshButton_clicked()
{
    // Get the list of exposures from our PMR repository

    mPmrWebService->requestExposures();
}

//==============================================================================

void PmrWindowWindow::gotExposures(const PMRSupport::PmrExposures &pExposures)
{
    // Ask our PMR widget to initialise itself

    mPmrWidget->initialize(pExposures, mGui->filterValue->text(), true);
}

//==============================================================================

void PmrWindowWindow::repositoryError(const QString &pErrorMessage,
                                      const bool &pInternetConnectionAvailable)
{
    // Tell our PMR widget we have a problem

    mPmrWidget->initialize(PMRSupport::PmrExposures(), QString(),
                           pInternetConnectionAvailable);

    showError(pErrorMessage);
}

//==============================================================================

void PmrWindowWindow::retrieveExposures(const bool &pVisible)
{
    // Retrieve the list of exposures, if we are becoming visible and the list
    // of exposures has never been requested before (through a single shot, this
    // to allow other events, such as the one asking OpenCOR's main window to
    // resize itself, to be handled properly)

    static bool firstTime = true;

    if (pVisible && firstTime) {
        firstTime = false;

        QTimer::singleShot(0, this, SLOT(on_refreshButton_clicked()));
    }
}

//==============================================================================

void PmrWindowWindow::cloneWorkspace(const QString &pExposureUrl)
{
    mPmrWebService->requestExposureWorkspaceClone(pExposureUrl);
}

//==============================================================================

void PmrWindowWindow::requestExposureFiles(const QString &pUrl)
{
    // Request a list of the exposure's files from our PMR web service

    mPmrWebService->requestExposureFiles(pUrl);
}

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
