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
// PMR Workspaces window commit dialog
//==============================================================================

#include "coreguiutils.h"
#include "pmrworkspaceswindowcommitdialog.h"

//==============================================================================

#include "ui_pmrworkspaceswindowcommitdialog.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PmrWorkspacesWindowCommitDialog::PmrWorkspacesWindowCommitDialog(QSettings *pSettings,
                                                                 const PMRSupport::StagedFiles &pStagedFiles,
                                                                 QWidget *pParent) :
    Core::Dialog(pSettings, pParent),
    mGui(new Ui::PmrWorkspacesWindowCommitDialog)
{
    // Set up the GUI

    mGui->setupUi(this);

    QString message =  "\n\n"
                      +tr("; Changes to be committed:")+"\n";
    QString status = QString();
    int counter = 0;
    int nbOfStagedFiles = pStagedFiles.count();

    foreach (const PMRSupport::StagedFile &stagedFile, pStagedFiles) {
        switch (stagedFile.second) {
        case GIT_STATUS_INDEX_NEW:
            status = tr("new");

            break;
        case GIT_STATUS_INDEX_MODIFIED:
            status = tr("modified");

            break;
        case GIT_STATUS_INDEX_DELETED:
            status = tr("deleted");

            break;
        case GIT_STATUS_INDEX_RENAMED:
            status = tr("renamed");

            break;
        case GIT_STATUS_INDEX_TYPECHANGE:
            status = tr("type change");

            break;
        default:
            // Note: we should never reach this point...

            status = "???";
        }

        ++counter;

        if (counter == nbOfStagedFiles) {
            message += tr(";   - %1 (%2).").arg(stagedFile.first, status)+"\n";
        } else if (counter == nbOfStagedFiles-1) {
            message += tr(";   - %1 (%2); and").arg(stagedFile.first, status)+"\n";
        } else {
            message += tr(";   - %1 (%2);").arg(stagedFile.first, status)+"\n";
        }
    }

    mGui->messageValue->setPlainText(message);

    // Connect some signals

    connect(mGui->buttonBox, SIGNAL(accepted()),
            this, SLOT(accept()));
    connect(mGui->buttonBox, SIGNAL(rejected()),
            this, SLOT(reject()));

    // Set our minimum size

    setMinimumSize(Core::minimumWidgetSize(this));
}

//==============================================================================

PmrWorkspacesWindowCommitDialog::~PmrWorkspacesWindowCommitDialog()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PmrWorkspacesWindowCommitDialog::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);
}

//==============================================================================

QString PmrWorkspacesWindowCommitDialog::message() const
{
    // Return the commit message

    return mGui->messageValue->toPlainText().trimmed();
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
