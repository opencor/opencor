//==============================================================================
// File interface
//==============================================================================

#include "fileinterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

FileType::FileType(FileInterface *pOwner, const QString &pMimeType,
                   const QString &pFileExtension) :
    mOwner(pOwner),
    mMimeType(pMimeType),
    mFileExtension(pFileExtension)
{
}

//==============================================================================

bool FileType::operator==(const FileType &pFileType) const
{
    // Return whether the current item is equal to the given one

    return    !mMimeType.compare(pFileType.mimeType())
           && !mFileExtension.compare(pFileType.fileExtension());
}

//==============================================================================

QString FileType::mimeType() const
{
    // Return the file's MIME type

    return mMimeType;
}

//==============================================================================

QString FileType::fileExtension() const
{
    // Return the file's extension

    return mFileExtension;
}

//==============================================================================

QString FileType::description() const
{
    // Return the file's description

    return mOwner->fileTypeDescription(mMimeType);
}

//==============================================================================

FileTypes FileInterface::fileTypes() const
{
    // By default, there are no file types

    return FileTypes();
}

//==============================================================================

QString FileInterface::fileTypeDescription(const QString &pMimeType) const
{
    // By default, there is no description for the requested MIME type

    Q_UNUSED(pMimeType);

    return QString();
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
