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
// PMR Workspaces window new workspace dialog
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrwebservice.h"
#include "pmrworkspace.h"
#include "pmrworkspaceswindownewworkspacedialog.h"

//==============================================================================

#include <QDir>
#include <QPushButton>
#include <QString>

//==============================================================================

#include "ui_pmrworkspaceswindownewworkspacedialog.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PmrWorkspacesWindowNewWorkspaceDialog::PmrWorkspacesWindowNewWorkspaceDialog(QSettings *pSettings, QWidget *pParent) :
    Core::Dialog(pSettings, pParent),
    mGui(new Ui::PmrWorkspacesWindowNewWorkspaceDialog)
{
    // Set up the GUI

    mGui->setupUi(this);

#ifdef Q_OS_MAC
    mGui->titleValue->setAttribute(Qt::WA_MacShowFocusRect, false);
    mGui->folderValue->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    // The Ok button is disabled to start with

    mGui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    // Connect some signals

    connect(mGui->titleValue, SIGNAL(textChanged(const QString &)),
            this, SLOT(updateOkButton()));
    connect(mGui->folderValue, SIGNAL(textChanged(const QString &)),
            this, SLOT(updateOkButton()));

    connect(mGui->folderToolButton, SIGNAL(clicked()),
            this, SLOT(choosePath()));

    connect(mGui->buttonBox, SIGNAL(accepted()),
            this, SLOT(accept()));
    connect(mGui->buttonBox, SIGNAL(rejected()),
            this, SLOT(reject()));

    // Set our minimum size

    setMinimumSize(Core::minimumWidgetSize(this));
}

//==============================================================================

PmrWorkspacesWindowNewWorkspaceDialog::~PmrWorkspacesWindowNewWorkspaceDialog()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PmrWorkspacesWindowNewWorkspaceDialog::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);
}

//==============================================================================

QString PmrWorkspacesWindowNewWorkspaceDialog::title() const
{
    // Return our title

    return mGui->titleValue->text().trimmed();
}

//==============================================================================

QString PmrWorkspacesWindowNewWorkspaceDialog::description() const
{
    // Return our description

    return mGui->descriptionValue->toPlainText().trimmed();
}

//==============================================================================

QString PmrWorkspacesWindowNewWorkspaceDialog::path() const
{
    // Return our path

    return mGui->folderValue->text();
}

//==============================================================================

void PmrWorkspacesWindowNewWorkspaceDialog::updateOkButton()
{
    // Enable the Ok button, but only if we have a title and a folder that both
    // exists and isn't already a Git repository

    QString dirName = mGui->folderValue->text().trimmed();

    mGui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(   !mGui->titleValue->text().trimmed().isEmpty()
                                                              &&  Core::isDirectory(dirName)
                                                              && !PMRSupport::PmrWebService::isGitDirectory(dirName));
}

//==============================================================================

void PmrWorkspacesWindowNewWorkspaceDialog::choosePath()
{
    // Choose a non-Git directory for our new workspace

    QString dirName = PMRSupport::PmrWebService::getNonGitDirectory();

    if (!dirName.isEmpty())
        mGui->folderValue->setText(dirName);
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
