/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
}

//==============================================================================

class QLabel;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace PMRSupport {
    class PmrWebService;
    class PmrWorkspace;
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

    Ui::PmrWorkspacesWindowWindow * gui() const;

    void update(const QString &pPmrUrl);

protected:
    virtual void resizeEvent(QResizeEvent *pEvent);

private:
    Ui::PmrWorkspacesWindowWindow *mGui;

    bool mInitialized;

    QString mSettingsGroup;

    QLabel *mPmrInstanceLabel;

    PMRSupport::PmrWebService *mPmrWebService;
    PmrWorkspacesWindowWidget *mPmrWorkspacesWindowWidget;

    QString mPmrUrl;

    bool mAuthenticated;

    QIcon mLoggedOnIcon;
    QIcon mLoggedOffIcon;

    bool mWaitingForPmrWebService;

    void retranslateActionPmr();

private slots:
    void on_actionNew_triggered();
    void on_actionReload_triggered();
    void on_actionPreferences_triggered();
    void on_actionPmr_triggered();

    void busy(const bool &pBusy);

    void showInformation(const QString &pMessage);
    void showWarning(const QString &pMessage);
    void showError(const QString &pMessage);

    void updateGui();

    void retrieveWorkspaces(const bool &pVisible);
};

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
