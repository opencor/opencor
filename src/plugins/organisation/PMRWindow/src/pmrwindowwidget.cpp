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
// PMR window widget
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "i18ninterface.h"
#include "pmrwindowwidget.h"
#include "treeviewwidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QFileIconProvider>
#include <QKeyEvent>
#include <QLayout>
#include <QMenu>

//==============================================================================

namespace OpenCOR {
namespace PMRWindow {

//==============================================================================

PmrWindowItem::PmrWindowItem(Type pType, const QString &pText,
                             const QString &pUrl) :
    QStandardItem(pText),
    mType(pType),
    mUrl(pUrl)
{
    // Customise ourselves

    QStandardItem::setIcon((pType == Type::Exposure)?
                               QFileIconProvider().icon(QFileIconProvider::Folder):
                               QFileIconProvider().icon(QFileIconProvider::File));

    setToolTip(pText);
}

//==============================================================================

int PmrWindowItem::type() const
{
    // Return our type

    return int(mType);
}

//==============================================================================

QString PmrWindowItem::url() const
{
    // Return our URL

    return mUrl;
}

//==============================================================================

PmrWindowWidget::PmrWindowWidget(QWidget *pParent) :
    Core::TreeViewWidget(pParent)
{
    // Customise ourselves

    mTreeViewModel = new QStandardItemModel(this);

    setContextMenuPolicy(Qt::CustomContextMenu);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHeaderHidden(true);
    setModel(mTreeViewModel);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(this, &PmrWindowWidget::customContextMenuRequested,
            this, &PmrWindowWidget::showCustomContextMenu);

    connect(this, &PmrWindowWidget::doubleClicked,
            this, QOverload<const QModelIndex &>::of(&PmrWindowWidget::itemDoubleClicked));
    connect(this, &PmrWindowWidget::doubleClicked,
            this, QOverload<>::of(&PmrWindowWidget::itemDoubleClicked));

    connect(this, &PmrWindowWidget::expanded,
            this, &PmrWindowWidget::resizeTreeViewToContents);
    connect(this, &PmrWindowWidget::collapsed,
            this, &PmrWindowWidget::resizeTreeViewToContents);

    // Create and set ourselves a layout

    auto layout = new QVBoxLayout(this);

    layout->setContentsMargins(QMargins());

    setLayout(layout);

    // Create and customise our user menssage

    mUserMessageWidget = new Core::UserMessageWidget(this);

    mUserMessageWidget->setScale(0.85);

    layout->addWidget(mUserMessageWidget);

    // Create and populate our context menu

    static const QIcon ArrowDownIcon = QIcon(":/oxygen/actions/arrow-down.png");

    QIcon folderIcon = Core::standardIcon(QStyle::SP_DirClosedIcon);
    int folderIconSize = folderIcon.availableSizes().first().width();
    int overlayIconSize = int(0.57*folderIconSize);

    mContextMenu = new QMenu(this);

    mViewInPmrAction = Core::newAction(QIcon(":/oxygen/categories/applications-internet.png"),
                                       this);
    mCopyUrlAction = Core::newAction(QIcon(":/oxygen/actions/edit-copy.png"),
                                     this);
    mMakeLocalCopyAction = Core::newAction(Core::overlayedIcon(folderIcon, ArrowDownIcon,
                                                               folderIconSize, folderIconSize,
                                                               folderIconSize-overlayIconSize, folderIconSize-overlayIconSize,
                                                               overlayIconSize, overlayIconSize),
                                           this);

    connect(mViewInPmrAction, &QAction::triggered,
            this, &PmrWindowWidget::viewInPmr);
    connect(mCopyUrlAction, &QAction::triggered,
            this, &PmrWindowWidget::copyUrl);
    connect(mMakeLocalCopyAction, &QAction::triggered,
            this, &PmrWindowWidget::makeLocalCopy);

    mContextMenu->addAction(mViewInPmrAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyUrlAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mMakeLocalCopyAction);
}

//==============================================================================

void PmrWindowWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mCopyUrlAction, tr("Copy URL"),
                                     tr("Copy the current workspace URL to the clipboard"));
    I18nInterface::retranslateAction(mMakeLocalCopyAction, tr("Make Local Copy..."),
                                     tr("Make a local copy of the current workspace"));

