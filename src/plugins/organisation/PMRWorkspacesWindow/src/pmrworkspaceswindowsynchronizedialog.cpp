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

#include "borderedwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "pmrworkspace.h"
#include "pmrworkspaceswindowsynchronizedialog.h"
#include "splitterwidget.h"
#include "webviewerwidget.h"

//==============================================================================

#include <QCheckBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <QDir>
#include <QKeyEvent>
#include <QLabel>
#include <QListView>
#include <QPushButton>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWebView>

//==============================================================================

#include "diff_match_patch.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

static const auto SettingsHorizontalSplitterSizes = QStringLiteral("HorizontalSplitterSizes");
static const auto SettingsVerticalSplitterSizes   = QStringLiteral("VerticalSplitterSizes");

//==============================================================================

PmrWorkspacesWindowSynchronizeDialog::PmrWorkspacesWindowSynchronizeDialog(const QString &pSettingsGroup,
                                                                           PMRSupport::PmrWorkspace *pWorkspace,
                                                                           QWidget *pParent) :
    Core::Dialog(pParent),
    mSettingsGroup(pSettingsGroup),
    mWorkspace(pWorkspace)
{
    // Set both our object name and title

    setObjectName("PmrWorkspacesWindowSynchronizeDialog");
    setWindowTitle(tr("Synchronise With PMR"));

    // Create and set our vertical layout

    QVBoxLayout *layout = new QVBoxLayout(this);

    setLayout(layout);

    // Create both our horizontal and vertical splitters

    mHorizontalSplitter = new Core::SplitterWidget(this);
    mVerticalSplitter = new Core::SplitterWidget(Qt::Vertical, mHorizontalSplitter);

    // Create our message-related widget, populate it, and add it to our
    // vertical splitter

    QWidget *messageWidget = new QWidget(mVerticalSplitter);
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

    mVerticalSplitter->addWidget(messageWidget);

    // Create our changes-related widget, populate it, and add it to our
    // vertical splitter

    QWidget *changesWidget = new QWidget(mVerticalSplitter);
    QVBoxLayout *changesLayout = new QVBoxLayout(changesWidget);

    changesLayout->setContentsMargins(0, margin >> 1, 0, 0);

    changesWidget->setLayout(changesLayout);

    mChangesLabel = new QLabel(changesWidget);
    QFont changesLabelFont = mChangesLabel->font();

    changesLabelFont.setBold(true);

    mChangesLabel->setFont(changesLabelFont);

    mChangesValue = new QListView(changesWidget);

#ifdef Q_OS_MAC
    mChangesValue->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif
    mChangesValue->setSelectionMode(QAbstractItemView::ExtendedSelection);

    mSelectAllChangesCheckBox = new QCheckBox(tr("Select all the changes"), changesWidget);

    mSelectAllChangesCheckBox->setTristate(true);

    changesLayout->addWidget(mChangesLabel);
    changesLayout->addWidget(mChangesValue);
    changesLayout->addWidget(mSelectAllChangesCheckBox);

    mVerticalSplitter->addWidget(changesWidget);

    // Customise our vertical splitter and add it to our horizontal splitter

    mVerticalSplitter->setCollapsible(0, false);
    mVerticalSplitter->setCollapsible(1, false);

    mHorizontalSplitter->addWidget(mVerticalSplitter);

    // Create our Web viewer and add it to our horizontal splitter

    mWebViewer = new WebViewerWidget::WebViewerWidget(mHorizontalSplitter);

    mWebViewer->setContextMenuPolicy(Qt::CustomContextMenu);
    mWebViewer->setOverrideCursor(true);
    mWebViewer->setZoomingEnabled(false);

    mHorizontalSplitter->addWidget(new Core::BorderedWidget(mWebViewer,
                                                            true, true, true, true));

    // Now, we can customise our horizontal splitter and add it to our layout

    mHorizontalSplitter->setCollapsible(0, false);
    mHorizontalSplitter->setCollapsible(1, false);

    layout->addWidget(mHorizontalSplitter);

    // Set the original sizes of our splitters

    QSettings settings;

    settings.beginGroup(mSettingsGroup);
        settings.beginGroup(objectName());
            mHorizontalSplitter->setSizes(qVariantListToIntList(settings.value(SettingsHorizontalSplitterSizes).toList()));
            mVerticalSplitter->setSizes(qVariantListToIntList(settings.value(SettingsVerticalSplitterSizes,
                                                                             QVariantList() << 222 << 555).toList()));
        settings.endGroup();
    settings.endGroup();

    // Add some dialog buttons

    mButtonBox = new QDialogButtonBox(this);

    mButtonBox->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

    layout->addWidget(mButtonBox);

    // Populate ourselves with the list of files that have changed and make sure
    // that the width of our list view is always such that we can see all of its
    // contents (though up to a point!)
    // Note: the +2 is to avoid getting a horizontal scroll bar...

    mModel = new QStandardItemModel(this);
    mProxyModel = new QSortFilterProxyModel(this);

    mProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    mProxyModel->setSourceModel(mModel);

    mChangesValue->setModel(mProxyModel);

    populateModel(pWorkspace->rootFileNode());

    mProxyModel->sort(0);

    mChangesLabel->setText((mModel->rowCount() == 1)?
                               tr("1 change:"):
                               tr("%1 changes:").arg(mModel->rowCount()));

    mSelectAllChangesCheckBox->setVisible(mModel->rowCount() != 1);

    mChangesValue->setMinimumWidth(qMin(qApp->desktop()->availableGeometry().width() >> 1,
                                        mChangesValue->sizeHintForColumn(0)+2));

    // Connect some signals

    connect(mMessageValue, SIGNAL(textChanged()),
            this, SLOT(updateOkButton()));

    connect(mModel, SIGNAL(itemChanged(QStandardItem *)),
            this, SLOT(updateSelectAllChangesCheckBox(QStandardItem *)));

    connect(mSelectAllChangesCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(updateOkButton()));
    connect(mSelectAllChangesCheckBox, SIGNAL(clicked(bool)),
            this, SLOT(selectAllChangesCheckBoxClicked()));

    connect(mButtonBox, SIGNAL(accepted()),
            this, SLOT(acceptSynchronization()));
    connect(mButtonBox, SIGNAL(rejected()),
            this, SLOT(reject()));

    connect(mChangesValue->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(updateDiffInformation(const QModelIndex &, const QModelIndex &)));

    // Select our first changes

    mChangesValue->setCurrentIndex(mProxyModel->index(0, 0));

    // Initialise (update) the checked state of our Select All check box

    updateSelectAllChangesCheckBox();

    // Set our minimum size and adjust our initial size
    // Note: the size adjustment is just so that it looks 'better' the very
    //       first time round. From there, it will indeed be up to the user to
    //       decide about the (position and) size of the dialog...

    QSize minimumSize = Core::minimumWidgetSize(this);

    setMinimumSize(minimumSize);

    if (!hasPositionAndSize()) {
        QSize size = minimumSize;

        size.setWidth(qMax(size.width(), int(0.85*size.height())));

        resize(size);
    }
}

