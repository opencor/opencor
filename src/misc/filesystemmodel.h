#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QFileSystemModel>

class FileSystemModel : public QFileSystemModel
{
    Q_OBJECT

public:
    explicit FileSystemModel(QObject *pParent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex &pIndex) const;

    virtual Qt::DropActions supportedDropActions() const;
};

#endif
