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
// CellML Model Repository window
//==============================================================================

#ifndef CELLMLMODELREPOSITORYWINDOWWINDOW_H
#define CELLMLMODELREPOSITORYWINDOWWINDOW_H

//==============================================================================

#include "organisationwidget.h"

//==============================================================================

#include <QList>
#include <QSslError>

//==============================================================================

namespace Ui {
    class CellmlModelRepositoryWindowWindow;
}

//==============================================================================

class QMenu;
class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepositoryWindow {

//==============================================================================

class CellmlModelRepositoryWindowWidget;

//==============================================================================

class CellmlModelRepositoryWindowWindow : public Core::OrganisationWidget
{
    Q_OBJECT

public:
    explicit CellmlModelRepositoryWindowWindow(QWidget *pParent);
    ~CellmlModelRepositoryWindowWindow();

    virtual void retranslateUi();

private:
    Ui::CellmlModelRepositoryWindowWindow *mGui;

    QStringList mModelNames;
    QStringList mModelUrls;

    CellmlModelRepositoryWindowWidget *mCellmlModelRepositoryWidget;

    QStringList mModelList;

    QNetworkAccessManager *mNetworkAccessManager;
    QString mErrorMessage;

    bool mModelListRequested;

    QMenu *mContextMenu;

    void outputModelList(const QStringList &pModelList);

private Q_SLOTS:
    void on_filterValue_textChanged(const QString &text);
    void on_actionCopy_triggered();
    void on_refreshButton_clicked();

    void finished(QNetworkReply *pNetworkReply);
    void sslErrors(QNetworkReply *pNetworkReply,
                   const QList<QSslError> &pSslErrors);

    void showCustomContextMenu(const QPoint &pPosition) const;

    void retrieveModelList(const bool &pVisible);
};

//==============================================================================

}   // namespace CellMLModelRepositoryWindow
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
