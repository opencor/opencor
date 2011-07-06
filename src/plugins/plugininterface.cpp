#include "plugininterface.h"

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>

namespace OpenCOR {

PluginInfo::PluginInfo(const PluginType &pType,
                       const QStringList &pDependencies,
                       const bool &pManageable) :
    mType(pType),
    mDependencies(pDependencies),
    mManageable(pManageable)
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

bool PluginInfo::manageable()
{
    // Return whether the plugin is manageable by the user

    return mManageable;
}

}
