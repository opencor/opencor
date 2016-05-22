/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// File type interface
//==============================================================================

#include "filetypeinterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

FileType::FileType(FileTypeInterface *pOwner, const QString &pMimeType,
                   const QString &pFileExtension) :
    mOwner(pOwner),
    mMimeType(pMimeType),
    mFileExtension(pFileExtension)
{
}

//==============================================================================

bool FileType::operator==(FileType *pFileType) const
{
    // Return whether the current item is equal to the given one

    return    !mMimeType.compare(pFileType->mimeType())
           && !mFileExtension.compare(pFileType->fileExtension());
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

FileTypeInterface::FileTypeInterface() :
    mFileTypes(FileTypes()),
    mDefaultViews(QStringList())
{
}

//==============================================================================

FileTypeInterface::~FileTypeInterface()
{
    // Delete some internal objects

    foreach (FileType *fileType, mFileTypes)
        delete fileType;
}

//==============================================================================

FileTypes FileTypeInterface::fileTypes() const
{
    // Return the file types that we support

    return mFileTypes;
}

//==============================================================================

QStringList FileTypeInterface::defaultViews() const
{
    // Return our default views

    return mDefaultViews;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
