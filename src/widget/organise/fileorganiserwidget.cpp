#include "fileorganiserwidget.h"

#include <QDir>
#include <QFileInfo>
#include <QHelpEvent>
#include <QSettings>
#include <QUrl>

enum FileOrganiserItemRole {
    FileOrganiserItemFolder = Qt::UserRole,
    FileOrganiserItemPath   = Qt::UserRole+1
};

static const QString CollapsedFolderIcon = ":oxygen/places/folder.png";
static const QString ExpandedFolderIcon  = ":oxygen/actions/document-open-folder.png";
static const QString FileIcon            = ":oxygen/mimetypes/application-x-zerosize.png";

QStringList FileOrganiserModel::mimeTypes() const
{
    // Return the mime types supported by our model

    return QStringList() << FileSystemMimeType << FileOrganiserMimeType;
}

void FileOrganiserModel::encodeHierarchyData(const QModelIndex &pIndex,
                                             QDataStream &pStream,
                                             const int &pLevel) const
{
    // Encode the item's hierarchy

    if (pIndex != QModelIndex()) {
        // The current index is valid, try to encode its parent's hierarchy

        encodeHierarchyData(pIndex.parent(), pStream, pLevel+1);

        // Now, we can encode the current index's hierarchy information

        pStream << pIndex.row();
    } else {
        // We are the top of the hierarchy, so encode the number of levels
        // (essential if we want to be able to decode the hierarchy)

        pStream << pLevel;
    }
}

QByteArray FileOrganiserModel::encodeData(const QModelIndexList &pIndexes) const
{
    // Encode the mime data

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    // The number of items

    stream << pIndexes.count();

    // Hierarchy to reach the various items

    for (QModelIndexList::ConstIterator iter = pIndexes.begin();
         iter != pIndexes.end(); ++iter)
        // Hierarchy to reach the current item

        encodeHierarchyData(*iter, stream);

    // We are all done, so...

    return data;
}

QModelIndexList FileOrganiserModel::decodeData(QByteArray &pData) const
{
    // Decode the mime data

    QModelIndexList decodedData;
    QDataStream stream(&pData, QIODevice::ReadOnly);

    // The number of items

    int nbOfItems;

    stream >> nbOfItems;

    // Hierarchy to reach the various items

    int nbOfLevels;
    int row;
    QStandardItem *crtItem;

    for (int i = 0; i < nbOfItems; ++i) {
        // Hierarchy to reach the current item

        stream >> nbOfLevels;

        crtItem = invisibleRootItem();

        for (int j = 0; j < nbOfLevels; ++j) {
            stream >> row;

            crtItem = crtItem->child(row, 0);
        }

        // Add the current item to our list

        decodedData.append(indexFromItem(crtItem));
    }

    // We are all done, so...

    return decodedData;
}

QMimeData * FileOrganiserModel::mimeData(const QModelIndexList &pIndexes) const
{
    QMimeData *mimeData = new QMimeData();
    QList<QUrl> urls;

    // Retrieve the URL of the different file (not folder) items
    // Note: this list of URLs is useful with regards to the FileSystemMimeType
    //       mime type on which external widgets (e.g. the central widget) rely
    //       on to extract the name of the vavarious files the mime data
    //       contains

    for (QList<QModelIndex>::const_iterator iter = pIndexes.begin();
         iter != pIndexes.end(); ++iter) {
        QString crtFilePath = filePath(*iter);

        if (!crtFilePath.isEmpty())
            urls << QUrl::fromLocalFile(crtFilePath);
    }

    mimeData->setUrls(urls);

    // Set the data which contains information on both the folder and file items
    // Note: this data is useful with regards to the FileOrganiserMimeType mime
    //       type on which the file organiser widget relies for moving folder
    //       and file items around

    mimeData->setData(FileOrganiserMimeType, encodeData(pIndexes));

    // All done, so...

    return mimeData;
}

QString FileOrganiserModel::filePath(const QModelIndex &pFileIndex) const
{
    // Return the file path of pFileIndex, if it exists and corresponds to a
    // file

    QStandardItem *fileItem = itemFromIndex(pFileIndex);

    if (fileItem && !fileItem->data(FileOrganiserItemFolder).toBool())
        return fileItem->data(FileOrganiserItemPath).toString();
    else
        return QString();
}

