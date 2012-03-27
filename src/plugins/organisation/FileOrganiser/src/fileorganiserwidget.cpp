//==============================================================================
// File organiser widget
//==============================================================================

#include "fileorganiserwidget.h"

//==============================================================================

#include <QDir>
#include <QFileInfo>
#include <QHelpEvent>
#include <QSettings>
#include <QUrl>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiser {

//==============================================================================

static const QString CollapsedFolderIcon = ":oxygen/places/folder.png";
static const QString ExpandedFolderIcon  = ":oxygen/actions/document-open-folder.png";
static const QString FileIcon            = ":oxygen/mimetypes/application-x-zerosize.png";
static const QString DeletedFileIcon     = ":oxygen/status/image-missing.png";

//==============================================================================

QStringList FileOrganiserModel::mimeTypes() const
{
    // Return the MIME types supported by our model

    return QStringList() << Core::FileSystemMimeType << FileOrganiserMimeType;
}

//==============================================================================

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

//==============================================================================

QByteArray FileOrganiserModel::encodeHierarchyData(const QModelIndex &pIndex) const
{
    // Encode the hierarchy data

    QByteArray res;
    QDataStream stream(&res, QIODevice::WriteOnly);

    encodeHierarchyData(pIndex, stream);

    return res;
}

//==============================================================================

QByteArray FileOrganiserModel::encodeData(const QModelIndexList &pIndexes) const
{
    QByteArray res;

    if (pIndexes.count()) {
        // Encode the MIME data

        QDataStream stream(&res, QIODevice::WriteOnly);

        // The number of items

        stream << pIndexes.count();

        // Hierarchy to reach the various items

        foreach (const QModelIndex &index, pIndexes)
            // Hierarchy to reach the current item

            encodeHierarchyData(index, stream);
    }

    // We are all done, so...

    return res;
}

//==============================================================================

QModelIndex FileOrganiserModel::decodeHierarchyData(QDataStream &pStream) const
{
    // Decode the hierarchy data

    // The number of levels

    int levelsCount;

    pStream >> levelsCount;

    // Hierarchy to reach the item

    int row;
    QStandardItem *crtItem = invisibleRootItem();

    for (int j = 0; j < levelsCount; ++j) {
        pStream >> row;

        crtItem = crtItem->child(row, 0);
    }

    // We are all done, so...

    return indexFromItem(crtItem);
}

//==============================================================================

QModelIndex FileOrganiserModel::decodeHierarchyData(QByteArray &pData) const
{
    // Decode the hierarchy data

    QDataStream stream(&pData, QIODevice::ReadOnly);

    return decodeHierarchyData(stream);
}

//==============================================================================

QModelIndexList FileOrganiserModel::decodeData(QByteArray &pData) const
{
    QModelIndexList res;

    if (pData.size()) {
        // Decode the MIME data

        QDataStream stream(&pData, QIODevice::ReadOnly);

        // The number of items

        int itemsCount;

        stream >> itemsCount;

        // Hierarchy to reach the various items

        for (int i = 0; i < itemsCount; ++i)
            res << decodeHierarchyData(stream);
    }

    // We are all done, so...

    return res;
}

//==============================================================================

QMimeData * FileOrganiserModel::mimeData(const QModelIndexList &pIndexes) const
{
    QMimeData *res = new QMimeData();
    QList<QUrl> urls;

    // Retrieve the URL of the different file (not folder) items
    // Note: this list of URLs is useful with regards to the FileSystemMimeType
    //       MIME type on which external widgets (e.g. the central widget) rely
    //       on to extract the name of the vavarious files the MIME data
    //       contains

    foreach (const QModelIndex &index, pIndexes) {
        QString crtFilePath = filePath(index);

        if (!crtFilePath.isEmpty())
            urls << QUrl::fromLocalFile(crtFilePath);
    }

    res->setUrls(urls);

    // Set the data which contains information on both the folder and file items
    // Note: this data is useful with regards to the FileOrganiserMimeType MIME
    //       type on which the file organiser widget relies for moving folder
    //       and file items around

    res->setData(FileOrganiserMimeType, encodeData(pIndexes));

    // All done, so...

    return res;
}

//==============================================================================

QString FileOrganiserModel::filePath(const QModelIndex &pFileIndex) const
{
    // Return the file path of pFileIndex, if it exists and corresponds to a
    // file

    QStandardItem *fileItem = itemFromIndex(pFileIndex);

    if (fileItem && !fileItem->data(Item::Folder).toBool())
        return fileItem->data(Item::Path).toString();
    else
        return QString();
}

