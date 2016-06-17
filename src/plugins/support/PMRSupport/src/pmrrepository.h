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
// PMR repository
//==============================================================================

#pragma once

//==============================================================================

#include "pmrauthentication.h"
#include "pmrexposure.h"
#include "pmrsupportglobal.h"
#include "pmrworkspace.h"

//==============================================================================

#include <QList>
#include <QObject>
#include <QString>

//==============================================================================

class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrRepositoryManager;

//==============================================================================

class PMRSUPPORT_EXPORT PmrRepository : public QObject
{
    Q_OBJECT

public:
    explicit PmrRepository(QObject *parent = 0);
    ~PmrRepository();

    QString url(void) const;

    void cloneWorkspace(const QString &pUrl, const QString &pDirName);
    void requestExposuresList(void);
    void requestExposureFiles(const QString &pUrl);
    void requestWorkspacesList(void);
    void requestWorkspaceDetails(const QString &pUrl);

private:


    enum Action {
        None,
        CloneExposureWorkspace,
        RequestExposureFiles
    };

    PmrRepositoryManager *mPmrRepositoryManager;

    int mNumberOfExposureFileUrlsLeft;

    QMap<QString, PmrWorkspace *> mWorkspaces;
    QMap<QString, QString> mExposureUrls;
    QMap<QString, QString> mExposureNames;
    QMap<QString, QString> mExposureFileNames;

    QString informationNoteMessage() const;

    void doShowExposureFiles(const QString &pExposureUrl);


Q_SIGNALS:
    void authenticated(const bool &pAuthenticated);
    //void authenticationChanged(const bool &pAuthenticated);

    void busy(const bool &pBusy);

    void error(const QString &pErrorMessage, const bool &pInternetConnectionAvailable);
    void information(const QString &pMessage);
    void warning(const QString &pMessage);

    void exposuresList(const PMRSupport::PmrExposureList &pExposureList,
                       const QString &pErrorMessage,
                       const bool &pInternetConnectionAvailable);

    void addExposureFiles(const QString &pUrl,
                          const QStringList &pExposureFiles);
    void showExposureFiles(const QString &pUrl);

    void workspacesList(const PMRSupport::PmrWorkspaceList &pWorkspaceList);

public Q_SLOTS:
    void authenticate(const bool &pLink = true);
    void getAuthenticationStatus(void);

private Q_SLOTS:

};

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
