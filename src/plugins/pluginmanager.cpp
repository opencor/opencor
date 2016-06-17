/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Plugin manager
//==============================================================================

#ifndef OpenCOR_MAIN
    #include "corecliutils.h"
#else
    #include "cliutils.h"
#endif
#include "plugin.h"
#include "pluginmanager.h"

//==============================================================================

#include <QCoreApplication>
#include <QDir>

//==============================================================================

namespace OpenCOR {

//==============================================================================

PluginManager::PluginManager(const bool &pGuiMode) :
    mPlugins(Plugins()),
    mLoadedPlugins(Plugins()),
    mCorePlugin(0)
{
    // Retrieve OpenCOR's plugins directory
    // Note: the plugin's directory is retrieved in main()...

    mPluginsDir = QCoreApplication::libraryPaths().first()+QDir::separator()+qAppName();

    // Retrieve the list of plugins available for loading

    QFileInfoList fileInfoList = QDir(mPluginsDir).entryInfoList(QStringList("*"+PluginExtension),
                                                                 QDir::Files);

    QStringList fileNames = QStringList();

    foreach (const QFileInfo &fileInfo, fileInfoList)
#ifndef OpenCOR_MAIN
        fileNames << Core::nativeCanonicalFileName(fileInfo.canonicalFilePath());
#else
        fileNames << nativeCanonicalFileName(fileInfo.canonicalFilePath());
#endif

    // Retrieve and initialise some information about the plugins

    QMap<QString, PluginInfo *> pluginsInfo = QMap<QString, PluginInfo *>();
    QMap<QString, QString> pluginsError = QMap<QString, QString>();

    foreach (const QString &fileName, fileNames) {
        QString pluginError;
        PluginInfo *pluginInfo = Plugin::info(fileName, &pluginError);
        // Note: if there is some plugin information, then it will get owned by
        //       the plugin itself. So, it's the plugin's responsibility to
        //       delete it (see Plugin::~Plugin())...
        QString pluginName = Plugin::name(fileName);

        pluginsInfo.insert(pluginName, pluginInfo);
        pluginsError.insert(pluginName, pluginError);

        // Keep track of the plugin's full dependencies, if possible

        if (pluginInfo)
            pluginInfo->setFullDependencies(Plugin::fullDependencies(mPluginsDir, pluginName));
    }

    // Determine in which order the plugins files should be analysed (i.e. take
    // into account the result of a plugin's loadBefore() function)

    QStringList sortedFileNames = QStringList();

    foreach (const QString &fileName, fileNames) {
        PluginInfo *pluginInfo = pluginsInfo.value(Plugin::name(fileName));

        if (pluginInfo) {
            int index = sortedFileNames.count();

            foreach (const QString &loadBefore, pluginInfo->loadBefore()) {
                int loadBeforeIndex = sortedFileNames.indexOf(Plugin::fileName(mPluginsDir, loadBefore));

                if (loadBeforeIndex < index)
                    index = loadBeforeIndex;
            }

            sortedFileNames.insert(index, fileName);
        }
    }

    // Determine which plugins, if any, are needed by others and which, if any,
    // are selectable

    QStringList neededPlugins = QStringList();
    QStringList wantedPlugins = QStringList();

    foreach (const QString &fileName, sortedFileNames) {
        QString pluginName = Plugin::name(fileName);
        PluginInfo *pluginInfo = pluginsInfo.value(pluginName);

        if (pluginInfo) {
            // Keep track of the plugin itself, should it be selectable and
            // requested by the user (if we are in GUI mode) or have CLI support
            // (if we are in CLI mode)

            if (   ( pGuiMode && pluginInfo->isSelectable() && Plugin::load(pluginName))
                || (!pGuiMode && pluginInfo->hasCliSupport())) {
                // Keep track of the plugin's dependencies

                neededPlugins << pluginsInfo.value(pluginName)->fullDependencies();

                // Also keep track of the plugin itself

                wantedPlugins << pluginName;
            }
        }
    }

    // We now have all our needed and wanted plugins with our needed plugins
    // nicely sorted based on their dependencies with one another. So, retrieve
    // their file name

    QStringList plugins = neededPlugins+wantedPlugins;
    QStringList pluginFileNames = QStringList();

    plugins.removeDuplicates();
    // Note: if anything, there should only be duplicates in neededPlugins, and
    //       not between neededPlugins and wantedPlugins. Then again, we better
    //       be safe than sorry since a selectable plugin (i.e. listed in
    //       wantedPlugins) might be (wrongly) needed by another plugin (i.e.
    //       listed in neededPlugins)...

    foreach (const QString &plugin, plugins)
        pluginFileNames << Plugin::fileName(mPluginsDir, plugin);

    // If we are in GUI mode, then we want to know about all the plugins,
    // including the ones that are not to be loaded (so that we can refer to
    // them, in the plugins window, as either not wanted or not needed)

    if (pGuiMode) {
        pluginFileNames << fileNames;

        pluginFileNames.removeDuplicates();
    }

    // Deal with all the plugins we need and want

    foreach (const QString &pluginFileName, pluginFileNames) {
        QString pluginName = Plugin::name(pluginFileName);

        Plugin *plugin = new Plugin(pluginFileName,
                                    pluginsInfo.value(pluginName),
                                    pluginsError.value(pluginName),
                                    plugins.contains(pluginName), this);

        // Keep track of the plugin and of the Core plugin, in particular, if it
        // is loaded

        mPlugins << plugin;

        if (plugin->status() == Plugin::Loaded) {
            mLoadedPlugins << plugin;

            if (!pluginName.compare(CorePluginName))
                mCorePlugin = plugin;
        }
    }
}

//==============================================================================

PluginManager::~PluginManager()
{
    // Delete all our plugins

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
    // Return a list of our loaded plugins

    return mLoadedPlugins;
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
    // Return the plugin, if any, which name is the one we have been passed

    foreach (Plugin *plugin, mPlugins) {
        if (!pName.compare(plugin->name()))
            return plugin;
    }

    return 0;
}

//==============================================================================

Plugin * PluginManager::corePlugin() const
{
    // Return our Core plugin

    return mCorePlugin;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
