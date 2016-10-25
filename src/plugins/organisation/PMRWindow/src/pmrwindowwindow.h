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

#pragma once

//==============================================================================

#include "organisationwidget.h"
#include "pmrexposure.h"
#include "pmrwindowwidget.h"

//==============================================================================

namespace Ui {
    class PmrWindowWindow;
}

//==============================================================================

class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace PMRSupport {
    class PmrWebService;
}   // namespace PMRSupport

//==============================================================================

namespace PMRWindow {

//==============================================================================

class PmrWindowWidget;

//==============================================================================

class PmrWindowWindow : public Core::OrganisationWidget
{
    Q_OBJECT

public:
    explicit PmrWindowWindow(QWidget *pParent);
    ~PmrWindowWindow();

    virtual void retranslateUi();

protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    Ui::PmrWindowWindow *mGui;

    PMRSupport::PmrWebService *mPmrWebService;

    PmrWindowWidget *mPmrWidget;

private slots:
    void on_filterValue_textChanged(const QString &pText);
    void on_refreshButton_clicked();

    void busy(const bool &pBusy);
    void showProgress(const double &pProgress);

    void pmrError(const QString &pErrorMessage,
                  const bool &pInternetConnectionAvailable);

    void showInformation(const QString &pMessage);
    void showWarning(const QString &pMessage);
    void showError(const QString &pMessage);

    void retrieveExposures(const bool &pVisible);

    void gotExposures(const PMRSupport::PmrExposures &pExposures);

    void requestExposureFiles(const QString &pUrl);

    void cloneWorkspace(const QString &pExposureUrl);
};

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
