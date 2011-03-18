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
}

QSize FileOrganiserWidget::sizeHint() const
{
    // Suggest a default size for the file organiser widget
    // Note: this is critical if we want a docked widget, with a file organiser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

QString FileOrganiserWidget::newFolderName(QStandardItem *pFolderItem)
{
    // Come up with the name for a new folder which is to be under pFolderItem

    static const QString baseFolderName = "New Folder";

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

bool FileOrganiserWidget::newFolder()
{
    // Create a folder item under the current one or root item, if no folder
    // item is currently selected

    QModelIndexList itemsList = selectionModel()->selectedIndexes();
    int nbOfSelectedItems = itemsList.count();

    if (nbOfSelectedItems <= 1) {
        // Either no or one folder item is currently selected, so create the new
        // folder item under the root item or the existing folder item,
        // respecitvely

        QStandardItem *crtFolderItem = !nbOfSelectedItems?
                                           mDataModel->invisibleRootItem():
                                           mDataModel->itemFromIndex(itemsList.at(0));
        QStandardItem *newFolderItem = new QStandardItem(QIcon(":folder"), newFolderName(crtFolderItem));

        crtFolderItem->appendRow(newFolderItem);

        return true;
    } else {
        // Several folder items are selected, so...
        // Note: we should never come here (i.e. the caller to this function
        //       should ensure that a folder can be created before calling this
        //       function), but it's better to be safe than sorry

        return false;
    }
}

bool FileOrganiserWidget::deleteItems()
{
    // Remove all the selected items

    QModelIndexList itemsList = selectionModel()->selectedIndexes();

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

            itemsList = selectionModel()->selectedIndexes();
        }

        return true;
    }
}
