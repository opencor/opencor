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

#include "corecliutils.h"
#include "fileorganiserwindowitem.h"

//==============================================================================

#include <QFileInfo>

//==============================================================================

namespace OpenCOR {
namespace FileOrganiserWindow {

//==============================================================================

FileOrganiserWindowItem::FileOrganiserWindowItem(const QIcon &pIcon,
                                                 const QString &pTextOrPath,
                                                 const bool &pFolder) :
    QStandardItem(pIcon,
                  pFolder?
                      pTextOrPath:
                      QFileInfo(pTextOrPath).fileName()),
    mFolder(pFolder),
    mExpanded(false),
    mPath(pFolder?QString():pTextOrPath)
{
    // Customise ourselves

    setToolTip(pFolder?
                   pTextOrPath:
                   Core::nativeCanonicalDirName(pTextOrPath));
}

//==============================================================================

bool FileOrganiserWindowItem::isFolder() const
{
    // Return whether we are a folder

    return mFolder;
}

//==============================================================================

bool FileOrganiserWindowItem::isExpanded() const
{
    // Return whether we are expanded

    return mExpanded;
}

//==============================================================================

void FileOrganiserWindowItem::setExpanded(const bool &pExpanded)
{
    // Set our expanded state

    mExpanded = mFolder?pExpanded:false;
}

//==============================================================================

QString FileOrganiserWindowItem::path() const
{
    // Return our path

    return mPath;
}

//==============================================================================

}   // namespace FileOrganiserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
