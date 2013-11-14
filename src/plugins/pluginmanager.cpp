/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Plugin manager
//==============================================================================

#include "cliinterface.h"
#include "coreinterface.h"
#include "plugin.h"
#include "pluginmanager.h"

//==============================================================================

#include <QCoreApplication>
#include <QDir>

//==============================================================================

namespace OpenCOR {

//==============================================================================

PluginManager::PluginManager(QCoreApplication *pApp, const bool &pGuiMode) :
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
    // [OpenCOR]/build/plugins/OpenCOR, we must skip the "../" bit. So, yes,
    // it's not neat, but... is there another solution?...

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

    // Determine which plugins, if any, are needed by others

    QMap<QString, PluginInfo *> pluginsInfo = QMap<QString, PluginInfo *>();

    QStringList validPlugins = QStringList();
    QStringList neededPlugins = QStringList();

    foreach (const QString &fileName, fileNames) {
        PluginInfo *pluginInfo = Plugin::info(fileName);
        // Note: if there is some plugin information, then it will get owned by
        //       the plugin itself and it will therefore be its responsibility
        //       to delete it (see Plugin::~Plugin())...
        QString pluginName = Plugin::name(fileName);

        pluginsInfo.insert(pluginName, pluginInfo);

        if (pluginInfo) {
            // Keep track of the plugin's full dependencies

            QStringList pluginFullDependencies = Plugin::fullDependencies(mPluginsDir, pluginName);

            pluginInfo->setFullDependencies(pluginFullDependencies);

            neededPlugins << pluginFullDependencies;

            // Keep track of the plugin itself

            validPlugins << pluginName;
        }
    }

    neededPlugins.removeDuplicates();

    // Determine which of our available plugins, if any, are manageable, i.e.
    // not used by any other plugin

    QStringList manageablePlugins = QStringList();

    foreach (const QString &validPlugin, validPlugins)
        if (!neededPlugins.contains(validPlugin)) {
            pluginsInfo.value(validPlugin)->setManageable(true);

            manageablePlugins << validPlugin;
        }

    // Determine which plugins are needed or wanted
    // Note: unmanageable plugins (e.g. the QScintilla plugin) don't get loaded
    //       by default, but the situation is obviously different if such a
    //       plugin is needed by another plugin (e.g. the Viewer plugin requires
    //       the Qwt plugin), in which case the unmanageable plugin must be
    //       loaded...

    QStringList wantedPlugins = QStringList();
    neededPlugins = QStringList();

    foreach (const QString &manageablePlugin, manageablePlugins)
        if ((pGuiMode && Plugin::load(manageablePlugin)) || !pGuiMode) {
            // We are in GUI mode and the user wants to load the plugin, or we
            // are not in GUI mode, so retrieve and keep track of the plugin's
            // dependencies
            // Note: in non-GUI mode (i.e. CLI mode), a manageable plugin gets
            //       automatically loaded no matter what, thus making sure that
            //       the CLI version of OpenCOR has access to all the plugins.
            //       The drawback with this approach is that non-CLI capable
            //       plugins will also be loaded, but there is not much we can
            //       do about it. Actually, we could have a boolean in the
            //       plugin information to tell us whether a plugin is
            //       CLI-capable, but this is error-prone (i.e. what would
            //       happen if the developer of a CLI-capable plugin was to
            //       forget to turn that flag on?). So, in the end, it's better
            //       to load all the plugins and then deal with only those that
            //       support the CLI interface...

            neededPlugins << pluginsInfo.value(manageablePlugin)->fullDependencies();

            // Also keep track of the plugin itself

            wantedPlugins << manageablePlugin;
        }

    // Remove possible duplicates in our list of needed plugins

    neededPlugins.removeDuplicates();

    // We now have all our needed and wanted plugins with our needed plugins
    // nicely sorted based on their dependencies with one another. So, retrieve
    // their file name

    QStringList plugins = neededPlugins+wantedPlugins;
    QStringList pluginFileNames = QStringList();

    foreach (const QString &plugin, plugins)
        pluginFileNames << Plugin::fileName(mPluginsDir, plugin);

    // If we are dealing with the GUI version of ourselves, then we want to know
    // about all the plugins, including the ones that are not to be loaded (so
    // that we can refer to them, in the plugins window, as either not wanted or
    // not needed)

    if (pGuiMode) {
        pluginFileNames << fileNames;

        pluginFileNames.removeDuplicates();
    }

    // Deal with all the plugins we found

    foreach (const QString &pluginFileName, pluginFileNames) {
        QString pluginName = Plugin::name(pluginFileName);

        mPlugins << new Plugin(pluginFileName, pluginsInfo.value(pluginName),
                               plugins.contains(pluginName), this);
    }
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
        if (   qobject_cast<CliInterface *>(plugin->instance())
            && (plugin->status() == Plugin::Loaded)) {
            res << plugin;
        }

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

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
