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
// PMR Workspaces window new workspace dialog
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrsupport.h"
#include "pmrworkspace.h"
#include "pmrworkspaceswindownewworkspacedialog.h"
#include "pmrworkspaceswindowplugin.h"

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
    Core::Dialog(pParent),
    mGui(new Ui::PmrWorkspacesWindowNewWorkspaceDialog)
{
    // Customise our settings

    mSettings.beginGroup(SettingsPlugins);
    mSettings.beginGroup(PluginName);
    mSettings.beginGroup("PmrWorkspacesWindowNewWorkspaceDialog");

    // Set up the GUI

    mGui->setupUi(this);

#ifdef Q_OS_MAC
    mGui->titleValue->setAttribute(Qt::WA_MacShowFocusRect, false);
    mGui->folderValue->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    // The Ok button is disabled to start with

    mGui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    // Connect some signals

    connect(mGui->titleValue, &QLineEdit::textChanged,
            this, &PmrWorkspacesWindowNewWorkspaceDialog::updateOkButton);
    connect(mGui->folderValue, &QLineEdit::textChanged,
            this, &PmrWorkspacesWindowNewWorkspaceDialog::updateOkButton);

    connect(mGui->folderToolButton, &QToolButton::clicked,
            this, &PmrWorkspacesWindowNewWorkspaceDialog::choosePath);

    connect(mGui->buttonBox, &QDialogButtonBox::accepted,
            this, &PmrWorkspacesWindowNewWorkspaceDialog::accept);
    connect(mGui->buttonBox, &QDialogButtonBox::rejected,
            this, &PmrWorkspacesWindowNewWorkspaceDialog::reject);

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
                                                              && !PMRSupport::isGitDirectory(dirName));
}

//==============================================================================

void PmrWorkspacesWindowNewWorkspaceDialog::choosePath()
{
    // Choose a non-Git directory for our new workspace

    QString dirName = PMRSupport::getNonGitDirectory();

    if (!dirName.isEmpty()) {
        mGui->folderValue->setText(QDir::toNativeSeparators(dirName));
    }
}

//==============================================================================

} // namespace PMRWorkspacesWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
