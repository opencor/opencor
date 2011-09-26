#include "apiinterface.h"

namespace OpenCOR {

FileType::FileType(ApiInterface *pOwner, const QString &pMimeType,
                   const QString &pFileExtension) :
    mOwner(pOwner),
    mMimeType(pMimeType),
    mFileExtension(pFileExtension)
{
}

QString FileType::mimeType() const
{
    // Return the file's Mime type

    return mMimeType;
}

QString FileType::fileExtension() const
{
    // Return the file's extension

    return mFileExtension;
}

QString FileType::description() const
{
    // Return the file's description

    return mOwner->fileTypeDescription(mMimeType);
}

QList<FileType> ApiInterface::fileTypes() const
{
    // By default, there are no file types

    return QList<FileType>();
}

QString ApiInterface::fileTypeDescription(const QString &) const
{
    // By default, there is no description for the requested Mime type

    return QString();
}

}
