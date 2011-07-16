#include "plugin.h"
#include "plugininterface.h"
#include "pluginmanager.h"

#include <QApplication>
#include <QDir>
#include <QPluginLoader>

namespace OpenCOR {

PluginManager::PluginManager(QSettings *pSettings,
                             const PluginInfo::PluginType &pGuiOrConsoleType) :
    mSettings(pSettings),
    mPluginInterfaceVersion(PluginInterface::V001),
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

    // Unmanageable plugins (e.g. the QScintilla plugin) don't, by default, get
    // loaded, but the situation is obviously different if such a plugin is
    // required by another plugin (e.g. the Viewer plugin requires the
    // QtMmlWidget plugin), in which case the unmanageable plugin must be
    // loaded. So, we must here determine which of those plugins must be
    // loaded...

    QStringList plugins;

    foreach (const QString &fileName, fileNames)
        if (   Plugin::info(fileName).manageable()
            && Plugin::load(mSettings, Plugin::name(fileName)))
            // The plugin is manageable and to be loaded, so retrieve its
            // dependencies

            plugins << Plugin::requiredPlugins(mPluginsDir,
                                               Plugin::name(fileName));

    plugins.removeDuplicates();

    // Knowing which plugins are required, we must now ensure that these are
    // loaded first. Note that this is not required on Windows (even though it
    // clearly doesn't harm having them loaded first!), but on Linux and Mac OS
    // X it certainly is since otherwise the plugin's status will be wrong
    // (indeed, on those systems, we check that dependencies are first loaded
    // before loading the plugin itself), so...

    QStringList orderedFileNames;

    foreach (const QString &plugin, plugins)
        orderedFileNames << Plugin::fileName(mPluginsDir, plugin);

    orderedFileNames << fileNames;

    orderedFileNames.removeDuplicates();

    // Deal with all the plugins we found

    foreach (const QString &fileName, orderedFileNames)
        mPlugins.insert(Plugin::name(fileName),
                        new Plugin(this, fileName, mGuiOrConsoleType,
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

QList<Plugin *> PluginManager::plugins(const bool &pOnlyLoadedPlugins) const
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

QList<Plugin *> PluginManager::loadedPlugins() const
{
    // Return a list of all the loaded plugins

    return plugins(true);
}

QString PluginManager::pluginsDir() const
{
    // Return the plugins directory

    return mPluginsDir;
}

Plugin * PluginManager::plugin(const QString &pName) const
{
    // Return the plugin which name is that we have been passed

    return mPlugins.value(pName);
}

PluginInterface::PluginInterfaceVersion PluginManager::pluginInterfaceVersion() const
{
    // Return the plugin interface version used by the plugin manager

    return mPluginInterfaceVersion;
}

QString PluginManager::pluginInterfaceVersionAsString(const PluginInterface::PluginInterfaceVersion &pPluginInterfaceVersion)
{
    // Return the passed plugin interface version as a string
    // Note: ideally, this function would be part of the PluginInterface class,
    //       but this class is used by all the plugins and because this method
    //       requires a translation, well... we can't have it there since a
    //       translation will otherwise be required for each plugin, so...
    //       another solution is to have it here...

    switch (pPluginInterfaceVersion) {
    case PluginInterface::V001:
        return tr("Version 001");
    default:
        return tr("Unknown version");
    }
}

QSettings * PluginManager::settings() const
{
    // Return the settings object

    return mSettings;
}

}
