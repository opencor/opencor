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

PmrWorkspacesWindowCommitDialog::PmrWorkspacesWindowCommitDialog(const PMRSupport::StagedFiles &pStagedFiles,
                                                                 QWidget *pParent) :
    QDialog(pParent),
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
