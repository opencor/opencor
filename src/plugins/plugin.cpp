//==============================================================================
// Plugin
//==============================================================================

#include "plugin.h"

//==============================================================================

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QSettings>

//==============================================================================

namespace OpenCOR {

//==============================================================================

Plugin::Plugin(const QString &pFileName,
               const PluginInfo::Type &pGuiOrConsoleType,
               const bool &pForceLoading,
               const PluginInfo::Version &pExpectedVersion,
               const QString &pPluginsDir
#ifndef Q_WS_WIN
               , const QMap<QString, Plugin *> &pMappedPlugins
#endif
              ) :
    mName(name(pFileName)),
    // Note: to get the name of the plugin from its file name, we must remove
    //       the plugin prefix part from it...
    mInfo(),
    mInstance(0),
    mStatus(UndefinedStatus)
{
    // Check whether the plugin physically exists

    if (QFileInfo(pFileName).exists()) {
        // The plugin exists, so retrieve its information

        mInfo = info(pFileName);

        // Now, retrieve the plugin's full dependencies (i.e. both its direct
        // and indirect dependencies)

        mInfo.mFullDependencies = requiredPlugins(pPluginsDir, mName);

        // Try to load the plugin, but only if it uses the right plugin version,
        // if it is either a general plugin or one of the type we are happy
        // with, and if it is manageable or is required by another plugin

        if (    (mInfo.version() == pExpectedVersion)
            && (   (mInfo.type() == PluginInfo::General)
                || (mInfo.type() == pGuiOrConsoleType))
            && (   (mInfo.manageable() && load(mName))
                || pForceLoading)) {
            // We are dealing with the right kind of plugin, so check that all
            // of its dependencies, if any, are loaded
            // Note: the reason we only do this on non-Windows systems is that
            //       on Windows a shared library's dependencies must be loaded
            //       before the shared library itself can be loaded while on
            //       Linux / Mac OS X, it's possible to load a shared library
            //       even if its dependencies are not loaded, so...

#ifndef Q_WS_WIN
            bool pluginDependenciesLoaded = true;

            mStatusErrors = "";

            foreach (const QString &dependency, mInfo.dependencies()) {
                Plugin *pluginDependency = pMappedPlugins.value(dependency);

                if (   !pluginDependency
                    || (   pluginDependency
                        && (pluginDependency->status() != Loaded))) {
                    // Either the plugin dependency couldn't be found or it
                    // could be found but it isn't loaded, so...

                    pluginDependenciesLoaded = false;

                    mStatus = MissingDependencies;

                    if (!mStatusErrors.isEmpty())
                        mStatusErrors += "\n";

                    mStatusErrors += " - "+dependency;
                }
            }

            if (statusErrorsCount() == 1)
                // There is only one error, so remove the leading " - "

                mStatusErrors = mStatusErrors.remove(0, 3);
#endif

            // Check whether all of the plugin's dependencies, if any, were
            // loaded, and if so then try to load the plugin itself

#ifndef Q_WS_WIN
            if (pluginDependenciesLoaded) {
#endif
                // All the plugin's dependencies, if any, were loaded, so try to
                // load the plugin itself

                QPluginLoader pluginLoader(pFileName);

                if (pluginLoader.load()) {
                    // The plugin has been properly loaded, so...

                    mInstance = pluginLoader.instance();

                    mStatus = Loaded;
                } else {
                    // The plugin couldn't be loaded for some reason (surely,
                    // this should never happen...?!), so...

                    mStatus = NotLoaded;
                    mStatusErrors = pluginLoader.errorString();
                }
#ifndef Q_WS_WIN
            }
#endif
        } else if (mInfo.type() == PluginInfo::UndefinedType) {
            // We couldn't retrieve the plugin information which means that we
            // are not dealing with an OpenCOR plugin or that one or several of
            // the plugin's dependencies weren't loaded, so...

#ifdef Q_WS_WIN
            mStatus = NotPluginOrMissingDependencies;
#else
            mStatus = NotPlugin;
#endif
        } else if (mInfo.version() != pExpectedVersion) {
            // We are dealing with a plugin which relies on a different version,
            // so...

            mStatus = IncompatibleVersion;
        } else if (   (mInfo.type() != PluginInfo::General)
                   && (mInfo.type() != pGuiOrConsoleType)) {
            // We are dealing with a plugin which is not of the type we are
            // happy with (i.e. it's a console plugin but we are running the GUI
            // version of OpenCOR, or it's a GUI plugin but we are running the
            // console version of OpenCOR), so...

            mStatus = NotSuitable;
        } else {
            // If none of the above applies then it means we are dealing with a
            // plugin which is either not wanted or not needed, depending on
            // whether it is manageable

            if (mInfo.manageable())
                mStatus = NotWanted;
            else
                mStatus = NotNeeded;
        }
    } else {
        // The plugin doesn't exist, so...

        mStatus = NotFound;
    }
}

//==============================================================================

QString Plugin::name() const
{
    // Return the plugin's name

    return mName;
}

//==============================================================================

PluginInfo Plugin::info() const
{
    // Return the plugin's information

    return mInfo;
}

//==============================================================================

QObject * Plugin::instance() const
{
    // Return the plugin's insance

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

PluginInfo Plugin::info(const QString &pFileName)
{
    // Return the plugin's information
    // Note: to retrieve a plugin's information, we must, on both Windows and
    //       Linux, be able to load any plugin on which a plugin depends. On
    //       Windows, we do this (by keeping track of the current directory
    //       and) by going to the directory where our plugins are located.
    //       However, this approach doesn't work on Linux, so instead we rely
    //       on LD_LIBRARY_PATH to get the result we are after (in fact, it is
    //       probably a better approach!), so...

    typedef PluginInfo (*PluginInfoFunc)();

#ifdef Q_WS_WIN
    QString origPath = QDir::currentPath();

    QDir::setCurrent(QFileInfo(pFileName).absolutePath());
#endif

    PluginInfoFunc pluginInfoFunc = (PluginInfoFunc) QLibrary::resolve(pFileName,
                                                                       qPrintable(name(pFileName)+"PluginInfo"));

#ifdef Q_WS_WIN
    QDir::setCurrent(origPath);
#endif

    // Check whether the plugin information function was found

    if (pluginInfoFunc)
        // The plugin information function was found, so we can extract the
        // information we are after

        return pluginInfoFunc();
    else
        // The plugin information couldn't be found, so...

        return PluginInfo();
}

//==============================================================================

static const QString SettingsLoad = "Load";

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

    QSettings settings(qApp->applicationName());

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

    QSettings settings(qApp->applicationName());

    settings.beginGroup(SettingsPlugins);
        settings.beginGroup(pName);
            settings.setValue(SettingsLoad, pToBeLoaded);
        settings.endGroup();
    settings.endGroup();
}

//==============================================================================

QStringList Plugin::requiredPlugins(const QString &pPluginsDir,
                                    const QString &pName,
                                    const int &pLevel)
{
    // Return the list of plugins required by a given plugin

    QStringList res;

    // Recursively look for the plugins required by the current plugin

    foreach (const QString &plugin,
             Plugin::info(Plugin::fileName(pPluginsDir, pName)).dependencies())
        res << requiredPlugins(pPluginsDir, plugin, pLevel+1);

    // Add the current plugin to the list, but only if it is not the original
    // plugin for which we want to know what its requirements are

    if (pLevel)
        // We are not dealing with the original plugin, so...

        res << pName;
    else
        // We are done, so remove any duplicate which might be present

        res.removeDuplicates();

    return res;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
