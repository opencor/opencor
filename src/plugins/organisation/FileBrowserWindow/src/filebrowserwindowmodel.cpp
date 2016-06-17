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
// File Browser window model
//==============================================================================

#include "filebrowserwindowmodel.h"

//==============================================================================

namespace OpenCOR {
namespace FileBrowserWindow {

//==============================================================================

FileBrowserWindowModel::FileBrowserWindowModel(QObject *pParent) :
    QFileSystemModel(pParent)
{
    // We want access to the full file system

    setRootPath(QString());
}

//==============================================================================

Qt::ItemFlags FileBrowserWindowModel::flags(const QModelIndex &pIndex) const
{
    // Specify the supported features for the current item

    Qt::ItemFlags res = QFileSystemModel::flags(pIndex);

    // Prevent folders from being draggable

    if (QFileInfo(filePath(pIndex)).isDir())
        res &= ~Qt::ItemIsDragEnabled;

    return res;
}

//==============================================================================

}   // namespace FileBrowserWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
