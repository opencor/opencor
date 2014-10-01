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
// File organiser model
//==============================================================================

#include "filemanager.h"
#include "fileorganiserwindowmodel.h"

//==============================================================================

#include <QByteArray>
#include <QIODevice>
#include <QMimeData>
#include <QModelIndex>
#include <QUrl>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiserWindow {

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
                                                   const int &pLevel) const
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

    return res;
}

//==============================================================================

QMimeData * FileOrganiserWindowModel::mimeData(const QModelIndexList &pIndexes) const
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

    QStandardItem *fileItem = itemFromIndex(pFileIndex);

    if (fileItem && !fileItem->data(Item::Folder).toBool())
        return fileItem->data(Item::Path).toString();
    else
        return QString();
}

//==============================================================================

}   // namespace FileOrganiserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
