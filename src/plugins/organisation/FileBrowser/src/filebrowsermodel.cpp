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
// File browser model
//==============================================================================

#include "filebrowsermodel.h"

//==============================================================================

namespace OpenCOR {
namespace FileBrowser {

//==============================================================================

FileBrowserModel::FileBrowserModel(QObject *pParent) :
    QFileSystemModel(pParent)
{
    // We want acces to the full file system

    setRootPath("");
}

//==============================================================================

Qt::ItemFlags FileBrowserModel::flags(const QModelIndex &pIndex) const
{
    // Specify the supported features for the current item

    Qt::ItemFlags res = QFileSystemModel::flags(pIndex);

    // Prevent some features for the item in case it's a folder

    if (QFileInfo(filePath(pIndex)).isDir())
        // We don't want a folder to be draggable

        res &= ~Qt::ItemIsDragEnabled;

    // We are all done, so...

    return res;
}

//==============================================================================

}   // namespace FileBrowser
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
