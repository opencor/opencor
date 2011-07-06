#include "plugininterface.h"

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>

namespace OpenCOR {

PluginInfo::PluginInfo(const PluginType &pType,
                       const QStringList &pDependencies) :
    mType(pType),
    mDependencies(pDependencies)
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

}