//==============================================================================

FileOrganiserWidget::FileOrganiserWidget(QWidget *pParent) :
    TreeView(pParent)
{
    // Create an instance of the data model that we want to view

    mDataModel = new FileOrganiserModel();

    // Create our 'local' file manager (as opposed to the 'global' file manager
    // that comes with the FileManager class)

    mFileManager = new Core::FileManager();

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

    // A connection to handle the change of selection

    connect(selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(emitItemsRelatedSignals()));

    // Some connections to handle our file manager

    connect(mFileManager, SIGNAL(fileContentsChanged(const QString &)),
            this, SLOT(fileContentsChanged(const QString &)));
    connect(mFileManager, SIGNAL(fileDeleted(const QString &)),
            this, SLOT(fileDeleted(const QString &)));
}

//==============================================================================

FileOrganiserWidget::~FileOrganiserWidget()
{
    // Delete some internal objects

    delete mFileManager;
    delete mDataModel;
}

//==============================================================================

static const QString SettingsDataModel    = "DataModel";
static const QString SettingsSelectedItem = "SelectedItem";

//==============================================================================

void FileOrganiserWidget::loadItemSettings(QSettings *pSettings,
                                           QStandardItem *pParentItem)
{
    // Recursively retrieve the item settings

    static int crtItemIndex = -1;
    QStringList itemInfo;

    itemInfo = pSettings->value(QString::number(++crtItemIndex)).toStringList();

    if (itemInfo != QStringList()) {
        QString textOrPath  = itemInfo.at(0);
        int parentItemIndex = itemInfo.at(1).toInt();
        int childItemsCount = itemInfo.at(2).toInt();
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

            if (childItemsCount >= 0) {
                // We are dealing with a folder item

                QStandardItem *folderItem = new QStandardItem(QIcon(CollapsedFolderIcon),
                                                              textOrPath);

                folderItem->setData(true, Item::Folder);

                pParentItem->appendRow(folderItem);

                // Expand the folder item, if necessary

                if (expanded)
                    setExpanded(folderItem->index(), true);

                // The folder item is to be the parent of any of its child item

                childParentItem = folderItem;
            } else {
                // We are dealing with a file item

                QFileInfo fileInfo = textOrPath;

                QStandardItem *fileItem = new QStandardItem(QIcon(fileInfo.exists()?
                                                                      FileIcon:
                                                                      DeletedFileIcon),
                                                            fileInfo.fileName());

                fileItem->setData(textOrPath, Item::Path);

                pParentItem->appendRow(fileItem);

                // Add the file to our file manager
                // Note: it doesn't matter whether or not the file is already
                //       being monitored, since if that's the case then the
                //       current instance will be ignored

                mFileManager->manage(textOrPath);

                // A file cannot have child items, so...

                childParentItem = 0;
            }
        }

        // Retrieve any child item
        // Note: the test on childParentItem is not necessary (since
        //       childItemsCount will be equal to -1 in the case of a file
        //       item), but it doesn't harm having it, so...

        if (childParentItem)
            for (int i = 0; i < childItemsCount; ++i)
                loadItemSettings(pSettings, childParentItem);
    }
}

//==============================================================================

void FileOrganiserWidget::loadSettings(QSettings *pSettings)
{
    // Let the user know of a few default things about ourselves by emitting a
    // few signals

    emitItemsRelatedSignals();

    // Retrieve the data model

    pSettings->beginGroup(SettingsDataModel);
        loadItemSettings(pSettings, 0);
    pSettings->endGroup();

    // Retrieve the currently selected item, if any

    QByteArray hierarchyData = pSettings->value(SettingsSelectedItem).toByteArray();

    setCurrentIndex(mDataModel->decodeHierarchyData(hierarchyData));

    // Resize the widget, just to be on the safe side

    resizeToContents();
}

//==============================================================================

void FileOrganiserWidget::saveItemSettings(QSettings *pSettings,
                                           QStandardItem *pItem,
                                           const int &pParentItemIndex) const
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
        || pItem->data(Item::Folder).toBool()) {
        // We are dealing with a folder item (be it the root folder item or not)

        itemInfo << pItem->text() << QString::number(pParentItemIndex)
                 << QString::number(pItem->rowCount())
                 << QString(isExpanded(pItem->index())?"1":"0");
    } else {
        // We are dealing with a file item

        QString fileName = pItem->data(Item::Path).toString();

        itemInfo << fileName
                 << QString::number(pParentItemIndex) << "-1" << "0";

        // Remove the file from our file manager
        // Note: it doesn't matter whether or not the file has already been
        //       removed, since if that's the case then nothing will be done

        mFileManager->unmanage(fileName);
    }

    pSettings->setValue(QString::number(++crtItemIndex), itemInfo);

    // Keep track of any child item

    int childParentItemIndex = crtItemIndex;

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i)
        saveItemSettings(pSettings, pItem->child(i), childParentItemIndex);
}

