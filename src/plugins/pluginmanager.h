#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "plugininterface.h"

#include <QMap>
#include <QObject>
#include <QSettings>

class Plugin;

namespace OpenCOR {

#ifdef Q_WS_WIN
    static const QString PluginPrefix = "";
    static const QString PluginExtension = ".dll";
#else
    static const QString PluginPrefix = "lib";

    #ifdef Q_WS_MAC
        static const QString PluginExtension = ".dylib";
    #else
        static const QString PluginExtension = ".so";
    #endif
#endif

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
    Plugin * plugin(const QString &pName) const;

    QSettings * settings() const;

    QStringList requiredPlugins(const QString &pFileName,
                                const int &pLevel = 0) const;

private:
    QSettings *mSettings;

    PluginInfo::PluginType mGuiOrConsoleType;

    QString mPluginsDir;
    QMap<QString, Plugin *> mPlugins;
};

}

#endif
