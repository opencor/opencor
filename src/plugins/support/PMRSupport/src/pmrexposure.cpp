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
#include "pmrworkspace.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

PmrExposure::PmrExposure(QObject *parent) :
    QObject(parent), mName(QString()), mUrl(QString())
{
}

//==============================================================================

PmrExposure::PmrExposure(const QString &pUrl, const QString &pName, QObject *parent) :
    QObject(parent),
    mName(pName),
    mUrl(pUrl),
    mExposureFileList(QStringList()),
    mFileUrlsLeftCount(-1),
    mWorkspace(new PmrWorkspace())
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

bool PmrExposure::isNull() const
{
    return mUrl.isNull();

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

QString PmrExposure::toHtml(void) const
{
    // Return a HTML description of ourself

    return QString("<a href=\"%1\">%2</a>").arg(mUrl, mName);
}

//==============================================================================

int PmrExposure::fileUrlsLeftCount(void) const
{
    return mFileUrlsLeftCount;

}

//==============================================================================

void PmrExposure::setFileUrlsLeftCount(const int &count)
{
    mFileUrlsLeftCount = count;
}

//==============================================================================

void PmrExposure::addExposureFile(const QString &pFileName)
{
    if (mFileUrlsLeftCount > 0) {
        mExposureFileList << pFileName;
        mFileUrlsLeftCount -= 1;
    }
}

//==============================================================================

void PmrExposure::addOtherFile(const QString &pFileName)
{
    if (!mExposureFileList.contains(pFileName))
        mExposureFileList << pFileName;
}

//==============================================================================
const QStringList PmrExposure::exposureFileList(void) const
{
    return mExposureFileList;
}

//==============================================================================

PmrWorkspace *PmrExposure::workspace(void) const
{
    return mWorkspace;
}

//==============================================================================

void PmrExposure::setWorkspace(PmrWorkspace *pWorkspace)
{
    mWorkspace = pWorkspace;
}

//==============================================================================
//==============================================================================

PmrExposureList::PmrExposureList() :
    QList<PmrExposure *>()
{
}

//==============================================================================

void PmrExposureList::add(const QString &pUrl, const QString &pName, QObject *parent)
{
    // Add a new exposure to the list

    QList::append(new PmrExposure(pUrl, pName, parent));
}

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
