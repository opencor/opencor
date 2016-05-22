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
// Plugin information
//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

PluginInfo::PluginInfo(const QString &pCategory, const bool &pSelectable,
                       const bool &pCliSupport,
                       const QStringList &pDependencies,
                       const Descriptions &pDescriptions,
                       const QStringList &pLoadBefore) :
    mCategory(pCategory),
    mSelectable(pSelectable),
    mCliSupport(pCliSupport),
    mDependencies(pDependencies),
    mFullDependencies(QStringList()),
    mDescriptions(pDescriptions),
    mLoadBefore(pLoadBefore)
{
}

//==============================================================================

QString PluginInfo::category() const
{
    // Return the plugin's category

    return mCategory;
}

//==============================================================================

bool PluginInfo::isSelectable() const
{
    // Return whether the plugin is selectable

    return mSelectable;
}

//==============================================================================

bool PluginInfo::hasCliSupport() const
{
    // Return whether the plugin has CLI support

    return mCliSupport;
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

    if (res.isEmpty()) {
        // There is no description for the required locale, so retrieve the
        // English description (which, hopefully, exists)

        res = mDescriptions.value("en");
    }

    return res;
}

//==============================================================================

Descriptions PluginInfo::descriptions() const
{
    // Return the plugin's descriptions

    return mDescriptions;
}

//==============================================================================

QStringList PluginInfo::loadBefore() const
{
    // Return the plugin before which we must load

    return mLoadBefore;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
