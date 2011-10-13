//==============================================================================
// Plugin information class
//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

PluginInfo::PluginInfo(const Version &pVersion, const Type &pType,
                       const Category &pCategory, const bool &pManageable,
                       const QStringList &pDependencies,
                       const Descriptions &pDescriptions) :
    mVersion(pVersion),
    mType(pType),
    mCategory(pCategory),
    mManageable(pManageable),
    mDependencies(pDependencies),
    mFullDependencies(QStringList()),
    mDescriptions(pDescriptions)
{
}

//==============================================================================

PluginInfo::Version PluginInfo::version() const
{
    // Return the version used by the plugin

    return mVersion;
}

//==============================================================================

PluginInfo::Type PluginInfo::type() const
{
    // Return the plugin's type

    return mType;
}

//==============================================================================

PluginInfo::Category PluginInfo::category() const
{
    // Return the plugin's category

    return mCategory;
}

//==============================================================================

bool PluginInfo::manageable() const
{
    // Return the plugin's manageability

    return mManageable;
}

//==============================================================================

QStringList PluginInfo::dependencies() const
{
    // Return the plugin's dependencies

    return mDependencies;
}

//==============================================================================

QStringList PluginInfo::fullDependencies() const
{
    // Return the plugin's full dependencies
    // Note: they are determined by the plugin itself (i.e. during the
    //       construction of a Plugin object)

    return mFullDependencies;
}

//==============================================================================

QString PluginInfo::description(const QString &pLocale) const
{
    // Return the plugin's description using the provided locale or the first
    // description if no description can be found for the provided locale

    return OpenCOR::description(mDescriptions, pLocale);
}

//==============================================================================

QString description(const Descriptions &pDescriptions, const QString &pLocale)
{
    // Return the description using the provided locale or the first description
    // if no description can be found for the provided locale

    if (pDescriptions.isEmpty()) {
        // No description is avalable, so...

        return QString();
    } else {
        // At least one description is available, so return the one that
        // matches our locale our the first description if there is no match

        QString res = pDescriptions.value(pLocale);

        return res.isEmpty()?pDescriptions.begin().value():res;
    }
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
