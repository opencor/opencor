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
// File Organiser window widget
//==============================================================================

#include "corecliutils.h"
#include "filemanager.h"
#include "fileorganiserwindowwidget.h"

//==============================================================================

#include <QDir>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QKeyEvent>
#include <QMimeData>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiserWindow {

//==============================================================================

FileOrganiserWindowItem::FileOrganiserWindowItem(const QIcon &pIcon,
                                                 const QString &pTextOrPath,
                                                 bool pFolder) :
    QStandardItem(pIcon,
                  pFolder?
                      pTextOrPath:
                      QFileInfo(pTextOrPath).fileName()),
    mFolder(pFolder),
    mPath(pFolder?QString():pTextOrPath)
{
    // Customise ourselves

    setToolTip(pFolder?
                   pTextOrPath:
                   QDir::toNativeSeparators(Core::canonicalDirName(pTextOrPath)));
}

//==============================================================================

FileOrganiserWindowItem::~FileOrganiserWindowItem() = default;

//==============================================================================

bool FileOrganiserWindowItem::isFolder() const
{
    // Return whether we are a folder

    return mFolder;
}

//==============================================================================

bool FileOrganiserWindowItem::isExpanded() const
{
    // Return whether we are expanded

    return mExpanded;
}

//==============================================================================

void FileOrganiserWindowItem::setExpanded(bool pExpanded)
{
    // Set our expanded state

    mExpanded = mFolder?pExpanded:false;
}

//==============================================================================

QString FileOrganiserWindowItem::path() const
{
    // Return our path

    return mPath;
}

//==============================================================================

static const char *FileOrganiserWindowMimeType = "opencor/file-organiser-window";

//==============================================================================

FileOrganiserWindowModel::FileOrganiserWindowModel(QObject *pParent) :
    QStandardItemModel(pParent)
{
}

//==============================================================================

QStringList FileOrganiserWindowModel::mimeTypes() const
{
    // Return the MIME types supported by our model

    return QStringList() << Core::FileSystemMimeType << FileOrganiserWindowMimeType;
}

//==============================================================================

