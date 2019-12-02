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
// PMR Workspaces window
//==============================================================================

#pragma once

//==============================================================================

#include "organisationwidget.h"

//==============================================================================

#include <QIcon>

//==============================================================================

namespace Ui {
    class PmrWorkspacesWindowWindow;
} // namespace Ui

//==============================================================================

class QLabel;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace PMRSupport {
    class PmrWebService;
    class PmrWorkspace;
} // namespace PMRSupport

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
    ~PmrWorkspacesWindowWindow() override;

    void retranslateUi() override;

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

    Ui::PmrWorkspacesWindowWindow * gui() const;

    void update(const QString &pPmrUrl);

protected:
    void resizeEvent(QResizeEvent *pEvent) override;

private:
    Ui::PmrWorkspacesWindowWindow *mGui;

    bool mInitialized = false;

    QLabel *mPmrInstanceLabel;

    PMRSupport::PmrWebService *mPmrWebService;
    PmrWorkspacesWindowWidget *mPmrWorkspacesWindowWidget;

    bool mFirstTimeRetrievingWorkspaces = true;

    QString mPmrUrl;

    bool mAuthenticated = false;

    QIcon mLoggedOnIcon;
    QIcon mLoggedOffIcon;

    bool mWaitingForPmrWebService = false;

    void retranslateActionPmr();

    void busy(bool pBusy, bool pResetCounter);

private slots:
    void actionNewTriggered();
    void actionReloadTriggered();
    void actionPreferencesTriggered();
    void actionPmrTriggered();

    void busy(bool pBusy);

    void showInformation(const QString &pMessage);
    void showWarning(const QString &pMessage);
    void showError(const QString &pMessage);

    void updateGui();

    void retrieveWorkspaces(bool pVisible);
};

//==============================================================================

} // namespace PMRWorkspacesWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