FileOrganiserWidget::FileOrganiserWidget(const QString &pName,
                                         QWidget *pParent) :
    TreeView(pName, this, pParent)
{
    // Create an instance of the data model that we want to view

    mDataModel = new FileOrganiserModel;

    // Set some properties for the file organiser widget itself

    setDragDropMode(QAbstractItemView::DragDrop);
    setEditTriggers(QAbstractItemView::EditKeyPressed);
    setHeaderHidden(true);
    setModel(mDataModel);

    // Some connections

    connect(this, SIGNAL(expanded(const QModelIndex &)),
            this, SLOT(expandedFolder(const QModelIndex &)));
    connect(this, SIGNAL(collapsed(const QModelIndex &)),
            this, SLOT(collapsedFolder(const QModelIndex &)));
}

static const QString SettingsDataModel = "DataModel";

void FileOrganiserWidget::loadItemSettings(QSettings &pSettings,
                                           QStandardItem *pParentItem)
{
    // Recursively retrieve the item settings

    static int crtItemIndex = -1;
    QStringList itemInfo;

    itemInfo = pSettings.value(QString::number(++crtItemIndex)).toStringList();

    if (itemInfo != QStringList()) {
        QString textOrPath  = itemInfo.at(0);
        int parentItemIndex = itemInfo.at(1).toInt();
        int nbOfChildItems  = itemInfo.at(2).toInt();
        bool expanded       = itemInfo.at(3).toInt();

        // Create the item, in case we are not dealing with the root folder item

        QStandardItem *childParentItem;

        if (parentItemIndex == -1) {
            // We are dealing with the root folder item, so don't do anything
            // except for keeping track of it for when retrieving its child
            // items, if any

            childParentItem = mDataModel->invisibleRootItem();
        } else {
            // This is not the root folder item, so we can create the item which
            // is either a folder or a file, depending on its number of child
            // items

            if (nbOfChildItems >= 0) {
                // We are dealing with a folder item

                QStandardItem *folderItem = new QStandardItem(QIcon(CollapsedFolderIcon),
                                                              textOrPath);

                folderItem->setData(true, FileOrganiserItemFolder);

                pParentItem->appendRow(folderItem);

                // Expand the folder item, if necessary

                if (expanded)
                    setExpanded(folderItem->index(), true);

                // The folder item is to be the parent of any of its child item

                childParentItem = folderItem;
            } else {
                // We are dealing with a file item

                QStandardItem *fileItem = new QStandardItem(QIcon(FileIcon),
                                                            QFileInfo(textOrPath).fileName());

                fileItem->setData(textOrPath, FileOrganiserItemPath);

                pParentItem->appendRow(fileItem);

                // A file cannot have child items, so...

                childParentItem = 0;
            }
        }

        // Retrieve any child item
        // Note: the test on childParentItem is not necessary (since
        //       nbOfChildItems will be equal to -1 in the case of a file item),
        //       but it doesn't harm having it, so...

        if (childParentItem)
            for (int i = 0; i < nbOfChildItems; ++i)
                loadItemSettings(pSettings, childParentItem);
    }
}

void FileOrganiserWidget::loadSettings(QSettings &pSettings)
{
    pSettings.beginGroup(objectName());
        // Retrieve the data model

        pSettings.beginGroup(SettingsDataModel);
            loadItemSettings(pSettings, 0);
        pSettings.endGroup();

        // Resize the widget, just to be on the safe side

        resizeToContents();
    pSettings.endGroup();
}

void FileOrganiserWidget::saveItemSettings(QSettings &pSettings,
                                           QStandardItem *pItem,
                                           const int &pParentItemIndex)
{
    // Recursively keep track of the item settings

    static int crtItemIndex = -1;
    QStringList itemInfo;

    // The item information consists of:
    //  - The name of the folder item or the path of the file item
    //  - The index of its parent
    //  - The number of child items the (folder) item has, if any
    //  - Whether the (folder) items is expanded or not

    if (   (pItem == mDataModel->invisibleRootItem())
        || pItem->data(FileOrganiserItemFolder).toBool())
        // We are dealing with a folder item (be it the root folder item or not)

        itemInfo << pItem->text() << QString::number(pParentItemIndex)
                 << QString::number(pItem->rowCount())
                 << QString(isExpanded(pItem->index())?"1":"0");
    else
        // We are dealing with a file item

        itemInfo << pItem->data(FileOrganiserItemPath).toString()
                 << QString::number(pParentItemIndex) << "-1" << "0";

    pSettings.setValue(QString::number(++crtItemIndex), itemInfo);

    // Keep track of any child item

    int childParentItemIndex = crtItemIndex;

    for (int i = 0; i < pItem->rowCount(); ++i)
        saveItemSettings(pSettings, pItem->child(i), childParentItemIndex);
}

