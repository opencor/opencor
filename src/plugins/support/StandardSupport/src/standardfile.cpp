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
// Standard file
//==============================================================================

#include "corecliutils.h"
#include "standardfile.h"

//==============================================================================

namespace OpenCOR {
namespace StandardSupport {

//==============================================================================

StandardFile::StandardFile(const QString &pFileName) :
    QObject(),
    mFileName(Core::nativeCanonicalFileName(pFileName))
{
}

//==============================================================================

void StandardFile::reset()
{
    // Nothing to do by default...
}

//==============================================================================

bool StandardFile::reload()
{
    // We want to reload the file, so we must first reset everything

    reset();

    // Now, we can try to (re)load the file

    return load();
}

//==============================================================================

QString StandardFile::fileName() const
{
    // Return the standard file's file name

    return mFileName;
}

//==============================================================================

void StandardFile::setFileName(const QString &pFileName)
{
    // Set the standard file's file name

    mFileName = pFileName;
}

//==============================================================================

}   // namespace StandardSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
