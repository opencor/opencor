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
    mWorkspace(pWorkspace),
    mDiffHtmls(QMap<QString, QString>())
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

    int halfMargin = margin >> 1;

    messageLayout->setContentsMargins(0, 0, halfMargin, halfMargin);

    messageWidget->setLayout(messageLayout);

    QLabel *messageLabel = new QLabel(tr("Message:"), messageWidget);
    QFont labelFont = messageLabel->font();

    labelFont.setBold(true);

    messageLabel->setFont(labelFont);

    mMessageValue = new QTextEdit(messageWidget);

    messageLayout->addWidget(messageLabel);
    messageLayout->addWidget(mMessageValue);

    mVerticalSplitter->addWidget(messageWidget);

    // Create our changes-related widget, populate it, and add it to our
    // vertical splitter

    QWidget *changesWidget = new QWidget(mVerticalSplitter);
    QVBoxLayout *changesLayout = new QVBoxLayout(changesWidget);

    changesLayout->setContentsMargins(0, halfMargin, halfMargin, 0);

    changesWidget->setLayout(changesLayout);

    QLabel *changesLabel = new QLabel(changesWidget);

    changesLabel->setFont(labelFont);

    mChangesValue = new QListView(changesWidget);

#ifdef Q_OS_MAC
    mChangesValue->setAttribute(Qt::WA_MacShowFocusRect, false);
#endif
    mChangesValue->setSelectionMode(QAbstractItemView::ExtendedSelection);

    mSelectAllChangesCheckBox = new QCheckBox(tr("Select all the changes"), changesWidget);

    mSelectAllChangesCheckBox->setTristate(true);

    changesLayout->addWidget(changesLabel);
    changesLayout->addWidget(mChangesValue);
    changesLayout->addWidget(mSelectAllChangesCheckBox);

    mVerticalSplitter->addWidget(changesWidget);

    // Customise our vertical splitter and add it to our horizontal splitter

    mVerticalSplitter->setCollapsible(0, false);
    mVerticalSplitter->setCollapsible(1, false);

    mHorizontalSplitter->addWidget(mVerticalSplitter);

    // Create our Web viewer and add it to our horizontal splitter

    QWidget *webViewerWidget = new QWidget(mVerticalSplitter);
    QVBoxLayout *webViewerLayout = new QVBoxLayout(webViewerWidget);

    webViewerLayout->setContentsMargins(halfMargin, 0, 0, 0);

    webViewerWidget->setLayout(webViewerLayout);

    QLabel *webViewerLabel = new QLabel(tr("Changes:"), webViewerWidget);

    webViewerLabel->setFont(labelFont);

    mWebViewer = new WebViewerWidget::WebViewerWidget(mHorizontalSplitter);

    mWebViewer->setContextMenuPolicy(Qt::CustomContextMenu);
    mWebViewer->setOverrideCursor(true);
    mWebViewer->setZoomingEnabled(false);

    webViewerLayout->addWidget(webViewerLabel);
    webViewerLayout->addWidget(new Core::BorderedWidget(mWebViewer,
                                                        true, true, true, true));

    mHorizontalSplitter->addWidget(webViewerWidget);

    // Now, we can customise our horizontal splitter and add it to our layout

    mHorizontalSplitter->setCollapsible(0, false);
    mHorizontalSplitter->setCollapsible(1, false);
    mHorizontalSplitter->setStretchFactor(1, 1);

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

    // Populate ourselves with the list of files that have changed

    mModel = new QStandardItemModel(this);
    mProxyModel = new QSortFilterProxyModel(this);

    mProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
    mProxyModel->setSourceModel(mModel);

    mChangesValue->setModel(mProxyModel);

    populateModel(pWorkspace->rootFileNode());

    mProxyModel->sort(0);

    changesLabel->setText((mModel->rowCount() == 1)?
                               tr("1 change:"):
                               tr("%1 changes:").arg(mModel->rowCount()));

    mSelectAllChangesCheckBox->setVisible(mModel->rowCount() != 1);

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

    // Retrieve our diff template

    Core::readFileContentsFromFile(":/PMRWorkspacesWindow/diff.html", mDiffTemplate);

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
                fileItem->setToolTip(fileNode->path());

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

QString PmrWorkspacesWindowSynchronizeDialog::diffHtml(const QString &pFileName)
{
    // Return the diff for the given file name

    QString relativeFileName = QDir(mWorkspace->path()).relativeFilePath(pFileName);
    QString headFileContents = mWorkspace->headFileContents(relativeFileName);
    QString workingFileContents;

    Core::readFileContentsFromFile(pFileName, workingFileContents);

    typedef diff_match_patch<std::wstring> DiffMatchPatch;

    DiffMatchPatch diffMatchPatch;
    DiffMatchPatch::Diffs diffs = diffMatchPatch.diff_main(headFileContents.toStdWString(), workingFileContents.toStdWString());

    diffMatchPatch.diff_cleanupEfficiency(diffs);

    std::wstring html = L"<code>";
    std::wstring text = std::wstring();

    for (DiffMatchPatch::Diffs::const_iterator diffIterator = diffs.begin(), endDiffIterator = diffs.end();
         diffIterator != endDiffIterator; ++diffIterator) {
        std::wstring::size_type textSize = (*diffIterator).text.size();
        std::wstring::const_pointer i;
        std::wstring::const_pointer iMax;

        for (i = (*diffIterator).text.c_str(), iMax = i+textSize; i != iMax; ++i) {
            switch (*i) {
            case L'<':
            case L'>':
            case L'\n':
                textSize += 3;

                break;
            case L'&':
                textSize += 4;

                break;
            case L' ':
                textSize += 5;

                break;
            }
        }

        if (textSize == (*diffIterator).text.size()) {
            text = (*diffIterator).text;
        } else {
            text.clear();
            text.reserve(textSize);

            for (i = (*diffIterator).text.c_str(); i != iMax; ++i) {
                switch (*i) {
                case L'<':
                    text += L"&lt;";

                    break;
                case L'>':
                    text += L"&gt;";

                    break;
                case L'\n':
                    text += L"<br>";

                    break;
                case L'&':
                    text += L"&amp;";

                    break;
                case L' ':
                    text += L"&nbsp;";

                    break;
                default:
                    text += *i;
                }
            }
        }

        switch ((*diffIterator).operation) {
        case DiffMatchPatch::EQUAL:
            html += text;

            break;
        case DiffMatchPatch::INSERT:
            html += L"<span style=\"background:#e6ffe6;\">"+text+L"</span>";

            break;
        case DiffMatchPatch::DELETE:
            html += L"<span style=\"background:#ffe6e6;\">"+text+L"</span>";

            break;
        }
    }

    html += L"</code>";

    return QString::fromStdWString(html);
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::updateDiffInformation(const QModelIndex &pNewIndex,
                                                                 const QModelIndex &pOldIndex)
{
    Q_UNUSED(pOldIndex);

    // Determine the name of the new item and see whether we have already
    // computed the differences for it, and then show them

    QString fileName = mModel->itemFromIndex(mProxyModel->mapToSource(pNewIndex))->text();
    QString html = mDiffHtmls.value(fileName);

    if (html.isEmpty()) {
        html = diffHtml(fileName);

        mDiffHtmls.insert(fileName, html);
    }

    mWebViewer->webView()->setHtml(mDiffTemplate.arg(html));
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
