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
// PMR web service
//==============================================================================

#pragma once

//==============================================================================

#include "pmrexposure.h"
#include "pmrsupportglobal.h"

//==============================================================================

#include <QList>
#include <QSslError>

//==============================================================================

class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PMRSUPPORT_EXPORT PmrWebService : public QObject
{
    Q_OBJECT

public:
    explicit PmrWebService();
    ~PmrWebService();

    void cloneWorkspace(const QString &pUrl, const QString &pDirName);
    void requestExposuresList(void);
    void requestExposureFiles(const QString &pUrl);

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

    QNetworkAccessManager *mNetworkAccessManager;

    int mNumberOfExposureFileUrlsLeft;

    QMap<QString, QString> mWorkspaces;
    QMap<QString, QString> mExposureUrls;
    QMap<QString, QString> mExposureNames;
    QMap<QString, QString> mExposureFileNames;

    QString informationNoteMessage() const;

    void doCloneWorkspace(const QString &pWorkspace, const QString &pDirName);
    void doShowExposureFiles(const QString &pExposureUrl);

    void sendPmrRequest(const PmrRequest &pPmrRequest,
                        const QString &pUrl = QString(),
                        const Action pAction = None,
                        const QString &pName = QString());

Q_SIGNALS:
    void busy(const bool &pBusy);

    void warning(const QString &pMessage);
    void information(const QString &pMessage);

    void exposuresList(const PMRSupport::PmrExposures &pExposures,
                       const QString &pErrorMessage,
                       const bool &pInternetConnectionAvailable);

    void addExposureFiles(const QString &pUrl,
                          const QStringList &pExposureFiles);
    void showExposureFiles(const QString &pUrl);

private Q_SLOTS:
    void finished(QNetworkReply *pNetworkReply = 0);
    void sslErrors(QNetworkReply *pNetworkReply,
                   const QList<QSslError> &pSslErrors);

};

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
