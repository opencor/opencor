/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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
} // namespace Ui

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
} // namespace PMRSupport

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
    ~PmrWindowWindow() override;

    void retranslateUi() override;

    void update(const QString &pPmrUrl);

protected:
    void resizeEvent(QResizeEvent *pEvent) override;

private:
    Ui::PmrWindowWindow *mGui;

    QLabel *mFilterLabel;
    QLineEdit *mFilterValue;

    QLabel *mPmrInstanceLabel;

    bool mFirstTimeRetrievingExposures = true;

    QString mPmrUrl;

    PMRSupport::PmrWebService *mPmrWebService;

    PmrWindowWidget *mPmrWindowWidget;

    bool mItemDoubleClicked = false;

    void retrieveExposures(bool pVisible, bool pForceRetrieval);

    void busy(bool pBusy, bool pResetCounter);

private slots:
    void actionReloadTriggered();

    void filterValueChanged(const QString &pText);

    void busy(bool pBusy);

    void showInformation(const QString &pMessage);
    void showWarning(const QString &pMessage);
    void showError(const QString &pMessage);

    void retrieveExposures(bool pVisible);

    void initializeWidget(const PMRSupport::PmrExposures &pExposures);

    void itemDoubleClicked();
};

//==============================================================================

} // namespace PMRWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
