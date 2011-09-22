#include "apiinterface.h"

namespace OpenCOR {

FileType::FileType(const QString &pMimeType, const QString &pFileExtension,
                   const Descriptions &pDescriptions) :
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

QString FileType::description(const QString &pLocale) const
{
    // Return the file type's description using the provided locale or the first
    // description if no description can be found for the provided locale

    return OpenCOR::description(mDescriptions, pLocale);
}

QList<FileType> ApiInterface::fileTypes() const
{
    // By default, there are no file types

    return QList<FileType>();
}

}
