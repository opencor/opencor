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
// Plugin information
//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

PluginInfo::PluginInfo(const Category &pCategory, const bool &pManageable,
                       const QStringList &pDependencies,
                       const Descriptions &pDescriptions) :
    mCategory(pCategory),
    mManageable(pManageable),
    mDependencies(pDependencies),
    mFullDependencies(QStringList()),
    mDescriptions(pDescriptions)
{
}

//==============================================================================

PluginInfo::Category PluginInfo::category() const
{
    // Return the plugin's category

    return mCategory;
}

//==============================================================================

bool PluginInfo::isManageable() const
{
    // Return the plugin's manageability

    return mManageable;
}

//==============================================================================

QStringList PluginInfo::dependencies() const
{
    // Return the plugin's (direct) dependencies

    return mDependencies;
}

//==============================================================================

QStringList PluginInfo::fullDependencies() const
{
    // Return the plugin's full dependencies (i.e. both its direct and indirect
    // dependencies)

    return mFullDependencies;
}

//==============================================================================

void PluginInfo::setFullDependencies(const QStringList &pFullDependencies)
{
    // Set the plugin's full dependencies

    mFullDependencies = pFullDependencies;
}

//==============================================================================

QString PluginInfo::description(const QString &pLocale) const
{
    // Return the plugin's description using the provided locale or an empty
    // string if no description can be found

    QString res = mDescriptions.value(pLocale);

    if (res.isEmpty())
        // There is no description for the required locale, so retrieve the
        // english description (which, hopefully, should exist)

        res = mDescriptions.value("en");

    return res;
}

//==============================================================================

Descriptions PluginInfo::descriptions() const
{
    // Return the plugin's descriptions

    return mDescriptions;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
