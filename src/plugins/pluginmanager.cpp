#include "plugin.h"
#include "plugininterface.h"
#include "pluginmanager.h"

#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>

#include <QMessageBox>

namespace OpenCOR {

PluginManager::PluginManager(QSettings *pSettings) :
    mSettings(pSettings)
{
#ifndef Q_WS_MAC
    const QString pluginsDir = "plugins";
#else
    const QString pluginsDir = "PlugIns";
#endif

    mPluginsDir =  QDir(qApp->applicationDirPath()).canonicalPath()
                  +QDir::separator()+QString("..")
                  +QDir::separator()+pluginsDir
                  +QDir::separator()+qApp->applicationName();

#ifndef Q_WS_MAC
    // The plugins directory should be correct, but in case we try to run
    // OpenCOR on Windows or Linux AND from within Qt Creator, then the binary
    // will be running [OpenCOR]/build/OpenCOR[.exe] rather than
    // [OpenCOR]/build/bin/OpenCOR[.exe] as we should if we were to mimic the
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

    foreach (Plugin *plugin, mPlugins)
        delete plugin;
}

void PluginManager::loadPlugin(const QString &pPluginFileName)
{
    // Check what type of plugin we are dealing with and what its dependencies
    // are, if any

    typedef PluginInfo (*PluginInfoFunc)();

#ifndef Q_WS_WIN
    const QString pluginName = QFileInfo(pPluginFileName).baseName().remove(0, 3);
    // Note: remove the "lib" part of the plugin file name...
#else
    const QString pluginName = QFileInfo(pPluginFileName).baseName();
#endif

    PluginInfoFunc pluginInfoFunc = (PluginInfoFunc) QLibrary::resolve(pPluginFileName,
                                                                       QString(pluginName+"PluginInfo").toLatin1().constData());
    if (pluginInfoFunc) {
        // The plugin information function was found, so extract the information
        // we are after

        PluginInfo pluginInfo = pluginInfoFunc();

        QString type;
        QString dependencies;

        switch (pluginInfo.type) {
        case Console:
            type = "console";

            break;
        case Gui:
            type = "GUI";

            break;
        default:
            type = "both console and GUI";

            break;
        }

        if (!pluginInfo.dependencies.count()) {
            dependencies = "none";
        } else {
            foreach (QString dependency, pluginInfo.dependencies)
                dependencies += dependency+" | ";

            dependencies.chop(3);
        }

        QMessageBox::information(0, "Plugin info",
                                 QString("Plugin name: %1\nType: %2\nDependencies: %3").arg(pluginName,
                                                                                            type,
                                                                                            dependencies));
    } else {
        QMessageBox::information(0, "Plugin info",
                                 QString("The %1 plugin CANNOT be loaded").arg(pluginName));
    }
}

void PluginManager::loadPlugins()
{
    // Try to load all the plugins

#ifdef Q_WS_WIN
    const QString extension = ".dll";
#elif defined(Q_WS_MAC)
    const QString extension = ".dylib";
#else
    const QString extension = ".so";
#endif

    QFileInfoList files = QDir(mPluginsDir).entryInfoList(QStringList("*"+extension), QDir::Files);

    foreach (const QFileInfo &file, files)
        loadPlugin(file.canonicalFilePath());
}

}