    // Retranslate the rest of our GUI by updating it, if we have been
    // initialised

    if (mInitialized) {
        updateGui();
    }
}

//==============================================================================

void PmrWindowWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    Core::TreeViewWidget::keyPressEvent(pEvent);

    // Retrieve all the exposure files that are currently selected
    // Note: if there is an exposure among the selected items, then ignore
    //       everything...

    QStringList exposureFileUrls = QStringList();
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    for (int i = 0, iMax = selectedIndexes.count(); i < iMax; ++i) {
        auto item = static_cast<PmrWindowItem *>(mTreeViewModel->itemFromIndex(selectedIndexes[i]));

        if (item->type() == int(PmrWindowItem::Type::Exposure)) {
            exposureFileUrls = QStringList();

            break;
        }

        exposureFileUrls << item->url();
    }

    // Let people know about a key having been pressed with the view of opening
    // one or several exposure files

    if (   !exposureFileUrls.isEmpty()
        &&  ((pEvent->key() == Qt::Key_Enter) || (pEvent->key() == Qt::Key_Return))) {
        // There are some exposure files that are selected and we want to open
        // them, so let people know about it

        emit openExposureFilesRequested(exposureFileUrls);
    }
}

//==============================================================================

void PmrWindowWidget::updateGui(bool pForceUserMessageVisibility)
{
    // Update the message to be displayed, if any

    if (mErrorMessage.isEmpty()) {
        if ((mNumberOfFilteredExposures == 0) && !mExposureNames.isEmpty()) {
            mUserMessageWidget->setIconMessage(":/oxygen/actions/help-about.png",
                                               tr("No exposures match your criteria..."));
        } else {
            mUserMessageWidget->resetMessage();
        }
    } else {
        mUserMessageWidget->setIconMessage(":/oxygen/emblems/emblem-important.png",
                                           Core::formatMessage(mErrorMessage, false, true));
    }

    // Show/hide our user message widget

    mUserMessageWidget->setVisible(    pForceUserMessageVisibility
                                   || !mUserMessageWidget->text().isEmpty());
}

//==============================================================================

void PmrWindowWidget::initialize(const PMRSupport::PmrExposures &pExposures,
                                 const QString &pFilter,
                                 const QString &pErrorMessage)
{
    // Initialise / keep track of some properties

    mTreeViewModel->clear();
    mExposureNames.clear();

    mErrorMessage = pErrorMessage;

    // Initialise our list of exposures

    QRegularExpression filterRegEx = QRegularExpression(pFilter, QRegularExpression::CaseInsensitiveOption);

    mNumberOfFilteredExposures = 0;

    for (int i = 0, iMax = pExposures.count(); i < iMax; ++i) {
        QString exposureName = pExposures[i]->name();
        bool exposureDisplayed = exposureName.contains(filterRegEx);
        auto item = new PmrWindowItem(PmrWindowItem::Type::Exposure,
                                      exposureName, pExposures[i]->url());

        mTreeViewModel->invisibleRootItem()->appendRow(item);

        setRowHidden(item->row(), mTreeViewModel->invisibleRootItem()->index(),
                     !exposureDisplayed);

        mExposureNames << exposureName;

        mNumberOfFilteredExposures += int(exposureDisplayed);
    }

    resizeTreeViewToContents();

    updateGui(pExposures.isEmpty() && pFilter.isEmpty() && pErrorMessage.isEmpty());

    mInitialized = true;
}

//==============================================================================

void PmrWindowWidget::filter(const QString &pFilter)
{
    // Filter our list of exposures and remove any duplicates (they will be
    // 'reintroduced' in the next step)

    QStringList filteredExposureNames = mExposureNames.filter(QRegularExpression(pFilter, QRegularExpression::CaseInsensitiveOption));

    mNumberOfFilteredExposures = filteredExposureNames.count();

    filteredExposureNames.removeDuplicates();

    // Update our GUI and show/hide the relevant exposures

    updateGui();

    for (int i = 0, iMax = mTreeViewModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        QStandardItem *item = mTreeViewModel->invisibleRootItem()->child(i);

        setRowHidden(item->row(), mTreeViewModel->invisibleRootItem()->index(),
                     !filteredExposureNames.contains(item->text()));
    }

    resizeTreeViewToContents();
}

