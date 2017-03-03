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

//==============================================================================

namespace Ui {
    class PmrWindowWindow;
}

//==============================================================================

class QLabel;
class QLineEdit;
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

    void update(const QString &pPmrUrl);

protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    Ui::PmrWindowWindow *mGui;

    QLabel *mFilterLabel;
    QLineEdit *mFilterValue;

    QLabel *mPmrInstanceLabel;

    QString mPmrUrl;

    PMRSupport::PmrWebService *mPmrWebService;

    PmrWindowWidget *mPmrWindowWidget;

    bool mItemDoubleClicked;

private slots:
    void on_actionReload_triggered();

    void filterValueChanged(const QString &pText);

    void busy(const bool &pBusy);

    void showInformation(const QString &pMessage);
    void showWarning(const QString &pMessage);
    void showError(const QString &pMessage);

    void retrieveExposures(const bool &pVisible);

    void initializeWidget(const PMRSupport::PmrExposures &pExposures);

    void itemDoubleClicked();
};

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