void FileOrganiserWidget::saveSettings(QSettings &pSettings)
{
    pSettings.beginGroup(objectName());
        // Keep track of the data model

        pSettings.remove(SettingsDataModel);
        pSettings.beginGroup(SettingsDataModel);
            saveItemSettings(pSettings, mDataModel->invisibleRootItem(), -1);
        pSettings.endGroup();
    pSettings.endGroup();
}

QSize FileOrganiserWidget::sizeHint() const
{
    // Suggest a default size for the file organiser widget
    // Note: this is critical if we want a docked widget, with a file organiser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

bool FileOrganiserWidget::viewportEvent(QEvent *pEvent)
{
    if (pEvent->type() == QEvent::ToolTip) {
        // We need to show a tool tip, so make sure that it's up to date by
        // setting it to the path of the current file item or to nothing if we
        // are dealing with a folder item

        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(pEvent);
        QStandardItem *crtItem = mDataModel->itemFromIndex(indexAt(helpEvent->pos()));

        if (crtItem)
            setToolTip(QDir::toNativeSeparators(crtItem->data(FileOrganiserItemFolder).toBool()?
                                                    "":
                                                    crtItem->data(FileOrganiserItemPath).toString()));
    }

    // Default handling of the event

    return TreeView::viewportEvent(pEvent);
}

void FileOrganiserWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if we are dropping
    // URIs

    if ((pEvent->mimeData()->hasFormat(FileSystemMimeType)) ||
        (pEvent->mimeData()->hasFormat(FileOrganiserMimeType)))
        pEvent->acceptProposedAction();
    else
        pEvent->ignore();
}

void FileOrganiserWidget::dragMoveEvent(QDragMoveEvent *pEvent)
{
    // Set the state to dragging, since we can only come here when dragging
    // Note: the state is properly set for file organiser objects being dragged,
    //       but should we be dragging external objects over our file organiser
    //       widget, then the state will (obviously) not be set. This wouldn't
    //       be a problem in itself if it was for the fact that this prevents
    //       the drop indicator from being painted, so...

    setState(QAbstractItemView::DraggingState);

    // Default handling of the event
    // Note: this gives us the drop indicator

    TreeView::dragMoveEvent(pEvent);

    // Accept the proposed action for the event, but only if there are objects
    // to drop and that we are not trying to drop them over a file item. Also,
    // should one object be dragged, then it cannot be dropped on itself...
    // Note #1: for the number of objects being dropped, we have to check the
    //          number of URLs information (i.e. external objects), as well as
    //          the mime data associated to FileOrganiserMimeType (i.e. objects
    //          from the file organiser widget)
    // Note #2: for the dropping location, it can be either a folder or a file
    //          (as long as the indicator position isn't on the folder)
    // Note #3: regarding the case where one object is being dragged, to check
    //          that it's not being dropped on itself only makes sense if the
    //          object comes from the file organiser widget...

    QByteArray data = pEvent->mimeData()->data(FileOrganiserMimeType);
    QDataStream stream(&data, QIODevice::ReadOnly);

    int nbOfFileOrganiserItemsDropped;

    if (data.size())
        stream >> nbOfFileOrganiserItemsDropped;
    else
        nbOfFileOrganiserItemsDropped = 0;

    QStandardItem *draggedItem;

    if (nbOfFileOrganiserItemsDropped == 1)
        draggedItem = 0;   //---GRY--- TO BE DONE
    else
        draggedItem = 0;

    QStandardItem *crtItem = mDataModel->itemFromIndex(indexAt(pEvent->pos()));

    if (   (pEvent->mimeData()->urls().count() || nbOfFileOrganiserItemsDropped)
        && (   (crtItem && crtItem->data(FileOrganiserItemFolder).toBool())
            || (dropIndicatorPosition() != QAbstractItemView::OnItem))
        && ((draggedItem != crtItem)))
        pEvent->acceptProposedAction();
    else
        pEvent->ignore();
}

