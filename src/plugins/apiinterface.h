#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include "plugininterface.h"

#include <QMap>
#include <QStringList>

namespace OpenCOR {

class FileType
{
public:
    explicit FileType(const QString &pMimeType, const QString &pFileExtension,
                      const Descriptions &pDescriptions);

    QString mimeType() const;
    QString fileExtension() const;
    QString description(const QString &pLocale = EnglishLocale) const;

private:
    QString mMimeType;
    QString mFileExtension;
    Descriptions mDescriptions;
};

class ApiInterface : public PluginInterface
{
public:
    virtual QList<FileType> fileTypes() const;
};

}

Q_DECLARE_INTERFACE(OpenCOR::ApiInterface, "OpenCOR.ApiInterface")

#endif
