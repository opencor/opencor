//==============================================================================
// File organiser model
//==============================================================================

#include "filemanager.h"
#include "fileorganisermodel.h"

//==============================================================================

#include <QMimeData>
#include <QUrl>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiser {

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

}   // namespace FileOrganiser
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
