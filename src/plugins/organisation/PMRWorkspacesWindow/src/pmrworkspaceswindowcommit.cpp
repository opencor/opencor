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
#include "pmrworkspace.h"
#include "pmrworkspaceswindowcommit.h"

//==============================================================================

#include <QString>

//==============================================================================

#include "ui_pmrworkspaceswindowcommit.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PmrWorkspacesWindowCommit::PmrWorkspacesWindowCommit(const QStringList &pStagedFiles, 
                                                     QWidget *pParent) :
    QDialog(pParent),
    mGui(new Ui::PmrWorkspacesWindowCommit)
{
    // Set up the GUI

    mGui->setupUi(this);

    /*
    ; Changes to be committed:
    ;       new file:   untitled.html
    ;       modified:   untitled.html
    ;       deleted:    untitled.html
    ;       renamed:    untitled.html
    */
    mGui->message->setTabStopWidth(40);
    mGui->message->setPlainText("\n; Changes to be committed:\n;\t"
                                + pStagedFiles.join("\n;\t"));

    connect(mGui->cancel_save, SIGNAL(accepted()), this, SLOT(accept()));
    connect(mGui->cancel_save, SIGNAL(rejected()), this, SLOT(reject()));
}

//==============================================================================

PmrWorkspacesWindowCommit::~PmrWorkspacesWindowCommit()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PmrWorkspacesWindowCommit::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);
}

//==============================================================================

const QString PmrWorkspacesWindowCommit::message() const
{
    return mGui->message->toPlainText().trimmed();
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