//==============================================================================

void FileOrganiserWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the data model

    pSettings->remove(SettingsDataModel);
    pSettings->beginGroup(SettingsDataModel);
        saveItemSettings(pSettings, mDataModel->invisibleRootItem(), -1);
    pSettings->endGroup();

    // Keep track of the currently selected item, but only if it is visible

    bool crtItemVisible = true;
    QModelIndex crtIndexParent = currentIndex().parent();

    while (crtIndexParent != QModelIndex())
        if (isExpanded(crtIndexParent)) {
            // The current parent is expanded, so check to its parent

            crtIndexParent = crtIndexParent.parent();
        } else {
            // The current parent is not expanded, so...

            crtItemVisible = false;

            break;
        }

    pSettings->setValue(SettingsSelectedItem, mDataModel->encodeHierarchyData(crtItemVisible?
                                                                                  currentIndex():
                                                                                  QModelIndex()));
}

//==============================================================================

QSize FileOrganiserWidget::sizeHint() const
{
    // Suggest a default size for the file organiser widget
    // Note: this is critical if we want a docked widget, with a file organiser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

//==============================================================================

bool FileOrganiserWidget::viewportEvent(QEvent *pEvent)
{
    if (pEvent->type() == QEvent::ToolTip) {
        // We need to show a tool tip, so make sure that it's up to date by
        // setting it to the path of the current file item or to nothing if we
        // are dealing with a folder item

        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(pEvent);
        QStandardItem *crtItem = mDataModel->itemFromIndex(indexAt(helpEvent->pos()));

        if (crtItem)
            setToolTip(QDir::toNativeSeparators(crtItem->data(Item::Folder).toBool()?
                                                    "":
                                                    crtItem->data(Item::Path).toString()));
    }

    // Default handling of the event

    return TreeView::viewportEvent(pEvent);
}

//==============================================================================

void FileOrganiserWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if we are dropping
    // URIs

    if (   (pEvent->mimeData()->hasFormat(Core::FileSystemMimeType))
        || (pEvent->mimeData()->hasFormat(FileOrganiserMimeType)))
        pEvent->acceptProposedAction();
    else
        pEvent->ignore();
}

//==============================================================================

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
    // to drop and if we are not trying to drop the objects above/on/below one
    // of them (should the objects come from the file organiser widget) or on a
    // file item
    // Note #1: for the number of objects being dropped, we have to check the
    //          number of URLs information (i.e. external objects), as well as
    //          the MIME data associated with FileOrganiserMimeType (i.e.
    //          objects from the file organiser widget, after we have )
    // Note #2: for the dropping location, it can be either a folder or a file
    //          (as long as the indicator position isn't on the item itself),
    //          but not above/on/below any of the objects (or any of their
    //          children) being dragged (only relevant when dragging items from
    //          the file organiser widget)

    QByteArray data = pEvent->mimeData()->data(FileOrganiserMimeType);
    QModelIndexList indexes = mDataModel->decodeData(data);
    QStandardItem *dropItem = mDataModel->itemFromIndex(indexAt(pEvent->pos()));
    bool draggingOnSelfOrChild = false;

    if (dropItem)
        for (int i = 0; (i < indexes.count()) && !draggingOnSelfOrChild; ++i)
            draggingOnSelfOrChild = itemIsOrIsChildOf(dropItem, mDataModel->itemFromIndex(indexes.at(i)));

    if (   (pEvent->mimeData()->urls().count() || indexes.count())
        && (   (dropItem && dropItem->data(Item::Folder).toBool())
            || (dropIndicatorPosition() != QAbstractItemView::OnItem))
        && !draggingOnSelfOrChild)
        pEvent->acceptProposedAction();
    else
        pEvent->ignore();
}

//==============================================================================

