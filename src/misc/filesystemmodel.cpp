#include "filesystemmodel.h"

FileSystemModel::FileSystemModel(QObject *pParent)
    : QFileSystemModel(pParent)
{
    // We want acces to the full file system

    setRootPath("");
}

Qt::ItemFlags FileSystemModel::flags(const QModelIndex &pIndex) const
{
    if (!pIndex.isValid())
        // The index is not valid, so...

        return 0;

    // Default flags value

    Qt::ItemFlags flags = Qt::ItemIsEnabled;

    // Only files are selectable and draggable, not folders

    if (!QFileInfo(filePath(pIndex)).isDir())
        flags |= Qt::ItemIsSelectable|Qt::ItemIsDragEnabled;

    // Return the flags for the index

    return flags;
}
