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
// File Organiser window item
//==============================================================================

#pragma once

//==============================================================================

#include <QStandardItem>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiserWindow {

//==============================================================================

class FileOrganiserWindowItem : public QStandardItem
{
public:
    explicit FileOrganiserWindowItem(const QIcon &pIcon,
                                     const QString &pTextOrPath,
                                     const bool &pFolder = false);

    bool isFolder() const;

    bool isExpanded() const;
    void setExpanded(const bool &pExpanded);

    QString path() const;

private:
    bool mFolder;
    bool mExpanded;

    QString mPath;
};

//==============================================================================

typedef QList<FileOrganiserWindowItem *> FileOrganiserWindowItems;

//==============================================================================

}   // namespace FileOrganiserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
