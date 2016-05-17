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

#pragma once

//==============================================================================

#include "pmrsupportglobal.h"

//==============================================================================

#include <QList>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PMRSUPPORT_EXPORT PmrExposure
{
public:
    explicit PmrExposure(const QString &pUrl, const QString &pName);

    static bool compare(const PmrExposure *pFirst, const PmrExposure *pSecond);

    QString url() const;
    QString name() const;

private:
    QString mUrl;
    QString mName;
};

//==============================================================================

class PmrExposures : public QList<PmrExposure *>
{
public:
    PmrExposures();
    virtual ~PmrExposures();

    void add(const QString &pUrl, const QString &pName);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
