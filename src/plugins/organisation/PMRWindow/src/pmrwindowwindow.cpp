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
    mGui(new Ui::PmrWindowWindow),
    mItemDoubleClicked(false)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create a tool bar widget with a URL value and refresh button
    // Note: the spacers is a little trick to improve the rendering of our tool
    //       bar widget...

    Core::ToolBarWidget *toolBarWidget = new Core::ToolBarWidget(this);
    QWidget *spacer = new QWidget(toolBarWidget);

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

    connect(mFilterValue, SIGNAL(textChanged(const QString &)),
            this, SLOT(filterValueChanged(const QString &)));

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

    newFont.setPointSize(0.75*newFont.pointSize());

    mPmrInstanceLabel->setAlignment(Qt::AlignCenter);
    mPmrInstanceLabel->setEnabled(false);
    mPmrInstanceLabel->setFont(newFont);

    mGui->layout->addWidget(new Core::BorderedWidget(mPmrInstanceLabel,
                                                     true, false, false, false));

    mGui->layout->setStretch(mGui->layout->count()-1, 1);

    // Create and add our PMR widget

    mPmrWindowWidget = new PmrWindowWidget(this);

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    mGui->layout->addWidget(new Core::BorderedWidget(mPmrWindowWidget,
                                                     true, true, true, true));
#elif defined(Q_OS_MAC)
    mGui->layout->addWidget(new Core::BorderedWidget(mPmrWindowWidget,
                                                     true, false, false, false));
#else
    #error Unsupported platform
#endif

    mGui->layout->setStretch(mGui->layout->count()-1, 99999);

    // Keep track of the window's visibility, so that we can request the list of
    // exposures, if necessary

    connect(this, SIGNAL(visibilityChanged(bool)),
            this, SLOT(retrieveExposures(const bool &)));

    // Create an instance of our PMR web service

    mPmrWebService = new PMRSupport::PmrWebService(this);

    // Initialise (update) our PMR URL

    update(PreferencesInterface::preference(PMRSupport::PluginName,
                                            PMRSupport::SettingsPreferencesPmrUrl,
                                            PMRSupport::SettingsPreferencesPmrUrlDefault).toString());

    // Some connections to process responses from our PMR web service

    connect(mPmrWebService, SIGNAL(busy(const bool &)),
            this, SLOT(busy(const bool &)));

    connect(mPmrWebService, SIGNAL(information(const QString &)),
            this, SLOT(showInformation(const QString &)));
    connect(mPmrWebService, SIGNAL(warning(const QString &)),
            this, SLOT(showWarning(const QString &)));
    connect(mPmrWebService, SIGNAL(error(const QString &)),
            this, SLOT(showError(const QString &)));

    connect(mPmrWebService, SIGNAL(exposures(const PMRSupport::PmrExposures &)),
            this, SLOT(initializeWidget(const PMRSupport::PmrExposures &)));

    connect(mPmrWebService, SIGNAL(exposureFiles(const QString &, const QStringList &)),
            mPmrWindowWidget, SLOT(addAndShowExposureFiles(const QString &, const QStringList &)));

    // Some connections to know what our PMR widget wants from us

    connect(mPmrWindowWidget, SIGNAL(cloneWorkspaceRequested(const QString &)),
            mPmrWebService, SLOT(requestExposureWorkspaceClone(const QString &)));
    connect(mPmrWindowWidget, SIGNAL(exposureFilesRequested(const QString &)),
            mPmrWebService, SLOT(requestExposureFiles(const QString &)));
    connect(mPmrWindowWidget, SIGNAL(openExposureFileRequested(const QString &)),
            this, SLOT(openFile(const QString &)));
    connect(mPmrWindowWidget, SIGNAL(openExposureFilesRequested(const QStringList &)),
            this, SLOT(openFiles(const QStringList &)));
    connect(mPmrWindowWidget, SIGNAL(itemDoubleClicked()),
            this, SLOT(itemDoubleClicked()));

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
    // Update our PMR web service and then reload ourselves, if needed

    if (pPmrUrl.compare(mPmrUrl)) {
        mPmrUrl = pPmrUrl;

        mPmrWebService->update(pPmrUrl);

        mPmrInstanceLabel->setText(mPmrWebService->siteName());

        on_actionReload_triggered();
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

void PmrWindowWindow::busy(const bool &pBusy)
{
    // Show ourselves as busy or not busy anymore

    static int counter = 0;

    counter += pBusy?1:-1;

    if (pBusy && (counter == 1)) {
        mPmrWindowWidget->showBusyWidget();

        mGui->dockWidgetContents->setEnabled(false);
    } else if (!pBusy && !counter) {
        // Re-enable the GUI side and give, within the current window, the focus
        // to mFilterValue, but only if the current window already has the
        // focus, or to mPmrWindowWidget if it was previously double clicked

        mPmrWindowWidget->hideBusyWidget();

        mGui->dockWidgetContents->setEnabled(true);

        if (mItemDoubleClicked) {
            mItemDoubleClicked = false;

            mPmrWindowWidget->setFocus();
        } else {
            Core::setFocusTo(mFilterValue);
        }
    }
}

//==============================================================================

void PmrWindowWindow::showInformation(const QString &pMessage)
{
    // Show the given message as an information message box, but only if we
    // already have some exposures
    // Note: indeed, the idea is not to break the user's workflow, should some
    //       information become available when trying to retrieve the list of
    //       exposures at startup...

    if (mPmrWindowWidget->hasExposures())
        Core::informationMessageBox(windowTitle(), pMessage);
}

//==============================================================================

void PmrWindowWindow::showWarning(const QString &pMessage)
{
    // Show the given message as a warning message box, but only if we already
    // have some exposures
    // Note: indeed, the idea is not to break the user's workflow, should a
    //       warning occur when trying to retrieve the list of exposures at
    //       startup...

    if (mPmrWindowWidget->hasExposures())
        Core::warningMessageBox(windowTitle(), pMessage);
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

    if (mPmrWindowWidget->hasExposures())
        Core::criticalMessageBox(windowTitle(), pMessage);
    else
        mPmrWindowWidget->initialize(PMRSupport::PmrExposures(), QString(), pMessage);
}

//==============================================================================

void PmrWindowWindow::on_actionReload_triggered()
{
    // Get the list of exposures from our PMR web service

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

void PmrWindowWindow::retrieveExposures(const bool &pVisible)
{
    // Retrieve the list of exposures, if we are becoming visible and the list
    // of exposures has never been requested before (through a single shot, this
    // to allow other events, such as the one asking OpenCOR's main window to
    // resize itself, to be handled properly)

    static bool firstTime = true;

    if (pVisible && firstTime) {
        firstTime = false;

        QTimer::singleShot(0, this, SLOT(on_actionReload_triggered()));
    }
}

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
