#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include <QtPlugin>

#include <QMap>
#include <QStringList>

namespace OpenCOR {

class ApiInterface;

class FileType
{
public:
    explicit FileType(ApiInterface *pOwner, const QString &pMimeType,
                      const QString &pFileExtension);

    QString mimeType() const;
    QString fileExtension() const;
    QString description() const;

private:
    ApiInterface *mOwner;
    QString mMimeType;
    QString mFileExtension;
};

class ApiInterface
{
public:
    virtual QList<FileType> fileTypes() const;
    virtual QString fileTypeDescription(const QString &) const;
};

}

Q_DECLARE_INTERFACE(OpenCOR::ApiInterface, "OpenCOR.ApiInterface")

#endif
