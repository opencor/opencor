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
// PMR Workspaces window synchronise dialog
//==============================================================================

#include "coreguiutils.h"
#include "pmrworkspace.h"
#include "pmrworkspaceswindowsynchronizedialog.h"

//==============================================================================

#include "ui_pmrworkspaceswindowsynchronizedialog.h"

//==============================================================================

#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PmrWorkspacesWindowSynchronizeDialog::PmrWorkspacesWindowSynchronizeDialog(PMRSupport::PmrWorkspace *pWorkspace,
                                                                           QWidget *pParent) :
    QDialog(pParent),
    mGui(new Ui::PmrWorkspacesWindowSynchronizeDialog)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Populate ourselves with the list of files that have changed

    mModel = new QStandardItemModel(this);

    mGui->changesValue->setModel(mModel);

    populateModel(pWorkspace->rootFileNode());

    // Connect some signals

    connect(mGui->buttonBox, SIGNAL(accepted()),
            this, SLOT(accept()));
    connect(mGui->buttonBox, SIGNAL(rejected()),
            this, SLOT(reject()));

    // Set our minimum size

    setMinimumSize(Core::minimumWidgetSize(this));
}

//==============================================================================

PmrWorkspacesWindowSynchronizeDialog::~PmrWorkspacesWindowSynchronizeDialog()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::populateModel(PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
Q_UNUSED(pFileNode);
//---GRY--- TO BE DONE...
}

//==============================================================================

QString PmrWorkspacesWindowSynchronizeDialog::message() const
{
    // Return the commit message

    return mGui->messageValue->toPlainText().trimmed();
}

//==============================================================================

QStringList PmrWorkspacesWindowSynchronizeDialog::fileNames() const
{
    // Return our file names
//---GRY--- TO BE DONE...

    return QStringList();
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::on_buttonBox_accepted()
{
    // Confirm that we accepted the changes

    done(QMessageBox::Ok);
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
