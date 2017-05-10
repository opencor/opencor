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
// Plugin information
//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int pluginInfoVersion()
{
    // Version of PluginInfo

    return 1;
}

//==============================================================================

PluginInfo::PluginInfo(const Category &pCategory, const bool &pSelectable,
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

PluginInfo::Category PluginInfo::category() const
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
