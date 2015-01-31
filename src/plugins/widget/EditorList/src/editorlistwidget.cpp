/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Editor list widget
//==============================================================================

#include "corecliutils.h"
#include "editorlistwidget.h"
#include "i18ninterface.h"

//==============================================================================

#include <QApplication>
#include <QKeyEvent>
#include <QMenu>
#include <QClipboard>

//==============================================================================

namespace OpenCOR {
namespace EditorList {

//==============================================================================

EditorListWidget::EditorListWidget(QWidget *pParent) :
    QListView(pParent),
    CommonWidget(pParent),
    mModel(new QStandardItemModel(this))
{
    // Customise ourselves

#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above removes the focus border since it messes up our look
#endif
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFrameShape(QFrame::NoFrame);
    setModel(mModel);

    // Create our context menu

    mContextMenu = new QMenu(this);

    mClearAction = new QAction(this);
    mCopyToClipboardAction = new QAction(this);

    connect(mClearAction, SIGNAL(triggered(bool)),
            this, SLOT(clear()));
    connect(mCopyToClipboardAction, SIGNAL(triggered(bool)),
            this, SLOT(copyToClipboard()));

    mContextMenu->addAction(mClearAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyToClipboardAction);

    // We want a context menu

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    // Make sure that we are properly initialised

    clear();

    // A connection to handle a double click on a given item

    connect(this, SIGNAL(doubleClicked(const QModelIndex &)),
            this, SLOT(requestItem(const QModelIndex &)));

    // Retranslate ourselves, so that our actions are properly initialised

    retranslateUi();
}

//==============================================================================

void EditorListWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mClearAction, tr("Clear List"),
                                     tr("Clear the list"));
    I18nInterface::retranslateAction(mCopyToClipboardAction, tr("Copy List To Clipboard"),
                                     tr("Copy the list to the clipboard"));
}

//==============================================================================

void EditorListWidget::clear()
{
    // Reset our list of items

    mModel->clear();

    mClearAction->setEnabled(false);
    mCopyToClipboardAction->setEnabled(false);
}

//==============================================================================

void EditorListWidget::addItem(const EditorListItem::Type &pType,
                               const int &pLine, const int &pColumn,
                               const QString &pMessage)
{
    // Add the given item to our list

    mModel->invisibleRootItem()->appendRow(new EditorListItem(pType, pLine, pColumn, pMessage));

    mClearAction->setEnabled(true);
    mCopyToClipboardAction->setEnabled(true);
}

//==============================================================================

void EditorListWidget::addItem(const EditorListItem::Type &pType,
                               const int &pLine, const QString &pMessage)
{
    // Add the given item to our list

    addItem(pType, pLine, -1, pMessage);
}

//==============================================================================

void EditorListWidget::addItem(const EditorListItem::Type &pType,
                               const QString &pMessage)
{
    // Add the given item to our list

    addItem(pType, -1, -1, pMessage);
}

//==============================================================================

int EditorListWidget::count() const
{
    // Return the number of items we hold

    return mModel->invisibleRootItem()->rowCount();
}

//==============================================================================

void EditorListWidget::selectFirstItem()
{
    // Select the first item in our list and 'request' it

    QStandardItem *firstItem = mModel->invisibleRootItem()->child(0);

    if (firstItem) {
        QModelIndex firstItemIndex = firstItem->index();

        setCurrentIndex(firstItemIndex);

        requestItem(firstItemIndex);
    }
}

//==============================================================================

void EditorListWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    QListView::keyPressEvent(pEvent);

    // Check whether the user wants the current item to be requested

    if ((pEvent->key() == Qt::Key_Enter) || (pEvent->key() == Qt::Key_Return))
        requestItem(currentIndex());
}

//==============================================================================

void EditorListWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Show our custom context menu

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

void EditorListWidget::copyToClipboard()
{
    // Copy our list to the clipboard

    QStringList list = QStringList();

    for (int i = 0, iMax = mModel->rowCount(); i < iMax; ++i) {
        EditorListItem *item = static_cast<EditorListItem *>(mModel->item(i));
        QString itemType;

        switch (item->type()) {
        case EditorListItem::Error:
            itemType = tr("Error");

            break;
        case EditorListItem::Warning:
            itemType = tr("Warning");

            break;
        default:
            itemType = tr("Hint");
        }

        list << "["+itemType+"] "+item->text();
    }

    QApplication::clipboard()->setText(list.join(Core::eolString())+Core::eolString());
}

//==============================================================================

void EditorListWidget::requestItem(const QModelIndex &pItemIndex)
{
    // Check what kind of item has been double clicked and if it is a file, then
    // open it

    EditorListItem *item = static_cast<EditorListItem *>(mModel->itemFromIndex(pItemIndex));

    if (item && (item->line() != -1))
        emit itemRequested(item);
}

//==============================================================================

}   // namespace EditorList
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
