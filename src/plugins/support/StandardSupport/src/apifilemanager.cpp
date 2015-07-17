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
// API file manager
//==============================================================================

#include "apifilemanager.h"
#include "apisupportplugin.h"
#include "corecliutils.h"
#include "filemanager.h"

//==============================================================================

namespace OpenCOR {
namespace APISupport {

//==============================================================================

ApiFileManager::ApiFileManager() :
    mApiFiles(ApiFiles())
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

ApiFileManager::~ApiFileManager()
{
    // Remove all the managed files

    foreach (ApiFile *apiFile, mApiFiles)
        delete apiFile;
}

//==============================================================================

ApiFile * ApiFileManager::apiFile(const QString &pFileName)
{
    // Return the ApiFile object, if any, associated with our given file

    return mApiFiles.value(Core::nativeCanonicalFileName(pFileName));
}

//==============================================================================

void ApiFileManager::manageFile(const QString &pFileName)
{
    QString nativeFileName = Core::nativeCanonicalFileName(pFileName);

    if (!apiFile(nativeFileName) && isApiFile(nativeFileName)) {
        // We are dealing with an API file, which is not already managed, so we
        // can add it to our list of managed API files

        mApiFiles.insert(nativeFileName, new ApiFile(nativeFileName));
    }
}

//==============================================================================

void ApiFileManager::unmanageFile(const QString &pFileName)
{
    ApiFile *crtApiFile = apiFile(pFileName);

    if (crtApiFile) {
        // We are dealing with a API file, so we can remove it from our list of
        // managed API files after having deleted it

        delete crtApiFile;

        mApiFiles.remove(Core::nativeCanonicalFileName(pFileName));
    }
}

//==============================================================================

void ApiFileManager::reloadFile(const QString &pFileName)
{
    // The file is to be reloaded, so reload it
    // Note: to reload a file here ensures that our different API-based views
    //       won't each do it, thus saving time and ensuring that an API-based
    //       view doesn't forget to do it...

    ApiFile *crtApiFile = apiFile(pFileName);

    if (crtApiFile) {
        // The file is managed, but should it still be (i.e. can it still be
        // considered as being an API file)?

        if (isApiFile(pFileName))
            crtApiFile->reload();
        else
            unmanageFile(pFileName);
    } else {
        // The file is not managed, which means that previously it wasn't
        // considered as being an API file, but things may be different now, so
        // try to remanage it and load it, if possible

        manageFile(pFileName);

        crtApiFile = apiFile(pFileName);

        if (crtApiFile)
            crtApiFile->load();
    }
}

//==============================================================================

void ApiFileManager::renameFile(const QString &pOldFileName,
                                const QString &pNewFileName)
{
    // The file has been renamed, so we need to update our API files mapping, if
    // needed

    ApiFile *crtApiFile = apiFile(pOldFileName);

    if (!crtApiFile)
        return;

    mApiFiles.insert(pNewFileName, crtApiFile);
    mApiFiles.remove(pOldFileName);

    // We also need to ensure that our API file object has its file name updated

    crtApiFile->setFileName(pNewFileName);
}

//==============================================================================

}   // namespace APISupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
