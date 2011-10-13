//==============================================================================
// Plugin manager class
//==============================================================================

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

//==============================================================================

#include "coreinterface.h"
#include "plugininfo.h"

//==============================================================================

#include <QMap>
#include <QObject>
#include <QSettings>

//==============================================================================

namespace OpenCOR {

//==============================================================================

class Plugin;

//==============================================================================

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QSettings *pSettings,
                           const PluginInfo::Type &pGuiOrConsoleType);
    ~PluginManager();

    QList<Plugin *> plugins(const bool &pOnlyLoadedPlugins = false) const;
    QList<Plugin *> loadedPlugins() const;

    QString pluginsDir() const;
    QMap<QString, Plugin *> mappedPlugins() const;
    Plugin * plugin(const QString &pName) const;

    QSettings * settings() const;

    PluginInfo::Version version() const;

private:
    QSettings *mSettings;

    PluginInfo::Version mVersion;
    PluginInfo::Type mGuiOrConsoleType;

    QString mPluginsDir;
    QMap<QString, Plugin *> mPlugins;
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