//==============================================================================

bool PmrWindowWidget::hasExposures() const
{
    // Return whether we have some exposures

    return !mExposureNames.isEmpty();
}

//==============================================================================

void PmrWindowWidget::addAndShowExposureFiles(const QString &pUrl,
                                              const QStringList &pExposureFiles)
{
    // Make sure that the exposure files are sorted

    QStringList sortedExposureFiles = QStringList(pExposureFiles);

    sortedExposureFiles.sort(Qt::CaseInsensitive);

    // Retrieve the item which URL is given

    PmrWindowItem *item = nullptr;

    for (int i = 0, iMax = mTreeViewModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        item = static_cast<PmrWindowItem *>(mTreeViewModel->invisibleRootItem()->child(i));

        if (pUrl == item->url()) {
            break;
        }
    }

    // Add the given exposure files to the exposure, if it could actually be
    // found, and show them

    if (item != nullptr) {
        static const QRegularExpression FilePathRegEx = QRegularExpression("^.*/");

        for (const auto &exposureFile : sortedExposureFiles) {
            item->appendRow(new PmrWindowItem(PmrWindowItem::Type::ExposureFile,
                                              QString(exposureFile).remove(FilePathRegEx),
                                              exposureFile));
        }

        if (mDontExpandExposures.contains(pUrl)) {
            mDontExpandExposures.removeOne(pUrl);
        } else {
            expand(item->index());
        }

        resizeTreeViewToContents();
    }
}

//==============================================================================

void PmrWindowWidget::resizeTreeViewToContents()
{
    // Resize our tree view widget so that all of its contents is visible

    resizeColumnToContents(0);
}

//==============================================================================

PmrWindowItem * PmrWindowWidget::currentItem() const
{
    // Return our current item

    return static_cast<PmrWindowItem *>(mTreeViewModel->itemFromIndex(currentIndex()));
}

//==============================================================================

void PmrWindowWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    // Determine whether to show the context menu based on whether we are over
    // an item

    PmrWindowItem *item = static_cast<PmrWindowItem *>(mTreeViewModel->itemFromIndex(indexAt(pPosition)));

    if (item != nullptr) {
        // We are over an item, so update our context menu and show it

        mMakeLocalCopyAction->setVisible(item->type() == int(PmrWindowItem::Type::Exposure));

        bool onlyOneItem = selectionModel()->selectedIndexes().count() == 1;

        I18nInterface::retranslateAction(mViewInPmrAction,
                                         tr("View In PMR"),
                                         onlyOneItem?
                                             tr("View the current workspace in PMR"):
                                             tr("View the current workspaces in PMR"));

        mCopyUrlAction->setEnabled(onlyOneItem);
        mMakeLocalCopyAction->setEnabled(onlyOneItem);

        mContextMenu->exec(QCursor::pos());
    }
}

//==============================================================================

void PmrWindowWidget::itemDoubleClicked(const QModelIndex &pIndex)
{
    // Ask for exposure files to be retrieved (if it hasn't already been done)
    // or an exposure file to be opened

    auto item = static_cast<PmrWindowItem *>(mTreeViewModel->itemFromIndex(pIndex));

    if (item->type() == int(PmrWindowItem::Type::Exposure)) {
        if (!item->hasChildren()) {
            emit exposureFilesRequested(item->url());
        }
    } else {
        emit openExposureFileRequested(item->url());
    }
}

//==============================================================================

void PmrWindowWidget::viewInPmr()
{
    // Show the selected items in PMR

    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    for (int i = 0, iMax = selectedIndexes.count(); i < iMax; ++i) {
        QDesktopServices::openUrl(static_cast<PmrWindowItem *>(mTreeViewModel->itemFromIndex(selectedIndexes[i]))->url());
    }
}

//==============================================================================

void PmrWindowWidget::copyUrl()
{
    // Copy the current item's URL to the clipboard

    QApplication::clipboard()->setText(currentItem()->url());
}

//==============================================================================

void PmrWindowWidget::makeLocalCopy()
{
    // Let people know that we want to make a local copy of the current
    // exposure's workspace

    QString url = currentItem()->url();

    mDontExpandExposures << url;

    emit cloneWorkspaceRequested(url);
}

//==============================================================================

} // namespace PMRWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
