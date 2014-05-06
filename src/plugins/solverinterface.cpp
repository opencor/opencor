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
// Solver interface
//==============================================================================

#include "solverinterface.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace Solver {

//==============================================================================

Property::Property(const Property::Type &pType, const QString &pId,
                   const Descriptions &pDescriptions,
                   const QVariant &pDefaultValue,
                   const bool &pHasVoiUnit) :
    mType(pType),
    mId(pId),
    mDescriptions(pDescriptions),
    mDefaultValue(pDefaultValue),
    mHasVoiUnit(pHasVoiUnit)
{
}

//==============================================================================

Property::Type Property::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

QString Property::id() const
{
    // Return our id

    return mId;
}

//==============================================================================

Descriptions Property::descriptions() const
{
    // Return our descriptions

    return mDescriptions;
}

//==============================================================================

QVariant Property::defaultValue() const
{
    // Return our default value

    return mDefaultValue;
}

//==============================================================================

bool Property::hasVoiUnit() const
{
    // Return whether we have a VOI unit

    return mHasVoiUnit;
}

//==============================================================================

}   // namespace Solver
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
