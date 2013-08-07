//==============================================================================
// Plugin manager
//==============================================================================

#include "plugin.h"
#include "coreinterface.h"
#include "pluginmanager.h"

//==============================================================================

#include <QCoreApplication>
#include <QDir>

//==============================================================================

namespace OpenCOR {

//==============================================================================

PluginManager::PluginManager(QCoreApplication *pApp, const bool &pGuiVersion) :
    mInterfaceVersion(PluginInfo::InterfaceVersion001),
    mPlugins(Plugins())
{
    mPluginsDir =  QDir(pApp->applicationDirPath()).canonicalPath()
                  +QDir::separator()+QString("..")
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
                  +QDir::separator()+"plugins"
#elif defined(Q_OS_MAC)
                  +QDir::separator()+"PlugIns"
#else
    #error Unsupported platform
#endif
                  +QDir::separator()+pApp->applicationName();

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    // The plugins directory should be correct, but in case we try to run
    // OpenCOR on Windows or Linux AND from within Qt Creator, then the binary
    // will be running from [OpenCOR]/build/OpenCOR[.exe] rather than
    // [OpenCOR]/build/bin/OpenCOR[.exe] as it should if we were to mimic the
    // case where OpenCOR has been deployed. Then, because the plugins are in
    // [OpenCOR]/build/plugins/OpenCOR, we must skip the "../" bit. Yes, it's
    // not neat, but... is there another solution?...

    if (!QDir(mPluginsDir).exists())
        mPluginsDir =  QDir(pApp->applicationDirPath()).canonicalPath()
                      +QDir::separator()+"plugins"
                      +QDir::separator()+pApp->applicationName();
#endif

    mPluginsDir = QDir::toNativeSeparators(QDir(mPluginsDir).canonicalPath());

    // Retrieve the list of plugins available for loading

    QFileInfoList fileInfoList = QDir(mPluginsDir).entryInfoList(QStringList("*"+PluginExtension),
                                                                 QDir::Files);

    QStringList fileNames = QStringList();

    foreach (const QFileInfo &file, fileInfoList)
        fileNames << QDir::toNativeSeparators(file.canonicalFilePath());

    // Unmanageable plugins (e.g. the QScintilla plugin) don't, by default, get
    // loaded, but the situation is obviously different if such a plugin is
    // required by another plugin (e.g. the Viewer plugin requires the Qwt
    // plugin), in which case the unmanageable plugin must be loaded. So, we
    // must here determine which of those plugins must be loaded...

    QStringList requiredPlugins = QStringList();
    QStringList wantedPlugins = QStringList();

    foreach (const QString &fileName, fileNames) {
        PluginInfo *pluginInfo = Plugin::info(fileName);
        QString pluginName = Plugin::name(fileName);

        if (    pluginInfo
            && (   ( pGuiVersion && pluginInfo->isManageable() && Plugin::load(pluginName))
                || (!pGuiVersion && pluginInfo->hasCliSupport()))) {
            // We want the GUI version of the plugin manager and the plugin is
            // manageable and to be loaded, or we don't want the GUI version of
            // the plugin manager and the plugin has support for CLI, so
            // retrieve and keep track of its dependencies

            requiredPlugins << Plugin::requiredPlugins(mPluginsDir,
                                                       Plugin::name(fileName));

            // Also keep track of the plugin itself

            wantedPlugins << pluginName;
        }

        delete pluginInfo;
    }

    // Remove possible duplicates in our list of required plugins

    requiredPlugins.removeDuplicates();

    // We now have all our required and wanted plugins with our required plugins
    // nicely sorted based on their dependency with one another. So, now, we
    // retrieve the file name associated with all our plugins

    QStringList plugins = requiredPlugins+wantedPlugins;
    QStringList pluginFileNames = QStringList();

    foreach (const QString &plugin, plugins)
        pluginFileNames << Plugin::fileName(mPluginsDir, plugin);

    // If we are dealing with the GUI version of ourselves, then we want to know
    // about all the plugins, including the ones that are not to be loaded (so
    // that we can refer to them, in the GUI, as either not wanted or not
    // needed)

    if (pGuiVersion) {
        pluginFileNames << fileNames;

        pluginFileNames.removeDuplicates();
    }

    // Deal with all the plugins we found

    foreach (const QString &pluginFileName, pluginFileNames)
        mPlugins << new Plugin(pluginFileName,
                               plugins.contains(Plugin::name(pluginFileName)),
                               interfaceVersion(), pluginsDir(), this);
}

//==============================================================================

PluginManager::~PluginManager()
{
    // Delete all of the plugins

    foreach (Plugin *plugin, mPlugins)
        delete plugin;
}

//==============================================================================

Plugins PluginManager::plugins() const
{
    // Return a list of all our plugins, whether loaded

    return mPlugins;
}

//==============================================================================

Plugins PluginManager::loadedPlugins() const
{
    // Return a list of only our loaded plugins

    Plugins res = Plugins();

    foreach (Plugin *plugin, mPlugins)
        if (plugin->status() == Plugin::Loaded)
            res << plugin;

    return res;
}

//==============================================================================

Plugins PluginManager::loadedCliPlugins() const
{
    // Return a list of only our loaded CLI plugins

    Plugins res = Plugins();

    foreach (Plugin *plugin, mPlugins)
        if (   (plugin->info()->hasCliSupport())
            && (plugin->status() == Plugin::Loaded))
            res << plugin;

    return res;
}

//==============================================================================

QString PluginManager::pluginsDir() const
{
    // Return the plugins directory

    return mPluginsDir;
}

//==============================================================================

Plugin * PluginManager::plugin(const QString &pName) const
{
    // Return the plugin which name is the one we have been passed

    foreach (Plugin *plugin, mPlugins)
        if (!pName.compare(plugin->name()))
            // This is the plugin we are after, so...

            return plugin;

    // The plugin we are after wasn't found, so...

    return 0;
}

//==============================================================================

PluginInfo::InterfaceVersion PluginManager::interfaceVersion() const
{
    // Return the interface version used by the plugin manager

    return mInterfaceVersion;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