void FileOrganiserWindowModel::encodeHierarchyData(const QModelIndex &pIndex,
                                                   QDataStream &pStream,
                                                   int pLevel) const
{
    // Encode the item's hierarchy

    if (pIndex.isValid()) {
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

QByteArray FileOrganiserWindowModel::encodeHierarchyData(const QModelIndex &pIndex) const
{
    // Encode the hierarchy data

    QByteArray res;
    QDataStream stream(&res, QIODevice::WriteOnly);

    encodeHierarchyData(pIndex, stream);

    return res;
}

//==============================================================================

QByteArray FileOrganiserWindowModel::encodeData(const QModelIndexList &pIndexes) const
{
    QByteArray res;

    if (!pIndexes.isEmpty()) {
        // Encode the MIME data

        QDataStream stream(&res, QIODevice::WriteOnly);

        // The number of items

        stream << pIndexes.count();

        // Hierarchy to reach the various items

        for (const auto &index : pIndexes) {
            // Hierarchy to reach the current item

            encodeHierarchyData(index, stream);
        }
    }

    return res;
}

//==============================================================================

QModelIndex FileOrganiserWindowModel::decodeHierarchyData(QDataStream &pStream) const
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

    return indexFromItem(crtItem);
}

//==============================================================================

QModelIndex FileOrganiserWindowModel::decodeHierarchyData(QByteArray &pData) const
{
    // Decode the hierarchy data

    QDataStream stream(&pData, QIODevice::ReadOnly);

    return decodeHierarchyData(stream);
}

//==============================================================================

QModelIndexList FileOrganiserWindowModel::decodeData(QByteArray &pData) const
{
    QModelIndexList res;

    if (!pData.isEmpty()) {
        // Decode the MIME data

        QDataStream stream(&pData, QIODevice::ReadOnly);

        // The number of items

        int itemsCount;

        stream >> itemsCount;

        // Hierarchy to reach the various items

        for (int i = 0; i < itemsCount; ++i) {
            res << decodeHierarchyData(stream);
        }
    }

    return res;
}

//==============================================================================

QMimeData * FileOrganiserWindowModel::mimeData(const QModelIndexList &pIndexes) const
{
    auto res = new QMimeData();
    QList<QUrl> urls = QList<QUrl>();

    // Retrieve the URL of the different file (not folder) items
    // Note: this list of URLs is useful with regards to the FileSystemMimeType
    //       MIME type on which external widgets (e.g. the central widget) rely
    //       on to extract the name of the vavarious files the MIME data
    //       contains

    for (const auto &index : pIndexes) {
        QString crtFilePath = filePath(index);

        if (!crtFilePath.isEmpty()) {
            urls << QUrl::fromLocalFile(crtFilePath);
        }
    }

    res->setUrls(urls);

    // Set the data that contains information on both the folder and file items
    // Note: this data is useful with regards to the FileOrganiserWindowMimeType
    //       MIME type on which the file organiser widget relies for moving
    //       folder and file items around

    res->setData(FileOrganiserWindowMimeType, encodeData(pIndexes));

    return res;
}

//==============================================================================

QString FileOrganiserWindowModel::filePath(const QModelIndex &pFileIndex) const
{
    // Return the file path of pFileIndex, if it exists and corresponds to a
    // file

    auto fileItem = static_cast<FileOrganiserWindowItem *>(itemFromIndex(pFileIndex));

    if ((fileItem != nullptr) && !fileItem->isFolder()) {
        return fileItem->path();
    }

    return {};
}

//==============================================================================

FileOrganiserWindowWidget::FileOrganiserWindowWidget(QWidget *pParent) :
    TreeViewWidget(pParent)
{
    // Create our 'local' file manager (as opposed to the 'global' file manager
    // that comes with the FileManager class)

    mFileManager = new Core::FileManager();

    // Customise ourselves

    mModel = new FileOrganiserWindowModel(this);

    setDragDropMode(QAbstractItemView::DragDrop);
    setEditTriggers(QAbstractItemView::EditKeyPressed);
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    setFrameShape(QFrame::StyledPanel);
#else
    setFrameShape(QFrame::Panel);
#endif
    setHeaderHidden(true);
    setModel(mModel);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    // Some connections

    connect(this, &FileOrganiserWindowWidget::expanded,
            this, &FileOrganiserWindowWidget::resizeToContents);
    connect(this, &FileOrganiserWindowWidget::collapsed,
            this, &FileOrganiserWindowWidget::resizeToContents);

    // A connection to handle the change of selection

    connect(selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &FileOrganiserWindowWidget::emitItemsRelatedSignals);

    // Some connections to handle our file manager

    connect(mFileManager, &Core::FileManager::fileDeleted,
            this, &FileOrganiserWindowWidget::fileDeleted);
}

//==============================================================================

FileOrganiserWindowWidget::~FileOrganiserWindowWidget()
{
    // Delete some internal objects

    delete mFileManager;
}

//==============================================================================

static const char *SettingsModel        = "Model";
static const char *SettingsSelectedItem = "SelectedItem";

//==============================================================================

void FileOrganiserWindowWidget::loadItemSettings(QSettings &pSettings,
                                                 QStandardItem *pParentItem)
{
    // Recursively retrieve the item settings

    static int crtItemIndex = -1;
    QStringList itemInfo = pSettings.value(QString::number(++crtItemIndex)).toStringList();

    if (!itemInfo.isEmpty()) {
        int childItemsCount = itemInfo[2].toInt();

        // Create the item, in case we are not dealing with the root folder item

        QStandardItem *childParentItem = nullptr;

        if (pParentItem != nullptr) {
            // This is not the root folder item, so we can create the item which
            // is either a folder or a file, depending on its number of child
            // items

            QString textOrPath = itemInfo[0];

            if (childItemsCount >= 0) {
                // We are dealing with a folder item

                auto folderItem = new FileOrganiserWindowItem(QFileIconProvider().icon(QFileIconProvider::Folder),
                                                              textOrPath, true);

                pParentItem->appendRow(folderItem);

                // Expand the folder item, if necessary

                if (itemInfo[3].toInt() != 0) {
                    setExpanded(folderItem->index(), true);
                }

                // The folder item is to be the parent of any of its child item

                childParentItem = folderItem;
            } else {
                // We are dealing with a file item

                if (QFileInfo::exists(textOrPath)) {
                    pParentItem->appendRow(new FileOrganiserWindowItem(QFileIconProvider().icon(QFileIconProvider::File),
                                                                       textOrPath));

                    // Add the file to our file manager
                    // Note: it doesn't matter whether or not the file is
                    //       already being monitored, since if that's the case
                    //       then the current instance will be ignored

                    mFileManager->manage(textOrPath);
                }
            }
        } else {
            // We are dealing with the root folder item, so don't do anything
            // except for keeping track of it for when retrieving its child
            // items, if any

            childParentItem = mModel->invisibleRootItem();
        }

        // Retrieve any child item
        // Note: the test on childParentItem is not necessary (since
        //       childItemsCount will be equal to -1 in the case of a file
        //       item), but it doesn't harm having it...

        if (childParentItem != nullptr) {
            for (int i = 0; i < childItemsCount; ++i) {
                loadItemSettings(pSettings, childParentItem);
            }
        }
    }
}

//==============================================================================

void FileOrganiserWindowWidget::loadSettings(QSettings &pSettings)
{
    // Let the user know of a few default things about ourselves by emitting a
    // few signals

    emitItemsRelatedSignals();

    // Retrieve the data model

    pSettings.beginGroup(SettingsModel);
        loadItemSettings(pSettings);
    pSettings.endGroup();

    // Retrieve the currently selected item, if any

    QByteArray hierarchyData = pSettings.value(SettingsSelectedItem).toByteArray();

    setCurrentIndex(mModel->decodeHierarchyData(hierarchyData));

    // Resize the widget, just to be on the safe side

    resizeToContents();
}

//==============================================================================

void FileOrganiserWindowWidget::saveItemSettings(QSettings &pSettings,
                                                 QStandardItem *pItem,
                                                 int pParentItemIndex) const
{
    // Recursively keep track of the item settings

    static int crtItemIndex = -1;
    QStringList itemInfo;

    // The item information consists of:
    //  - The name of the folder item or the path of the file item
    //  - The index of its parent
    //  - The number of child items the (folder) item has, if any
    //  - Whether the (folder) items is expanded

    auto item = static_cast<FileOrganiserWindowItem *>(pItem);

    if ((item == mModel->invisibleRootItem()) || item->isFolder()) {
        // We are dealing with a folder item (be it the root folder item or not)

        itemInfo << item->text() << QString::number(pParentItemIndex)
                 << QString::number(item->rowCount())
                 << QString(isExpanded(item->index())?"1":"0");
    } else {
        // We are dealing with a file item

        QString fileName = item->path();

        itemInfo << fileName
                 << QString::number(pParentItemIndex) << "-1" << "0";

        // Remove the file from our file manager
        // Note: it doesn't matter whether or not the file has already been
        //       removed, since if that's the case then nothing will be done

        mFileManager->unmanage(fileName);
    }

    pSettings.setValue(QString::number(++crtItemIndex), itemInfo);

    // Keep track of any child item

    int childParentItemIndex = crtItemIndex;

    for (int i = 0, iMax = item->rowCount(); i < iMax; ++i) {
        saveItemSettings(pSettings, item->child(i), childParentItemIndex);
    }
}

//==============================================================================

void FileOrganiserWindowWidget::saveSettings(QSettings &pSettings) const
{
    // Keep track of the data model
    // Note: we first clear all previous settings, so that we are 100% sure that
    //       we don't have any 'dead' items left (e.g. if we used to have three
    //       items and we now have two of them, then not to clear everything
    //       first would mean that the 'old' third item would be back the next
    //       time we start OpenCOR)...

    pSettings.remove(SettingsModel);

    pSettings.beginGroup(SettingsModel);
        saveItemSettings(pSettings, mModel->invisibleRootItem(), -1);
    pSettings.endGroup();

    // Keep track of the currently selected item, but only if it is visible

    bool crtItemVisible = true;
    QModelIndex crtIndexParent = currentIndex().parent();

    while (crtIndexParent.isValid()) {
        if (isExpanded(crtIndexParent)) {
            // The current parent is expanded, so check to its parent

            crtIndexParent = crtIndexParent.parent();
        } else {
            crtItemVisible = false;

            break;
        }
    }

    pSettings.setValue(SettingsSelectedItem, mModel->encodeHierarchyData(crtItemVisible?currentIndex():QModelIndex()));
}

//==============================================================================

void FileOrganiserWindowWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if we are dropping
    // URIs

    if (   pEvent->mimeData()->hasFormat(Core::FileSystemMimeType)
        || pEvent->mimeData()->hasFormat(FileOrganiserWindowMimeType)) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void FileOrganiserWindowWidget::dragMoveEvent(QDragMoveEvent *pEvent)
{
    // Set the state to dragging, since we can only come here when dragging
    // Note: the state is properly set for file organiser objects being dragged,
    //       but should we be dragging external objects over our file organiser
    //       widget, then the state will (obviously) not be set. This wouldn't
    //       be a problem in itself if it was for the fact that this prevents
    //       the drop indicator from being painted...

    setState(QAbstractItemView::DraggingState);

    // Default handling of the event
    // Note: this gives us the drop indicator...

    TreeViewWidget::dragMoveEvent(pEvent);

    // Accept the proposed action for the event, but only if there are objects
    // to drop and if we are not trying to drop the objects above/on/below one
    // of them (should the objects come from the file organiser widget) or on a
    // file item
    // Note #1: for the number of objects being dropped, we have to check the
    //          number of URLs information (i.e. external objects), as well as
    //          the MIME data associated with FileOrganiserWindowMimeType (i.e.
    //          objects from the file organiser widget, after we have )
    // Note #2: for the dropping location, it can be either a folder or a file
    //          (as long as the indicator position isn't on the item itself),
    //          but not above/on/below any of the objects (or any of their
    //          children) being dragged (only relevant when dragging items from
    //          the file organiser widget)

    QByteArray data = pEvent->mimeData()->data(FileOrganiserWindowMimeType);
    QModelIndexList indexes = mModel->decodeData(data);
    FileOrganiserWindowItem *dropItem = static_cast<FileOrganiserWindowItem *>(mModel->itemFromIndex(indexAt(pEvent->pos())));
    bool draggingOnSelfOrChild = false;

    if (dropItem != nullptr) {
        for (int i = 0; (i < indexes.count()) && !draggingOnSelfOrChild; ++i) {
            draggingOnSelfOrChild = itemIsOrIsChildOf(dropItem,
                                                      mModel->itemFromIndex(indexes[i]));
        }
    }

    if (   (!pEvent->mimeData()->urls().isEmpty() || !indexes.isEmpty())
        && (   ((dropItem != nullptr) && dropItem->isFolder())
            || (dropIndicatorPosition() != QAbstractItemView::OnItem))
        && !draggingOnSelfOrChild) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

void FileOrganiserWindowWidget::dropEvent(QDropEvent *pEvent)
{
    // Note: the MIME data definitely contains the FileSystemMimeType MIME type
    //       (for objects originating from outside this widget), but it may also
    //       contain the FileOrganiserWindowMimeType MIME type (for objects
    //       originating from within this widget). FileOrganiserWindowMimeType
    //       is used by this widget while FileSystemMimeType by external
    //       widgets. So, this means that we must check for
    //       FileOrganiserWindowMimeType first

    // Files have been dropped, so add them to the widget and this at the right
    // place (i.e. above/on/below a folder, above/below a file or on the
    // invisible root folder)

    // First, determine the item above/on/below which objects are to be dropped,
    // as well as the drop position (i.e. above, on or below)

    QStandardItem *dropItem;
    DropIndicatorPosition dropPosition = dropIndicatorPosition();

    if (dropPosition == QAbstractItemView::OnViewport) {
        // We dropped the files on the viewport

        dropItem = mModel->invisibleRootItem();

        // Change the drop position since we know that we want want the objects
        // to be dropped on the root folder

        dropPosition = QAbstractItemView::OnItem;
    } else {
        // We dropped the files above/on/below a folder or above/below a file

        dropItem = mModel->itemFromIndex(indexAt(pEvent->pos()));
    }

    // Check the type of MIME data to be dropped

    if (pEvent->mimeData()->hasFormat(FileOrganiserWindowMimeType)) {
        // The user is dropping folders/files from ourselves, i.e. s/he wants
        // some folders/files to be moved around

        // Retrieve the list of indexes to move around and clean it

        QByteArray data = pEvent->mimeData()->data(FileOrganiserWindowMimeType);
        QModelIndexList indexes = cleanIndexList(mModel->decodeData(data));

        // Convert our list of indexes to a list of items
        // Note: indeed, by moving the item corresponding to a particular index,
        //       we may mess up the other indexes, meaning that we may not be
        //       able to retrieve their corresponding item...

        QList<QStandardItem *> items;

        for (int i = 0, iMax = indexes.count(); i < iMax; ++i) {
            items << mModel->itemFromIndex(indexes[i]);
        }

        // Move the contents of the list to its final destination

        if (dropPosition != QAbstractItemView::BelowItem) {
            // Move the items in the order they were dropped

            for (int i = 0, iMax = items.count(); i < iMax; ++i) {
                moveItem(items[i], dropItem, dropPosition);
            }
        } else {
            // Move the items in a reverse order to that they were dropped since
            // we want them moved below the current item

            for (int i = items.count()-1; i >= 0; --i) {
                moveItem(items[i], dropItem, dropPosition);
            }
        }
    } else {
        // The user wants to drop files, so add them to the widget and this at
        // the right place

        QList<QUrl> urls = pEvent->mimeData()->urls();

        if (dropPosition != QAbstractItemView::BelowItem) {
            // Add the files in the order they were dropped

            for (int i = 0, iMax = urls.count(); i < iMax; ++i) {
                addFile(urls[i].toLocalFile(), dropItem, dropPosition);
            }
        } else {
            // Add the files in a reverse order to that they were dropped since
            // we want them added below the current item

            for (int i = urls.count()-1; i >= 0; --i) {
                addFile(urls[i].toLocalFile(), dropItem, dropPosition);
            }
        }
    }

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();

    // Reset the state of the widget
    // Note: there doesn't seem to be and there shouldn't be a need to reset the
    //       state after the widget (the resetting seems to be done elsewhere),
    //       but then if we don't reset the state of the widget, then the drop
    //       indicator may, in some cases, remain visible (a bug in Qt?)...

    setState(QAbstractItemView::NoState);
}

//==============================================================================

void FileOrganiserWindowWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Default handling of the event

    TreeViewWidget::keyPressEvent(pEvent);

    // Retrieve all the files that are currently selected
    // Note: if there is a folder among the selected items, then ignore all of
    //       them...

    QStringList fileNames = QStringList();
    QModelIndexList crtSelectedIndexes = selectedIndexes();

    for (int i = 0, iMax = crtSelectedIndexes.count(); i < iMax; ++i) {
        QString fileName = filePath(crtSelectedIndexes[i]);

        if (fileName.isEmpty()) {
            fileNames = QStringList();

            break;
        }

        fileNames << fileName;
    }

    // Let people know about a key having been pressed with the view of opening
    // one or several files

    if (   !fileNames.isEmpty()
        &&  ((pEvent->key() == Qt::Key_Enter) || (pEvent->key() == Qt::Key_Return))) {
        // There are some files that are selected and we want to open them, so
        // let people know about it

        emit openFilesRequested(fileNames);
    }
}

//==============================================================================

bool FileOrganiserWindowWidget::parentIndexExists(const QModelIndex &pIndex,
                                                  const QModelIndexList &pIndexes) const
{
    // Recursively determine whether one of the parents of the given index is in
    // the provided list

    QModelIndex parentIndex = pIndex.parent();

    if (parentIndex.isValid()) {
        // The current index has a valid parent, so check whether the parent is
        // in the list

        if (pIndexes.indexOf(parentIndex) != -1) {
            return true;
        }

        // The parent index couldn't be found, but what about the parent index's
        // parent?

        return parentIndexExists(parentIndex, pIndexes);
    }

    return false;
}

//==============================================================================

QModelIndexList FileOrganiserWindowWidget::cleanIndexList(const QModelIndexList &pIndexes) const
{
    // A list of indexes may contain indexes that are not relevant or
    // effectively the duplicate of another existing index

    QModelIndexList res;

    // If both the index of a folder and some (if not all) of its (in)direct
    // contents is in the original list, then we only keep track of the index of
    // the folder

    for (int i = 0, iMax = pIndexes.count(); i < iMax; ++i) {
        // Check whether one of the current index's parents is already in the
        // list. If so, then skip the current index

        QModelIndex crtIndex = pIndexes[i];

        if (!parentIndexExists(crtIndex, pIndexes)) {
            // None of the index's parents is in the list, so we can safely add
            // the index to the list

            res << crtIndex;

            // If the index refers to a folder, then we must double check that
            // the list of cleaned indexes doesn't contain any of the index's
            // children. If it does, then we must remove all of them

            auto crtItem = static_cast<FileOrganiserWindowItem *>(mModel->itemFromIndex(crtIndex));

            if ((crtItem != nullptr) && crtItem->isFolder()) {
                for (int j = res.count()-1; j >= 0; --j) {
                    if (parentIndexExists(res[j], res)) {
                        res.removeAt(j);
                    }
                }
            }
        }
    }

    // At this stage, we have indexes for folders that are unique, but we may
    // still have indexes for files that are effectively the duplicate of
    // another file, so these are to be removed from the cleaned list and from
    // the model

    for (int i = res.count()-1; i >= 0; --i) {
        auto crtItem = static_cast<FileOrganiserWindowItem *>(mModel->itemFromIndex(res[i]));

        if ((crtItem != nullptr) && !crtItem->isFolder()) {
            // The index corresponds to a valid file item, so check whether in
            // the cleaned list there is another file item referencing the same
            // physical file and, if so, remove it from the cleaned list and the
            // model

            for (int j = 0; j < i; ++j) {
                auto testItem = static_cast<FileOrganiserWindowItem *>(mModel->itemFromIndex(res[j]));

                if (    (testItem != nullptr)
                    && !testItem->isFolder()
                    &&  (crtItem->path() == testItem->path())) {
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
    }

    return res;
}

//==============================================================================

bool FileOrganiserWindowWidget::itemIsOrIsChildOf(QStandardItem *pItem,
                                                  QStandardItem *pOtherItem) const
{
    // Check whether the given items are the same or whether one of them is the
    // (in)direct child of the other

    if (pItem == pOtherItem) {
        return true;
    }

    if (pOtherItem->hasChildren()) {
        // pOtherItem has children, so check against them

        for (int i = 0, iMax = pOtherItem->rowCount(); i < iMax; ++i) {
            if (itemIsOrIsChildOf(pItem, pOtherItem->child(i))) {
                return true;
            }
        }

        return false;
    }

    return false;
}

//==============================================================================

void FileOrganiserWindowWidget::backupExpandedInformation(QStandardItem *pItem) const
{
    // Recursively backup the expanded state of the item, should it be a folder,
    // and of any of its children, should it have some

    auto item = static_cast<FileOrganiserWindowItem *>(pItem);

    if (item->isFolder()) {
        // Keep track of the expanded state of pItem

        item->setExpanded(isExpanded(mModel->indexFromItem(item)));

        // Do the same with all of pItem's children, if any

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i) {
            backupExpandedInformation(item->child(i));
        }
    }
}

//==============================================================================

void FileOrganiserWindowWidget::restoreExpandedInformation(QStandardItem *pItem)
{
    // Recursively restore the expanded state of the item, should it be a
    // folder, and of any of its children, should it have some

    auto item = static_cast<FileOrganiserWindowItem *>(pItem);

    if (item->isFolder()) {
        // Retrieve the expanded state of pItem

        setExpanded(mModel->indexFromItem(item), item->isExpanded());

        // Do the same with all of pItem's children, if any

        for (int i = 0, iMax = item->rowCount(); i < iMax; ++i) {
            restoreExpandedInformation(item->child(i));
        }
    }
}

//==============================================================================

QString FileOrganiserWindowWidget::newFolderName(QStandardItem *pFolderItem) const
{
    // Come up with the name for a new folder which is to be under pFolderItem

    // Retrieve the name of the folders under pFolderItem

    QStringList subFolderNames;

    for (int i = 0, iMax = pFolderItem->rowCount(); i < iMax; ++i) {
        subFolderNames << pFolderItem->child(i)->text();
    }

    // Compare the suggested name of our new folder with that of the folders
    // under pFolderItem, this until we come up with a suggested name which is
    // not already taken

    QString baseFolderName = tr("New Folder");
    QString templateFolderName = baseFolderName+" (%1)";

    int folderNb = 1;
    QString res = baseFolderName;

    while (subFolderNames.contains(res)) {
        res = templateFolderName.arg(++folderNb);
    }

    return res;
}

//==============================================================================

void FileOrganiserWindowWidget::newFolder()
{
    // Make sure that the conditions are met to create a new folder

    if (!canCreateNewFolder()) {
        return;
    }

    // Either create a folder item below the current folder item or below the
    // root item, depending on the situation

    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();
    int selectedIndexesCount = selectedIndexes.count();
    QStandardItem *crtItem = (selectedIndexesCount != 0)?
                                 mModel->itemFromIndex(selectedIndexes.first()):
                                 mModel->invisibleRootItem();
    auto newFolderItem = new FileOrganiserWindowItem(QFileIconProvider().icon(QFileIconProvider::Folder),
                                                     newFolderName(crtItem), true);

    crtItem->appendRow(newFolderItem);

    // Some post-processing, but only if no other item is currently being edited

    if (!isEditing()) {
        // Expand the current index (so that we can see the new folder)
        // Note: this is only relevant in the case of a folder item being
        //       currently selected (i.e. it's not the root folder item)

        if (selectedIndexesCount != 0) {
            setExpanded(crtItem->index(), true);
        }

        // Offer the user to edit the newly added folder item

        edit(newFolderItem->index());
    }

    // Resize the widget, just to be on the safe side

    resizeToContents();
}

//==============================================================================

QStandardItem * FileOrganiserWindowWidget::parentItem(QStandardItem *pDropItem,
                                                      const QAbstractItemView::DropIndicatorPosition &pDropPosition)
{
    // Return who should be the parent item of pDropItem based on pDropPosition

    return (pDropPosition == QAbstractItemView::OnItem)?
                pDropItem:
                (pDropItem->parent() != nullptr)?
                    pDropItem->parent():
                    mModel->invisibleRootItem();
}

//==============================================================================

bool FileOrganiserWindowWidget::ownedBy(const QString &pFileName,
                                        QStandardItem *pItem)
{
    // Check whether pFileName is already owned by pItem

    for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
        // Check whether the current item is a file with pFileName as its name

        auto crtItem = static_cast<FileOrganiserWindowItem *>(pItem->child(i));

        if (!crtItem->isFolder() && (crtItem->path() == pFileName)) {
            return true;
        }
    }

    return false;
}

//==============================================================================

void FileOrganiserWindowWidget::dropItems(QStandardItem *pDropItem,
                                          const QAbstractItemView::DropIndicatorPosition &pDropPosition,
                                          QStandardItem *pNewParentItem,
                                          const QList<QStandardItem *> &pItems)
{
    // Drop pItems based on pDropPosition's value

    switch (pDropPosition) {
    case QAbstractItemView::OnItem:
    case QAbstractItemView::OnViewport:
        pNewParentItem->appendRow(pItems);

        // Expand pNewParentItem, so the user knows that the item has been moved
        // to it (assuming that pNewParentItem was collapsed)

        setExpanded(pNewParentItem->index(), true);

        break;
    case QAbstractItemView::AboveItem:
        pNewParentItem->insertRow(pDropItem->row(), pItems);

        break;
    case QAbstractItemView::BelowItem:
        pNewParentItem->insertRow(pDropItem->row()+1, pItems);

        break;
    }
}

//==============================================================================

void FileOrganiserWindowWidget::dropItem(QStandardItem *pDropItem,
                                         const QAbstractItemView::DropIndicatorPosition &pDropPosition,
                                         QStandardItem *pNewParentItem,
                                         QStandardItem *pItem)
{
    // Drop the item as if we wanted to drop a list of items

    dropItems(pDropItem, pDropPosition, pNewParentItem,
              QList<QStandardItem *>() << pItem);
}

//==============================================================================

void FileOrganiserWindowWidget::addFile(const QString &pFileName,
                                        QStandardItem *pDropItem,
                                        const QAbstractItemView::DropIndicatorPosition &pDropPosition)
{
    // Make sure that we have a drop item

    if (pDropItem == nullptr) {
        return;
    }

    // Make sure that we are indeed dealing with a file

    QFileInfo fileInfo = pFileName;

    if (!fileInfo.isFile()) {
        return;
    }

    // Check whether we are dropping a symbolic link

    QString fileName = pFileName;

    if (fileInfo.isSymLink()) {
        // We are dropping a symbolic link, so retrieve its target

        fileName = fileInfo.symLinkTarget();
    }

    // Check whether the file exists

    if (QFile::exists(fileName)) {
        // The target file exists, so add it above/on/below pDropItem, depending
        // on the drop position and only if the file isn't already present

        // First, determine the item that will own the file

        QStandardItem *newParentItem = parentItem(pDropItem, pDropPosition);

        // Second, if the file is not already owned, then add it to
        // newParentItem and this to the right place, depending on the value of
        // pDropPosition

        if (!ownedBy(fileName, newParentItem)) {
            auto newFileItem = new FileOrganiserWindowItem(QFileIconProvider().icon(QFileIconProvider::File),
                                                           fileName);

            dropItem(pDropItem, pDropPosition, newParentItem, newFileItem);

            // Add the file to our file manager
            // Note: it doesn't matter whether or not the file is already being
            //       monitored, since if that's the case then the current
            //       instance will be ignored

            mFileManager->manage(fileName);

            // Resize the widget, just in case the new file takes more space
            // than is visible

            resizeToContents();
        }
    }
}

//==============================================================================

void FileOrganiserWindowWidget::moveItem(QStandardItem *pItem,
                                         QStandardItem *pDropItem,
                                         const QAbstractItemView::DropIndicatorPosition &pDropPosition)
{
    // Make sure that we have a drop item

    if (pDropItem == nullptr) {
        return;
    }

    // Move pItem above/on/below pDropItem, depending on the drop position, but
    // first, determine the item that will own pItem

    auto item = static_cast<FileOrganiserWindowItem *>(pItem);
    QStandardItem *crtParentItem = (item->parent() != nullptr)?
                                       item->parent():
                                       mModel->invisibleRootItem();
    QStandardItem *newParentItem = parentItem(pDropItem, pDropPosition);

    // Second, check whether the (file) item points to a file which is already
    // owned by newParentItem

    bool fileAlreadyOwned = false;

    if (!item->isFolder()) {
        // The current item is a file item, so retrieve its file name and check
        // whether it's already owned by newParentItem

        fileAlreadyOwned = ownedBy(item->path(), newParentItem);
    }

    // Third, move pItem to newParentItem and this to the right place, depending
    // on the value of pDropPosition and only if the destination doesn't already
    // have that item (should it be a file item, since folder items are always
    // moved)

    if (!fileAlreadyOwned || (crtParentItem == newParentItem)) {
        // Either newParentItem doesn't already own an item which points to the
        // same file as pItem or pItem's current parent is the same as
        // newParentItem in which case it means that we want to move the item
        // within its current location

        // First, check whether the item is a folder and, if so, whether it's
        // expanded (and the same with any (in)direct child folder it may
        // contain)

        backupExpandedInformation(item);

        // Second, move the item (and any of its children)

        dropItems(pDropItem, pDropPosition, newParentItem,
                  crtParentItem->takeRow(item->row()));

        // Third, re-expand folders, if necessary

        restoreExpandedInformation(item);

        // Fourth, resize the widget, just in case the new location of the
        // item(s) requires more space than is visible

        resizeToContents();
    } else {
        // A (file) item pointing to the same file is already owned by
        // newParentItem, so just remove the item rather than move it

        crtParentItem->removeRow(item->row());
    }
}

//==============================================================================

void FileOrganiserWindowWidget::collapseEmptyFolders(QStandardItem *pFolder)
{
    // Recursively collapse any empty child folder

    for (int i = 0, iMax = pFolder->rowCount(); i < iMax; ++i) {
        auto folderItem = static_cast<FileOrganiserWindowItem *>(pFolder->child(i));

        if (folderItem->isFolder()) {
            collapseEmptyFolders(folderItem);
        }
    }

    // Collapse the current folder, if necessary and if it isn't the root folder

    if ((pFolder != mModel->invisibleRootItem()) && (pFolder->rowCount() == 0)) {
        collapse(mModel->indexFromItem(pFolder));
    }
}

//==============================================================================

void FileOrganiserWindowWidget::deleteItems()
{
    // Check whether whether there are selected items to delete

    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();

    if (selectedIndexes.isEmpty()) {
        return;
    }

    // Delete the selected items one by one, making sure that we update our list
    // of items every time (this is because the row value of the remaining
    // selected items may become different after deleting an item)

    while (!selectedIndexes.isEmpty()) {
        // Remove the file from our file manager, should the index refer to a
        // file item
        // Note: it doesn't matter whether or not the file has already been
        //       removed, since if that's the case then nothing will be done...

        QModelIndex crtIndex = selectedIndexes.first();
        auto crtItem = static_cast<FileOrganiserWindowItem *>(mModel->itemFromIndex(crtIndex));

        if ((crtItem != nullptr) && !crtItem->isFolder()) {
            mFileManager->unmanage(crtItem->path());
        }

        // Remove the item from the model itself

        mModel->removeRow(crtIndex.row(), crtIndex.parent());

        // Update our list of selected indexes

        selectedIndexes = selectionModel()->selectedIndexes();
    }

    // Collapse any folder that doesn't contain any file/folder anymore

    collapseEmptyFolders(mModel->invisibleRootItem());

    // Resize the widget to its contents in case its width was too wide (and
    // therefore required a horizontal scroll bar), but is now fine

    resizeToContents();
}

//==============================================================================

QString FileOrganiserWindowWidget::filePath(const QModelIndex &pFileIndex) const
{
    // Return the file path of pFileIndex

    return mModel->filePath(pFileIndex);
}

//==============================================================================

void FileOrganiserWindowWidget::resizeToContents()
{
    // Make sure that the first column allows for all of its contents to be
    // visible

    resizeColumnToContents(0);
}

//==============================================================================

bool FileOrganiserWindowWidget::canCreateNewFolder() const
{
    QModelIndexList selectedIndexes = selectionModel()->selectedIndexes();
    int selectedIndexesCount = selectedIndexes.count();

    // Determnine whether we can create a new folder

    if (selectedIndexesCount == 1) {
        // One item is currently selected, so the only way we could create a new
        // folder is if the current item is also a folder

        return static_cast<FileOrganiserWindowItem *>(mModel->itemFromIndex(selectedIndexes.first()))->isFolder();
    }

    // Either no item or several items are currently selected, so the only way
    // we could create a new folder is if no item is currently selected

    return selectedIndexesCount == 0;
}

//==============================================================================

void FileOrganiserWindowWidget::emitItemsRelatedSignals()
{
    // Let the user know whether a new folder could be created and whether items
    // could be deleted

    emit newFolderEnabled(canCreateNewFolder());
    emit deleteItemsEnabled(selectionModel()->selectedIndexes().count() >= 1);
}

//==============================================================================

void FileOrganiserWindowWidget::selectFileItem(QStandardItem *pItem,
                                               const QString &pFileName) const
{
    // Recursively delete the file items that refer to pFileName

    auto item = static_cast<FileOrganiserWindowItem *>(pItem);

    if (!item->isFolder() && (item->path() == pFileName)) {
        // The current item is a file item and it refers to pFileName, so delete
        // it

        selectionModel()->select(pItem->index(), QItemSelectionModel::Select);
    } else {
        // Update our child file items, if any

        for (int i = 0, iMax = pItem->rowCount(); i < iMax; ++i) {
            selectFileItem(pItem->child(i), pFileName);
        }
    }
}

//==============================================================================

void FileOrganiserWindowWidget::fileDeleted(const QString &pFileName)
{
    // A file has been deleted, so select all the (file) items that refer to the
    // given file and delete them

    selectFileItem(mModel->invisibleRootItem(), pFileName);

    deleteItems();
}

//==============================================================================

} // namespace FileOrganiserWindow
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
