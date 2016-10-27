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
// PMR exposure
//==============================================================================

#include "pmrexposure.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrExposure::PmrExposure(const QString &pUrl, const QString &pName,
                         QObject *pParent) :
    QObject(pParent),
    mUrl(pUrl),
    mName(pName),
    mWorkspace(0),
    mExposureFiles(QStringList())
{
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

bool sortExposures(PmrExposure *pExposure1, PmrExposure *pExposure2)
{
    // Return whether the first exposure is lower than the second one (without
    // worrying about casing)

    return pExposure1->name().compare(pExposure2->name(), Qt::CaseInsensitive) < 0;
}

//==============================================================================

void PmrExposures::sort()
{
    // Sort our exposures

    std::sort(begin(), end(), sortExposures);
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
