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

PmrWindowItem::PmrWindowItem(const Type &pType, const QString &pText, const QString &pUrl) :
    QStandardItem(pText),
    mType(pType),
    mUrl(pUrl)
{
    // Set the icon for the item

    setIcon(pType);
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

void PmrWindowItem::setIcon(const Type &pType)
{
    // Determine the icon to be used for the item

    static const QIcon ExposureIcon     = QIcon(":/oxygen/places/folder.png");
    static const QIcon ExposureFileIcon = QIcon(":/oxygen/mimetypes/application-x-zerosize.png");

    if (pType == Exposure)
        QStandardItem::setIcon(ExposureIcon);
    else
        QStandardItem::setIcon(ExposureFileIcon);
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
    // Create and customise our message label

    mMessageLabel = new QLabel(this);

    mMessageLabel->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    mMessageLabel->setStyleSheet("QLabel {"
                                 "     background-color: white;"
                                 "     padding: 8px 8px 0px 8px;"
                                 "}");
    mMessageLabel->setWordWrap(true);

    // Create an instance of the data model that we want to view

    mTreeViewModel = new QStandardItemModel(this);

    // Create and customise our tree view

    mTreeViewWidget = new Core::TreeViewWidget(this);

    mTreeViewWidget->setStyleSheet( mTreeViewWidget->styleSheet()
                                   +"QTreeView {"
                                    "     padding-left: 8px;"
                                    "}");

    mTreeViewWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    mTreeViewWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTreeViewWidget->setHeaderHidden(true);
    mTreeViewWidget->setModel(mTreeViewModel);
    mTreeViewWidget->setRootIsDecorated(false);

    connect(mTreeViewWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    // Populate ourselves

    createLayout();

    layout()->addWidget(mMessageLabel);
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
    // Determine the message to be displayed, if any

    QString message = QString();

    if (mErrorMessage.isEmpty()) {
        if (!mNumberOfFilteredExposures) {
            if (!mExposureNames.isEmpty())
                message = tr("No exposures match your criteria.");
        } else if (mNumberOfFilteredExposures == 1) {
            message = tr("<strong>1</strong> exposure was found:");
        } else {
            message = tr("<strong>%1</strong> exposures were found:").arg(QLocale().toString(mNumberOfFilteredExposures));
        }
    } else {
        message = tr("<strong>Error:</strong> ")+Core::formatMessage(mErrorMessage, true, true);
    }

    // Update our message label

    mMessageLabel->setText(message);
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
    // Add the given exposure files to the exposure and show them

    QStringList sortedExposureFiles = QStringList(pExposureFiles);

    sortedExposureFiles.sort(Qt::CaseInsensitive);

//    static const QRegularExpression FilePathRegEx = QRegularExpression("^.*/");

/*---GRY---
    QWebElement ulElement = page()->mainFrame()->documentElement().findFirst(QString("ul[id=exposureFiles_%1]").arg(mExposureUrlId.value(pUrl)));

    foreach (const QString &exposureFile, sortedExposureFiles) {
        ulElement.appendInside(QString("<li class=\"exposureFile\">"
                                       "    <a href=\"%1\">%2</a>"
                                       "</li>").arg(exposureFile, QString(exposureFile).remove(FilePathRegEx)));
    }
*/

    showExposureFiles(pUrl, true);
}

//==============================================================================

void PmrWindowWidget::showExposureFiles(const QString &pUrl, const bool &pShow)
{
    // Show the exposure files for the given exposure

Q_UNUSED(pUrl);
Q_UNUSED(pShow);
/*---GRY---
    int id = mExposureUrlId.value(pUrl);
    QWebElement documentElement = page()->mainFrame()->documentElement();
    QWebElement imgElement = documentElement.findFirst(QString("img[id=exposureFilesButton_%1]").arg(id));
    QWebElement ulElement = documentElement.findFirst(QString("ul[id=exposureFiles_%1]").arg(id));

    if (pShow) {
        imgElement.removeClass("button");
        imgElement.addClass("downButton");

        ulElement.addClass("visible");
        ulElement.setStyleProperty("display", "table-row");
    } else {
        imgElement.addClass("button");
        imgElement.removeClass("downButton");

        ulElement.removeClass("visible");
        ulElement.setStyleProperty("display", "none");
    }
*/
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
        // item and show it, if it isn't empty

        QMenu menu;

        if (item->type() == PmrWindowItem::Exposure) {
            menu.addAction(mViewInPmrAction);
            menu.addSeparator();
            menu.addAction(mCloneWorkspaceAction);
        }

        if (!menu.isEmpty())
            menu.exec(QCursor::pos());
    }
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