void FileOrganiserWidget::dropEvent(QDropEvent *pEvent)
{
    // Note: the mime data will definitely contain the FileSystemMimeType mime
    //       type (if the objects originated from outside this widget), but it
    //       may also contain the FileOrganiserMimeType mime type (if the
    //       objects originated from this widget). The FileOrganiserMimeType
    //       mime type is used by this widget while the latter by external
    //       widgets. So, this means that we must check for the
    //       FileOrganiserMimeType mime type first

    if (pEvent->mimeData()->hasFormat(FileOrganiserMimeType)) {
        // The user is dropping folders/files from ourselves, i.e. he wants some
        // folders/files to be moved around

//---GRY--- TO BE DONE...

        QByteArray data = pEvent->mimeData()->data(FileOrganiserMimeType);
        QDataStream stream(&data, QIODevice::ReadOnly);

        int nbOfFileOrganiserItemsDropped;

        stream >> nbOfFileOrganiserItemsDropped;

        for (int i = 0; i < nbOfFileOrganiserItemsDropped; ++i) {
            int row;
            QMap<int, QVariant> itemData;

            stream >> row >> itemData;
        }
    } else {
        // Files have been dropped, so add them to the widget and this at the
        // right place (i.e. above/on/below a folder, above/below a file or on
        // the invisible root folder)

        QStandardItem *crtItem;
        DropIndicatorPosition dropPosition = dropIndicatorPosition();

        if (dropPosition == QAbstractItemView::OnViewport) {
            // We dropped the files on the viewport, so...

            crtItem = mDataModel->invisibleRootItem();

            // Change the drop position since we know that we want want the
            // objects to be dropped on the root folder

            dropPosition = QAbstractItemView::OnItem;
        } else {
            // We dropped the files above/on/below a folder or above/below a
            // file, so...

            crtItem = mDataModel->itemFromIndex(indexAt(pEvent->pos()));
        }

        // Effectively add the files to the widget and this at the right place

        QList<QUrl> urls = pEvent->mimeData()->urls();

        if (dropPosition != QAbstractItemView::BelowItem)
            // Add the files in the order they were dropped

            for (int i = 0; i < urls.count(); ++i)
                addFile(urls.at(i).toLocalFile(), crtItem, dropPosition);
        else
            // Add the files in a reverse order to that they were dropped since
            // we want them added below the current item

            for (int i = urls.count()-1; i >= 0; --i)
                addFile(urls.at(i).toLocalFile(), crtItem, dropPosition);
    }

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();

    // Reset the state of the widget
    // Note: there doesn't seem to be and there shouldn't be a need to reset the
    //       state after the widget (the resetting seems to be done elsewhere),
    //       but if don't reset the state of the widget, then the drop indicator
    //       may, in some cases, remain visible (a bug in Qt?), so...

    setState(QAbstractItemView::NoState);
}

bool FileOrganiserWidget::isFolderItem(const QModelIndex &pItemIndex)
{
    return mDataModel->itemFromIndex(pItemIndex)->data(FileOrganiserItemFolder).toBool();
}

QString FileOrganiserWidget::newFolderName(QStandardItem *pFolderItem)
{
    // Come up with the name for a new folder which is to be under pFolderItem

    // Retrieve the name of the folders under pFolderItem

    QStringList subFolderNames;

    for (int i = 0; i < pFolderItem->rowCount(); ++i)
        subFolderNames.append(pFolderItem->child(i)->text());

    // Compare the suggested name of our new folder with that of the folders
    // under pFolderItem, this until we come up with a suggested name which is
    // not already taken

    static const QString baseFolderName = "New Folder";
    static const QString templateFolderName = baseFolderName+" (%1)";
    int folderNb = 1;
    QString folderName = baseFolderName;

    while (subFolderNames.contains(folderName))
        folderName = templateFolderName.arg(++folderNb);

    return folderName;
}

