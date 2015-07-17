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
// SED-ML file manager
//==============================================================================

#include "corecliutils.h"
#include "filemanager.h"
#include "sedmlfilemanager.h"
#include "sedmlsupportplugin.h"

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

SedmlFileManager::SedmlFileManager() :
    mSedmlFiles(SedmlFiles())
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

SedmlFileManager::~SedmlFileManager()
{
    // Remove all the managed files

    foreach (SedmlFile *sedmlFile, mSedmlFiles)
        delete sedmlFile;
}

//==============================================================================

SedmlFileManager * SedmlFileManager::instance()
{
    // Return the 'global' instance of our SED-ML file manager class

    static SedmlFileManager instance;

    return static_cast<SedmlFileManager *>(Core::globalInstance("OpenCOR::SEDMLSupport::SedmlFileManager",
                                                                &instance));
}

//==============================================================================

SedmlFile * SedmlFileManager::sedmlFile(const QString &pFileName)
{
    // Return the SedmlFile object, if any, associated with the given file

    return mSedmlFiles.value(Core::nativeCanonicalFileName(pFileName));
}

//==============================================================================

void SedmlFileManager::manageFile(const QString &pFileName)
{
    QString nativeFileName = Core::nativeCanonicalFileName(pFileName);

    if (!sedmlFile(nativeFileName) && isSedmlFile(nativeFileName)) {
        // We are dealing with a SED-ML file, which is not already managed, so
        // we can add it to our list of managed SED-ML files

        mSedmlFiles.insert(nativeFileName, new SedmlFile(nativeFileName));
    }
}

//==============================================================================

void SedmlFileManager::unmanageFile(const QString &pFileName)
{
    SedmlFile *crtSedmlFile = sedmlFile(pFileName);

    if (crtSedmlFile) {
        // We are dealing with a SED-ML file, so we can remove it from our list
        // of managed SED-ML files after having deleted it

        delete crtSedmlFile;

        mSedmlFiles.remove(Core::nativeCanonicalFileName(pFileName));
    }
}

//==============================================================================

void SedmlFileManager::reloadFile(const QString &pFileName)
{
    // The file is to be reloaded, so reload it
    // Note: to reload a file here ensures that our different SED-ML based views
    //       won't each do it, thus saving time and ensuring that a SED-ML based
    //       view doesn't forget to do it...

    SedmlFile *crtSedmlFile = sedmlFile(pFileName);

    if (crtSedmlFile) {
        // The file is managed, but should it still be (i.e. can it still be
        // considered as being a SED-ML file)?

        if (isSedmlFile(pFileName))
            crtSedmlFile->reload();
        else
            unmanageFile(pFileName);
    } else {
        // The file is not managed, which means that previously it wasn't
        // considered as being a SED-ML file, but things may be different now,
        // so try to remanage it and load it, if possible

        manageFile(pFileName);

        crtSedmlFile = sedmlFile(pFileName);

        if (crtSedmlFile)
            crtSedmlFile->load();
    }
}

//==============================================================================

void SedmlFileManager::renameFile(const QString &pOldFileName,
                                  const QString &pNewFileName)
{
    // The file has been renamed, so we need to update our SED-ML files mapping,
    // if needed

    SedmlFile *crtSedmlFile = sedmlFile(pOldFileName);

    if (!crtSedmlFile)
        return;

    mSedmlFiles.insert(pNewFileName, crtSedmlFile);
    mSedmlFiles.remove(pOldFileName);

    // We also need to ensure that our SED-ML file object has its file name
    // updated

    crtSedmlFile->setFileName(pNewFileName);
}

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
