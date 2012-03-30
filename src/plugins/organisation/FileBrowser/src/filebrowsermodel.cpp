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
