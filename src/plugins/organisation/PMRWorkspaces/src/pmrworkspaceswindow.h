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
// PMR Workspaces window
//==============================================================================

#pragma once

//==============================================================================

#include "pmrworkspacesglobal.h"

//==============================================================================

#include "organisationwidget.h"
#include "pmrrepository.h"

//==============================================================================

#include <QAbstractItemModel>

//==============================================================================

namespace Ui {
    class PmrWorkspacesWindow;
}

//==============================================================================

class QMenu;

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspaces {

//==============================================================================

class PmrWorkspacesWidget;

//==============================================================================

class PMRWORKSPACE_EXPORT PmrWorkspacesWindow : public Core::OrganisationWidget
{
    Q_OBJECT

public:
    explicit PmrWorkspacesWindow(QWidget *pParent);
    ~PmrWorkspacesWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

private:
    Ui::PmrWorkspacesWindow *mGui;

    PMRSupport::PmrRepository *mPmrRepository;
    PmrWorkspacesWidget *mWorkspacesWidget;

    QMenu *mContextMenu;

private Q_SLOTS:
    void busy(const bool &pBusy);
    void showError(const QString &pMessage);
    void showInformation(const QString &pMessage);
    void showWarning(const QString &pMessage);

    void getAuthenticationStatus(void);
    void retrieveWorkspacesList(const bool &pVisible);
    void updateAuthenticationStatus(const bool &pAuthenticated);

    void on_actionAuthenticate_triggered();
    void on_actionNew_triggered();
    void on_actionRefresh_triggered();
    void on_actionUnauthenticate_triggered();

    void workspaceCloned(PMRSupport::PmrWorkspace *pWorkspace);
    void workspaceCreated(const QString &pUrl);
    void showProgress(const double &pProgress);

    void showCustomContextMenu() const;
};

//==============================================================================

}   // namespace PMRWorkspaces
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