void FileOrganiserWidget::dropEvent(QDropEvent *pEvent)
{
    // Note: the MIME data definitely contains the FileSystemMimeType MIME type
    //       (for objects originating from outside this widget), but it may also
    //       contain the FileOrganiserMimeType MIME type (for objects
    //       originating from within this widget). FileOrganiserMimeType is used
    //       by this widget while FileSystemMimeType by external widgets. So,
    //       this means that we must check for FileOrganiserMimeType first

    // Files have been dropped, so add them to the widget and this at the right
    // place (i.e. above/on/below a folder, above/below a file or on the
    // invisible root folder)

    // First, determine the item above/on/below which objects are to be dropped,
    // as well as the drop position (i.e. above, on or below)

    QStandardItem *dropItem;
    DropIndicatorPosition dropPosition = dropIndicatorPosition();

    if (dropPosition == QAbstractItemView::OnViewport) {
        // We dropped the files on the viewport, so...

        dropItem = mDataModel->invisibleRootItem();

        // Change the drop position since we know that we want want the objects
        // to be dropped on the root folder

        dropPosition = QAbstractItemView::OnItem;
    } else {
        // We dropped the files above/on/below a folder or above/below a file,
        // so...

        dropItem = mDataModel->itemFromIndex(indexAt(pEvent->pos()));
    }

    // Check the type of MIME data to be dropped

    if (pEvent->mimeData()->hasFormat(FileOrganiserMimeType)) {
        // The user is dropping folders/files from ourselves, i.e. s/he wants
        // some folders/files to be moved around

        // Retrieve the list of indexes to move around and clean it

        QByteArray data = pEvent->mimeData()->data(FileOrganiserMimeType);
        QModelIndexList indexes = cleanIndexList(mDataModel->decodeData(data));

        // Convert our list of indexes to a list of items
        // Note: indeed, by moving the item corresponding to a particular index,
        //       we may mess up the other indexes, meaning that we may not be
        //       able to retrieve their corresponding item, so...

        QList<QStandardItem *> items;

        for (int i = 0, iMax = indexes.count(); i < iMax; ++i)
            items << mDataModel->itemFromIndex(indexes.at(i));

        // Move the contents of the list to its final destination

        if (dropPosition != QAbstractItemView::BelowItem)
            // Move the items in the order they were dropped

            for (int i = 0, iMax = items.count(); i < iMax; ++i)
                moveItem(items.at(i), dropItem, dropPosition);
        else
            // Move the items in a reverse order to that they were dropped since
            // we want them moved below the current item

            for (int i = items.count()-1; i >= 0; --i)
                moveItem(items.at(i), dropItem, dropPosition);
    } else {
        // The user wants to drop files, so add them to the widget and this at
        // the right place

        QList<QUrl> urls = pEvent->mimeData()->urls();

        if (dropPosition != QAbstractItemView::BelowItem)
            // Add the files in the order they were dropped

            for (int i = 0, iMax = urls.count(); i < iMax; ++i)
                addFile(urls.at(i).toLocalFile(), dropItem, dropPosition);
        else
            // Add the files in a reverse order to that they were dropped since
            // we want them added below the current item

            for (int i = urls.count()-1; i >= 0; --i)
                addFile(urls.at(i).toLocalFile(), dropItem, dropPosition);
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

//==============================================================================

bool FileOrganiserWidget::parentIndexExists(const QModelIndex &pIndex,
                                            const QModelIndexList &pIndexes) const
{
    // Recursively determine whether one of the parents of the given index is in
    // the provided list

    QModelIndex parentIndex = pIndex.parent();

    if (parentIndex != QModelIndex()) {
        // The current index has a valid parent, so check whether the parent is
        // in the list

        if (pIndexes.indexOf(parentIndex) != -1)
            // The parent index could be found, so...

            return true;
        else
            // The parent index couldn't be found, but what about the parent
            // index's parent?

            return parentIndexExists(parentIndex, pIndexes);
    } else {
        // The current index doesn't have a valid parent, so...

        return false;
    }
}

//==============================================================================

QModelIndexList FileOrganiserWidget::cleanIndexList(const QModelIndexList &pIndexes) const
{
    // A list of indexes may contain indexes that are not relevant or
    // effectively the duplicate of another existing index, so...

    QModelIndexList res;

    // If both the index of a folder and some (if not all) of its (in)direct
    // contents is in the original list, then we only keep track of the index of
    // the folder

    for (int i = 0, iMax = pIndexes.count(); i < iMax; ++i) {
        // Check whether one of the current index's parents is already in the
        // list. If so, then skip the current index

        QModelIndex crtIndex = pIndexes.at(i);

        if (!parentIndexExists(crtIndex, pIndexes)) {
            // None of the index's parents is in the list, so we can safely add
            // the index to the list

            res << crtIndex;

            // If the index refers to a folder, then we must double check that
            // the list of cleaned indexes doesn't contain any of the index's
            // children. If it does, then we must remove all of them

            QStandardItem *crtItem = mDataModel->itemFromIndex(crtIndex);

            if (crtItem && crtItem->data(Item::Folder).toBool())
                for (int j = res.count()-1; j >= 0; --j)
                    if (parentIndexExists(res.at(j), res))
                        res.removeAt(j);
        }
    }

    // At this stage, we have indexes for folders that are unique, but we may
    // still have indexes for files that are effectively the duplicate of
    // another file, so these are to be removed from the cleaned list and from
    // the model

    for (int i = res.count()-1; i >= 0; --i) {
        QStandardItem *crtItem = mDataModel->itemFromIndex(res.at(i));

        if (crtItem && !crtItem->data(Item::Folder).toBool())
            // The index corresponds to a valid file item, so check whether in
            // the cleaned list there is another file item referencing the same
            // physical file and, if so, remove it from the cleaned list and the
            // model

            for (int j = 0; j < i; ++j) {
                QStandardItem *testItem = mDataModel->itemFromIndex(res.at(j));

                if (   testItem
                    && !testItem->data(Item::Folder).toBool()
                    && !crtItem->data(Item::Path).toString().compare(testItem->data(Item::Path).toString())) {
                    // The test item is a valid file item and references the
                    // same physical file as our current item, so remove the
                    // current item from the cleaned list

                    res.removeAt(i);

                    // Also remove the current item from the model

                    crtItem->parent()->removeRow(crtItem->row());

                    // Go to our next current item

                    break;
                }
            }
    }

    // We are all done, so...

    return res;
}

//==============================================================================

bool FileOrganiserWidget::itemIsOrIsChildOf(QStandardItem *pItem,
                                            QStandardItem *pOtherItem) const
{
    if (pItem == pOtherItem) {
        // pItem is the same as pOtherItem, so...

        return true;
    } else if (pOtherItem->rowCount()) {
        // pOtherItem has children, so check against them

        for (int i = 0, iMax = pOtherItem->rowCount(); i < iMax; ++i)
            if (itemIsOrIsChildOf(pItem, pOtherItem->child(i)))
                // pItem is a (in)direct child of pOtherItem, so...

                return true;

        // pItem is not the (in)direct child of pOtherItem, so...

        return false;
    } else {
        // pOtherItem doesn't have any children, so...

        return false;
    }
}

//==============================================================================

void FileOrganiserWidget::backupExpandedInformation(QStandardItem *pItem) const
{
    // Recursively backup the expanded state of the item, should it be a folder,
    // and of any of its children, should it have some

    if (pItem->data(Item::Folder).toBool()) {
        // Keep track of the expanded state of pItem

        pItem->setData(isExpanded(mDataModel->indexFromItem(pItem)),
                       Item::Expanded);

        // Do the same with all of pItem's children, if any

        for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i)
            backupExpandedInformation(pItem->child(i));
    }
}

//==============================================================================

void FileOrganiserWidget::restoreExpandedInformation(QStandardItem *pItem)
{
    // Recursively restore the expanded state of the item, should it be a
    // folder, and of any of its children, should it have some

    if (pItem->data(Item::Folder).toBool()) {
        // Retrieve the expanded state of pItem

        setExpanded(mDataModel->indexFromItem(pItem),
                    pItem->data(Item::Expanded).toBool());

        // Do the same with all of pItem's children, if any

        for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i)
            restoreExpandedInformation(pItem->child(i));
    }
}

