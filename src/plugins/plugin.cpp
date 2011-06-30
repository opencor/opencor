#include "plugin.h"
#include "plugininterface.h"

#include <QFileInfo>
#include <QLibrary>

namespace OpenCOR {

Plugin::Plugin(const QString &pName) :
    mName(pName),
    mLoaded(false)
{
}

Plugin::~Plugin()
{
}

QString Plugin::name()
{
    // Return the name of the plugin

    return mName;
}

bool Plugin::isLoaded()
{
    // Return whether the plugin has been loaded or not

    return mLoaded;
}

QString Plugin::pluginName(const QString &pPluginFileName)
{
    // Return the name of the plugin based on its file name

#ifdef Q_WS_WIN
    return QFileInfo(pPluginFileName).baseName();
#else
    return QFileInfo(pPluginFileName).baseName().remove(0, 3);
    // Note: remove the "lib" part of the plugin file name...
#endif
}

PluginInfo Plugin::pluginInfo(const QString &pPluginFileName)
{
    // Return the information associated to the plugin

    PluginInfo pluginInfo;

    typedef PluginInfo (*PluginInfoFunc)();

    PluginInfoFunc pluginInfoFunc = (PluginInfoFunc) QLibrary::resolve(pPluginFileName,
                                                                       QString(pluginName(pPluginFileName)+"PluginInfo").toLatin1().constData());

    if (pluginInfoFunc)
        // The plugin information function was found, so extract the information
        // we are after

        pluginInfo = pluginInfoFunc();
    else
        pluginInfo.type = PluginInfo::Unknown;

    return pluginInfo;
}

}