bool FileOrganiserWidget::newFolder()
{
    // Create a folder item below the current folder item or root item,
    // depending on the situation

    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();
    int nbOfSelectedIndexes = selectedIndexes.count();

    if (nbOfSelectedIndexes <= 1) {
        // Either no or one item is currently selected, so retrieve that item
        // and check that it is a folder item

        QStandardItem *crtItem = !nbOfSelectedIndexes?
                                           mDataModel->invisibleRootItem():
                                           mDataModel->itemFromIndex(selectedIndexes.first());

        if (   (crtItem == mDataModel->invisibleRootItem())
            || crtItem->data(FileOrganiserItemFolder).toBool()) {
            // The current item is a folder item, so we can create the new
            // folder item under the root item or the existing folder item

            QStandardItem *newFolderItem = new QStandardItem(QIcon(CollapsedFolderIcon),
                                                             newFolderName(crtItem));

            newFolderItem->setData(true, FileOrganiserItemFolder);

            crtItem->appendRow(newFolderItem);

            // Some post-processing, but only if no other item is currently
            // being edited

            if (state() != QAbstractItemView::EditingState) {
                // Expand the current index (so that we can see the new folder)
                // Note: this is only relevant in the case of a folder item
                //       being currently selected (i.e. it's not the root folder
                //       item)

                if (nbOfSelectedIndexes == 1)
                    setExpanded(crtItem->index(), true);

                // Offer the user to edit the newly added folder item

                edit(newFolderItem->index());
            }

            // Resize the widget, just to be on the safe side

            resizeToContents();

            return true;
        } else {
            // The current item is not a folder item, so...
            // Note: we should never come here (i.e. the caller to this function
            //       should ensure that a folder can be created before calling
            //       this function), but it's better to be safe than sorry

            return false;
        }
    } else {
        // Several folder items are selected, so...
        // Note: we should never come here (i.e. the caller to this function
        //       should ensure that a folder can be created before calling this
        //       function), but it's better to be safe than sorry

        return false;
    }
}

bool FileOrganiserWidget::addFileItem(const QString &pFileName,
                                      QStandardItem *pItem,
                                      const QAbstractItemView::DropIndicatorPosition &pDropPosition)
{
    // Add the file above/on/below the passed item

    if (pItem) {
        // The passed item is valid, so add the file above/on/below it,
        // depending on the drop position and only if the file isn't already
        // present

        // First, determine the item that will own the file

        QStandardItem *parentItem = (pDropPosition == QAbstractItemView::OnItem)?
                                        pItem:
                                        pItem->parent()?
                                            pItem->parent():
                                            mDataModel->invisibleRootItem();

        // Second, check whether the file is already owned by the future parent
        // item or not

        bool fileExists = false;

        for (int i = 0; (i < parentItem->rowCount()) && !fileExists; ++i) {
            // Check whether the current item is a file and whether it's the one
            // we want to add

            QStandardItem *crtItem = parentItem->child(i);

            fileExists =    !crtItem->data(FileOrganiserItemFolder).toBool()
                         && (crtItem->data(FileOrganiserItemPath).toString() == pFileName);
        }

        // Third, if the file is not already owned, then add it to the parent
        // item and this to the right place, depending on the drop position

        if (!fileExists) {
            QStandardItem *newFileItem = new QStandardItem(QIcon(FileIcon),
                                                           QFileInfo(pFileName).fileName());

            newFileItem->setData(pFileName, FileOrganiserItemPath);

            switch (pDropPosition) {
            case QAbstractItemView::AboveItem:
                // We dropped the file above the item, so...

                parentItem->insertRow(pItem->row(), newFileItem);

                break;
            case QAbstractItemView::BelowItem:
                // We dropped the file below the item, so...

                parentItem->insertRow(pItem->row()+1, newFileItem);

                break;
            default:
                // We directly dropped the file onto the item, so...

                parentItem->appendRow(newFileItem);

                // Expand the parent item, so the user knows that the file has
                // been added (assuming it was collapsed)

                setExpanded(parentItem->index(), true);

                break;
            }

            // Resize the widget, just in case the new file takes more space
            // that is visible

            resizeToContents();

            return true;
        } else {
            // The file is already present, so...

            return false;
        }
    } else {
        // The passed item is not valid, so...
        // Note: we should never come here (i.e. the caller to this function
        //       should ensure that the passed item is valid), but it's better
        //       to be safe than sorry

        return false;
    }
}

bool FileOrganiserWidget::addFile(const QString &pFileName,
                                  QStandardItem *pItem,
                                  const QAbstractItemView::DropIndicatorPosition &pDropPosition)
{
    QFileInfo fileInfo = pFileName;

    if (fileInfo.isFile()) {
        if (fileInfo.isSymLink()) {
            // We are dropping a symbolic link, so retrieve its target
            // and check that it exists, and if it does then add it

            QString fileName = fileInfo.symLinkTarget();

            if (QFileInfo(fileName).exists())
                // The target file exists, so...

                return addFileItem(fileName, pItem, pDropPosition);
            else
                // The target file doesn't exist, so...

                return false;
        } else {
            // We are dropping a file, so we can just add it

            return addFileItem(pFileName, pItem, pDropPosition);
        }
    } else {
        // This is not a file, so...

        return false;
    }
}