//==============================================================================

PmrWorkspacesWindowSynchronizeDialog::~PmrWorkspacesWindowSynchronizeDialog()
{
    // Keep track of our veritcal splitter's sizes

    QSettings settings;

    settings.beginGroup(mSettingsGroup);
        settings.beginGroup(objectName());
            settings.setValue(SettingsHorizontalSplitterSizes,
                              qIntListToVariantList(mHorizontalSplitter->sizes()));
            settings.setValue(SettingsVerticalSplitterSizes,
                              qIntListToVariantList(mVerticalSplitter->sizes()));
        settings.endGroup();
    settings.endGroup();
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::keyPressEvent(QKeyEvent *pEvent)
{
    // Check whether we are trying to quick synchronise

    if (   (pEvent->modifiers() & Qt::ControlModifier)
        && (pEvent->key() == Qt::Key_Return)) {
        // Pretend that we clicked on the Ok button, if possible

        if (mButtonBox->button(QDialogButtonBox::Ok)->isEnabled())
            acceptSynchronization();

        // Accept the event

        pEvent->accept();
    } else {
        // Default handling of the event

        Core::Dialog::keyPressEvent(pEvent);
    }
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::populateModel(PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
    // List all the files that have changed

    foreach (PMRSupport::PmrWorkspaceFileNode *fileNode, pFileNode->children()) {
        if (fileNode->hasChildren()) {
            populateModel(fileNode);
        } else {
            QChar status = fileNode->status().second;

            if ((status != '\0') && (status != ' ')) {
                QStandardItem *fileItem = new QStandardItem(fileNode->path());

                fileItem->setCheckable(true);
                fileItem->setCheckState(Qt::Checked);
                fileItem->setEditable(false);

                mModel->appendRow(fileItem);
            }
        }
    }
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

    QStringList res = QStringList();

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        QStandardItem *fileItem = mModel->invisibleRootItem()->child(i);

        if (fileItem->checkState() == Qt::Checked)
            res << mModel->invisibleRootItem()->child(i)->text();
    }

    return res;
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::updateSelectAllChangesCheckBox(QStandardItem *pItem)
{
    // Un/check the selected items, if any
    // Note: we temporally disable the handling of the itemChanged() signal
    //       since we 'manually' set everything ourselves...

    if (pItem) {
        disconnect(mModel, SIGNAL(itemChanged(QStandardItem *)),
                   this, SLOT(updateSelectAllChangesCheckBox(QStandardItem *)));

        foreach (const QModelIndex &fileIndex, mChangesValue->selectionModel()->selectedIndexes())
            mModel->itemFromIndex(mProxyModel->mapToSource(fileIndex))->setCheckState(pItem->checkState());

        connect(mModel, SIGNAL(itemChanged(QStandardItem *)),
                this, SLOT(updateSelectAllChangesCheckBox(QStandardItem *)));
    }

    // Update the checked state of our Select All check box

    int nbOfCheckedFiles = 0;

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i)
        nbOfCheckedFiles += mModel->invisibleRootItem()->child(i)->checkState() == Qt::Checked;

    mSelectAllChangesCheckBox->setCheckState(nbOfCheckedFiles?
                                                 (nbOfCheckedFiles == mModel->rowCount())?
                                                     Qt::Checked:
                                                     Qt::PartiallyChecked:
                                                 Qt::Unchecked);
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::selectAllChangesCheckBoxClicked()
{
    // If our checked state is partially checked, then we want to make it fully
    // so

    if (mSelectAllChangesCheckBox->checkState() == Qt::PartiallyChecked)
        mSelectAllChangesCheckBox->setCheckState(Qt::Checked);

    // Un/select all the files

    Qt::CheckState checkState = mSelectAllChangesCheckBox->isChecked()?Qt::Checked:Qt::Unchecked;

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i)
        mModel->invisibleRootItem()->child(i)->setCheckState(checkState);
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::updateOkButton()
{
    // Update the state of our Ok button

    mButtonBox->button(QDialogButtonBox::Ok)->setEnabled(   !message().isEmpty()
                                                         &&  (mSelectAllChangesCheckBox->checkState() != Qt::Unchecked));
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::acceptSynchronization()
{
    // Confirm that we accept the synchronisation

    done(QMessageBox::Ok);
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::updateDiffInformation(const QModelIndex &pNewIndex,
                                                                 const QModelIndex &pOldIndex)
{
    Q_UNUSED(pOldIndex);

//---GRY--- TO BE DONE...

    QString fullFileName = mModel->itemFromIndex(mProxyModel->mapToSource(pNewIndex))->text();
    QString relativeFileName = QDir(mWorkspace->path()).relativeFilePath(fullFileName);
    QString headFileContents = mWorkspace->headFileContents(relativeFileName);
    QString workingFileContents;

    Core::readFileContentsFromFile(fullFileName, workingFileContents);

    diff_match_patch<std::wstring> dmp;
    diff_match_patch<std::wstring>::Diffs diffs = dmp.diff_main(headFileContents.toStdWString(), workingFileContents.toStdWString());

    mWebViewer->webView()->setHtml(QString::fromStdWString(dmp.diff_prettyHtml(diffs)));
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
