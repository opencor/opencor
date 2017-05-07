/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Plugin manager
//==============================================================================

#ifdef OpenCOR_MAIN
    #include "cliutils.h"
#else
    #include "corecliutils.h"
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
    mGuiMode(pGuiMode),
    mPlugins(Plugins()),
    mLoadedPlugins(Plugins()),
    mCorePlugin(0)
{
    // Retrieve OpenCOR's plugins directory
    // Note: the plugin's directory is retrieved in main()...

    mPluginsDir = QCoreApplication::libraryPaths().first()+QDir::separator()+qAppName();

    // Retrieve the list of plugins available for loading

    QFileInfoList fileInfoList = QDir(mPluginsDir).entryInfoList(QStringList("*"+PluginExtension), QDir::Files);
    QStringList fileNames = QStringList();

    foreach (const QFileInfo &fileInfo, fileInfoList) {
#ifdef OpenCOR_MAIN
        fileNames << nativeCanonicalFileName(fileInfo.canonicalFilePath());
#else
        fileNames << Core::nativeCanonicalFileName(fileInfo.canonicalFilePath());
#endif
    }

    // Retrieve and initialise some information about the plugins

    QMap<QString, PluginInfo *> pluginsInfo = QMap<QString, PluginInfo *>();
    QMap<QString, QString> pluginsError = QMap<QString, QString>();

    foreach (const QString &fileName, fileNames) {
        QString pluginName = Plugin::name(fileName);
        QString pluginError = QString();
        PluginInfo *pluginInfo = (Plugin::pluginInfoVersion(fileName) == pluginInfoVersion())?
                                     Plugin::info(fileName, &pluginError):
                                     0;

        pluginsInfo.insert(pluginName, pluginInfo);
        pluginsError.insert(pluginName, pluginError);

        // Keep track of the plugin's full dependencies, if possible
        // Note: if there is some plugin information, then it will get owned by
        //       the plugin itself. So, it will be the plugin's responsibility
        //       to delete it (see Plugin::~Plugin())...

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
    // Delete some internal objects

#ifdef OpenCOR_MAIN
    resetList(mPlugins);
#else
    Core::resetList(mPlugins);
#endif
}

//==============================================================================

bool PluginManager::guiMode() const
{
    // Return whether we are in GUI mode

    return mGuiMode;
}

//==============================================================================

Plugins PluginManager::plugins() const
{
    // Return a list of all our plugins, whether loaded or not

    return mPlugins;
}

//==============================================================================

Plugins PluginManager::sortedPlugins() const
{
    // Return a sorted version of mPlugins

    Plugins res = mPlugins;

    std::sort(res.begin(), res.end(), Plugin::compare);

    return res;
}

//==============================================================================

Plugins PluginManager::loadedPlugins() const
{
    // Return a list of our loaded plugins

    return mLoadedPlugins;
}

//==============================================================================

Plugins PluginManager::sortedLoadedPlugins() const
{
    // Return a sorted version of mLoadedPlugins

    Plugins res = mLoadedPlugins;

    std::sort(res.begin(), res.end(), Plugin::compare);

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
