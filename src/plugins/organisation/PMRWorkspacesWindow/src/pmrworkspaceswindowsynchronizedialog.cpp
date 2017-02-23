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
#include "cellmlfilemanager.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "i18ninterface.h"
#include "pmrworkspace.h"
#include "pmrworkspaceswindowsynchronizedialog.h"
#include "splitterwidget.h"
#include "toolbarwidget.h"
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
#include <QTimer>
#include <QVBoxLayout>
#include <QWebView>

//==============================================================================

#include "diff_match_patch.h"

//==============================================================================

namespace OpenCOR {
namespace PMRWorkspacesWindow {

//==============================================================================

PmrWorkspacesWindowSynchronizeDialogItem::PmrWorkspacesWindowSynchronizeDialogItem(PMRSupport::PmrWorkspaceFileNode *pFileNode) :
    QStandardItem(pFileNode->path()),
    mFileNode(pFileNode)
{
}

//==============================================================================

PMRSupport::PmrWorkspaceFileNode * PmrWorkspacesWindowSynchronizeDialogItem::fileNode() const
{
    // Return our file node

    return mFileNode;
}

//==============================================================================

static const auto SettingsCellmlTextFormatSupport = QStringLiteral("CellmlTextFormatSupport");
static const auto SettingsHorizontalSplitterSizes = QStringLiteral("HorizontalSplitterSizes");
static const auto SettingsVerticalSplitterSizes   = QStringLiteral("VerticalSplitterSizes");

//==============================================================================

PmrWorkspacesWindowSynchronizeDialog::PmrWorkspacesWindowSynchronizeDialog(const QString &pSettingsGroup,
                                                                           PMRSupport::PmrWorkspace *pWorkspace,
                                                                           QTimer *pTimer,
                                                                           QWidget *pParent) :
    Core::Dialog(pParent),
    mSettingsGroup(pSettingsGroup),
    mWorkspace(pWorkspace),
    mDiffHtmls(QMap<QString, QString>()),
    mCellmlDiffHtmls(QMap<QString, QString>()),
    mPreviouslySelectedIndexes(QModelIndexList())
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

    Core::ToolBarWidget *webViewerToolBarWidget = new Core::ToolBarWidget(webViewerWidget);
    QLabel *webViewerLabel = new QLabel(tr("Changes:"), webViewerWidget);
    QWidget *webViewerSpacer = new QWidget(webViewerToolBarWidget);
    QAction *webViewerNormalSizeAction = Core::newAction(QIcon(":/oxygen/actions/zoom-original.png"), webViewerToolBarWidget);
    QAction *webViewerZoomInAction = Core::newAction(QIcon(":/oxygen/actions/zoom-in.png"), webViewerToolBarWidget);
    QAction *webViewerZoomOutAction = Core::newAction(QIcon(":/oxygen/actions/zoom-out.png"), webViewerToolBarWidget);

    mWebViewerCellmlTextFormatAction = Core::newAction(QIcon(":/CellMLSupport/logo.png"), webViewerToolBarWidget);

    webViewerLabel->setAlignment(Qt::AlignBottom);
    webViewerLabel->setFont(labelFont);

    webViewerSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    I18nInterface::retranslateAction(mWebViewerCellmlTextFormatAction, tr("CellML Text Format"),
                                     tr("Try to use the CellML Text format whenever possible"));
    I18nInterface::retranslateAction(webViewerNormalSizeAction, tr("Normal Size"),
                                     tr("Reset the size of the changes"));
    I18nInterface::retranslateAction(webViewerZoomInAction, tr("Zoom In"),
                                     tr("Zoom in the changes"));
    I18nInterface::retranslateAction(webViewerZoomOutAction, tr("Zoom Out"),
                                     tr("Zoom out the changes"));

    mWebViewerCellmlTextFormatAction->setCheckable(true);

