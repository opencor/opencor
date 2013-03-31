//==============================================================================
// Enhanced tree view widget
//==============================================================================

#include "treeviewwidget.h"

//==============================================================================

#include <QDrag>
#include <QHeaderView>
#include <QMouseEvent>
#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

TreeViewWidget::TreeViewWidget(QWidget *pParent) :
    QTreeView(pParent),
    CommonWidget(pParent)
{
    // Set some properties for our tree view widget

    setAllColumnsShowFocus(true);
#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up the look of
    //       our tree view widget...
#endif
    setFrameShape(QFrame::NoFrame);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setUniformRowHeights(true);
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

        if (treeViewItem)
            // The requested item exists, so select it...

            setCurrentIndex(treeViewItem->index());
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

QSize TreeViewWidget::sizeHint() const
{
    // Suggest a default size for our tree view widget
    // Note: this is critical if we want a docked widget, with a tree view
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

//==============================================================================

void TreeViewWidget::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    // Retrieve the index of the item which row is the same as our current item,
    // but which column is 0, so that we can expand/collapse properly
    // Note: to retrieve the current index, we would normally use
    //       currentIndex(), but we may be double clicking on a disabled item,
    //       which means that it will never become the current index, hence we
    //       use indexAt() instead...

    QModelIndex currIndex = indexAt(pEvent->pos());
    QModelIndex index = model()->index(currIndex.row(), 0, currIndex.parent());

    if (model()->hasChildren(index))
        // The item has some children, so expand/collapse it

        setExpanded(index, !isExpanded(index));
    else
        // Default handling of the event

        QTreeView::mouseDoubleClickEvent(pEvent);
}

//==============================================================================

void TreeViewWidget::startDrag(Qt::DropActions pSupportedActions)
{
    // This a reimplementation of QAbstractItemView::startDrag, so that we can
    // provide OpenCOR with a better pixmap for the drag object
    // Note: indeed, on Windows, the pixmap only shows the dragged item that are
    //       visible in the QTreeView. Also, if there an item covers several
    //       columns, then the pixmap will show several 'cells' some of them
    //       empty if a column is empty, so... instead we want to provide a
    //       generic pixmap which looks 'good' on all platforms...

    // Retrieve the selected draggable items, if any
    // Note: the following code is based on
    //       QAbstractItemViewPrivate::selectedDraggableIndexes...

    QModelIndexList selectedDraggableIndexes = selectedIndexes();

    for (int i = selectedDraggableIndexes.count()-1; i >= 0; --i)
        if (   !(model()->flags(selectedDraggableIndexes[i]) & Qt::ItemIsDragEnabled)
            || selectedDraggableIndexes[i].column())
            // The current selected item is not draggable or is not in the first
            // column
            // Note: regarding the test on the column number, it is because we
            //       may have a model data that requires several columns (e.g.
            //       QFileSystemModel) in which case selectedIndexes would
            //       return a number of indexes equal to the number of rows
            //       times the number of columns while we only want a number of
            //       indexes to be equal to the number of rows (since we have a
            //       selection mode of QAbstractItemView::ExtendedSelection)

            selectedDraggableIndexes.removeAt(i);

    // Start the dragging action is there is at least one selected draggable
    // item

    if (selectedDraggableIndexes.count()) {
        // There is at least one selected draggable item, so create a QMimeData
        // object for it

        QMimeData *mimeData = model()->mimeData(selectedDraggableIndexes);

        if (!mimeData)
            return;

        // Create the pixmap that will be associated with the dragging action

        QPixmap pixmap((selectedDraggableIndexes.count() == 1)?
                           ":oxygen/mimetypes/application-x-zerosize.png":
                           ":oxygen/places/document-multiple.png");

        // Create the drag object

        QDrag *drag = new QDrag(this);

        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(0.5*pixmap.width(), 0.5*pixmap.height()));

        // Do the dragging itself

        Qt::DropAction realDefaultDropAction = Qt::IgnoreAction;

        if (   (defaultDropAction() != Qt::IgnoreAction)
            && (pSupportedActions & defaultDropAction()))
            realDefaultDropAction = defaultDropAction();
        else if (   (pSupportedActions & Qt::CopyAction)
                 && (dragDropMode() != QAbstractItemView::InternalMove))
            realDefaultDropAction = Qt::CopyAction;

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

                    for (QMap<int, QVariant>::Iterator iter = roles.begin(),
                                                       iterEnd = roles.end();
                         iter != iterEnd; ++iter)
                        iter.value() = QVariant();

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
