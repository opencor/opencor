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
// PMR window widget
//==============================================================================

#include "coreguiutils.h"
#include "i18ninterface.h"
#include "pmrwindowwidget.h"
#include "treeviewwidget.h"
#include "usermessagewidget.h"

//==============================================================================

#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QDesktopServices>
#include <QIODevice>
#include <QLabel>
#include <QLayout>
#include <QMenu>
#include <QRegularExpression>

//==============================================================================

namespace OpenCOR {
namespace PMRWindow {

//==============================================================================

PmrWindowItem::PmrWindowItem(const Type &pType, const QString &pText,
                             const QString &pUrl) :
    QStandardItem(pText),
    mType(pType),
    mUrl(pUrl)
{
    // Customise ourselves

    static const QIcon ExposureIcon     = QIcon(":/oxygen/places/folder.png");
    static const QIcon ExposureFileIcon = QIcon(":/oxygen/mimetypes/application-x-zerosize.png");

    QStandardItem::setIcon((pType == Exposure)?ExposureIcon:ExposureFileIcon);

    static const QString ToolTip = "<table>\n"
                                   "    <tbody>\n"
                                   "        <tr>\n"
                                   "            <td style=\"font-weight: bold;\">\n"
                                   "                Name:\n"
                                   "            </td>\n"
                                   "            <td></td>\n"
                                   "            <td>\n"
                                   "                %1\n"
                                   "            </td>\n"
                                   "        </tr>\n"
                                   "        <tr>\n"
                                   "            <td style=\"font-weight: bold;\">\n"
                                   "                URL:\n"
                                   "            </td>\n"
                                   "            <td></td>\n"
                                   "            <td>\n"
                                   "                %2\n"
                                   "            </td>\n"
                                   "        </tr>\n"
                                   "    </tbody>\n"
                                   "</table>\n";

    setToolTip(ToolTip.arg(pText, pUrl));
}

//==============================================================================

int PmrWindowItem::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

QString PmrWindowItem::url() const
{
    // Return our URL

    return mUrl;
}

//==============================================================================

PmrWindowWidget::PmrWindowWidget(QWidget *pParent) :
    Core::Widget(pParent),
    mExposureNames(QStringList()),
    mExposureUrlId(QMap<QString, int>()),
    mInitialized(false),
    mErrorMessage(QString()),
    mNumberOfFilteredExposures(0)
{
    // Create and customise some objects

    mUserMessageWidget = new Core::UserMessageWidget(this);

    mUserMessageWidget->setScale(0.85);

    mTreeViewModel = new QStandardItemModel(this);
    mTreeViewWidget = new Core::TreeViewWidget(this);

    mTreeViewWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    mTreeViewWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeViewWidget->setHeaderHidden(true);
    mTreeViewWidget->setModel(mTreeViewModel);

    connect(mTreeViewWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));
    connect(mTreeViewWidget, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(itemDoubleClicked(const QModelIndex &)));

    // Some connections

    connect(mTreeViewWidget, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(expandedExposure(const QModelIndex &)));
    connect(mTreeViewWidget, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(collapsedExposure(const QModelIndex &)));

    // Populate ourselves

    createLayout();

    layout()->addWidget(mUserMessageWidget);
    layout()->addWidget(mTreeViewWidget);

    // Create our actions

    mViewInPmrAction = Core::newAction(this);
    mCloneWorkspaceAction = Core::newAction(this);

    connect(mViewInPmrAction, SIGNAL(triggered(bool)),
            this, SLOT(viewInPmr()));
    connect(mCloneWorkspaceAction, SIGNAL(triggered(bool)),
            this, SLOT(cloneWorkspace()));
}

//==============================================================================

void PmrWindowWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mViewInPmrAction, tr("View In PMR"),
                                     tr("View the exposure in PMR"));
    I18nInterface::retranslateAction(mCloneWorkspaceAction, tr("Clone Workspace"),
                                     tr("Clone the corresponding workspace"));

    // Retranslate our message, if we have been initialised

    if (mInitialized)
        updateMessage();
}

//==============================================================================

QSize PmrWindowWidget::sizeHint() const
{
    // Suggest a default size for our PMR widget
    // Note: this is critical if we want a docked widget, with a PMR widget on
    //       it, to have a decent size when docked to the main window...

    return defaultSize(0.15);
}

//==============================================================================

void PmrWindowWidget::updateMessage()
{
    // Update the message to be displayed, if any

    if (mErrorMessage.isEmpty()) {
        if (!mNumberOfFilteredExposures && !mExposureNames.isEmpty()) {
            mUserMessageWidget->setIconMessage(":/oxygen/actions/help-about.png",
                                               tr("No exposures match your criteria..."));
        } else {
            mUserMessageWidget->resetMessage();
        }
    } else {
        mUserMessageWidget->setIconMessage(":/oxygen/emblems/emblem-important.png",
                                           Core::formatMessage(mErrorMessage, false, true));
    }

    // Show/hide our user message widget and our tree view widget

    mUserMessageWidget->setVisible(!mUserMessageWidget->text().isEmpty());
    mTreeViewWidget->setVisible(mUserMessageWidget->text().isEmpty());
}

//==============================================================================

