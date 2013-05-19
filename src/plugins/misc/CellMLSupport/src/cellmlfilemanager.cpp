//==============================================================================
// CellML file manager
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "coreutils.h"
#include "filemanager.h"

//==============================================================================

#include <QDir>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellMLFileManager::CellMLFileManager() :
    mCellMLFiles(CellMLFiles())
{
    // Create some connections to keep track of some events related to our
    // 'global' file manager

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();

    connect(fileManagerInstance, SIGNAL(fileManaged(const QString &)),
            this, SLOT(manageFile(const QString &)));
    connect(fileManagerInstance, SIGNAL(fileUnmanaged(const QString &)),
            this, SLOT(unmanageFile(const QString &)));

    connect(fileManagerInstance, SIGNAL(fileRenamed(const QString &, const QString &)),
            this, SLOT(renameFile(const QString &, const QString &)));
}

//==============================================================================

CellMLFileManager::~CellMLFileManager()
{
    // Remove all the managed files

    foreach (CellMLFile *cellmlFile, mCellMLFiles)
        delete cellmlFile;
}

//==============================================================================

CellMLFileManager * CellMLFileManager::instance()
{
    // Return the 'global' instance of our CellML file manager class

    static CellMLFileManager instance;

    return static_cast<CellMLFileManager *>(Core::globalInstance("OpenCOR::CellMLSupport::CellMLFileManager",
                                                                 &instance));
}

//==============================================================================

CellMLFile * CellMLFileManager::cellmlFile(const QString &pFileName)
{
    // Return the CellMLFile object, if any, associated with the requested file

    return mCellMLFiles.value(Core::nativeCanonicalFileName(pFileName));
}

//==============================================================================

void CellMLFileManager::manageFile(const QString &pFileName)
{
    if (isCellMLFile(pFileName))
        // We are dealing with a CellML file, so we can add it to our list of
        // managed CellML files

        mCellMLFiles.insert(pFileName, new CellMLFile(pFileName));
}

//==============================================================================

void CellMLFileManager::unmanageFile(const QString &pFileName)
{
    if (isCellMLFile(pFileName)) {
        // We are dealing with a CellML file, so we can remove it from our list
        // of managed CellML files after having deleted it

        delete cellmlFile(pFileName);

        mCellMLFiles.remove(pFileName);
    }
}

//==============================================================================

void CellMLFileManager::renameFile(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    // The file has been renamed, so we need to update our CellML files mapping

    mCellMLFiles.insert(pNewFileName, mCellMLFiles.value(pOldFileName));
    mCellMLFiles.remove(pOldFileName);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
