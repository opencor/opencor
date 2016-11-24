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

#pragma once

//==============================================================================

#include <QObject>
#include <QStringList>

//==============================================================================

namespace OpenCOR {

//==============================================================================

class FileTypeInterface
{
public:
#define INTERFACE_DEFINITION
    #include "filetypeinterface.inl"
#undef INTERFACE_DEFINITION
};

//==============================================================================

typedef QList<FileTypeInterface *> FileTypeInterfaces;

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::FileTypeInterface, "OpenCOR::FileTypeInterface")

//==============================================================================
// End of file
//==============================================================================
