//==============================================================================
// CellML file manager
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "coreutils.h"
#include "filemanager.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFile::CellmlFile(const QString &pFileName) :
    mFileName(pFileName)
{
}

//==============================================================================

QString CellmlFile::fileName() const
{
    // Return the file name of the CellML file

    return mFileName;
}

//==============================================================================

CellmlFileManager::CellmlFileManager()
{
    // Create some connections to keep track of some events related to our
    // 'global' file manager

    connect(Core::FileManager::instance(), SIGNAL(fileManaged(const QString &)),
            this, SLOT(fileManaged(const QString &)));
    connect(Core::FileManager::instance(), SIGNAL(fileUnmanaged(const QString &)),
            this, SLOT(fileUnmanaged(const QString &)));
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

void CellmlFileManager::fileManaged(const QString &pFileName)
{
    if (isCellmlFile(pFileName))
        // We are dealing with a CellML file, so we can add it to our list of
        // managed CellML files

        mCellmlFiles << new CellmlFile(pFileName);
}

//==============================================================================

void CellmlFileManager::fileUnmanaged(const QString &pFileName)
{
    if (isCellmlFile(pFileName))
        // We are dealing with a CellML file, so we can remove it from our list
        // of managed CellML files

        foreach (CellmlFile *cellmlFile, mCellmlFiles)
            if (cellmlFile->fileName() == pFileName) {
                // The CellML file has been found, so remove it

                mCellmlFiles.removeAt(mCellmlFiles.indexOf(cellmlFile));

                delete cellmlFile;

                break;
            }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