//==============================================================================

QString FileOrganiserWidget::newFolderName(QStandardItem *pFolderItem) const
{
    // Come up with the name for a new folder which is to be under pFolderItem

    // Retrieve the name of the folders under pFolderItem

    QStringList subFolderNames;

    for (int i = 0, iMax = pFolderItem->rowCount(); i < iMax; ++i)
        subFolderNames << pFolderItem->child(i)->text();

    // Compare the suggested name of our new folder with that of the folders
    // under pFolderItem, this until we come up with a suggested name which is
    // not already taken

    QString baseFolderName = tr("New Folder");
    QString templateFolderName = baseFolderName+" (%1)";

    int folderNb = 1;
    QString res = baseFolderName;

    while (subFolderNames.contains(res))
        res = templateFolderName.arg(++folderNb);

    return res;
}

//==============================================================================

void FileOrganiserWidget::newFolder()
{
    if (!canCreateNewFolder())
        // The conditions are not met to create a new folder, so...

        return;

    // Either create a folder item below the current folder item or below the
    // root item, depending on the situation

    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();
    int selectedIndexesCount = selectedIndexes.count();
    QStandardItem *crtItem = !selectedIndexesCount?
                                mDataModel->invisibleRootItem():
                                mDataModel->itemFromIndex(selectedIndexes.first());
    QStandardItem *newFolderItem = new QStandardItem(QIcon(CollapsedFolderIcon),
                                                     newFolderName(crtItem));

    newFolderItem->setData(true, Item::Folder);

    crtItem->appendRow(newFolderItem);

    // Some post-processing, but only if no other item is currently being edited

    if (state() != QAbstractItemView::EditingState) {
        // Expand the current index (so that we can see the new folder)
        // Note: this is only relevant in the case of a folder item being
        //       currently selected (i.e. it's not the root folder item)

        if (selectedIndexesCount)
            setExpanded(crtItem->index(), true);

        // Offer the user to edit the newly added folder item

        edit(newFolderItem->index());
    }

    // Resize the widget, just to be on the safe side

    resizeToContents();
}

