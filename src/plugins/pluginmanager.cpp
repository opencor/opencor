#include "plugin.h"
#include "plugininterface.h"
#include "pluginmanager.h"

#include <QApplication>
#include <QDir>
#include <QPluginLoader>

namespace OpenCOR {

PluginManager::PluginManager(const PluginInfo::PluginType &pGuiOrConsoleType) :
    mGuiOrConsoleType(pGuiOrConsoleType)
{
    mPluginsDir =  QDir(qApp->applicationDirPath()).canonicalPath()
                  +QDir::separator()+QString("..")
#ifndef Q_WS_MAC
                  +QDir::separator()+"plugins"
#else
                  +QDir::separator()+"PlugIns"
#endif
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
                      +QDir::separator()+"plugins"
                      +QDir::separator()+qApp->applicationName();
#endif

    // Retrieve the list of plugins available for loading

    QFileInfoList fileInfoList = QDir(mPluginsDir).entryInfoList(QStringList("*"+PluginExtension),
                                                                 QDir::Files);

    QStringList fileNames;

    foreach (const QFileInfo &file, fileInfoList)
        fileNames << QDir::toNativeSeparators(file.canonicalFilePath());

    // Self-contained plugins (e.g. the Core plugin) don't, by default, get
    // loaded, but the situation is obviously different if such a plugin is
    // required by another plugin (e.g. the Help plugin requires the Core
    // plugin), in which case the self-contained plugin should be loaded. So, we
    // must here determine which of those plugins need to be loaded...

    QStringList plugins;

    foreach (const QString &fileName, fileNames)
        plugins << requiredPlugins(fileName);

    plugins.removeDuplicates();

    // Try to load all the plugins we can find, but only if nothing has been
    // done about plugins before

    foreach (const QString &fileName, fileNames)
        mPlugins.insert(Plugin::name(fileName),
                        new Plugin(fileName,
                                   mGuiOrConsoleType,
                                   plugins.contains(Plugin::name(fileName))));
}

PluginManager::~PluginManager()
{
    // Delete all of the plugins

    while (!mPlugins.isEmpty()) {
        delete mPlugins.begin().value();

        mPlugins.erase(mPlugins.begin());
    }
}

QList<Plugin *> PluginManager::plugins(const bool &pOnlyLoadedPlugins)
{
    // Return a list of all the plugins

    QList<Plugin *> res;

    QMap<QString, Plugin *>::const_iterator iter = mPlugins.begin();

    while (iter != mPlugins.constEnd()) {
        if (   !pOnlyLoadedPlugins
            || (   pOnlyLoadedPlugins
                && (iter.value()->status() == Plugin::Loaded)))
        res << iter.value();

        ++iter;
    }

    return res;
}

QList<Plugin *> PluginManager::loadedPlugins()
{
    // Return a list of all the loaded plugins

    return plugins(true);
}

Plugin * PluginManager::plugin(const QString &pName)
{
    // Return the plugin which name is that we have been passed

    return mPlugins.value(pName);
}

QStringList PluginManager::requiredPlugins(const QString &pFileName,
                                           const int &pLevel)
{
    // Return the list of plugins required by a given plugin

    QStringList res;

    // Recursively look for the plugins required by the current plugin

    foreach(const QString &plugin, Plugin::info(pFileName).dependencies())
        res << requiredPlugins(Plugin::fileName(mPluginsDir, plugin), pLevel+1);

    // Add the current plugin to the list if it is not the original plugin for
    // which we want to know what its requirements are

    if (pLevel)
        res << Plugin::name(pFileName);

    return res;
}

}
