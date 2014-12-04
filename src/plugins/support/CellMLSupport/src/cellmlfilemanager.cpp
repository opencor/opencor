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
// CellML file manager
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmlsupportplugin.h"
#include "corecliutils.h"
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

    Core::FileManager *fileManagerInstance = Core::FileManager::instance();

    connect(fileManagerInstance, SIGNAL(fileManaged(const QString &)),
            this, SLOT(manageFile(const QString &)));
    connect(fileManagerInstance, SIGNAL(fileUnmanaged(const QString &)),
            this, SLOT(unmanageFile(const QString &)));

    connect(fileManagerInstance, SIGNAL(fileReloaded(const QString &)),
            this, SLOT(reloadFile(const QString &)));

    connect(fileManagerInstance, SIGNAL(fileRenamed(const QString &, const QString &)),
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

    return static_cast<CellmlFileManager *>(Core::globalInstance("OpenCOR::CellMLSupport::CellmlFileManager",
                                                                 &instance));
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
    QString nativeFileName = Core::nativeCanonicalFileName(pFileName);

    if (isCellmlFile(nativeFileName))
        // We are dealing with a CellML file, so we can add it to our list of
        // managed CellML files

        mCellmlFiles.insert(nativeFileName, new CellmlFile(nativeFileName));
}

//==============================================================================

void CellmlFileManager::unmanageFile(const QString &pFileName)
{
    CellmlFile *crtCellmlFile = cellmlFile(pFileName);

    if (crtCellmlFile) {
        // We are dealing with a CellML file, so we can remove it from our list
        // of managed CellML files after having deleted it

        delete crtCellmlFile;

        mCellmlFiles.remove(Core::nativeCanonicalFileName(pFileName));
    }
}

//==============================================================================

void CellmlFileManager::reloadFile(const QString &pFileName)
{
    // The file is to be reloaded, so reload it
    // Note: to reload a file here ensures that our different CellML-based views
    //       won't each do it, thus saving time, etc.

    CellmlFile *crtCellmlFile = cellmlFile(pFileName);

    if (crtCellmlFile)
        crtCellmlFile->reload();
}

//==============================================================================

void CellmlFileManager::renameFile(const QString &pOldFileName,
                                   const QString &pNewFileName)
{
    // The file has been renamed, so we need to update our CellML files mapping,
    // if needed

    CellmlFile *crtCellmlFile = cellmlFile(pOldFileName);

    if (!crtCellmlFile)
        return;

    mCellmlFiles.insert(pNewFileName, crtCellmlFile);
    mCellmlFiles.remove(pOldFileName);

    // We also need to ensure that our CellML file object has its file name
    // updated

    crtCellmlFile->setFileName(pNewFileName);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
