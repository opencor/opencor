#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "plugininterface.h"

#include <QMap>
#include <QObject>

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
    explicit PluginManager(const PluginInfo::PluginType &pGuiOrConsoleType);
    ~PluginManager();

private:
    PluginInfo::PluginType mGuiOrConsoleType;

    QMap<QString, Plugin *> mPlugins;

    Plugin * plugin(const QString &pFileName);
};

}

#endif
