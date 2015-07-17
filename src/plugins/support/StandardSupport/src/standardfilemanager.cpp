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
// Standard file manager
//==============================================================================

#include "corecliutils.h"
#include "filemanager.h"
#include "standardfilemanager.h"
#include "standardsupportplugin.h"

//==============================================================================

namespace OpenCOR {
namespace StandardSupport {

//==============================================================================

StandardFileManager::StandardFileManager() :
    mStandardFiles(StandardFiles())
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

StandardFileManager::~StandardFileManager()
{
    // Remove all the managed files

    foreach (QObject *standardFile, mStandardFiles)
        delete standardFile;
}

//==============================================================================

QObject * StandardFileManager::standardFile(const QString &pFileName)
{
    // Return the StandardFile object, if any, associated with the given file

    return mStandardFiles.value(Core::nativeCanonicalFileName(pFileName));
}

//==============================================================================

void StandardFileManager::manageFile(const QString &pFileName)
{
    QString nativeFileName = Core::nativeCanonicalFileName(pFileName);

    if (!standardFile(nativeFileName) && isStandardFile(nativeFileName)) {
        // We are dealing with a standard file, which is not already managed, so
        // we can add it to our list of managed standard files

        mStandardFiles.insert(nativeFileName, newStandardFile(nativeFileName));
    }
}

//==============================================================================

void StandardFileManager::unmanageFile(const QString &pFileName)
{
    QObject *crtStandardFile = standardFile(pFileName);

    if (crtStandardFile) {
        // We are dealing with a standard file, so we can remove it from our
        // list of managed standard files after having deleted it

        deleteStandardFile(crtStandardFile);

        mStandardFiles.remove(Core::nativeCanonicalFileName(pFileName));
    }
}

//==============================================================================

void StandardFileManager::reloadFile(const QString &pFileName)
{
    // The file is to be reloaded, so reload it
    // Note: to reload a file here ensures that our different standard-based
    //       views won't each do it, thus saving time and ensuring that a
    //       standard-based view doesn't forget to do it...

    QObject *crtStandardFile = standardFile(pFileName);

    if (crtStandardFile) {
        // The file is managed, but should it still be (i.e. can it still be
        // considered as being a standard file)?

        if (isStandardFile(pFileName))
            qobject_cast<StandardFile *>(crtStandardFile)->reload();
        else
            unmanageFile(pFileName);
    } else {
        // The file is not managed, which means that previously it wasn't
        // considered as being a standard file, but things may be different now,
        // so try to remanage it and load it, if possible

        manageFile(pFileName);

        crtStandardFile = standardFile(pFileName);

        if (crtStandardFile)
            qobject_cast<StandardFile *>(crtStandardFile)->load();
    }
}

//==============================================================================

void StandardFileManager::renameFile(const QString &pOldFileName,
                                     const QString &pNewFileName)
{
    // The file has been renamed, so we need to update our standard files
    // mapping, if needed

    QObject *crtStandardFile = standardFile(pOldFileName);

    if (!crtStandardFile)
        return;

    mStandardFiles.insert(pNewFileName, crtStandardFile);
    mStandardFiles.remove(pOldFileName);

    // We also need to ensure that our standard file object has its file name
    // updated

    qobject_cast<StandardFile *>(crtStandardFile)->setFileName(pNewFileName);
}

//==============================================================================

}   // namespace StandardSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
