#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "plugininterface.h"

#include <QMap>
#include <QObject>

class Plugin;

class QSettings;

namespace OpenCOR {

#ifdef Q_WS_WIN
    static const QString PluginPrefix = "";
    static const QString PluginExtension = ".dll";
#else
    static const QString PluginPrefix = "lib";

    #ifdef(Q_WS_MAC)
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

    void loadPlugins();

private:
    QSettings *mSettings;

    PluginInfo::PluginType mGuiOrConsoleType;

    QString mPluginsDir;

    QMap<QString, Plugin *> mPlugins;

    Plugin *plugin(const QString &pPluginName);

    void loadPlugin(const QString &pPluginFileName);
};

}

#endif
