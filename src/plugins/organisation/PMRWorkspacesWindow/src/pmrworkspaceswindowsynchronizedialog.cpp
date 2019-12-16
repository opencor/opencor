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
// PMR Workspaces window synchronise dialog
//==============================================================================

#include "borderedwidget.h"
#include "cellmlfilemanager.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "file.h"
#include "i18ninterface.h"
#include "pmrworkspace.h"
#include "pmrworkspaceswindowplugin.h"
#include "pmrworkspaceswindowsynchronizedialog.h"
#include "splitterwidget.h"
#include "toolbarwidget.h"
#include "webviewerwidget.h"

//==============================================================================

#include <QApplication>
#include <QCheckBox>
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
#include <QWebElement>
#include <QWebFrame>
#include <QWebView>

//==============================================================================

#include "libxdiffbegin.h"
    #include "diff_match_patch.h"
#include "libxdiffend.h"

//==============================================================================

#include "xdiff.h"

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

PmrWorkspacesWindowSynchronizeDialogItem::~PmrWorkspacesWindowSynchronizeDialogItem() = default;

//==============================================================================

PMRSupport::PmrWorkspaceFileNode * PmrWorkspacesWindowSynchronizeDialogItem::fileNode() const
{
    // Return our file node

    return mFileNode;
}

//==============================================================================

static const char *SettingsCellmlTextFormatSupport = "CellmlTextFormatSupport";
static const char *SettingsHorizontalSplitterSizes = "HorizontalSplitterSizes";
static const char *SettingsVerticalSplitterSizes   = "VerticalSplitterSizes";

//==============================================================================

