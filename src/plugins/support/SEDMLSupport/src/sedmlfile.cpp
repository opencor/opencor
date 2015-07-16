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
// SED-ML file class
//==============================================================================

#include "corecliutils.h"
#include "sedmlfile.h"

//==============================================================================

namespace OpenCOR {
namespace SEDMLSupport {

//==============================================================================

SedmlFile::SedmlFile(const QString &pFileName) :
    mFileName(Core::nativeCanonicalFileName(pFileName))
{
}

//==============================================================================

void SedmlFile::reset()
{
}

//==============================================================================

bool SedmlFile::load()
{
    // Consider the file loaded

    return true;
}

//==============================================================================

bool SedmlFile::reload()
{
    // We want to reload the file, so we must first reset everything

    reset();

    // Now, we can try to (re)load the file

    return load();
}

//==============================================================================

QString SedmlFile::fileName() const
{
    // Return the SED-ML file's file name

    return mFileName;
}

//==============================================================================

void SedmlFile::setFileName(const QString &pFileName)
{
    // Set the SED-ML file's file name

    mFileName = pFileName;
}

//==============================================================================

}   // namespace SEDMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
