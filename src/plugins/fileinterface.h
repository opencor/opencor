//==============================================================================
// File interface
//==============================================================================

#ifndef FILEINTERFACE_H
#define FILEINTERFACE_H

//==============================================================================

#include "interface.h"

//==============================================================================

#include <QtPlugin>

//==============================================================================

#include <QMap>
#include <QStringList>

//==============================================================================

namespace OpenCOR {

//==============================================================================

class FileInterface;

//==============================================================================

class FileType
{
public:
    explicit FileType(FileInterface *pOwner, const QString &pMimeType,
                      const QString &pFileExtension);

    QString mimeType() const;
    QString fileExtension() const;
    QString description() const;

private:
    FileInterface *mOwner;
    QString mMimeType;
    QString mFileExtension;
};

//==============================================================================

class FileInterface : Interface
{
    friend class MainWindow;

public:
    virtual QList<FileType> fileTypes() const;
    virtual QString fileTypeDescription(const QString &pMimeType) const;
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::FileInterface, "OpenCOR.FileInterface")

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