    webViewerToolBarWidget->addWidget(webViewerLabel);
    webViewerToolBarWidget->addWidget(webViewerSpacer);
    webViewerToolBarWidget->addAction(mWebViewerCellmlTextFormatAction);
    webViewerToolBarWidget->addSeparator();
    webViewerToolBarWidget->addAction(webViewerNormalSizeAction);
    webViewerToolBarWidget->addSeparator();
    webViewerToolBarWidget->addAction(webViewerZoomInAction);
    webViewerToolBarWidget->addAction(webViewerZoomOutAction);

    mWebViewer = new WebViewerWidget::WebViewerWidget(mHorizontalSplitter);

    mWebViewer->setContextMenuPolicy(Qt::CustomContextMenu);
    mWebViewer->setOverrideCursor(true);

    webViewerLayout->addWidget(webViewerToolBarWidget);
    webViewerLayout->addWidget(new Core::BorderedWidget(mWebViewer,
                                                        true, true, true, true));

    mHorizontalSplitter->addWidget(webViewerWidget);

    // Now, we can customise our horizontal splitter and add it to our layout

    mHorizontalSplitter->setCollapsible(0, false);
    mHorizontalSplitter->setCollapsible(1, false);
    mHorizontalSplitter->setStretchFactor(1, 1);

    layout->addWidget(mHorizontalSplitter);

    // Retrieve our user's settings

    QSettings settings;

    settings.beginGroup(mSettingsGroup);
        settings.beginGroup(objectName());
            mWebViewer->loadSettings(&settings);

            mWebViewerCellmlTextFormatAction->setChecked(settings.value(SettingsCellmlTextFormatSupport, true).toBool());

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

    connect(pTimer, SIGNAL(timeout()),
            this, SLOT(refreshChanges()));

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

    connect(mChangesValue->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(updateDiffInformation()));

    connect(mWebViewerCellmlTextFormatAction, SIGNAL(toggled(bool)),
            this, SLOT(updateDiffInformation()));
    connect(webViewerNormalSizeAction, SIGNAL(triggered(bool)),
            mWebViewer, SLOT(resetZoom()));
    connect(webViewerZoomInAction, SIGNAL(triggered(bool)),
            mWebViewer, SLOT(zoomIn()));
    connect(webViewerZoomOutAction, SIGNAL(triggered(bool)),
            mWebViewer, SLOT(zoomOut()));

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
    // Keep track of our user's settings

    QSettings settings;

    settings.beginGroup(mSettingsGroup);
        settings.beginGroup(objectName());
            mWebViewer->saveSettings(&settings);

            settings.setValue(SettingsCellmlTextFormatSupport,
                              mWebViewerCellmlTextFormatAction->isChecked());

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

PmrWorkspacesWindowSynchronizeDialogItems PmrWorkspacesWindowSynchronizeDialog::populateModel(PMRSupport::PmrWorkspaceFileNode *pFileNode)
{
    // List all the files that have changed

    PmrWorkspacesWindowSynchronizeDialogItems res = PmrWorkspacesWindowSynchronizeDialogItems();

    foreach (PMRSupport::PmrWorkspaceFileNode *fileNode, pFileNode->children()) {
        if (fileNode->hasChildren()) {
            // This is a folder, so populate ourselves with its children

            res << populateModel(fileNode);
        } else {
            // This is a file, so check whether it has changes

            QChar status = fileNode->status().second;

            if ((status != '\0') && (status != ' ')) {
                // This is a changed file, so check whether we already know
                // about it

                PmrWorkspacesWindowSynchronizeDialogItem *fileItem = 0;

                for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
                    PmrWorkspacesWindowSynchronizeDialogItem *item = static_cast<PmrWorkspacesWindowSynchronizeDialogItem *>(mModel->invisibleRootItem()->child(i));

                    if (item->fileNode() == fileNode) {
                        fileItem = item;

                        break;
                    }
                }

                // Create a new item, if needed

                if (!fileItem) {
                    fileItem = new PmrWorkspacesWindowSynchronizeDialogItem(fileNode);

                    fileItem->setCheckable(true);
                    fileItem->setCheckState(Qt::Checked);
                    fileItem->setEditable(false);
                    fileItem->setToolTip(fileNode->path());

                    mModel->appendRow(fileItem);
                }

                res << fileItem;
            }
        }
    }

