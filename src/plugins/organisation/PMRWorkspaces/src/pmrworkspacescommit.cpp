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
// Create a new PMR workspace dialog
//==============================================================================

#include "coreguiutils.h"
#include "pmrworkspace.h"
#include "pmrworkspacescommit.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "ui_pmrworkspacescommit.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspaces {

//==============================================================================

PmrWorkspacesCommit::PmrWorkspacesCommit(QWidget *pParent) :
    QDialog(pParent),
    mGui(new Ui::PmrWorkspacesCommit)
{
    // Set up the GUI

    mGui->setupUi(this);

    connect(mGui->cancel_save, SIGNAL(accepted()), this, SLOT(accept()));
    connect(mGui->cancel_save, SIGNAL(rejected()), this, SLOT(reject()));
}

//==============================================================================

PmrWorkspacesCommit::~PmrWorkspacesCommit()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PmrWorkspacesCommit::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);
}

//==============================================================================

const QString PmrWorkspacesCommit::message(void) const
{
    return mGui->message->toPlainText().trimmed();
}

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
