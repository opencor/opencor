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

PmrExposure::PmrExposure(const QString &pUrl, const QString &pName) :
    mUrl(pUrl),
    mName(pName)
{
}

//==============================================================================

bool PmrExposure::compare(const PmrExposure *pFirst, const PmrExposure *pSecond)
{
    // Return whether the first exposure is lower than the second one (without
    // worrying about casing)

    return pFirst->name().compare(pSecond->name(), Qt::CaseInsensitive) < 0;
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

PmrExposures::PmrExposures() :
    QList<PmrExposure *>()
{
}

//==============================================================================

PmrExposures::~PmrExposures()
{
    // Delete the list of exposures

    while (!isEmpty())
        delete takeFirst();
}

//==============================================================================

void PmrExposures::add(const QString &pUrl, const QString &pName)
{
    // Add a new exposure to the list

    QList::append(new PmrExposure(pUrl, pName));
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
