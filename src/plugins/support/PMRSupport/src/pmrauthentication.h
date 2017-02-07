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
// PMR authentication
//==============================================================================

#pragma once

//==============================================================================

#include "o1.h"

//==============================================================================

namespace OpenCOR {
namespace PMRSupport {

//==============================================================================

class PmrWebServiceManager;

//==============================================================================

class PmrAuthentication: public O1
{
    Q_OBJECT

public:
    explicit PmrAuthentication(const QString &pUrl,
                               PmrWebServiceManager *pParent);

private slots:
    void pmrUrlChanged(const QString &pPmrUrl);
};

//==============================================================================

}   // namespace PMRSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