void PmrWindowWidget::initialize(const PMRSupport::PmrExposures &pExposures,
                                 const QString &pFilter,
                                 const QString &pErrorMessage)
{
    // Initialise / keep track of some properties

    mTreeViewModel->clear();

    mExposureNames.clear();
    mExposureUrlId.clear();

    mErrorMessage = pErrorMessage;

    // Initialise our list of exposures

    QRegularExpression filterRegEx = QRegularExpression(pFilter, QRegularExpression::CaseInsensitiveOption);

    mNumberOfFilteredExposures = 0;

    for (int i = 0, iMax = pExposures.count(); i < iMax; ++i) {
        QString exposureUrl = pExposures[i]->url();
        QString exposureName = pExposures[i]->name();
        bool exposureDisplayed = exposureName.contains(filterRegEx);
        QStandardItem *item = new PmrWindowItem(PmrWindowItem::Exposure, exposureName, exposureUrl);

        mTreeViewModel->invisibleRootItem()->appendRow(item);

        mTreeViewWidget->setRowHidden(item->row(),
                                      mTreeViewModel->invisibleRootItem()->index(),
                                      !exposureDisplayed);

        mExposureNames << exposureName;
        mExposureUrlId.insert(exposureUrl, i);

        mNumberOfFilteredExposures += exposureDisplayed;
    }

    resizeTreeViewToContents();

    updateMessage();

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

    // Update our message and show/hide the relevant exposures

    updateMessage();

    for (int i = 0, iMax = mTreeViewModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        QStandardItem *item = mTreeViewModel->invisibleRootItem()->child(i);

        mTreeViewWidget->setRowHidden(item->row(),
                                      mTreeViewModel->invisibleRootItem()->index(),
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

    PmrWindowItem *item = 0;

    for (int i = 0, iMax = mTreeViewModel->invisibleRootItem()->rowCount(); i < iMax; ++i) {
        item = static_cast<PmrWindowItem *>(mTreeViewModel->invisibleRootItem()->child(i));

        if (!pUrl.compare(item->url()))
            break;
    }

    // Add the given exposure files to the exposure and show them

    static const QRegularExpression FilePathRegEx = QRegularExpression("^.*/");

    foreach (const QString &exposureFile, sortedExposureFiles) {
        item->appendRow(new PmrWindowItem(PmrWindowItem::ExposureFile,
                                          QString(exposureFile).remove(FilePathRegEx),
                                          exposureFile));
    }

    mTreeViewWidget->expand(item->index());

    resizeTreeViewToContents();
}

//==============================================================================

void PmrWindowWidget::resizeTreeViewToContents()
{
    // Resize our tree view widget so that all of its contents is visible

    mTreeViewWidget->resizeColumnToContents(0);
}

//==============================================================================

PmrWindowItem * PmrWindowWidget::currentItem() const
{
    // Return our current item

    return static_cast<PmrWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewWidget->currentIndex()));
}

//==============================================================================

void PmrWindowWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    // Determine whether to show the context menu based on whether we are over
    // an item

    PmrWindowItem *item = static_cast<PmrWindowItem *>(mTreeViewModel->itemFromIndex(mTreeViewWidget->indexAt(pPosition)));

    if (item) {
        // We are over an item, so create a custom context menu for our current
        // item and show it

        QMenu menu;

        menu.addAction(mViewInPmrAction);

        if (item->type() == PmrWindowItem::Exposure) {
            menu.addSeparator();
            menu.addAction(mCloneWorkspaceAction);
        }

        menu.exec(QCursor::pos());
    }
}

//==============================================================================

void PmrWindowWidget::itemDoubleClicked(const QModelIndex &pIndex)
{
    // Expand/collapse an exposure or open an exposure file

    PmrWindowItem *item = static_cast<PmrWindowItem *>(mTreeViewModel->itemFromIndex(pIndex));

    if (item->type() == PmrWindowItem::Exposure) {
        // It's an exposure, so check whether we have already retrieved its
        // exposure files and, if not, let people know that we need to retrieve
        // them

        if (!item->rowCount())
            emit exposureFilesRequested(item->url());
    } else {
        // It's an exposure file, so ask for it to be opened

        emit openExposureFileRequested(item->url());
    }
}

//==============================================================================

void PmrWindowWidget::expandedExposure(const QModelIndex &pExposureIndex)
{
    // The exposure is being expanded, so update its icon to reflect its new
    // state

    static const QIcon ExpandedExposureIcon = QIcon(":/oxygen/actions/document-open-folder.png");

    mTreeViewModel->itemFromIndex(pExposureIndex)->setIcon(ExpandedExposureIcon);

    // Resize our tree view widget, just to be on the safe side

    resizeTreeViewToContents();
}

//==============================================================================

void PmrWindowWidget::collapsedExposure(const QModelIndex &pExposureIndex)
{
    // The exposure is being expanded, so update its icon to reflect its new
    // state

    static const QIcon CollapsedExposureIcon = QIcon(":/oxygen/places/folder.png");

    mTreeViewModel->itemFromIndex(pExposureIndex)->setIcon(CollapsedExposureIcon);

    // Resize our tree view widget, just to be on the safe side

    resizeTreeViewToContents();
}

//==============================================================================

void PmrWindowWidget::viewInPmr()
{
    // Show the current exposure in PMR

    QDesktopServices::openUrl(currentItem()->url());
}

//==============================================================================

void PmrWindowWidget::cloneWorkspace()
{
    // Let people know that we want to clone the current exposure's workspace

    emit cloneWorkspaceRequested(currentItem()->url());
}

//==============================================================================

}   // namespace PMRWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
