/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Tree view widget
//==============================================================================

#include "treeviewwidget.h"

//==============================================================================

#include <QDrag>
#include <QFileIconProvider>
#include <QHeaderView>
#include <QModelIndex>
#include <QMouseEvent>
#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

TreeViewWidget::TreeViewWidget(QWidget *pParent) :
    QTreeView(pParent),
    CommonWidget(this)
{
    // Set some properties

    setAllColumnsShowFocus(true);
#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, false);
#endif
    setFrameShape(QFrame::NoFrame);
}

//==============================================================================

void TreeViewWidget::resizeColumnsToContents()
{
    // Resize all our columns to their contents

    for (int i = 0, iMax = header()->count(); i < iMax; ++i)
        resizeColumnToContents(i);
}

//==============================================================================

void TreeViewWidget::selectItem(const int &pRow, const int &pColumn)
{
    // Select the requested item, if any

    QStandardItemModel *treeViewModel = qobject_cast<QStandardItemModel *>(model());

    if (treeViewModel) {
        // The tree view has a model associated with it, so we can retrieve the
        // requested item

        QStandardItem *treeViewItem = treeViewModel->invisibleRootItem()->child(pRow, pColumn);

        if (treeViewItem) {
            // The requested item exists, so select it...

            setCurrentIndex(treeViewItem->index());
        }
    }
}

//==============================================================================

void TreeViewWidget::selectFirstItem()
{
    // Select the first item

    selectItem();
}

//==============================================================================

bool TreeViewWidget::isEditing() const
{
    // Return whether our tree view widget is in editing mode

    return state() == QAbstractItemView::EditingState;
}

//==============================================================================

void TreeViewWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Check some key combinations

    if (   !(pEvent->modifiers() & Qt::ShiftModifier)
        && !(pEvent->modifiers() & Qt::ControlModifier)
        && !(pEvent->modifiers() & Qt::AltModifier)
        && !(pEvent->modifiers() & Qt::MetaModifier)) {
        // None of the modifiers is selected

        if (pEvent->key() == Qt::Key_Left) {
            // The user wants to collapse the current item or go to its parent,
            // depending on the collapsed state of the current item

            QModelIndex crtIndex = currentIndex();

            if (crtIndex.column()) {
                // We are not dealing with the (row, 0) item, so make sure we do

                crtIndex = crtIndex.sibling(crtIndex.row(), 0);
            }

            if (model()->hasChildren(crtIndex) && isExpanded(crtIndex)) {
                // The current item has children and it is expanded, so collapse
                // it

                setExpanded(crtIndex, false);

                // Accept the event

                pEvent->accept();
            } else {
                // Either the current item has no children or it is collapsed,
                // so select its parent, if it has one

                if (crtIndex.parent() != QModelIndex()) {
                    setCurrentIndex(crtIndex.parent());

                    // Accept the event

                    pEvent->accept();
                } else {
                    // Default handling of the event

                    QTreeView::keyPressEvent(pEvent);
                }
            }
        } else if (pEvent->key() == Qt::Key_Right) {
            // The user wants to expand the current item or go to its first
            // child, should it have children

            QModelIndex crtIndex = currentIndex();

            if (crtIndex.column()) {
                // We are not dealing with the (row, 0) item, so make sure we do

                crtIndex = crtIndex.sibling(crtIndex.row(), 0);
            }

            if (model()->hasChildren(crtIndex)) {
                if (!isExpanded(crtIndex)) {
                    // The current item is collapsed, so expand it

                    setExpanded(crtIndex, true);

                    // Accept the event

                    pEvent->accept();
                } else {
                    // The current item is expanded, so select its first child

                    setCurrentIndex(crtIndex.model()->index(0, 0, crtIndex));

                    // Accept the event

                    pEvent->accept();
                }
            } else {
                // The current item doesn't have any children, so do nothing
                // Note: normally, we would do the default handling of the
                //       event, but this will potentially shift the whole tree
                //       view contents to the right, so do nothing indeed...

                ;
            }
        } else {
            // Default handling of the event

            QTreeView::keyPressEvent(pEvent);
        }
    } else {
        // Default handling of the event

        QTreeView::keyPressEvent(pEvent);
    }
}

//==============================================================================

void TreeViewWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QTreeView::mousePressEvent(pEvent);

    // Make sure that we are over a selected item (if any), if we are right
    // clicking
    // Note: the idea is that if we want to show a context menu, then none will
    //       be shown if we are not over at least one selected item...

    if (pEvent->button() == Qt::RightButton) {
        QModelIndex indexAtPosition = indexAt(pEvent->pos());

        if (!selectedIndexes().contains(indexAtPosition))
            setCurrentIndex(indexAtPosition);
    }
}

//==============================================================================

void TreeViewWidget::startDrag(Qt::DropActions pSupportedActions)
{
    // This a reimplementation of QAbstractItemView::startDrag, so that we can
    // provide OpenCOR with a better pixmap for the drag object
    // Note: indeed, on Windows, the pixmap only shows the dragged item that are
    //       visible in the QTreeView. Also, if an item covers several columns,
    //       then the pixmap will show several 'cells', some of them empty if a
    //       column is empty, so instead we want to provide a generic pixmap
    //       that looks 'good' on all platforms...

    // Retrieve the selected draggable items, if any
    // Note: the following code is based on
    //       QAbstractItemViewPrivate::selectedDraggableIndexes...

    QModelIndexList selectedDraggableIndexes = selectedIndexes();

    for (int i = selectedDraggableIndexes.count()-1; i >= 0; --i) {
        if (   !(model()->flags(selectedDraggableIndexes[i]) & Qt::ItemIsDragEnabled)
            || selectedDraggableIndexes[i].column()) {
            // The current selected item is not draggable or is not in the first
            // column
            // Note: regarding the test on the column number, it is because we
            //       may have a model data that requires several columns (e.g.
            //       QFileSystemModel) in which case selectedIndexes will return
            //       a number of indexes equal to the number of rows times the
            //       number of columns while we only want a number of indexes to
            //       be equal to the number of rows (since we have a selection
            //       mode of QAbstractItemView::ExtendedSelection)...

            selectedDraggableIndexes.removeAt(i);
        }
    }

    // Start the dragging action is there is at least one selected draggable
    // item

    if (selectedDraggableIndexes.count()) {
        // There is at least one selected draggable item, so create a QMimeData
        // object for it

        QMimeData *mimeData = model()->mimeData(selectedDraggableIndexes);

        if (!mimeData)
            return;

        // Create the pixmap that will be associated with the dragging action

        QIcon icon = QFileIconProvider().icon(QFileIconProvider::File);
        QPixmap pixmap(icon.pixmap(32, 32));

        // Create the drag object

        QDrag *drag = new QDrag(this);

        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(0.5*pixmap.width(), 0.5*pixmap.height()));

        // Do the dragging itself

        Qt::DropAction realDefaultDropAction = Qt::IgnoreAction;

        if (   (defaultDropAction() != Qt::IgnoreAction)
            && (pSupportedActions & defaultDropAction())) {
            realDefaultDropAction = defaultDropAction();
        } else if (   (pSupportedActions & Qt::CopyAction)
                   && (dragDropMode() != QAbstractItemView::InternalMove)) {
            realDefaultDropAction = Qt::CopyAction;
        }

        if (drag->exec(pSupportedActions, realDefaultDropAction) == Qt::MoveAction) {
            // We want to move the items
            // Note: the following code is based on
            //       QAbstractItemViewPrivate::clearOrRemove...

            const QItemSelection selection = selectionModel()->selection();

            if (!dragDropOverwriteMode()) {
                foreach (const QItemSelectionRange &itemSelectionRange, selection) {
                    QModelIndex parent = itemSelectionRange.parent();

                    if (itemSelectionRange.left())
                        continue;

                    if (itemSelectionRange.right() != (model()->columnCount(parent)-1))
                        continue;

                    model()->removeRows(itemSelectionRange.top(),
                                        itemSelectionRange.bottom()-itemSelectionRange.top()+1,
                                        parent);
                }
            } else {
                // We can't remove the rows so reset the items (i.e. the view
                // is like a table)

                foreach (const QModelIndex &index, selection.indexes()) {
                    QMap<int, QVariant> roles = model()->itemData(index);

                    for (auto role = roles.begin(), roleEnd = roles.end();
                         role != roleEnd; ++role) {
                        role.value() = QVariant();
                    }

                    model()->setItemData(index, roles);
                }
            }
        }
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
