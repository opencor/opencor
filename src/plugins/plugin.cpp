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
// Plugin
//==============================================================================

#include "plugin.h"
#include "pluginmanager.h"

//==============================================================================

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QSettings>

//==============================================================================

namespace OpenCOR {

//==============================================================================

Plugin::Plugin(const QString &pFileName, PluginInfo *pInfo, const bool &pLoad,
               PluginManager *pPluginManager) :
    mName(name(pFileName)),
    mInfo(pInfo),
    mInstance(0),
    mStatus(UndefinedStatus)
{
    if (pInfo) {
        // We are dealing with a plugin, so try to load it, but only if the user
        // wants

        if (pLoad) {
            // Make sure that the plugin's dependencies, if any, are loaded
            // before loading the plugin itself
            // Note: normally, we would only do this on non-Windows systems
            //       since, on Windows, a shared library's dependencies must be
            //       loaded before the shared library itself can be loaded,
            //       while on Linux / OS X, it's possible to load a shared
            //       library even if its dependencies are not loaded. Still, it
            //       doesn't harm doing the same on Linux / OS X, so...

            bool pluginDependenciesLoaded = true;

            mStatusErrors = "";

            foreach (const QString &dependency, pInfo->dependencies()) {
                Plugin *pluginDependency = pPluginManager->plugin(dependency);

                if (   !pluginDependency
                    || (   pluginDependency
                        && (pluginDependency->status() != Loaded))) {
                    // Either the plugin dependency couldn't be found or it
                    // could be found but it isn't loaded, so...

                    pluginDependenciesLoaded = false;

                    mStatus = MissingOrInvalidDependencies;

                    if (!mStatusErrors.isEmpty())
                        mStatusErrors += "\n";

                    mStatusErrors += " - "+dependency;
                }
            }

            if (statusErrorsCount() == 1)
                // There is only one error, so remove the leading " - "

                mStatusErrors = mStatusErrors.remove(0, 3);

            // Check whether all of the plugin's dependencies, if any, were
            // loaded, and if so then try to load the plugin itself

            if (pluginDependenciesLoaded) {
                // All the plugin's dependencies, if any, were loaded, so try to
                // load the plugin itself

                QPluginLoader pluginLoader(pFileName);

                if (pluginLoader.load()) {
                    mInstance = pluginLoader.instance();

                    mStatus = Loaded;
                } else {
                    mStatus = NotLoaded;
                    mStatusErrors = pluginLoader.errorString();
                }
            }
        } else {
            // The plugin is not to be loaded, which means that it is either not
            // wanted or not needed, depending on whether it is manageable

            if (pInfo->isManageable())
                mStatus = NotWanted;
            else
                mStatus = NotNeeded;
        }
    } else {
       // What we thought was a plugin is not actually a plugin...

       mStatus = NotPlugin;
    }
}

//==============================================================================

Plugin::~Plugin()
{
    // Delete some internal objects

    delete mInfo;
}

//==============================================================================

QString Plugin::name() const
{
    // Return the plugin's name

    return mName;
}

//==============================================================================

PluginInfo * Plugin::info() const
{
    // Return the plugin's information

    return mInfo;
}

//==============================================================================

QObject * Plugin::instance() const
{
    // Return the plugin's instance

    return mInstance;
}

//==============================================================================

Plugin::Status Plugin::status() const
{
    // Return the plugin's status

    return mStatus;
}

//==============================================================================

QString Plugin::statusErrors() const
{
    // Return the plugin's status errors

    return mStatusErrors;
}

//==============================================================================

int Plugin::statusErrorsCount() const
{
    // Return the number of plugin's status errors

    return mStatusErrors.count("\n")+1;
}

//==============================================================================

QString Plugin::name(const QString &pFileName)
{
    // Return the plugin's name based on its file name

    return QFileInfo(pFileName).baseName().remove(0, strlen(PluginPrefix));
    // Note: we must remove the plugin prefix part from the plugin file name...
}

//==============================================================================

QString Plugin::fileName(const QString &pPluginsDir, const QString &pName)
{
    // Return the plugin's file name based on its name

    return pPluginsDir+QDir::separator()+QString(PluginPrefix)+pName+QString(PluginExtension);
    // Note: we must add the plugin prefix part to the plugin file name...
}

//==============================================================================

PluginInfo * Plugin::info(const QString &pFileName)
{
    // Return the plugin's information
    // Note: to retrieve a plugin's information, we must, on both Windows and
    //       Linux, be able to load any plugin on which a plugin depends. On
    //       Windows, we do this (by keeping track of the current directory
    //       and) by going to the directory where our plugins are located.
    //       However, this approach doesn't work on Linux, so instead we rely
    //       on RPATH to get the result we are after...

    typedef void * (*PluginInfoFunc)();

#ifdef Q_OS_WIN
    QString origPath = QDir::currentPath();

    QDir::setCurrent(QFileInfo(pFileName).absolutePath());
#endif

    PluginInfoFunc pluginInfoFunc = (PluginInfoFunc) QLibrary::resolve(pFileName,
                                                                       qPrintable(name(pFileName)+"PluginInfo"));

#ifdef Q_OS_WIN
    QDir::setCurrent(origPath);
#endif

    // Check whether the plugin information function was found

    if (pluginInfoFunc)
        // The plugin information function was found, so we can extract the
        // information we are after

        return static_cast<PluginInfo *>(pluginInfoFunc());
    else
        // The plugin information couldn't be found, so...

        return 0;
}

//==============================================================================

static const char *SettingsLoad = "Load";

//==============================================================================

bool Plugin::load(const QString &pName)
{
    // Retrieve the plugin's loading requirement
    // Note: the plugin's loading requirement information is always located
    //       under ~/Plugins/<PluginName>, so to be on the safe side we use our
    //       own QSettings rather than that of MainWindow since the latter might
    //       not point to ~ when reaching this point. Indeed, we may come here
    //       from PluginManager::PluginManager (which points to ~) or
    //       PluginsWindow::PluginsWindow (which points to ~/PluginsWindow),
    //       so...

    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsPlugins);
        settings.beginGroup(pName);
            bool res = settings.value(SettingsLoad, true).toBool();
        settings.endGroup();
    settings.endGroup();

    return res;
}

//==============================================================================

void Plugin::setLoad(const QString &pName, const bool &pToBeLoaded)
{
    // Keep track of the plugin's loading requirement
    // Note: see the Plugin::load's note...

    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsPlugins);
        settings.beginGroup(pName);
            settings.setValue(SettingsLoad, pToBeLoaded);
        settings.endGroup();
    settings.endGroup();
}

//==============================================================================

QStringList Plugin::fullDependencies(const QString &pPluginsDir,
                                     const QString &pName, const int &pLevel)
{
    // Return the given plugin's full dependencies

    QStringList res = QStringList();

    // Recursively look for the plugin's full dependencies

    PluginInfo *pluginInfo = Plugin::info(Plugin::fileName(pPluginsDir, pName));

    if (!pluginInfo)
        return res;

    foreach (const QString &plugin, pluginInfo->dependencies())
        res << fullDependencies(pPluginsDir, plugin, pLevel+1);

    delete pluginInfo;

    // Add the current plugin to the list, but only if it is not the original
    // plugin, otherwise remove any duplicates

    if (pLevel)
        res << pName;
    else
        res.removeDuplicates();

    return res;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
