/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
