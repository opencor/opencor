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

CellmlFileManager::CellmlFileManager() :
    mCellmlFiles(CellmlFiles())
{
    // Create some connections to keep track of some events related to our
    // 'global' file manager

    connect(Core::FileManager::instance(), SIGNAL(fileManaged(const QString &)),
            this, SLOT(manageFile(const QString &)));
    connect(Core::FileManager::instance(), SIGNAL(fileUnmanaged(const QString &)),
            this, SLOT(unmanageFile(const QString &)));

    connect(Core::FileManager::instance(), SIGNAL(fileModified(const QString &, const bool &)),
            this, SLOT(setModified(const QString &, const bool &)));
    connect(this, SIGNAL(fileModified(const QString &, const bool &)),
            Core::FileManager::instance(), SLOT(setModified(const QString &, const bool &)));

    connect(Core::FileManager::instance(), SIGNAL(fileRenamed(const QString &, const QString &)),
            this, SLOT(renameFile(const QString &, const QString &)));
}

//==============================================================================

CellmlFileManager::~CellmlFileManager()
{
    // Remove all the managed files

    foreach (CellmlFile *cellmlFile, mCellmlFiles)
        delete cellmlFile;
}

//==============================================================================

CellmlFileManager * CellmlFileManager::instance()
{
    // Return the 'global' instance of our CellML file manager class

    static CellmlFileManager instance;

    return (CellmlFileManager *) Core::instance("OpenCOR::CellMLSupport::CellmlFileManager", &instance);
}

//==============================================================================

CellmlFile * CellmlFileManager::cellmlFile(const QString &pFileName)
{
    // Return the CellmlFile object, if any, associated with the requested file

    return mCellmlFiles.value(Core::nativeCanonicalFileName(pFileName));
}

//==============================================================================

void CellmlFileManager::manageFile(const QString &pFileName)
{
    if (isCellmlFile(pFileName))
        // We are dealing with a CellML file, so we can add it to our list of
        // managed CellML files

        mCellmlFiles.insert(pFileName, new CellmlFile(pFileName));
}

//==============================================================================

void CellmlFileManager::unmanageFile(const QString &pFileName)
{
    if (isCellmlFile(pFileName)) {
        // We are dealing with a CellML file, so we can remove it from our list
        // of managed CellML files after having deleted it

        delete cellmlFile(pFileName);

        mCellmlFiles.remove(pFileName);
    }
}

//==============================================================================

void CellmlFileManager::setModified(const QString &pFileName,
                                    const bool &pModified)
{
    QString nativeFileName = Core::nativeCanonicalFileName(pFileName);

    if (cellmlFile(nativeFileName))
        // We are dealing with a managed CellML file, so we can let the main
        // file manager know about the new modified status of the CellML file,
        // should this be needed (otherwise, we will end up in a vicious circle)

        if (pModified != Core::FileManager::instance()->isModified(nativeFileName))
            emit fileModified(nativeFileName, pModified);
}

//==============================================================================

void CellmlFileManager::renameFile(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    // Retrieve the CellmlFile object associated with the 'old' file

    CellmlFile *oldNewCellmlFile = cellmlFile(pOldFileName);

    if (oldNewCellmlFile)
        // We are dealing with a CellML file, so we can rename it

        oldNewCellmlFile->setFileName(pNewFileName);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
