//==============================================================================
// File organiser model
//==============================================================================

#ifndef FILEORGANISERMODEL_H
#define FILEORGANISERMODEL_H

//==============================================================================

#include <QStandardItemModel>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiser {

//==============================================================================

namespace Item {
    static const int Folder   = Qt::UserRole;
    static const int Path     = Qt::UserRole+1;
    static const int Expanded = Qt::UserRole+2;
}

//==============================================================================

static const QString FileOrganiserMimeType = "opencor/file-organiser";

//==============================================================================

class FileOrganiserModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit FileOrganiserModel(QObject *pParent);

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

}   // namespace FileOrganiser
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
