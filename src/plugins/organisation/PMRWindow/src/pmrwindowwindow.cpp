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
#include "pmrrepository.h"
#include "pmrwindowwidget.h"
#include "pmrwindowwindow.h"

//==============================================================================

#include "ui_pmrwindowwindow.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QMainWindow>
#include <QMessageBox>
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
            this, SLOT(retrieveExposuresList(const bool &)));

    // Get a PMR repository

    mPmrRepository = new PMRSupport::PmrRepository(this);

    // Some connections to process responses from the PMR repository

    connect(mPmrRepository, SIGNAL(busy(const bool &)),
            this, SLOT(busy(const bool &)));
    connect(mPmrRepository, SIGNAL(progress(double)),
            this, SLOT(showProgress(double)));

    connect(mPmrRepository, SIGNAL(error(const QString &, const bool &)),
            this, SLOT(repositoryError(const QString &, const bool &)));
    connect(mPmrRepository, SIGNAL(information(const QString &)),
            this, SLOT(showInformation(const QString &)));
    connect(mPmrRepository, SIGNAL(warning(const QString &)),
            this, SLOT(showWarning(const QString &)));

    connect(mPmrRepository, SIGNAL(exposuresList(const PMRSupport::PmrExposureList &)),
            this, SLOT(gotExposuresList(const PMRSupport::PmrExposureList &)));

    connect(mPmrRepository, SIGNAL(exposureFilesList(const QString &, const QStringList &)),
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

void PmrWindowWindow::busy(const bool &pBusy)
{
    // Show ourselves as busy or not busy anymore

    static int counter = 0;

    counter += pBusy?1:-1;

    if (pBusy && (counter == 1)) {
        showBusyWidget(mPmrWidget);

        mGui->dockWidgetContents->setEnabled(false);
    } else if (!pBusy && !counter) {
        // Re-enable the GUI side and give, within the current window, the focus
        // to mGui->filterValue, but only if the current window already has the
        // focus

        hideBusyWidget();

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

    QMessageBox::critical(Core::mainWindow(), windowTitle(), pMessage);
}

//==============================================================================

void PmrWindowWindow::showInformation(const QString &pMessage)
{
    // Show the given message as informative text

    QMessageBox::information(Core::mainWindow(), windowTitle(), pMessage);
}

//==============================================================================

void PmrWindowWindow::showWarning(const QString &pMessage)
{
    // Show the given message as a warning

    QMessageBox::warning(Core::mainWindow(), windowTitle(), pMessage);
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

    mPmrRepository->requestExposuresList();
}

//==============================================================================

void PmrWindowWindow::gotExposuresList(const PMRSupport::PmrExposureList &pExposures)
{
    // Ask our PMR widget to initialise itself

    mPmrWidget->initialize(pExposures, mGui->filterValue->text(), true);
}

//==============================================================================

void PmrWindowWindow::repositoryError(const QString &pErrorMessage,
                                      const bool &pInternetConnectionAvailable)
{
    // Tell our PMR widget we have a problem

    mPmrWidget->initialize(PMRSupport::PmrExposureList(), "", pInternetConnectionAvailable);
    showError(pErrorMessage);
}

//==============================================================================

void PmrWindowWindow::retrieveExposuresList(const bool &pVisible)
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

void PmrWindowWindow::cloneWorkspace(const QString &pUrl)
{
    // Retrieve the name of an empty directory

    QString dirName = Core::getExistingDirectory(tr("Select Empty Directory"),
                                                 QString(), true);

    if (!dirName.isEmpty()) {
        // We have got a directory name where we can clone the workspace, so
        // request a clone of it

        mPmrRepository->requestExposureWorkspaceClone(pUrl, dirName);
    }
}

//==============================================================================

void PmrWindowWindow::requestExposureFiles(const QString &pUrl)
{
    // Request a list of the exposure's files from our PMR repository

    mPmrRepository->requestExposureFiles(pUrl);
}

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
