#include "plugininterface.h"

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>

namespace OpenCOR {

PluginInfo::PluginInfo(const PluginType &pType,
                       const QStringList &pDependencies,
                       const PluginInfoDescriptions &pDescriptions) :
    mType(pType),
    mDependencies(pDependencies),
    mDescriptions(pDescriptions)
{
}

PluginInfo::PluginType PluginInfo::type()
{
    // Return the plugin's type

    return mType;
}

QStringList PluginInfo::dependencies()
{
    // Return the plugin's dependencies

    return mDependencies;
}

QString PluginInfo::description(const QString &pLocale)
{
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

void PluginInterface::initialize()
{
    // Nothing to do by default...
}

void PluginInterface::finalize()
{
    // Nothing to do by default...
}

}
