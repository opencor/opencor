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
// Physiome Model Repository window
//==============================================================================

#ifndef PHYSIOMEMODELREPOSITORYWINDOWWINDOW_H
#define PHYSIOMEMODELREPOSITORYWINDOWWINDOW_H

//==============================================================================

#include "organisationwidget.h"

//==============================================================================

#include <QList>
#include <QSslError>

//==============================================================================

namespace Ui {
    class PhysiomeModelRepositoryWindowWindow;
}

//==============================================================================

class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace PhysiomeModelRepositoryWindow {

//==============================================================================

class PhysiomeModelRepositoryWindowWidget;

//==============================================================================

class PhysiomeModelRepositoryWindowWindow : public Core::OrganisationWidget
{
    Q_OBJECT

public:
    explicit PhysiomeModelRepositoryWindowWindow(QWidget *pParent);
    ~PhysiomeModelRepositoryWindowWindow();

    virtual void retranslateUi();

private:
    Ui::PhysiomeModelRepositoryWindowWindow *mGui;

    PhysiomeModelRepositoryWindowWidget *mPhysiomeModelRepositoryWidget;

    QNetworkAccessManager *mNetworkAccessManager;

    int mNumberOfUntreatedSourceFiles;

    QMap<QString, QString> mWorkspaces;
    QMap<QString, QString> mSourceFiles;

    enum PmrRequest {
        ModelList,
        BookmarkUrlsForCloning,
        BookmarkUrlsForShowingFiles,
        SourceFileForCloning,
        SourceFileForShowingFiles
    };

    void busy(const bool &pBusy);

    void sendPmrRequest(const PmrRequest &pPmrRequest,
                        const QString &pUrl = QString(),
                        const QString &pExtra = QString());

    void cloneWorkspace(const QString &pWorkspace);

private Q_SLOTS:
    void on_filterValue_textChanged(const QString &text);
    void on_refreshButton_clicked();

    void finished(QNetworkReply *pNetworkReply);
    void sslErrors(QNetworkReply *pNetworkReply,
                   const QList<QSslError> &pSslErrors);

    void retrieveModelList(const bool &pVisible);

    void cloneModel(const QString &pUrl, const QString &pDescription);
    void showModelFiles(const QString &pUrl, const QString &pDescription);
};

//==============================================================================

}   // namespace PhysiomeModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
