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
// PMR exposure
//==============================================================================

#include "pmrexposure.h"
#include "pmrwebservice.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrExposure::PmrExposure(const QString &pUrl, const QString &pName,
                         PmrWebService *pParent) :
    QObject(pParent),
    mUrl(pUrl),
    mName(pName),
    mWorkspace(0),
    mExposureFiles(QStringList())
{
}

//==============================================================================

bool PmrExposure::compare(PmrExposure *pExposure1, PmrExposure *pExposure2)
{
    // Return whether the first exposure is lower than the second one (without
    // worrying about casing)

    return pExposure1->name().compare(pExposure2->name(), Qt::CaseInsensitive) < 0;
}

//==============================================================================

QString PmrExposure::url() const
{
    // Return our URL

    return mUrl;
}

//==============================================================================

QString PmrExposure::name() const
{
    // Return our name

    return mName;
}

//==============================================================================

PmrWorkspace * PmrExposure::workspace() const
{
    // Return our workspace

    return mWorkspace;
}

//==============================================================================

void PmrExposure::setWorkspace(PmrWorkspace *pWorkspace)
{
    // Set our workspace

    mWorkspace = pWorkspace;
}

//==============================================================================

QStringList PmrExposure::exposureFiles() const
{
    // Return our list of exposure files

    return mExposureFiles;
}

//==============================================================================

void PmrExposure::addExposureFile(const QString &pFileName)
{
    // Keep track of the given exposure file, if it's not already tracked

    if (!mExposureFiles.contains(pFileName))
        mExposureFiles << pFileName;
}

//==============================================================================

QString PmrExposure::toHtml() const
{
    // Return an HTML description of ourselves

    return QString("<a href=\"%1\">%2</a>").arg(mUrl, mName);
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
