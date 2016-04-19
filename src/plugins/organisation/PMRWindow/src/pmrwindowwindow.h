/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// PMR window
//==============================================================================

#pragma once

//==============================================================================

#include "organisationwidget.h"

//==============================================================================

#include <QList>
#include <QSslError>

//==============================================================================

namespace Ui {
    class PmrWindowWindow;
}

//==============================================================================

class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {
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

private:
    enum PmrRequest {
        ExposuresList,
        ExposureInformation,
        WorkspaceInformation,
        ExposureFileInformation
    };

    enum Action {
        None,
        CloneWorkspace,
        ShowExposureFiles
    };

    Ui::PmrWindowWindow *mGui;

    PmrWindowWidget *mPmrWidget;

    QNetworkAccessManager *mNetworkAccessManager;

    int mNumberOfExposureFileUrlsLeft;

    QMap<QString, QString> mWorkspaces;
    QMap<QString, QString> mExposureUrls;
    QMap<QString, QString> mExposureNames;
    QMap<QString, QString> mExposureFileNames;

    QString mInformationNoteMessage;

    void busy(const bool &pBusy);

    void sendPmrRequest(const PmrRequest &pPmrRequest,
                        const QString &pUrl = QString(),
                        const Action pAction = None);

    void doCloneWorkspace(const QString &pWorkspace);
    void doShowExposureFiles(const QString &pExposureUrl);

private Q_SLOTS:
    void on_filterValue_textChanged(const QString &pText);
    void on_refreshButton_clicked();

    void finished(QNetworkReply *pNetworkReply = 0);
    void sslErrors(QNetworkReply *pNetworkReply,
                   const QList<QSslError> &pSslErrors);

    void retrieveExposuresList(const bool &pVisible);

    void cloneWorkspace(const QString &pUrl);
    void showExposureFiles(const QString &pUrl);
};

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
