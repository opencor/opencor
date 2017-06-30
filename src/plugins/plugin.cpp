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
// Plugin
//==============================================================================

#include "cliinterface.h"
#include "coreinterface.h"
#include "datastoreinterface.h"
#include "filehandlinginterface.h"
#include "filetypeinterface.h"
#ifndef CLI_VERSION
    #include "guiinterface.h"
    #include "i18ninterface.h"
#endif
#include "plugin.h"
#include "plugininterface.h"
#include "pluginmanager.h"
#ifndef CLI_VERSION
    #include "preferencesinterface.h"
#endif
#include "solverinterface.h"
#ifndef CLI_VERSION
    #include "viewinterface.h"
    #include "windowinterface.h"
#endif

//==============================================================================

#include <QDir>
#include <QFileInfo>
#include <QLibrary>
#include <QPluginLoader>
#include <QSettings>

//==============================================================================

namespace OpenCOR {

//==============================================================================

Plugin::Plugin(const QString &pFileName, PluginInfo *pInfo,
               const QString &pErrorMessage, const bool &pLoad,
               PluginManager *pPluginManager) :
    mName(name(pFileName)),
    mInfo(pInfo),
    mErrorMessage(pErrorMessage),
    mInstance(0)
{
    if (pInfo) {
        // We are dealing with a plugin, so try to load it, but only if the user
        // wants

        if (pLoad) {
            // Make sure that the plugin's dependencies, if any, are loaded
            // before loading the plugin itself
            // Note: normally, we would only do this on Windows systems since,
            //       on Windows, a shared library's dependencies must be loaded
            //       before the shared library itself can be loaded, while on
            //       Linux/macOS, it's possible to load a shared library even if
            //       its dependencies are not loaded. Still, it doesn't harm
            //       doing the same on Linux/macOS...

            bool pluginDependenciesLoaded = true;

            mStatusErrors = QString();

            foreach (const QString &dependency, pInfo->dependencies()) {
                Plugin *pluginDependency = pPluginManager->plugin(dependency);

                if (   !pluginDependency
                    || (   pluginDependency
                        && (pluginDependency->status() != Loaded))) {
                    // Either the plugin dependency couldn't be found or it
                    // could be found but it isn't loaded

                    pluginDependenciesLoaded = false;

                    mStatus = MissingOrInvalidDependencies;

                    if (!mStatusErrors.isEmpty())
                        mStatusErrors += '\n';

                    mStatusErrors +=  (pPluginManager->guiMode()?QString():QString("   "))
                                     +" - "+dependency;
                }
            }

            if (statusErrorsCount() == 1) {
                // There is only one error, so remove the leading " - "

                mStatusErrors = mStatusErrors.remove(0, pPluginManager->guiMode()?3:6);
            }

            // Check whether all of the plugin's dependencies, if any, were
            // loaded, and if so then try to load the plugin itself

            if (pluginDependenciesLoaded) {
                // All the plugin's dependencies, if any, were loaded, so try to
                // load the plugin itself

                QPluginLoader pluginLoader(pFileName);

                if (pluginLoader.load()) {
                    mInstance = pluginLoader.instance();

                    // Make sure that the plugin uses the right version of the
                    // official interfaces it supports

                    bool unloadPlugin = true;

                    CliInterface *cliInterface = qobject_cast<CliInterface *>(mInstance);
                    CoreInterface *coreInterface = qobject_cast<CoreInterface *>(mInstance);
                    PluginInterface *pluginInterface = qobject_cast<PluginInterface *>(mInstance);

                    if (   (   cliInterface
                            && (interfaceVersion(pFileName, "cliInterfaceVersion") != cliInterfaceVersion()))
                        || (   coreInterface
                            && (interfaceVersion(pFileName, "coreInterfaceVersion") != coreInterfaceVersion()))
                        || (   qobject_cast<DataStoreInterface *>(mInstance)
                            && (interfaceVersion(pFileName, "dataStoreInterfaceVersion") != dataStoreInterfaceVersion()))
                        || (   qobject_cast<FileHandlingInterface *>(mInstance)
                            && (interfaceVersion(pFileName, "fileHandlingInterfaceVersion") != fileHandlingInterfaceVersion()))
                        || (   qobject_cast<FileTypeInterface *>(mInstance)
                            && (interfaceVersion(pFileName, "fileTypeInterfaceVersion") != fileTypeInterfaceVersion()))
#ifndef CLI_VERSION
                        || (   qobject_cast<GuiInterface *>(mInstance)
                            && (interfaceVersion(pFileName, "guiInterfaceVersion") != guiInterfaceVersion()))
                        || (   qobject_cast<I18nInterface *>(mInstance)
                            && (interfaceVersion(pFileName, "i18nInterfaceVersion") != i18nInterfaceVersion()))
#endif
                        || (   pluginInterface
                            && (interfaceVersion(pFileName, "pluginInterfaceVersion") != pluginInterfaceVersion()))
#ifndef CLI_VERSION
                        || (   qobject_cast<PreferencesInterface *>(mInstance)
                            && (interfaceVersion(pFileName, "preferencesInterfaceVersion") != preferencesInterfaceVersion()))
#endif
                        || (   qobject_cast<SolverInterface *>(mInstance)
                            && (interfaceVersion(pFileName, "solverInterfaceVersion") != solverInterfaceVersion()))
#ifndef CLI_VERSION
                        || (   qobject_cast<ViewInterface *>(mInstance)
                            && (interfaceVersion(pFileName, "viewInterfaceVersion") != viewInterfaceVersion()))
                        || (   qobject_cast<WindowInterface *>(mInstance)
                            && (interfaceVersion(pFileName, "windowInterfaceVersion") != windowInterfaceVersion()))
#endif
                       ) {
                        // One or several of the official interfaces used by the
                        // plugin are too old

                        mStatus = OldPlugin;
                    } else {
                        // Check for the Core plugin/interface and CLI support

                        if (coreInterface && mName.compare(CorePluginName)) {
                            // We are dealing with a plugin that supports the
                            // Core interface, but it's not the Core plugin

                            mStatus = NotCorePlugin;
                        } else if (!coreInterface && !mName.compare(CorePluginName)) {
                            // We are dealing with the Core plugin, but it
                            // doesn't support the Core interface

                            mStatus = InvalidCorePlugin;
                        } else if (cliInterface && !pInfo->hasCliSupport()) {
                            // We are dealing with a plugin that supports the
                            // CLI interface, but it doesn't claim to have CLI
                            // support

                            mStatus = NotCliPluginNoCliSupport;
                        } else if (!cliInterface && pInfo->hasCliSupport()) {
                            // We are dealing with a plugin that is supposed to
                            // have CLI support, but it doesn't support the CLI
                            // interface

                            mStatus = NotCliPluginNoCliInterface;
                        } else {
                            // So far so good, so now check whether the plugin
                            // defines new interfaces and, if so, keep track of
                            // them

                            static PluginInterfaces pluginsWithInterfaces = PluginInterfaces();

                            if (   pluginInterface
                                && pluginInterface->definesPluginInterfaces()) {
                                pluginsWithInterfaces << pluginInterface;
                            }

                            // Check whether the plugin supports any of the
                            // interfaces defined by the other plugins and, if
                            // so, whether it is of the right version

                            bool pluginInterfacesOk = true;

                            foreach (PluginInterface *pluginWithInterfaces, pluginsWithInterfaces) {
                                if (!pluginWithInterfaces->pluginInterfacesOk(pFileName, mInstance)) {
                                    pluginInterfacesOk = false;

                                    break;
                                }
                            }

                            if (pluginInterfacesOk) {
                                unloadPlugin = false;

                                mStatus = Loaded;
                            } else {
                                mStatus = OldPlugin;
                            }
                        }
                    }

                    if (unloadPlugin) {
                        pluginLoader.unload();

                        mInstance = 0;
                    }
                } else {
                    mStatus = NotLoaded;
                    mStatusErrors = pluginLoader.errorString();
                }
            }
        } else {
            // The plugin is not to be loaded, which means that it is either not
            // wanted or not needed, depending on whether it is selectable

            if (pInfo->isSelectable())
                mStatus = NotWanted;
            else
                mStatus = NotNeeded;
        }
    } else {
        // What we thought was a plugin is not actually a plugin or it is a
        // plugin that uses an old version of PluginInfo, so consider it as
        // invalid...
        // Note: to call Plugin::pluginInfoVersion() to distinguish between the
        //       two cases is not good enough since an old plugin may or not
        //       implement the pluginInfoVersion() method, which means that
        //       Plugin::pluginInfoVersion() would return 0 whether it's a
        //       library that is not a plugin or a plugin that uses an old
        //       version of PluginInfo. So, instead, we call Plugin::info()
        //       since we know it will only work for a plugin that uses an old
        //       version of PluginInfo...

        mStatus = Plugin::info(pFileName)?OldPlugin:NotPlugin;

        if (mStatus == NotPlugin) {
            // Apparently, we are not dealing with a plugin, so load it so that
            // we can retrieve its corresponding error

            QPluginLoader pluginLoader(pFileName);

            pluginLoader.load();

            mStatusErrors = pluginLoader.errorString();
        }
    }
}

//==============================================================================

Plugin::~Plugin()
{
    // Delete some internal objects

    delete mInfo;
}

//==============================================================================

bool Plugin::compare(Plugin *pPlugin1, Plugin *pPlugin2)
{
    // Determine which of the two plugins should be first based on their name
    // Note: the comparison is case insensitive, so that it's easier for people
    //       to find a plugin (when we list them)...

    return pPlugin1->name().compare(pPlugin2->name(), Qt::CaseInsensitive) < 0;
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

QString Plugin::errorMessage() const
{
    // Return the plugin's error message

    return mErrorMessage;
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

    int res = mStatusErrors.count('\n');

    if (res)
        return res+1;
    else
        return !mStatusErrors.isEmpty();
}

//==============================================================================

QString Plugin::name(const QString &pFileName)
{
    // Return the plugin's name based on its file name

    return QFileInfo(pFileName).baseName().remove(0, PluginPrefix.length());
    // Note: we must remove the plugin prefix part from the plugin file name...
}

//==============================================================================

QString Plugin::fileName(const QString &pPluginsDir, const QString &pName)
{
    // Return the plugin's file name based on its name

    return pPluginsDir+QDir::separator()+PluginPrefix+pName+PluginExtension;
    // Note: we must add the plugin prefix part to the plugin file name...
}

//==============================================================================

int Plugin::pluginInfoVersion(const QString &pFileName)
{
    // Return the version of PluginInfo used by the plugin

    typedef int (*PluginInfoVersionFunc)();

#ifdef Q_OS_WIN
    QString origPath = QDir::currentPath();

    QDir::setCurrent(QFileInfo(pFileName).absolutePath());
#endif

    QLibrary plugin(pFileName);

    PluginInfoVersionFunc function = (PluginInfoVersionFunc) plugin.resolve("pluginInfoVersion");

#ifdef Q_OS_WIN
    QDir::setCurrent(origPath);
#endif

    return function?function():0;
}

//==============================================================================

int Plugin::interfaceVersion(const QString &pFileName,
                             const QString &pFunctionName)
{
    // Return the version of the given interface used by the plugin

    typedef int (*InterfaceVersionFunc)();

#ifdef Q_OS_WIN
    QString origPath = QDir::currentPath();

    QDir::setCurrent(QFileInfo(pFileName).absolutePath());
#endif

    QLibrary plugin(pFileName);

    InterfaceVersionFunc function = (InterfaceVersionFunc) plugin.resolve(qPrintable(pFunctionName));

#ifdef Q_OS_WIN
    QDir::setCurrent(origPath);
#endif

    return function?function():0;
}

//==============================================================================

PluginInfo * Plugin::info(const QString &pFileName, QString *pErrorMessage)
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

    QLibrary plugin(pFileName);

    PluginInfoFunc function = (PluginInfoFunc) plugin.resolve(qPrintable(name(pFileName)+"PluginInfo"));

#ifdef Q_OS_WIN
    QDir::setCurrent(origPath);
#endif

    // Check whether the plugin information could be retrieved and, if not,
    // retrieve the error and format it a bit

    if (function) {
        if (pErrorMessage)
            *pErrorMessage = QString();

        return static_cast<PluginInfo *>(function());
    } else {
        if (pErrorMessage) {
            *pErrorMessage = plugin.errorString();

            (*pErrorMessage)[0] = (*pErrorMessage)[0].toLower();

            if (!pErrorMessage->endsWith('.'))
                *pErrorMessage += '.';

            pErrorMessage->replace('\n', ";");
            pErrorMessage->replace("  ", " ");

            int errorMessageSize = pErrorMessage->size();
            int from = 0;
            int pos;

            while ((pos = pErrorMessage->indexOf(':', from)) != -1) {
                pos += 2;

                if (pos < errorMessageSize)
                    (*pErrorMessage)[pos] = (*pErrorMessage)[pos].toLower();

                from = pos;
            }
        }

        return 0;
    }
}

//==============================================================================

static const auto SettingsLoad = QStringLiteral("Load");

//==============================================================================

bool Plugin::load(const QString &pName)
{
    // Retrieve the plugin's loading requirement

    QSettings settings;

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

    QSettings settings;

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
