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

PmrWorkspacesWindowNewWorkspaceDialog::PmrWorkspacesWindowNewWorkspaceDialog(QWidget *pParent) :
    QDialog(pParent),
    mGui(new Ui::PmrWorkspacesWindowNewWorkspaceDialog),
    mPathChosen(false)
{
    // Set up the GUI

    mGui->setupUi(this);

    // The save button is disabled until we have a title

    mGui->cancel_save->button(QDialogButtonBox::Save)->setEnabled(false);

    // Connect some signals

    connect(mGui->choosePath, SIGNAL(clicked(bool)),
            this, SLOT(choosePath(const bool &)));

    connect(mGui->path, SIGNAL(textChanged(const QString &)),
            this, SLOT(setPathToolTip(const QString &)));

    connect(mGui->title, SIGNAL(textChanged(const QString &)),
            this, SLOT(titleTextChanged(const QString &)));

    connect(mGui->cancel_save, SIGNAL(accepted()),
            this, SLOT(accept()));
    connect(mGui->cancel_save, SIGNAL(rejected()),
            this, SLOT(reject()));
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

const QString PmrWorkspacesWindowNewWorkspaceDialog::description() const
{
    return mGui->description->toPlainText().trimmed();
}

//==============================================================================

const QString PmrWorkspacesWindowNewWorkspaceDialog::path() const
{
    return mGui->path->text();
}

//==============================================================================

const QString PmrWorkspacesWindowNewWorkspaceDialog::title() const
{
    return mGui->title->text().trimmed();
}

//==============================================================================

void PmrWorkspacesWindowNewWorkspaceDialog::titleTextChanged(const QString &pText)
{
    // Only save if there is a title

    mGui->cancel_save->button(QDialogButtonBox::Save)->setEnabled(   !pText.trimmed().isEmpty()
                                                                  &&  mPathChosen);
}

//==============================================================================

void PmrWorkspacesWindowNewWorkspaceDialog::choosePath(const bool &pChecked)
{
    Q_UNUSED(pChecked);

    QString dirName = PMRSupport::PmrWebService::getEmptyDirectory();

    if (!dirName.isEmpty()) {
        mGui->path->setText(dirName);

        mPathChosen = true;

        mGui->cancel_save->button(QDialogButtonBox::Save)->setEnabled(!title().isEmpty());
    } else {
        mGui->cancel_save->button(QDialogButtonBox::Save)->setEnabled(false);
    }
}

//==============================================================================

void PmrWorkspacesWindowNewWorkspaceDialog::setPathToolTip(const QString &pText)
{
    // Show the full path as a tooltip if its display field is too short

    QFont font = mGui->path->font();
    QFontMetrics metrics(font);

    int width = mGui->path->width();

    if(metrics.width(pText) > width)
        mGui->path->setToolTip(pText);
    else
        mGui->path->setToolTip("");
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