//==============================================================================

QStandardItem * FileOrganiserWidget::parentItem(QStandardItem *pDropItem,
                                                const QAbstractItemView::DropIndicatorPosition &pDropPosition)
{
    // Return who should be the parent item of pDropItem based on pDropPosition

    return (pDropPosition == QAbstractItemView::OnItem)?
                pDropItem:
                pDropItem->parent()?
                    pDropItem->parent():
                    mDataModel->invisibleRootItem();
}

//==============================================================================

bool FileOrganiserWidget::ownedBy(const QString &pFileName,
                                  QStandardItem *pItem)
{
    // Check whether pFileName is already owned by pItem

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
        // Check whether the current item is a file with pFileName as its name

        QStandardItem *crtItem = pItem->child(i);

        if (   !crtItem->data(Item::Folder).toBool()
            && (crtItem->data(Item::Path).toString() == pFileName))
            return true;
    }

    // We couldn't find a file with pFileName as its name in pItem, so...

    return false;
}

//==============================================================================

void FileOrganiserWidget::dropItems(QStandardItem *pDropItem,
                                    const QAbstractItemView::DropIndicatorPosition &pDropPosition,
                                    QStandardItem *pNewParentItem,
                                    QList<QStandardItem *> pItems)
{
    // Drop pItems based on pDropPosition's value

    switch (pDropPosition) {
        case QAbstractItemView::AboveItem:
            // We dropped pItems above pDropItem, so...

            pNewParentItem->insertRow(pDropItem->row(), pItems);

            break;
        case QAbstractItemView::BelowItem:
            // We dropped pItems below pDropItem, so...

            pNewParentItem->insertRow(pDropItem->row()+1, pItems);

            break;
        default:
            // We directly dropped pItems on pDropItem, so...

            pNewParentItem->appendRow(pItems);

            // Expand pNewParentItem, so the user knows that the item has been
            // moved to it (assuming that pNewParentItem was collapsed)

            setExpanded(pNewParentItem->index(), true);
    }
}

//==============================================================================

void FileOrganiserWidget::dropItem(QStandardItem *pDropItem,
                                   const QAbstractItemView::DropIndicatorPosition &pDropPosition,
                                   QStandardItem *pNewParentItem,
                                   QStandardItem *pItem)
{
    // Drop the item as if we wanted to drop a list of items

    dropItems(pDropItem, pDropPosition, pNewParentItem,
              QList<QStandardItem *>() << pItem);
}

//==============================================================================

void FileOrganiserWidget::addFile(const QString &pFileName,
                                  QStandardItem *pDropItem,
                                  const QAbstractItemView::DropIndicatorPosition &pDropPosition)
{
    if (!pDropItem)
        // pDropItem is not valid, so...

        return;

    // Check that we are indeed dealing with a file

    QFileInfo fileInfo = pFileName;

    if (!fileInfo.isFile())
        // We are not dealing with a file, so...

        return;

    // Check whether we are dropping a symbolic link

    QString fileName = pFileName;

    if (fileInfo.isSymLink())
        // We are dropping a symbolic link, so retrieve its target

        fileName = fileInfo.symLinkTarget();

    // Check whether the file exists

    if (QFileInfo(fileName).exists()) {
        // The target file exists, so add it above/on/below pDropItem, depending
        // on the drop position and only if the file isn't already present

        // First, determine the item that will own the file

        QStandardItem *newParentItem = parentItem(pDropItem, pDropPosition);

        // Second, if the file is not already owned, then add it to
        // newParentItem and this to the right place, depending on the value of
        // pDropPosition

        if (!ownedBy(fileName, newParentItem)) {
            QStandardItem *newFileItem = new QStandardItem(QIcon(FileIcon),
                                                           QFileInfo(fileName).fileName());

            newFileItem->setData(fileName, Item::Path);

            dropItem(pDropItem, pDropPosition, newParentItem, newFileItem);

            // Add the file to our file manager
            // Note: it doesn't matter whether or not the file is already being
            //       monitored, since if that's the case then the current
            //       instance will be ignored

            mFileManager->manage(fileName);

            // Resize the widget, just in case the new file takes more space
            // than is visible

            resizeToContents();
        } else {
            // The file is already owned by newParentItem, so just repaint the
            // widget to make sure that the dragging & dropping overlay
            // disappears

            repaint();
        }
    }
}

