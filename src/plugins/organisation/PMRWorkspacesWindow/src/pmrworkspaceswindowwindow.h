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
// PMR Workspaces window
//==============================================================================

#pragma once

//==============================================================================

#include "organisationwidget.h"

//==============================================================================

#include <QAbstractItemModel>

//==============================================================================

namespace Ui {
    class PmrWorkspacesWindowWindow;
}

//==============================================================================

class QMenu;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace PMRSupport {
    class PmrWebService;
}   // namespace PMRSupport

//==============================================================================

namespace PMRWorkspacesWindow {

//==============================================================================

class PmrWorkspacesWindowWidget;

//==============================================================================

class PmrWorkspacesWindowWindow : public Core::OrganisationWidget
{
    Q_OBJECT

public:
    explicit PmrWorkspacesWindowWindow(QWidget *pParent);
    ~PmrWorkspacesWindowWindow();

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void fileRenamed(const QString &pOldFileName, const QString &pNewFileName);
    void fileReloaded(const QString &pFileName);

protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    Ui::PmrWorkspacesWindowWindow *mGui;

    PMRSupport::PmrWebService *mPmrWebService;
    PmrWorkspacesWindowWidget *mWorkspacesWindowWidget;

    QMenu *mContextMenu;

private slots:
    void busy(const bool &pBusy);
    void showError(const QString &pMessage);
    void showInformation(const QString &pMessage);
    void showWarning(const QString &pMessage);

    void getAuthenticationStatus();
    void retrieveWorkspaces(const bool &pVisible);
    void updateAuthenticationStatus(const bool &pAuthenticated);

    void on_actionAuthenticate_triggered();
    void on_actionNew_triggered();
    void on_actionRefresh_triggered();
    void on_actionRescan_triggered();
    void on_actionUnauthenticate_triggered();

    void showProgress(const double &pProgress);

    void showCustomContextMenu() const;

    void fileCreated(const QString &pFileName, const QString &pUrl);
    void fileDeleted(const QString &pFileName);
    void fileDuplicated(const QString &pFileName);
};

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
