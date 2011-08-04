#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "plugininterface.h"

#include <QMap>
#include <QObject>
#include <QSettings>

namespace OpenCOR {

class Plugin;

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QSettings *pSettings,
                           const PluginInfo::PluginType &pGuiOrConsoleType);
    ~PluginManager();

    QList<Plugin *> plugins(const bool &pOnlyLoadedPlugins = false) const;
    QList<Plugin *> loadedPlugins() const;

    QString pluginsDir() const;
    QMap<QString, Plugin *> mappedPlugins() const;
    Plugin * plugin(const QString &pName) const;

    QSettings * settings() const;

    PluginInterface::Version interfaceVersion() const;

private:
    QSettings *mSettings;

    PluginInterface::Version mInterfaceVersion;
    PluginInfo::PluginType mGuiOrConsoleType;

    QString mPluginsDir;
    QMap<QString, Plugin *> mPlugins;
};

}

#endif