//==============================================================================

void FileOrganiserWidget::moveItem(QStandardItem *pItem,
                                   QStandardItem *pDropItem,
                                   const QAbstractItemView::DropIndicatorPosition &pDropPosition)
{
    if (!pDropItem)
        // pDropItem is not valid, so...

        return;

    // Move pItem above/on/below pDropItem, depending on the drop position, but
    // first, determine the item that will own pItem

    QStandardItem *crtParentItem = pItem->parent()?
                                       pItem->parent():
                                       mDataModel->invisibleRootItem();
    QStandardItem *newParentItem = parentItem(pDropItem, pDropPosition);

    // Second, check whether or not the (file) item points to a file which is
    // already owned by newParentItem

    bool fileAlreadyOwned = false;

    if (!pItem->data(Item::Folder).toBool())
        // The current item is a file item, so retrieve its file name and check
        // whether or not it's already owned by newParentItem

        fileAlreadyOwned = ownedBy(pItem->data(Item::Path).toString(), newParentItem);

    // Third, move pItem to newParentItem and this to the right place, depending
    // on the value of pDropPosition and only if the destination doesn't already
    // have that item (should it be a file item, since folder items are always
    // moved)

    if (!fileAlreadyOwned || (crtParentItem == newParentItem)) {
        // Either newParentItem doesn't already own an item which points to the
        // same file as pItem or pItem's current parent is the same as
        // newParentItem in which case it means that we want to move the item
        // within its current location

        // First, check whether the item is a folder and, if so, whether or not
        // it's expanded (and the same with any (in)direct child folder it may
        // contain)

        backupExpandedInformation(pItem);

        // Second, move the item (and any of its children)

        dropItems(pDropItem, pDropPosition, newParentItem,
                  crtParentItem->takeRow(pItem->row()));

        // Third, re-expand folders, if necessary

        restoreExpandedInformation(pItem);

        // Fourth, resize the widget, just in case the new location of the
        // item(s) requires more space than is visible

        resizeToContents();
    } else {
        // A (file) item pointing to the same file is already owned by
        // newParentItem, so just remove the item rather than move it

        crtParentItem->removeRow(pItem->row());
    }
}

//==============================================================================

void FileOrganiserWidget::collapseEmptyFolders(QStandardItem *pFolder)
{
    // Recursively collapse any empty child folder

    for (int i = 0, iMax = pFolder->rowCount(); i < iMax; ++i)
        if (pFolder->child(i)->data(Item::Folder).toBool())
            collapseEmptyFolders(pFolder->child(i));

    // Collapse the current folder, if necessary and if it isn't the root folder

    if ((pFolder != mDataModel->invisibleRootItem()) && !pFolder->rowCount())
        collapse(mDataModel->indexFromItem(pFolder));
}

//==============================================================================

void FileOrganiserWidget::deleteItems()
{
    // Remove all the selected items

    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    if (selectedIndexes.isEmpty())
        // Nothing to delete, so...

        return;

    // Delete the items one by one, making sure that we update our list of items
    // every time (this is because the row value of the remaining selected items
    // may become different after deleting an item)

    while (!selectedIndexes.isEmpty()) {
        // Remove the file from our file manager, should the index refer to a
        // file item
        // Note: it doesn't matter whether or not the file has already been
        //       removed, since if that's the case then nothing will be done...

        QModelIndex crtIndex = selectedIndexes.first();
        QStandardItem *crtItem = mDataModel->itemFromIndex(crtIndex);

        if (crtItem && !crtItem->data(Item::Folder).toBool())
            mFileManager->unmanage(crtItem->data(Item::Path).toString());

        // Remove the item from the model itself

        mDataModel->removeRow(crtIndex.row(), crtIndex.parent());

        // Update our list of selected indexes

        selectedIndexes = selectionModel()->selectedIndexes();
    }

    // Collapse any folder that doesn't contain any file/folder anymore

    collapseEmptyFolders(mDataModel->invisibleRootItem());

    // Resize the widget to its contents in case its width was too wide (and
    // therefore required a horizontal scrollbar), but is now fine

    resizeToContents();
}

