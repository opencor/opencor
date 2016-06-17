/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// File Organiser window model
//==============================================================================

#pragma once

//==============================================================================

#include <Qt>

//==============================================================================

#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiserWindow {

//==============================================================================

namespace Item {
    enum {
        Folder   = Qt::UserRole,
        Path     = Qt::UserRole+1,
        Expanded = Qt::UserRole+2
    };
}

//==============================================================================

static const auto FileOrganiserWindowMimeType = QStringLiteral("opencor/file-organiser-window");

//==============================================================================

class FileOrganiserWindowModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit FileOrganiserWindowModel(QObject *pParent);

    virtual QStringList mimeTypes() const;
    virtual QMimeData * mimeData(const QModelIndexList &pIndexes) const;

    QByteArray encodeHierarchyData(const QModelIndex &pIndex) const;

    QModelIndex decodeHierarchyData(QByteArray &pData) const;
    QModelIndexList decodeData(QByteArray &pData) const;

    QString filePath(const QModelIndex &pFileIndex) const;

private:
    void encodeHierarchyData(const QModelIndex &pIndex, QDataStream &pStream,
                             const int &pLevel = 0) const;
    QByteArray encodeData(const QModelIndexList &pIndexes) const;

    QModelIndex decodeHierarchyData(QDataStream &pStream) const;
};

//==============================================================================

}   // namespace FileOrganiserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