PmrWorkspacesWindowSynchronizeDialog::PmrWorkspacesWindowSynchronizeDialog(PMRSupport::PmrWorkspace *pWorkspace,
                                                                           QTimer *pTimer,
                                                                           QWidget *pParent) :
    Core::Dialog(pParent),
    mWorkspace(pWorkspace)
{
    // Customise our settings

    mSettings.beginGroup(SettingsPlugins);
    mSettings.beginGroup(PluginName);
    mSettings.beginGroup("PmrWorkspacesWindowSynchronizeDialog");

    // Set both our object name and title

    setWindowTitle(tr("Synchronise With PMR"));

    // Create and set our vertical layout

    auto layout = new QVBoxLayout(this);

    setLayout(layout);

    // Create both our horizontal and vertical splitters

    mHorizontalSplitter = new Core::SplitterWidget(this);
    mVerticalSplitter = new Core::SplitterWidget(Qt::Vertical, mHorizontalSplitter);

    // Create our changes-related widget, populate it, and add it to our
    // vertical splitter

    auto changesWidget = new QWidget(mVerticalSplitter);
    auto changesLayout = new QVBoxLayout(changesWidget);
    int margin;

    changesLayout->getContentsMargins(nullptr, nullptr, nullptr, &margin);

    int halfMargin = margin/2;

    changesLayout->setContentsMargins(0, halfMargin, halfMargin, 0);

    changesWidget->setLayout(changesLayout);

    auto changesLabel = new QLabel(changesWidget);
    QFont newFont = changesLabel->font();

    newFont.setBold(true);

    changesLabel->setFont(newFont);

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

    // Create our message-related widget, populate it, and add it to our
    // vertical splitter

    auto messageWidget = new QWidget(mVerticalSplitter);
    auto messageLayout = new QVBoxLayout(messageWidget);

    messageLayout->setContentsMargins(0, 0, halfMargin, halfMargin);

    messageWidget->setLayout(messageLayout);

    auto messageLabel = new QLabel(tr("Message:"), messageWidget);

    messageLabel->setFont(newFont);

    mMessageValue = new QTextEdit(messageWidget);

    messageLayout->addWidget(messageLabel);
    messageLayout->addWidget(mMessageValue);

    mVerticalSplitter->addWidget(messageWidget);

    // Customise our vertical splitter and add it to our horizontal splitter

    mVerticalSplitter->setCollapsible(0, false);
    mVerticalSplitter->setCollapsible(1, false);
    mVerticalSplitter->setStretchFactor(0, 1);

    mHorizontalSplitter->addWidget(mVerticalSplitter);

    // Create our Web viewer and add it to our horizontal splitter

    auto webViewerWidget = new QWidget(mHorizontalSplitter);
    auto webViewerLayout = new QVBoxLayout(webViewerWidget);

    webViewerLayout->setContentsMargins(halfMargin, 0, 0, 0);

    webViewerWidget->setLayout(webViewerLayout);

    auto webViewerToolBarWidget = new Core::ToolBarWidget();
    auto webViewerLabel = new QLabel(tr("Changes:"), webViewerWidget);

    webViewerLabel->setAlignment(Qt::AlignBottom);
    webViewerLabel->setFont(newFont);

    auto webViewerSpacer = new QWidget(webViewerToolBarWidget);

    webViewerSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto webViewerNormalSizeAction = Core::newAction(QIcon(":/oxygen/actions/zoom-original.png"), webViewerToolBarWidget);
    auto webViewerZoomInAction = Core::newAction(QIcon(":/oxygen/actions/zoom-in.png"), webViewerToolBarWidget);
    auto webViewerZoomOutAction = Core::newAction(QIcon(":/oxygen/actions/zoom-out.png"), webViewerToolBarWidget);

    mWebViewerCellmlTextFormatAction = Core::newAction(QIcon(":/CellMLSupport/logo.png"), webViewerToolBarWidget);

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

    mWebViewer->webView()->setContextMenuPolicy(Qt::CustomContextMenu);
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

    mWebViewer->loadSettings(mSettings);

    mWebViewerCellmlTextFormatAction->setChecked(mSettings.value(SettingsCellmlTextFormatSupport, true).toBool());

    mHorizontalSplitter->setSizes(qVariantListToIntList(mSettings.value(SettingsHorizontalSplitterSizes).toList()));
    mVerticalSplitter->setSizes(qVariantListToIntList(mSettings.value(SettingsVerticalSplitterSizes,
                                                                      QVariantList() << 555 << 222).toList()));

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

    connect(pTimer, &QTimer::timeout,
            this, &PmrWorkspacesWindowSynchronizeDialog::refreshChanges);

    connect(mMessageValue, &QTextEdit::textChanged,
            this, &PmrWorkspacesWindowSynchronizeDialog::updateOkButton);

    connect(mModel, &QStandardItemModel::itemChanged,
            this, &PmrWorkspacesWindowSynchronizeDialog::updateSelectAllChangesCheckBox);

    connect(mSelectAllChangesCheckBox, &QCheckBox::toggled,
            this, &PmrWorkspacesWindowSynchronizeDialog::updateOkButton);
    connect(mSelectAllChangesCheckBox, &QCheckBox::clicked,
            this, &PmrWorkspacesWindowSynchronizeDialog::selectAllChangesCheckBoxClicked);

    connect(mButtonBox, &QDialogButtonBox::accepted,
            this, &PmrWorkspacesWindowSynchronizeDialog::acceptSynchronization);
    connect(mButtonBox, &QDialogButtonBox::rejected,
            this, &PmrWorkspacesWindowSynchronizeDialog::reject);

    connect(mChangesValue->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &PmrWorkspacesWindowSynchronizeDialog::updateDiffInformation);

    connect(mWebViewerCellmlTextFormatAction, &QAction::toggled,
            this, &PmrWorkspacesWindowSynchronizeDialog::updateDiffInformation);
    connect(webViewerNormalSizeAction, &QAction::triggered,
            mWebViewer, &WebViewerWidget::WebViewerWidget::resetZoom);
    connect(webViewerZoomInAction, &QAction::triggered,
            mWebViewer, &WebViewerWidget::WebViewerWidget::zoomIn);
    connect(webViewerZoomOutAction, &QAction::triggered,
            mWebViewer, &WebViewerWidget::WebViewerWidget::zoomOut);

    // Retrieve our diff template

    Core::readFile(":/PMRWorkspacesWindow/diff.html", mDiffTemplate);

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

    mWebViewer->saveSettings(mSettings);

    mSettings.setValue(SettingsCellmlTextFormatSupport,
                       mWebViewerCellmlTextFormatAction->isChecked());

    mSettings.setValue(SettingsHorizontalSplitterSizes,
                       qIntListToVariantList(mHorizontalSplitter->sizes()));
    mSettings.setValue(SettingsVerticalSplitterSizes,
                       qIntListToVariantList(mVerticalSplitter->sizes()));
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::keyPressEvent(QKeyEvent *pEvent)
{
    // Check whether we are trying to quick synchronise

    if (   (pEvent->modifiers() == Qt::ControlModifier)
        && (pEvent->key() == Qt::Key_Return)) {
        // Pretend that we clicked on the Ok button, if possible

        if (mButtonBox->button(QDialogButtonBox::Ok)->isEnabled()) {
            acceptSynchronization();
        }

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

    for (auto fileNode : pFileNode->children()) {
        if (fileNode->hasChildren()) {
            // This is a folder, so populate ourselves with its children

            res << populateModel(fileNode);
        } else {
            // This is a file, so check whether it has un/staged changes

            QChar iStatus = fileNode->status().first;
            QChar wStatus = fileNode->status().second;
            bool stagedFile =    ((iStatus != nullptr) && (iStatus != ' '))
                              && ((wStatus == nullptr) || (wStatus == ' '));
            bool unstagedFile = (wStatus != nullptr) && (wStatus != ' ');

            if (stagedFile || unstagedFile) {
                // This is a un/staged file, so check whether we already know
                // about it and, if so, whether its SHA-1 is still the same and
                // if that's not the case then reset a few things

                PmrWorkspacesWindowSynchronizeDialogItem *fileItem = nullptr;
                QString fileName = fileNode->path();
                QString sha1 = Core::File::sha1(fileName);

                for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
                    auto item = static_cast<PmrWorkspacesWindowSynchronizeDialogItem *>(mModel->invisibleRootItem()->child(i));

                    if (item->fileNode() == fileNode) {
                        fileItem = item;

                        if (sha1 != mSha1s.value(fileName)) {
                            mSha1s.insert(fileName, sha1);

                            mDiffHtmls.remove(fileName);
                            mCellmlDiffHtmls.remove(fileName);

                            if (mChangesValue->selectionModel()->isSelected(mProxyModel->mapFromSource(item->index()))) {
                                mNeedUpdateDiffInformation = true;
                            }
                        }

                        break;
                    }
                }

                // Create a new item, if needed

                if (fileItem == nullptr) {
                    mNbOfCheckableFiles += int(unstagedFile);

                    fileItem = new PmrWorkspacesWindowSynchronizeDialogItem(fileNode);

                    fileItem->setCheckable(unstagedFile);
                    fileItem->setCheckState(Qt::Checked);
                    fileItem->setEditable(false);
                    fileItem->setEnabled(unstagedFile);
                    fileItem->setSelectable(unstagedFile);
                    fileItem->setToolTip(QDir::toNativeSeparators(fileName));

                    mModel->appendRow(fileItem);

                    mSha1s.insert(fileName, sha1);
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

        if (fileItem->checkState() == Qt::Checked) {
            res << mModel->invisibleRootItem()->child(i)->text();
        }
    }

    return res;
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::refreshChanges()
{
    // Refresh our changes, but only if we are visible

    if (!isVisible()) {
        return;
    }

    // Keep track of our existing items

    PmrWorkspacesWindowSynchronizeDialogItems oldItems = PmrWorkspacesWindowSynchronizeDialogItems();

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        oldItems << static_cast<PmrWorkspacesWindowSynchronizeDialogItem *>(mModel->invisibleRootItem()->child(i));
    }

    // Update our model by (re)populating it
    // Note: we don't need to refresh the status of our workspace since it has
    //       been done in PmrWorkspacesWindowWidget::refreshWorkspace()...

    PmrWorkspacesWindowSynchronizeDialogItems newItems = populateModel(mWorkspace->rootFileNode());

    // Delete old unused items

    for (auto oldItem : oldItems) {
        if (!newItems.contains(oldItem)) {
            mDiffHtmls.remove(oldItem->text());
            mCellmlDiffHtmls.remove(oldItem->text());

            mModel->invisibleRootItem()->removeRow(oldItem->row());
        }
    }

    // Check whether we still have at least one item selected and, if not,
    // select the 'new' first one

    if (mChangesValue->selectionModel()->selectedIndexes().isEmpty()) {
        mChangesValue->setCurrentIndex(mProxyModel->index(0, 0));
    }

    // Update our diff information, if needed

    if (mNeedUpdateDiffInformation) {
        mNeedUpdateDiffInformation = false;

        updateDiffInformation();
    }
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::updateSelectAllChangesCheckBox(QStandardItem *pItem)
{
    // Un/check the selected items, if any
    // Note: we temporally disable the handling of the itemChanged() signal
    //       since we 'manually' set everything ourselves...

    if (pItem != nullptr) {
        disconnect(mModel, &QStandardItemModel::itemChanged,
                   this, &PmrWorkspacesWindowSynchronizeDialog::updateSelectAllChangesCheckBox);

        if (mChangesValue->selectionModel()->isSelected(mProxyModel->mapFromSource(pItem->index()))) {
            for (const auto &fileIndex : mChangesValue->selectionModel()->selectedIndexes()) {
                mModel->itemFromIndex(mProxyModel->mapToSource(fileIndex))->setCheckState(pItem->checkState());
            }
        }

        connect(mModel, &QStandardItemModel::itemChanged,
                this, &PmrWorkspacesWindowSynchronizeDialog::updateSelectAllChangesCheckBox);
    }

    // Update the checked state of our Select All check box

    int nbOfCheckedFiles = 0;

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        QStandardItem *fileItem = mModel->invisibleRootItem()->child(i);

        nbOfCheckedFiles += int(   fileItem->isEnabled()
                                && (fileItem->checkState() == Qt::Checked));
    }

    mSelectAllChangesCheckBox->setCheckState((nbOfCheckedFiles != 0)?
                                                 (nbOfCheckedFiles == mNbOfCheckableFiles)?
                                                     Qt::Checked:
                                                     Qt::PartiallyChecked:
                                                 Qt::Unchecked);
}

//==============================================================================

void PmrWorkspacesWindowSynchronizeDialog::selectAllChangesCheckBoxClicked()
{
    // If our checked state is partially checked, then we want to make it fully
    // so

    if (mSelectAllChangesCheckBox->checkState() == Qt::PartiallyChecked) {
        mSelectAllChangesCheckBox->setCheckState(Qt::Checked);
    }

    // Un/select all the files

    Qt::CheckState checkState = mSelectAllChangesCheckBox->isChecked()?Qt::Checked:Qt::Unchecked;

    for (int i = 0, iMax = mModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        QStandardItem *fileItem = mModel->invisibleRootItem()->child(i);

        if (fileItem->isEnabled()) {
            fileItem->setCheckState(checkState);
        }
    }
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
    // Note: we want to run the CLI version of OpenCOR, so on Windows this means
    //       that we need to replace the program's file extension from ".exe" to
    //       ".com"...

#ifdef Q_OS_WIN
    static const QRegularExpression ExeExtensionRegEx = QRegularExpression("\\.exe$");
#endif

    QString program = qApp->applicationFilePath();

#ifdef Q_OS_WIN
    program.replace(ExeExtensionRegEx, ".com");
#endif

    return Core::exec(program,
                      QStringList() << "-c"
                                    << "CellMLTextView::import"
                                    << pFileName,
                      pCellmlText) == 0;
}

//==============================================================================

int xdiffCallback(void *data, mmbuffer_t *pBuffer, int pBufferSize)
{
    // Add the given buffer to the given data

    for (int i = 0; i < pBufferSize; ++i) {
        *static_cast<QString *>(data) += QString(pBuffer[i].ptr).leftRef(int(pBuffer[i].size));
    }

    return 0;
}

//==============================================================================

static const char *Row = R"(    <tr class="%1">)""\n"
                         R"(        <td class="linenumber shrink rightborder">)""\n"
                          "            <code>%2</code>\n"
                          "        </td>\n"
                         R"(        <td class="linenumber shrink rightborder">)""\n"
                          "            <code>%3</code>\n"
                          "        </td>\n"
                         R"(        <td class="tag shrink">)""\n"
                          "            <code>%4</code>\n"
                          "        </td>\n"
                         R"(        <td class="expand">)""\n"
                          "            <code>%5</code>\n"
                          "        </td>\n"
                          "    </tr>\n";

//==============================================================================

PmrWorkspacesWindowSynchronizeDialog::DifferenceData PmrWorkspacesWindowSynchronizeDialog::differenceData(const QString &pOperation,
                                                                                                          const QString &pRemoveLineNumber,
                                                                                                          const QString &pAddLineNumber,
                                                                                                          const QChar &pTag,
                                                                                                          const QString &pDifference)
{
    PmrWorkspacesWindowSynchronizeDialog::DifferenceData res = PmrWorkspacesWindowSynchronizeDialog::DifferenceData();

    res.operation = pOperation;
    res.removeLineNumber = pRemoveLineNumber;
    res.addLineNumber = pAddLineNumber;
    res.tag = pTag;
    res.difference = pDifference;

    return res;
}

//==============================================================================

QString PmrWorkspacesWindowSynchronizeDialog::diffHtml(DifferencesData &pDifferencesData)
{
    // Make sure that we have some differences data

    if (pDifferencesData.isEmpty()) {
        return {};
    }

    // Highlight the differences within our differences data' difference field

    static const QChar Separator = QChar(7);
    // Note: strings, for which we want to show the difference, should never
    //       contain the Bell character, so it should be safe to use that
    //       character as our separator...

    QString oldString = QString();
    QString newString = QString();

    for (const auto &differenceData : pDifferencesData) {
        if (differenceData.tag == '+') {
            newString += differenceData.difference+Separator;
        } else {
            oldString += differenceData.difference+Separator;
        }
    }

    using DiffMatchPatch = diff_match_patch<std::wstring>;

    DiffMatchPatch diffMatchPatch;
    DiffMatchPatch::Diffs diffs = diffMatchPatch.diff_main(oldString.toStdWString(), newString.toStdWString());

    diffMatchPatch.diff_cleanupEfficiency(diffs);

    QString oldDiffString = QString();
    QString newDiffString = QString();

    for (const auto &diff : diffs) {
        QString text = cleanHtmlEscaped(QString::fromStdWString(diff.text));

        switch (diff.operation) {
        case DiffMatchPatch::EQUAL:
            oldDiffString += text;
            newDiffString += text;

            break;
        case DiffMatchPatch::INSERT:
            newDiffString += oldString.isEmpty()?
                                 text:
                                 text.contains('\n')?
                                     text.endsWith('\n')?
                                         QString(R"(<span class="add">%1</span>)""\n").arg(text.remove(Separator)):
                                         QString(R"(<span class="add">%1</span>)").arg(text.replace(Separator, QString("</span>\n")+R"(<span class="add">)")):
                                     QString(R"(<span class="add">%1</span>)").arg(text);

            break;
        case DiffMatchPatch::DELETE:
            oldDiffString += newString.isEmpty()?
                                 text:
                                 text.contains('\n')?
                                     text.endsWith('\n')?
                                         QString(R"(<span class="remove">%1</span>)""\n").arg(text.remove(Separator)):
                                         QString(R"(<span class="remove">%1</span>)").arg(text.replace(Separator, QString("</span>\n")+R"(<span class="remove">)")):
                                     QString(R"(<span class="remove">%1</span>)").arg(text);

            break;
        }
    }

    // Generate the HTML code for any differences data that we may have been
    // given

    QString html = QString();
    QStringList oldDiffStrings = oldDiffString.split(Separator);
    QStringList newDiffStrings = newDiffString.split(Separator);
    int addLineNumber = -1;
    int removeLineNumber = -1;

    for (const auto &differenceData : pDifferencesData) {
        html += QString(Row).arg(differenceData.operation,
                                 differenceData.removeLineNumber,
                                 differenceData.addLineNumber,
                                 differenceData.tag,
                                 (differenceData.tag == '+')?
                                     newDiffStrings[++addLineNumber]:
                                     oldDiffStrings[++removeLineNumber]);
    }

    pDifferencesData.clear();

    return html;
}

//==============================================================================

QString PmrWorkspacesWindowSynchronizeDialog::diffHtml(const QString &pOld,
                                                       const QString &pNew)
{
    // Retrieve the UNIX-like differences between the given old and new strings

    mmfile_t oldBlock;
    mmfile_t newBlock;
    QByteArray oldByteArray = pOld.toUtf8();
    QByteArray newByteArray = pNew.toUtf8();

    xdl_init_mmfile(&oldBlock, oldByteArray.size(), XDL_MMF_ATOMIC); // NOLINT(hicpp-signed-bitwise)
    xdl_init_mmfile(&newBlock, newByteArray.size(), XDL_MMF_ATOMIC); // NOLINT(hicpp-signed-bitwise)

    memcpy(xdl_mmfile_writeallocate(&oldBlock, oldByteArray.size()),
           oldByteArray.constData(), size_t(oldByteArray.size()));
    memcpy(xdl_mmfile_writeallocate(&newBlock, newByteArray.size()),
           newByteArray.constData(), size_t(newByteArray.size()));

    xpparam_t parameters;
    xdemitconf_t context;
    QString differences = QString();
    xdemitcb_t callback;

    parameters.flags = 0;

    context.ctxlen = 3;

    callback.priv = &differences;
    callback.outf = xdiffCallback;

    xdl_diff(&oldBlock, &newBlock, &parameters, &context, &callback);

    xdl_free_mmfile(&oldBlock);
    xdl_free_mmfile(&newBlock);

    // Generate the HTML code for those differences

    static const QRegularExpression BeforeAddLineNumberRegEx = QRegularExpression("[^\\+]*\\+");
    static const QRegularExpression BeforeAddNumberOfLinesRegEx = QRegularExpression("[^,]*,");
    static const QRegularExpression BeforeRemoveLineNumberRegEx = QRegularExpression("[^-]*-");
    static const QRegularExpression AfterLineNumberRegEx = QRegularExpression(",.*");
    static const QRegularExpression AfterNumberOfLinesRegEx = QRegularExpression(" .*");

    QString html = QString();
    QStringList differencesList = differences.split('\n');
    int differenceNumber = 0;
    int differenceMaxNumber = differencesList.count()-1;
    int addLineNumber = 0;
    int addMaxLineNumber = 0;
    int removeLineNumber = 0;
    DifferencesData differencesData = DifferencesData();

    for (const auto &difference : differencesList) {
        ++differenceNumber;

        if (difference.startsWith("@@") && difference.endsWith("@@")) {
            addLineNumber = QString(difference).remove(BeforeAddLineNumberRegEx).remove(AfterLineNumberRegEx).toInt()-1;
            addMaxLineNumber = addLineNumber+QString(difference).remove(BeforeAddNumberOfLinesRegEx).remove(AfterNumberOfLinesRegEx).toInt();

            removeLineNumber = QString(difference).remove(BeforeRemoveLineNumberRegEx).remove(AfterLineNumberRegEx).toInt()-1;

            html += QString(Row).arg("header",
                                     "...",
                                     "...",
                                     QString(),
                                     difference);
        } else {
            QString diff = difference;
            QChar tag = diff[0];

            diff.remove(0, 1);

            if (tag == '+') {
                ++addLineNumber;

                differencesData << differenceData((differenceNumber == differenceMaxNumber)?"last add":"add",
                                                  QString(), QString::number(addLineNumber),
                                                  '+', diff);
            } else if (tag == '-') {
                ++removeLineNumber;

                differencesData << differenceData((differenceNumber == differenceMaxNumber)?"last remove":"remove",
                                                  QString::number(removeLineNumber), QString(),
                                                  '-', diff);
            } else if (addLineNumber != addMaxLineNumber) {
                // Output any differences data that we may have

                html += diffHtml(differencesData);

                // Output the current line

                ++addLineNumber;
                ++removeLineNumber;

                html += QString(Row).arg((differenceNumber == differenceMaxNumber)?
                                             "last default":
                                             "default")
                                    .arg(removeLineNumber)
                                    .arg(addLineNumber)
                                    .arg(QString(),
                                         cleanHtmlEscaped(diff));
            }
        }
    }

    // Output any differences data that may be left

    html += diffHtml(differencesData);

    return html;
}

//==============================================================================

QString PmrWorkspacesWindowSynchronizeDialog::diffHtml(const QString &pFileName)
{
    // Temporarily save the contents of the head version of the given file

    QString oldFileName = Core::temporaryFileName();
    QByteArray oldFileContents = mWorkspace->headFileContents(QDir(mWorkspace->path()).relativeFilePath(pFileName));

    Core::writeFile(oldFileName, oldFileContents);

    // Retrieve the contents of the working version of the given file

    QByteArray newFileContents;

    Core::readFile(pFileName, newFileContents);

    // Check whether both the head and working versions of the given file are
    // text files
    // Note: to retrieve the contents of a binary file may result in an empty
    //       string, so if we both the old and new contents is empty it means
    //       that we are dealing with a binary file...

    QString res = QString();
    bool oldFileEmpty = oldFileContents.isEmpty();
    bool newFileEmpty = newFileContents.isEmpty();

    if (   !(oldFileEmpty && newFileEmpty)
        &&  (oldFileEmpty || Core::isTextFile(oldFileName))
        &&  (newFileEmpty || Core::isTextFile(pFileName))) {
        // Both versions of the given file are text files, so check whether they
        // are also CellML 1.0/1.1 files

        CellMLSupport::CellmlFile::Version oldCellmlVersion = CellMLSupport::CellmlFile::fileVersion(oldFileName);
        CellMLSupport::CellmlFile::Version newCellmlVersion = CellMLSupport::CellmlFile::fileVersion(pFileName);

        if (   mWebViewerCellmlTextFormatAction->isChecked()
            && !mInvalidCellmlCode.contains(oldFileContents)
            && !mInvalidCellmlCode.contains(newFileContents)
            && (oldFileEmpty || (oldCellmlVersion == CellMLSupport::CellmlFile::Version::Cellml_1_0)
                             || (oldCellmlVersion == CellMLSupport::CellmlFile::Version::Cellml_1_1))
            && (newFileEmpty || (newCellmlVersion == CellMLSupport::CellmlFile::Version::Cellml_1_0)
                             || (newCellmlVersion == CellMLSupport::CellmlFile::Version::Cellml_1_1))) {
            // We are dealing with a CellML 1.0/1.1 file, so generate the CellML
            // Text version of the file, this for both its head and working
            // versions, and if successful then diff them

            QString oldCellmlTextContents = QString();
            QString newCellmlTextContents = QString();

            if (   (oldFileEmpty || cellmlText(oldFileName, oldCellmlTextContents))
                && (newFileEmpty || cellmlText(pFileName, newCellmlTextContents))) {
                res = diffHtml(oldCellmlTextContents, newCellmlTextContents);

                mCellmlDiffHtmls.insert(pFileName, res);
            } else {
                // The conversion failed, so keep track of that fact (so as not
                // to try to convert everytime this file gets selected)

                if (!oldFileEmpty) {
                    mInvalidCellmlCode << oldFileContents;
                }

                if (!newFileEmpty) {
                    mInvalidCellmlCode << newFileContents;
                }
            }
        }

        // At this stage, if we haven't got any diff information then it means
        // that we are either dealing with a text file, that we don't want to
        // use the CellML Text format or that we couldn't convert both the head
        // and working versions of the file to CellML Text format, in which case
        // we diff their raw contents

        if (res.isEmpty()) {
            res = diffHtml(oldFileContents, newFileContents);

            mDiffHtmls.insert(pFileName, res);
        }
    } else {
        // We are dealing with a binary file

        static const QString BinaryFile = R"(    <tr class="binaryfile">)""\n"
                                           "        <td colspan=4>\n"
                                           "            <code>%1</code>\n"
                                           "        </td>\n"
                                           "    </tr>\n";

        res = BinaryFile.arg("["+tr("Binary File")+"]");

        mDiffHtmls.insert(pFileName, res);
    }

    QFile::remove(oldFileName);

    return res;
}

//==============================================================================

QString PmrWorkspacesWindowSynchronizeDialog::cleanHtmlEscaped(const QString &pString)
{
    // Return a "clean" HTML-escaped version of the given string

    return pString.toHtmlEscaped()
                  .replace(' ', "&nbsp;");
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

        for (const auto &index : mPreviouslySelectedIndexes) {
            mChangesValue->selectionModel()->select(index, QItemSelectionModel::Select);
        }
    } else {
        // We have some selected indexes, so keep track of them

        mPreviouslySelectedIndexes = indexes;

        // Go through each selected index and retrieve its differences, if it
        // hasn't already been done, and show them (respecting the order in whic
        // they are listed)

        static const QString Space = R"(    <tr class="space"/>)""\n";
        static const QString FileName = R"(    <tr class="filename">)""\n"
                                         "        <td colspan=4>\n"
                                         "            %1\n"
                                         "        </td>\n"
                                         "    </tr>\n";

        QString html = "<table>\n";
        bool firstFile = true;

        for (int i = 0, iMax = mProxyModel->rowCount(); i < iMax; ++i) {
            QModelIndex index = mProxyModel->index(i, 0);

            if (indexes.contains(index)) {
                // Output the name of the current file

                QString fileName = mModel->itemFromIndex(mProxyModel->mapToSource(index))->text();

                if (!firstFile) {
                    html += Space;
                }

                html += FileName.arg(fileName);

                // Try to retrieve the diff for the CellML Text based version
                // or, failing that, the diff for the raw version and if that
                // still fails, we compute the diff

                QString fileDiffHtml = mWebViewerCellmlTextFormatAction->isChecked()?
                                           mCellmlDiffHtmls.value(fileName):
                                           mDiffHtmls.value(fileName);

                if (   !mWebViewerCellmlTextFormatAction->isChecked()
                    &&  fileDiffHtml.isEmpty()) {
                    fileDiffHtml = mDiffHtmls.value(fileName);
                }

                if (fileDiffHtml.isEmpty()) {
                    fileDiffHtml = diffHtml(fileName);
                }

                html += fileDiffHtml;

                firstFile = false;
            }
        }

        html += "</table>\n";

        mWebViewer->webView()->setHtml(mDiffTemplate.arg(html));
    }
}

//==============================================================================

} // namespace PMRWorkspacesWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
