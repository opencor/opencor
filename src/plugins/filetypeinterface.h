/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// File type interface
//==============================================================================

#pragma once

//==============================================================================

#include <QtPlugin>

//==============================================================================

#include <QStringList>

//==============================================================================

namespace OpenCOR {

//==============================================================================

class FileTypeInterface;

//==============================================================================

class FileType
{
public:
    explicit FileType(FileTypeInterface *pOwner, const QString &pMimeType,
                      const QString &pFileExtension);

    bool operator==(FileType *pFileType) const;

    QString mimeType() const;
    QString fileExtension() const;
    QString description() const;

private:
    FileTypeInterface *mOwner;

    QString mMimeType;
    QString mFileExtension;
};

//==============================================================================

typedef QList<FileType *> FileTypes;

//==============================================================================

class FileTypeInterface
{
public:
    explicit FileTypeInterface();
    ~FileTypeInterface();

#define INTERFACE_DEFINITION
    #include "filetypeinterface.inl"
#undef INTERFACE_DEFINITION

    FileTypes fileTypes() const;
    QStringList defaultViews() const;

protected:
    FileTypes mFileTypes;
    QStringList mDefaultViews;
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::FileTypeInterface, "OpenCOR::FileTypeInterface")

//==============================================================================
// End of file
//==============================================================================
