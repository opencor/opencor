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
// PMR Workspaces window new workspace dialog
//==============================================================================

#pragma once

//==============================================================================

#include "coreguiutils.h"

//==============================================================================

namespace Ui {
    class PmrWorkspacesWindowNewWorkspaceDialog;
}

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

class PmrWorkspacesWindowNewWorkspaceDialog : public Core::Dialog
{
    Q_OBJECT

public:
    explicit PmrWorkspacesWindowNewWorkspaceDialog(QSettings *pSettings,
                                                   QWidget *pParent);
    ~PmrWorkspacesWindowNewWorkspaceDialog();

    virtual void retranslateUi();

    QString title() const;
    QString description() const;
    QString path() const;

private:
    Ui::PmrWorkspacesWindowNewWorkspaceDialog *mGui;

private slots:
    void updateOkButton();

    void choosePath();
};

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
