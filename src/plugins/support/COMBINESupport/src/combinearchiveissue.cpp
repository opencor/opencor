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
// COMBINE archive issue
//==============================================================================

#include "combinearchiveissue.h"

//==============================================================================

#include <Qt>

//==============================================================================

namespace OpenCOR {
namespace COMBINESupport {

//==============================================================================

CombineArchiveIssue::CombineArchiveIssue(const Type &pType,
                                         const QString &pMessage) :
    mType(pType),
    mMessage(pMessage)
{
}

//==============================================================================

CombineArchiveIssue::Type CombineArchiveIssue::type() const
{
    // Return the issue's type

    return mType;
}

//==============================================================================

QString CombineArchiveIssue::message() const
{
    // Return the issue's message

    return mMessage;
}

//==============================================================================

}   // namespace COMBINESupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
