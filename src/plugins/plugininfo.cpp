//==============================================================================
// Plugin information
//==============================================================================

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

PluginInfo::PluginInfo(const FormatVersion &pFormatVersion, const Type &pType,
                       const Category &pCategory, const bool &pManageable,
                       const QStringList &pDependencies,
                       const Descriptions &pDescriptions) :
    mFormatVersion(pFormatVersion),
    mType(pType),
    mCategory(pCategory),
    mManageable(pManageable),
    mDependencies(pDependencies),
    mFullDependencies(QStringList()),
    mDescriptions(pDescriptions)
{
}

//==============================================================================

PluginInfo::FormatVersion PluginInfo::formatVersion() const
{
    // Return the format version used by the plugin

    return mFormatVersion;
}

//==============================================================================

void PluginInfo::setFormatVersion(const FormatVersion &pFormatVersion)
{
    // Set the plugin's format version

    mFormatVersion = pFormatVersion;
}

//==============================================================================

PluginInfo::Type PluginInfo::type() const
{
    // Return the plugin's type

    return mType;
}

//==============================================================================

void PluginInfo::setType(const Type &pType)
{
    // Set the plugin's type

    mType = pType;
}

//==============================================================================

PluginInfo::Category PluginInfo::category() const
{
    // Return the plugin's category

    return mCategory;
}

//==============================================================================

void PluginInfo::setCategory(const Category &pCategory)
{
    // Set the plugin's category

    mCategory = pCategory;
}

//==============================================================================

bool PluginInfo::manageable() const
{
    // Return the plugin's manageability

    return mManageable;
}

//==============================================================================

void PluginInfo::setManageable(const bool &pManageable)
{
    // Set the plugin's manageability

    mManageable = pManageable;
}

//==============================================================================

QStringList PluginInfo::dependencies() const
{
    // Return the plugin's (direct) dependencies

    return mDependencies;
}

//==============================================================================

void PluginInfo::setDependencies(const QStringList &pDependencies)
{
    // Set the plugin's dependencies

    mDependencies = pDependencies;
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
    // Return the plugin's description using the provided locale or the first
    // description if no description can be found for the provided locale

    // Return the plugin's description using the provided locale or the first
    // description if no description can be found for the provided locale

    if (mDescriptions.isEmpty()) {
        // No description is avalable, so...

        return QString();
    } else {
        // At least one description is available, so return the one that
        // matches our locale our the first description if there is no match

        QString res = mDescriptions.value(pLocale);

        return res.isEmpty()?mDescriptions.begin().value():res;
    }
}

//==============================================================================

Descriptions PluginInfo::descriptions() const
{
    // Return the plugin's descriptions

    return mDescriptions;
}

//==============================================================================

void PluginInfo::setDescriptions(const Descriptions &pDescriptions)
{
    // Set the plugin's descriptions

    mDescriptions = pDescriptions;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