    return res;
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

void PmrWorkspacesWindowSynchronizeDialog::refreshChanges()
{
    // Keep track of our existing items

    PmrWorkspacesWindowSynchronizeDialogItems oldItems = PmrWorkspacesWindowSynchronizeDialogItems();

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i)
        oldItems << static_cast<PmrWorkspacesWindowSynchronizeDialogItem *>(mModel->invisibleRootItem()->child(i));

    // Update our model by (re)populating it
    // Note: we don't need to refresh the status of our workspace since it has
    //       been done in PmrWorkspacesWindowWidget::refreshWorkspace()...

    PmrWorkspacesWindowSynchronizeDialogItems newItems = populateModel(mWorkspace->rootFileNode());

    // Delete old unused items

    PmrWorkspacesWindowSynchronizeDialogItems oldItemsToDelete = PmrWorkspacesWindowSynchronizeDialogItems();

    foreach (PmrWorkspacesWindowSynchronizeDialogItem *oldItem, oldItems) {
        if (!newItems.contains(oldItem))
            oldItemsToDelete << oldItem;
    }

    foreach (PmrWorkspacesWindowSynchronizeDialogItem *oldItemsToDelete, oldItemsToDelete)
        mModel->invisibleRootItem()->removeRow(oldItemsToDelete->row());
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

bool PmrWorkspacesWindowSynchronizeDialog::cellmlText(const QString &pFileName,
                                                      QString &pCellmlText)
{
    // Try to generate the CellML Text version of the given CellML file

    if (!Core::exec(qApp->applicationFilePath(),
                    QStringList() << "-c"
                                  << "CellMLTextView::import"
                                  << pFileName,
                    pCellmlText)) {
        return true;
    } else {
        return false;
    }
}

//==============================================================================

QString PmrWorkspacesWindowSynchronizeDialog::diffHtml(const QString &pOld,
                                                       const QString &pNew)
{
    // Return the diff between the given old and new strings

    typedef diff_match_patch<std::wstring> DiffMatchPatch;

    DiffMatchPatch diffMatchPatch;
    DiffMatchPatch::Diffs diffs = diffMatchPatch.diff_main(pOld.toStdWString(), pNew.toStdWString());

    diffMatchPatch.diff_cleanupEfficiency(diffs);

    std::wstring html = L"<pre>";
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
                textSize += 3;

                break;
            case L'&':
                textSize += 4;

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
                case L'&':
                    text += L"&amp;";

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
            html += L"<span class=\"insert\">"+text+L"</span>";

            break;
        case DiffMatchPatch::DELETE:
            html += L"<span class=\"delete\">"+text+L"</span>";

            break;
        }
    }

    html += L"</pre>";

    return QString::fromStdWString(html);
}

//==============================================================================

