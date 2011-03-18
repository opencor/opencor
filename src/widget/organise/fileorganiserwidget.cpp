#include "fileorganiserwidget.h"

#include <QPaintEvent>
#include <QStandardItemModel>

FileOrganiserWidget::FileOrganiserWidget(const QString &pName,
                                         QWidget *pParent) :
    QTreeView(pParent),
    CommonWidget(pName, this, pParent)
{
    // Create an instance of the data model that we want to view

    mDataModel = new QStandardItemModel;

    // Set some properties for the file organiser widget itself

#ifdef Q_WS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    // Note: the above removes the focus border since it messes up our toolbar
#endif
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    setHeaderHidden(true);
    setModel(mDataModel);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setUniformRowHeights(true);

    // Some connections

    connect(this, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(resizeToContents()));
    connect(this, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(resizeToContents()));
}

QSize FileOrganiserWidget::sizeHint() const
{
    // Suggest a default size for the file organiser widget
    // Note: this is critical if we want a docked widget, with a file organiser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

void FileOrganiserWidget::mousePressEvent(QMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::RightButton)
        // We are pressing the right mouse button which may be used to display a
        // context menu and we don't want the row beneath the mouse to be
        // selected (in case it isn't already), so...

        pEvent->accept();
    else
        // We are not pressing the right mouse button, so carry on with the
        // default handling of the event

        QTreeView::mousePressEvent(pEvent);
}

QString FileOrganiserWidget::newFolderName(QStandardItem *pFolderItem)
{
    // Come up with the name for a new folder which is to be under pFolderItem

    static const QString baseFolderName = "New wer wer wer Folder";

    if (!pFolderItem->rowCount()) {
        // pFolderItem doesn't have any sub-folder items, so we can use our base
        // folder name

        return baseFolderName;
    } else {
        // pFolderItem has one or more sub-folder items, so we need to come up
        // with a name for the new folder that is not already in use which means
        // retrieving the name of all the sub-folder items

        QStringList subFolderNames;

        for (int i = 0; i < pFolderItem->rowCount(); ++i)
            subFolderNames.append(pFolderItem->child(i)->text());

        static const QString templateFolderName = baseFolderName+" (%1)";
        int folderNb = 1;
        QString folderName;

        do {
            folderName = templateFolderName.arg(++folderNb);
        } while(subFolderNames.contains(folderName));

        return folderName;
    }
}

bool FileOrganiserWidget::newFolder(const QModelIndex &pItemIndex)
{
    // Create a folder item under the passed item, current folder item or root
    // item, depending on the situation

    QModelIndexList itemsList = (pItemIndex != QModelIndex())?
                                    QModelIndexList() << pItemIndex:
                                    selectionModel()->selectedIndexes();
    int nbOfSelectedItems = itemsList.count();

    if (nbOfSelectedItems <= 1) {
        // Either no or one folder item is currently selected, so create the new
        // folder item under the root item or the existing folder item, resp.

        QStandardItem *crtFolderItem = !nbOfSelectedItems?
                                           mDataModel->invisibleRootItem():
                                           mDataModel->itemFromIndex(itemsList.at(0));
        QStandardItem *newFolderItem = new QStandardItem(QIcon(":folder"),
                                                         newFolderName(crtFolderItem));

        crtFolderItem->appendRow(newFolderItem);

        // Some post-processing, but only if no other item is currently being
        // edited

        if (state() != QAbstractItemView::EditingState) {
            // Expand the current index (so that we can see the new folder)
            // Note: this is only relevant in the case of a folder item being
            //       currently selected

            if (nbOfSelectedItems == 1)
                setExpanded(crtFolderItem->index(), true);

            // Offer the user to edit the newly added folder item

            edit(newFolderItem->index());
        }

        return true;
    } else {
        // Several folder items are selected, so...
        // Note: we should never come here (i.e. the caller to this function
        //       should ensure that a folder can be created before calling this
        //       function), but it's better to be safe than sorry

        return false;
    }
}

bool FileOrganiserWidget::deleteItems(const QModelIndex &pItemIndex)
{
    // Remove all the selected items

    QModelIndexList itemsList = (pItemIndex != QModelIndex())?
                                    QModelIndexList() << pItemIndex:
                                    selectionModel()->selectedIndexes();

    if (itemsList.isEmpty()) {
        // Nothing to delete, so...
        // Note: we should never come here (i.e. the caller to this function
        //       should ensure that a folder can be created before calling this
        //       function), but it's better to be safe than sorry

        return false;
    } else {
        // There are some items to delete, so delete them one by one, making
        // sure we update our list of items (this is because the row value of
        // the remaining selected items may become different after deleting an
        // item)

        while(!itemsList.isEmpty()) {
            mDataModel->removeRow(itemsList.first().row(),
                                  itemsList.first().parent());

            itemsList = (pItemIndex != QModelIndex())?
                            QModelIndexList():
                            selectionModel()->selectedIndexes();
        }

        // Resize the widget to its contents in case its width was too wide (and
        // therefore required a horizontal scrollbar), but is now fine

        resizeToContents();

        return true;
    }
}

void FileOrganiserWidget::resizeToContents()
{
    // Make sure that the first column allows for all of its contents to be
    // visible

    resizeColumnToContents(0);
}
