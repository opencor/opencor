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
#include "pmrworkspacesnewworkspace.h"

//==============================================================================

#include <QDebug>
#include <QDir>
#include <QPushButton>
#include <QStandardPaths> // Temp
#include <QString>

//==============================================================================

#include "ui_pmrworkspacesnewworkspace.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspaces {

//==============================================================================

PmrWorkspacesNewWorkspace::PmrWorkspacesNewWorkspace(QWidget *pParent) :
    QDialog(pParent),
    mGui(new Ui::PmrWorkspacesNewWorkspace),
    mPathChosen(false)
{
    // Set up the GUI

    mGui->setupUi(this);

    // The save button is disabled until we have a title

    mGui->cancel_save->button(QDialogButtonBox::Save)->setEnabled(false);

    // Connect some signals

    connect(mGui->choosePath, SIGNAL(clicked(bool)),
            this, SLOT(choosePath(bool)));

    connect(mGui->path, SIGNAL(textChanged(const QString &)),
            this, SLOT(setPathToolTip(const QString &)));

    connect(mGui->title, SIGNAL(textChanged(const QString &)),
            this, SLOT(titleTextChanged(const QString &)));

    connect(mGui->cancel_save, SIGNAL(accepted()), this, SLOT(accept()));
    connect(mGui->cancel_save, SIGNAL(rejected()), this, SLOT(reject()));
}

//==============================================================================

PmrWorkspacesNewWorkspace::~PmrWorkspacesNewWorkspace()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PmrWorkspacesNewWorkspace::retranslateUi()
{
    // Retranslate our GUI

    mGui->retranslateUi(this);
}

//==============================================================================

const QString PmrWorkspacesNewWorkspace::description(void) const
{
    return mGui->description->toPlainText().trimmed();
}

//==============================================================================

const QString PmrWorkspacesNewWorkspace::path(void) const
{
    return mGui->path->text();
}

//==============================================================================

const QString PmrWorkspacesNewWorkspace::title(void) const
{
    return mGui->title->text().trimmed();
}

//==============================================================================

void PmrWorkspacesNewWorkspace::titleTextChanged(const QString &text)
{
    // Only save if there is a title

    mGui->cancel_save->button(QDialogButtonBox::Save)->setEnabled(!text.trimmed().isEmpty());
}

//==============================================================================

void PmrWorkspacesNewWorkspace::choosePath(const bool &checked)
{
    Q_UNUSED(checked)

    QString dirName = Core::getExistingDirectory(tr("Select Empty Directory"),
                                                 PMRSupport::PmrWorkspace::WorkspacesDirectory(),
                                                 true);

    if (!dirName.isEmpty()) {
        mGui->path->setText(dirName);
        mPathChosen = true;
        }
}

//==============================================================================

void PmrWorkspacesNewWorkspace::setPathToolTip(const QString &text)
{
    // Show the full path as a tooltip if its display field is too short

    QFont font = mGui->path->font();
    QFontMetrics metrics(font);

    int width = mGui->path->width();

    if(metrics.width(text) > width)
        mGui->path->setToolTip(text);
    else
        mGui->path->setToolTip("");
}

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