QString PmrWorkspacesWindowSynchronizeDialog::diffHtml(const QString &pFileName)
{
    // Temporarily save the contents of the head version of the given file

    QString oldFileName = Core::temporaryFileName();
    QString oldFileContents = mWorkspace->headFileContents(QDir(mWorkspace->path()).relativeFilePath(pFileName));

    Core::writeFileContentsToFile(oldFileName, oldFileContents);

    // Check whether both the head and working versions of the given file are
    // text files

    QString res = QString();
    bool oldFileEmpty = oldFileContents.isEmpty();
    bool newFileExists = QFile::exists(pFileName);

    if (   Core::isTextFile(oldFileName)
        && (!newFileExists || Core::isTextFile(pFileName))) {
        // Both versions of the given file are not text files, so check whether
        // they are also CellML files

        if (   mWebViewerCellmlTextFormatAction->isChecked()
            && (oldFileEmpty || CellMLSupport::CellmlFileManager::instance()->isCellmlFile(oldFileName))
            && (!newFileExists ||  CellMLSupport::CellmlFileManager::instance()->isCellmlFile(pFileName))) {
            // We are dealing with a CellML file, so generate the CellML Text
            // version of the file, this for both its head and working versions,
            // and if successful then diff them

            QString oldCellmlTextContents = QString();
            QString newCellmlTextContents = QString();

            if (   (oldFileEmpty || cellmlText(oldFileName, oldCellmlTextContents))
                && (!newFileExists || cellmlText(pFileName, newCellmlTextContents))) {
                res = diffHtml(oldCellmlTextContents, newCellmlTextContents);

                mCellmlDiffHtmls.insert(pFileName, res);
            }
        }

        // At this stage, if we haven't got any diff information then it means
        // that we are either dealing with a text file, that we don't want to
        // use the CellML Text format or that we couldn't convert both the head
        // and working versions of the file to CellML Text format, in which case
        // we diff their raw contents

        if (res.isEmpty()) {
            QString newFileContents;

            Core::readFileContentsFromFile(pFileName, newFileContents);

            res = diffHtml(oldFileContents, newFileContents);

            mDiffHtmls.insert(pFileName, res);
        }
    } else {
        // We are dealing with a binary file

        res = "<pre><span class=\"warning\">["+tr("Binary File")+"]</span></pre>";

        mDiffHtmls.insert(pFileName, res);
    }

    QFile::remove(oldFileName);

    return res;
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::updateDiffInformation()
{
    // If there are no selected indexes then select the indexes that were
    // previously selected otherwise retrieve the file name of the selected
    // indexes and see whether we have already computed their differences, and
    // then show those differences

    QModelIndexList indexes = mChangesValue->selectionModel()->selectedIndexes();

    if (indexes.isEmpty()) {
        // No selected indexes, so select the previously selected indexes

        foreach (const QModelIndex &index, mPreviouslySelectedIndexes)
            mChangesValue->selectionModel()->select(index, QItemSelectionModel::Select);
    } else {
        // We have some selected indexes, so keep track of them

        mPreviouslySelectedIndexes = indexes;

        // Go through each selected index and retrieve its differences, if it
        // hasn't already been done, and show them (respecting the order in whic
        // they are listed)

        QString html = QString();
        bool oneFile = indexes.count() == 1;
        bool firstFile = true;

        for (int i = 0, iMax = mProxyModel->rowCount(); i < iMax; ++i) {
            QModelIndex index = mProxyModel->index(i, 0);

            if (indexes.contains(index)) {
                // Try to retrieve the diff for the CellML Text based version
                // or, failing that, the diff for the raw version and if that
                // still fails, we compute the diff

                QString fileName = mModel->itemFromIndex(mProxyModel->mapToSource(index))->text();
                QString fileDiffHtml = mWebViewerCellmlTextFormatAction->isChecked()?
                                           mCellmlDiffHtmls.value(fileName):
                                           mDiffHtmls.value(fileName);

                if (   !mWebViewerCellmlTextFormatAction->isChecked()
                    &&  fileDiffHtml.isEmpty()) {
                    fileDiffHtml = mDiffHtmls.value(fileName);
                }

                if (fileDiffHtml.isEmpty())
                    fileDiffHtml = diffHtml(fileName);

                if (!oneFile && !firstFile)
                    html += "<br>";

                if (!oneFile)
                    html += "<div>"+fileName+"</div>";

                html += fileDiffHtml;

                firstFile = false;
            }
        }

        mWebViewer->webView()->setHtml(mDiffTemplate.arg(html));
    }
}

//==============================================================================

}   // namespace PMRWorkspacesWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
