#include "plugin.h"
#include "plugininterface.h"
#include "pluginmanager.h"

#include <QDir>
#include <QFileInfo>
#include <QLibrary>

namespace OpenCOR {

Plugin::Plugin(const QString &pName) :
    mName(pName),
    mStatus(Undefined)
{
}

Plugin::~Plugin()
{
}

QString Plugin::name()
{
    // Return the plugin's

    return mName;
}

Plugin::PluginStatus Plugin::status()
{
    // Return the plugin's status

    return mStatus;
}

void Plugin::setStatus(const PluginStatus &pStatus)
{
    // Set the plugin's new status

    mStatus = pStatus;
}

QString Plugin::name(const QString &pPluginFileName)
{
    // Return the plugin's name based on its file name

    return QFileInfo(pPluginFileName).baseName().remove(0, PluginPrefix.length());
    // Note: we must remove the plugin prefix part from the plugin file name...
}

QString Plugin::fileName(const QString &pPluginsDir, const QString &pPluginName)
{
    // Return the plugin's file name based on its name

    return QDir::toNativeSeparators(pPluginsDir+QDir::separator()+PluginPrefix+pPluginName+PluginExtension);
    // Note: we must add the plugin prefix part to the plugin file name...
}

PluginInfo Plugin::info(const QString &pPluginFileName)
{
    // Return the information associated to the plugin

    PluginInfo pluginInfo;

    typedef PluginInfo (*PluginInfoFunc)();

    PluginInfoFunc pluginInfoFunc = (PluginInfoFunc) QLibrary::resolve(pPluginFileName,
                                                                       QString(name(pPluginFileName)+"PluginInfo").toLatin1().constData());

    if (pluginInfoFunc)
        // The plugin information function was found, so we can extract the
        // information we are after
        pluginInfo = pluginInfoFunc();
    else
        pluginInfo.type = PluginInfo::Undefined;

    return pluginInfo;
}

}