void FileOrganiserWidget::collapseEmptyFolders(QStandardItem *pFolder)
{
    // Recursively collapse any empty child folder

    for (int i = 0; i < pFolder->rowCount(); ++i)
        if (pFolder->child(i)->data(FileOrganiserItemFolder).toBool())
            collapseEmptyFolders(pFolder->child(i));

    // Collapse the current folder, if necessary and if it isn't the root folder

    if ((pFolder != mDataModel->invisibleRootItem()) && !pFolder->rowCount())
        collapse(mDataModel->indexFromItem(pFolder));
}

bool FileOrganiserWidget::deleteItems()
{
    // Remove all the selected items

    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    if (selectedIndexes.isEmpty()) {
        // Nothing to delete, so...
        // Note: we should never come here (i.e. the caller to this function
        //       should ensure that a folder can be created before calling this
        //       function), but it's better to be safe than sorry

        return false;
    } else {
        // There are some items to delete, so delete them one by one, making
        // sure that we update our list of items every time (this is because the
        // row value of the remaining selected items may become different after
        // deleting an item)

        while(!selectedIndexes.isEmpty()) {
            mDataModel->removeRow(selectedIndexes.first().row(),
                                  selectedIndexes.first().parent());

            selectedIndexes = selectionModel()->selectedIndexes();
        }

        // Collapse any folder that doesn't contain any file/folder anymore

        collapseEmptyFolders(mDataModel->invisibleRootItem());

        // Resize the widget to its contents in case its width was too wide (and
        // therefore required a horizontal scrollbar), but is now fine

        resizeToContents();

        return true;
    }
}

QString FileOrganiserWidget::filePath(const QModelIndex &pFileIndex)
{
    // Return the file path of pFileIndex

    return mDataModel->filePath(pFileIndex);
}

void FileOrganiserWidget::resizeToContents()
{
    // Make sure that the first column allows for all of its contents to be
    // visible

    resizeColumnToContents(0);
}

QStringList FileOrganiserWidget::selectedFiles()
{
    // Retrieve all the files that are currently selected
    // Note: if there is a folder among the selected items, then we return an
    //       empty list

    QStringList crtSelectedFiles;
    QModelIndexList crtSelectedIndexes = selectedIndexes();

    for (int i = 0; i < crtSelectedIndexes.count(); ++i) {
        QString fileName = filePath(crtSelectedIndexes.at(i));

        if (fileName.isEmpty())
            // The current item is not a file, so return an empty list

            return QStringList();
        else
            // The current item is a file, so just add to the list

            crtSelectedFiles.append(fileName);
    }

    return crtSelectedFiles;
}

void FileOrganiserWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    TreeView::keyPressEvent(pEvent);

    // Let people know about a key having been pressed with the view of opening
    // one or several files

    QStringList crtSelectedFiles = selectedFiles();

    if (   crtSelectedFiles.count()
#ifndef Q_WS_MAC
        && (   (pEvent->key() == Qt::Key_Enter)
            || (pEvent->key() == Qt::Key_Return)))
#else
        && (   (pEvent->modifiers() & Qt::ControlModifier)
            && (pEvent->key() == Qt::Key_Down)))
#endif
        // There are some files that are selected and we want to open them, so
        // let people know about it

        emit filesOpened(crtSelectedFiles);
}

void FileOrganiserWidget::expandedFolder(const QModelIndex &pFolderIndex)
{
    // The folder is being expanded, so update its icon to reflect its new state

    mDataModel->itemFromIndex(pFolderIndex)->setIcon(QIcon(ExpandedFolderIcon));

    // Resize the widget, just to be on the safe side

    resizeToContents();
}

void FileOrganiserWidget::collapsedFolder(const QModelIndex &pFolderIndex)
{
    // The folder is being expanded, so update its icon to reflect its new state

    mDataModel->itemFromIndex(pFolderIndex)->setIcon(QIcon(CollapsedFolderIcon));

    // Resize the widget, just to be on the safe side

    resizeToContents();
}
