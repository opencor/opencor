#include "plugin.h"
#include "plugininterface.h"
#include "pluginmanager.h"

#include <QApplication>
#include <QDir>
#include <QSettings>

#include <QDebug>

namespace OpenCOR {

PluginManager::PluginManager(QSettings *pSettings,
                             const PluginInfo::PluginType &pGuiOrConsoleType) :
    mSettings(pSettings),
    mGuiOrConsoleType(pGuiOrConsoleType)
{
#ifndef Q_WS_MAC
    static const QString pluginsDir = "plugins";
#else
    static const QString pluginsDir = "PlugIns";
#endif

    mPluginsDir =  QDir(qApp->applicationDirPath()).canonicalPath()
                  +QDir::separator()+QString("..")
                  +QDir::separator()+pluginsDir
                  +QDir::separator()+qApp->applicationName();

#ifndef Q_WS_MAC
    // The plugins directory should be correct, but in case we try to run
    // OpenCOR on Windows or Linux AND from within Qt Creator, then the binary
    // will be running from [OpenCOR]/build/OpenCOR[.exe] rather than
    // [OpenCOR]/build/bin/OpenCOR[.exe] as it should if we were to mimic the
    // case where OpenCOR has been deployed. Then, because the plugins are in
    // [OpenCOR]/build/plugins/OpenCOR, we must skip the "../" bit. Yes, it's
    // not neat, but... is there another solution?...

    if (!QDir(mPluginsDir).exists())
        mPluginsDir =  QDir(qApp->applicationDirPath()).canonicalPath()
                      +QDir::separator()+pluginsDir
                      +QDir::separator()+qApp->applicationName();
#endif
}

PluginManager::~PluginManager()
{
    // Remove all the plugins

    while (!mPlugins.isEmpty()) {
        delete mPlugins.begin().value();

        mPlugins.erase(mPlugins.begin());
    }
}

Plugin * PluginManager::plugin(const QString &pPluginName)
{
    // Check whether the plugin exists

    Plugin *plugin = mPlugins.value(pPluginName);

    if (!plugin) {
        // The plugin doesn't exist, so create it

        plugin = new Plugin(pPluginName);

        mPlugins.insert(pPluginName, plugin);
    }

    // Return the plugin

    return plugin;
}

void PluginManager::loadPlugin(const QString &pPluginFileName)
{
}

void PluginManager::loadPlugins()
{
    // Retrieve the file name of all the plugins

#ifdef Q_WS_WIN
    static const QString extension = ".dll";
#elif defined(Q_WS_MAC)
    static const QString extension = ".dylib";
#else
    static const QString extension = ".so";
#endif

    QFileInfoList files = QDir(mPluginsDir).entryInfoList(QStringList("*"+extension), QDir::Files);

    QStringList pluginFileNames;

    foreach (const QFileInfo &file, files)
        pluginFileNames << file.canonicalFilePath();

    // Try to load all the plugins

    while (pluginFileNames.count()) {
        // Retrieve the plugin information

        QString pluginFileName = pluginFileNames.first();

        pluginFileNames.removeFirst();

        PluginInfo pluginInfo = Plugin::pluginInfo(pluginFileName);

        if (   (pluginInfo.type == PluginInfo::General)
            || (pluginInfo.type == mGuiOrConsoleType)) {
            // The file is either a general or GUI plugin, so we can try to load
            // it

qDebug() << "    OK:" << Plugin::pluginName(pluginFileName);

//---GRY--- TO BE DONE...
        }
else
qDebug() << "NOT OK:" << Plugin::pluginName(pluginFileName);
    }
}

}
