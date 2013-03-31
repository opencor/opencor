//==============================================================================
// Plugin manager
//==============================================================================

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

//==============================================================================

#include "coreinterface.h"
#include "plugininfo.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {

//==============================================================================

class Plugin;

//==============================================================================

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(const PluginInfo::Type &pGuiOrConsoleType);
    ~PluginManager();

    Plugins plugins() const;
    Plugins loadedPlugins() const;

    QString pluginsDir() const;
    Plugin * plugin(const QString &pName) const;

    PluginInfo::InterfaceVersion interfaceVersion() const;

private:
    PluginInfo::InterfaceVersion mInterfaceVersion;
    PluginInfo::Type mGuiOrConsoleType;

    QString mPluginsDir;
    Plugins mPlugins;
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
