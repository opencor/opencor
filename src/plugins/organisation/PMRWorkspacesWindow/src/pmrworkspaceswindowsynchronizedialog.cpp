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

#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrworkspace.h"
#include "pmrworkspaceswindowsynchronizedialog.h"
#include "splitterwidget.h"

//==============================================================================

#include <QDialogButtonBox>
#include <QLabel>
#include <QListView>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTextEdit>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PmrWorkspacesWindowSynchronizeDialog::PmrWorkspacesWindowSynchronizeDialog(PMRSupport::PmrWorkspace *pWorkspace,
                                                                           QWidget *pParent) :
    QDialog(pParent)
{
    // Set our title

    setWindowTitle(tr("Synchronise With PMR"));

    // Create and set our vertical layout

    QVBoxLayout *layout = new QVBoxLayout(this);

    setLayout(layout);

    // Create our splitter

    mSplitter = new Core::SplitterWidget(Qt::Vertical, this);

    // Create our message-related widget, populate it, and add it to our
    // splitter

    QWidget *messageWidget = new QWidget(mSplitter);
    QVBoxLayout *messageLayout = new QVBoxLayout(messageWidget);
    int margin;

    messageLayout->getContentsMargins(0, 0, 0, &margin);
    messageLayout->setContentsMargins(0, 0, 0, margin >> 1);

    messageWidget->setLayout(messageLayout);

    QLabel *messageLabel = new QLabel(tr("Message:"), messageWidget);
    QFont messageLabelFont = messageLabel->font();

    messageLabelFont.setBold(true);

    messageLabel->setFont(messageLabelFont);

    mMessageValue = new QTextEdit(messageWidget);

    messageLayout->addWidget(messageLabel);
    messageLayout->addWidget(mMessageValue);

    mSplitter->addWidget(messageWidget);

    // Create our changes-related widget, populate it, and add it to our
    // splitter

    QWidget *changesWidget = new QWidget(mSplitter);
    QVBoxLayout *changesLayout = new QVBoxLayout(changesWidget);

    changesLayout->setContentsMargins(0, margin >> 1, 0, 0);

    changesWidget->setLayout(changesLayout);

    mChangesLabel = new QLabel(changesWidget);
    QFont changesLabelFont = mChangesLabel->font();

    changesLabelFont.setBold(true);

    mChangesLabel->setFont(changesLabelFont);

    QListView *changesValue = new QListView(changesWidget);

#ifdef Q_OS_MAC
    changesValue->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif

    changesLayout->addWidget(mChangesLabel);
    changesLayout->addWidget(changesValue);

    mSplitter->addWidget(changesWidget);

    // Customise our splitter and add it to our layout

    mSplitter->setCollapsible(0, false);
    mSplitter->setCollapsible(1, false);
    mSplitter->setSizes(QIntList() << 1 << 999);

    layout->addWidget(mSplitter);

    // Add some dialog buttons

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);

    buttonBox->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

    layout->addWidget(buttonBox);

    // Populate ourselves with the list of files that have changed

    mModel = new QStandardItemModel(this);
    mProxyModel = new QSortFilterProxyModel(this);

    mProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    mProxyModel->setSourceModel(mModel);

    changesValue->setModel(mProxyModel);

    populateModel(pWorkspace->rootFileNode(), true);

    // Connect some signals

    connect(buttonBox, SIGNAL(accepted()),
            this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()),
            this, SLOT(reject()));

    // Set our minimum size

    setMinimumSize(Core::minimumWidgetSize(this));
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::populateModel(PMRSupport::PmrWorkspaceFileNode *pFileNode,
                                                         const bool &pRootFileNode)
{
    // List all the files that have changed

    int nbOfChanges = 0;

    foreach (PMRSupport::PmrWorkspaceFileNode *fileNode, pFileNode->children()) {
        if (fileNode->hasChildren()) {
            populateModel(fileNode);
        } else {
            QChar status = fileNode->status().second;

            if ((status != '\0') && (status != ' ')) {
                ++nbOfChanges;

                QStandardItem *dataItem = new QStandardItem(fileNode->path());

                dataItem->setCheckable(true);
                dataItem->setCheckState(Qt::Checked);
                dataItem->setEditable(false);

                mModel->appendRow(dataItem);
            }
        }
    }

    mChangesLabel->setText((nbOfChanges == 1)?
                               tr("1 change:"):
                               tr("%1 changes:").arg(nbOfChanges));

    // Sort ourselves if we are the root file node

    if (pRootFileNode)
        mProxyModel->sort(0);
}

//==============================================================================

QString PmrWorkspacesWindowSynchronizeDialog::message() const
{
    // Return the commit message

    return mMessageValue->toPlainText().trimmed();
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
