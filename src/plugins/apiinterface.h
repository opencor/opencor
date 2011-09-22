#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include "plugininterface.h"

#include <QMap>
#include <QStringList>

namespace OpenCOR {

typedef QMap<QString, QString> FileDescriptions;

class FileType
{
public:
    explicit FileType(const QString &pMimeType, const QString &pFileExtension,
                      const FileDescriptions &pDescriptions);

    QString mimeType() const;
    QString fileExtension() const;
    QString description(const QString &pLocale = "en") const;

private:
    QString mMimeType;
    QString mFileExtension;
    FileDescriptions mDescriptions;
};

class ApiInterface : public PluginInterface
{
public:
    virtual QList<FileType> fileTypes() const;
};

}

Q_DECLARE_INTERFACE(OpenCOR::ApiInterface, "OpenCOR.ApiInterface")

#endif