//==============================================================================

QString FileOrganiserWidget::filePath(const QModelIndex &pFileIndex) const
{
    // Return the file path of pFileIndex

    return mDataModel->filePath(pFileIndex);
}

//==============================================================================

void FileOrganiserWidget::resizeToContents()
{
    // Make sure that the first column allows for all of its contents to be
    // visible

    resizeColumnToContents(0);
}

//==============================================================================

QStringList FileOrganiserWidget::selectedFiles() const
{
    // Retrieve all the files that are currently selected
    // Note: if there is a folder among the selected items, then we return an
    //       empty list

    QStringList res;
    QModelIndexList crtSelectedIndexes = selectedIndexes();

    for (int i = 0, iMax = crtSelectedIndexes.count(); i < iMax; ++i) {
        QString fileName = filePath(crtSelectedIndexes.at(i));

        if (fileName.isEmpty())
            // The current item is not a file, so return an empty list

            return QStringList();
        else
            // The current item is a file, so just add to the list

            res << fileName;
    }

    return res;
}

//==============================================================================

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

//==============================================================================

void FileOrganiserWidget::expandedFolder(const QModelIndex &pFolderIndex)
{
    // The folder is being expanded, so update its icon to reflect its new state

    mDataModel->itemFromIndex(pFolderIndex)->setIcon(QIcon(ExpandedFolderIcon));

    // Resize the widget, just to be on the safe side

    resizeToContents();
}

//==============================================================================

void FileOrganiserWidget::collapsedFolder(const QModelIndex &pFolderIndex)
{
    // The folder is being expanded, so update its icon to reflect its new state

    mDataModel->itemFromIndex(pFolderIndex)->setIcon(QIcon(CollapsedFolderIcon));

    // Resize the widget, just to be on the safe side

    resizeToContents();
}

//==============================================================================

bool FileOrganiserWidget::canCreateNewFolder() const
{
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();
    int selectedIndexesCount = selectedIndexes.count();

    // Determnine whether we can create a new folder

    if (selectedIndexesCount == 1)
        // One item is currently selected, so the only way we could create a new
        // folder is if the current item is also a folder

        return mDataModel->itemFromIndex(selectedIndexes.first())->data(Item::Folder).toBool();
    else
        // Either no item or several items are currently selected, so the only
        // way we could create a new folder is if no item is currently selected

        return !selectedIndexesCount;
}

//==============================================================================

void FileOrganiserWidget::emitItemsRelatedSignals()
{
    // Let the user know whether a new folder could be created and whether items
    // could be deleted

    emit newFolderEnabled(canCreateNewFolder());
    emit deleteItemsEnabled(selectionModel()->selectedIndexes().count() >= 1);
}

//==============================================================================

void FileOrganiserWidget::updateFileItems(QStandardItem *pItem,
                                          const QString &pFileName,
                                          const Core::File::Status &pStatus) const
{
    // Recursively update the icon of all file items that refer to pFileName

    if (   !pItem->data(Item::Folder).toBool()
        && !pItem->data(Item::Path).toString().compare(pFileName))
        // The current item is a file item and it refers to pFileName, so update
        // its icon based on the value of pStatus

        pItem->setIcon(QIcon((pStatus == Core::File::Deleted)?
                                 DeletedFileIcon:
                                 FileIcon));

    // Update our child file items, if any

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i)
        updateFileItems(pItem->child(i), pFileName, pStatus);
}

//==============================================================================

void FileOrganiserWidget::fileContentsChanged(const QString &pFileName) const
{
    // The contents of a file has been changed which may also mean that a file
    // may have been deleted and recreated, so go through all the (file) items
    // and update the icon of the ones that refer to the file in question

    updateFileItems(mDataModel->invisibleRootItem(), pFileName,
                    Core::File::Changed);
}

//==============================================================================

void FileOrganiserWidget::fileDeleted(const QString &pFileName) const
{
    // A file has been deleted, so...

    updateFileItems(mDataModel->invisibleRootItem(), pFileName,
                    Core::File::Deleted);
}

//==============================================================================

}   // namespace FileOrganiser
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
