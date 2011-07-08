#include "plugininterface.h"

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>

namespace OpenCOR {

PluginInfo::PluginInfo(const PluginType &pType,
                       const QStringList &pDependencies,
                       const QString &pDescription) :
    mType(pType),
    mDependencies(pDependencies),
    mDescription(pDescription)
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

QString PluginInfo::description()
{
    // Return the plugin's description

    return mDescription;
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
