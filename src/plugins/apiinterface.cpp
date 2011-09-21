#include "apiinterface.h"

namespace OpenCOR {

FileType::FileType(const QString &pMimeType, const QString &pFileExtension,
                   const FileDescriptions &pDescriptions) :
    mMimeType(pMimeType),
    mFileExtension(pFileExtension),
    mDescriptions(pDescriptions)
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

FileDescriptions FileType::descriptions() const
{
    // Return the file's descriptions

    return mDescriptions;
}

QList<FileType> ApiInterface::fileTypes() const
{
    // By default, there are no file types

    return QList<FileType>();
}

}
