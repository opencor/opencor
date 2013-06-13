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

class QCoreApplication;

//==============================================================================

namespace OpenCOR {

//==============================================================================

class Plugin;

//==============================================================================

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QCoreApplication *pApp,
                           const bool &pCliSupport = false);
    ~PluginManager();

    Plugins plugins() const;
    Plugins loadedPlugins() const;

    QString pluginsDir() const;
    Plugin * plugin(const QString &pName) const;

    PluginInfo::InterfaceVersion interfaceVersion() const;

private:
    PluginInfo::InterfaceVersion mInterfaceVersion;

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
