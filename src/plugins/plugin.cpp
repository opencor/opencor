#include "plugin.h"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>
#include <QSettings>

namespace OpenCOR {

Plugin::Plugin(const QString &pFileName,
               const PluginInfo::Type &pGuiOrConsoleType,
               const bool &pForceLoading,
               const PluginInterface::Version &pExpectedInterfaceVersion,
               QSettings *pSettings, const QString &pPluginsDir,
               const QMap<QString, Plugin *> &pMappedPlugins) :
    mName(name(pFileName)),
    // Note: to get the name of the plugin from its file name, we must remove
    //       the plugin prefix part from it...
    mInfo(),
    mInstance(0),
    mStatus(Undefined)
{
    // Check whether the plugin physically exists

    if (QFileInfo(pFileName).exists()) {
        // The plugin exists, so retrieve its information

        mInfo = info(pFileName);

        // Now, retrieve the plugin's full dependencies (i.e. both its direct
        // and indirect dependencies)

        mInfo.mFullDependencies << requiredPlugins(pPluginsDir, mName);

        // Try to load the plugin, but only if it uses the right plugin
        // interface version, if it is either a general plugin or one of the
        // type we are happy with, and if it is manageable or is required by
        // another plugin

        if (    (mInfo.interfaceVersion() == pExpectedInterfaceVersion)
            && (   (mInfo.type() == PluginInfo::General)
                || (mInfo.type() == pGuiOrConsoleType))
            && (   (mInfo.manageable() && load(pSettings, mName))
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

            if (nbOfStatusErrors() == 1)
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
        } else if (mInfo.type() == PluginInfo::Undefined) {
            // We couldn't retrieve the plugin information which means that we
            // are not dealing with an OpenCOR plugin or that one or several of
            // the plugin's dependencies weren't loaded, so...

#ifdef Q_WS_WIN
            mStatus = NotPluginOrMissingDependencies;
#else
            mStatus = NotPlugin;
#endif
        } else if (mInfo.interfaceVersion() != pExpectedInterfaceVersion) {
            // We are dealing with a plugin which relies on a different version
            // of the interface, so...

            mStatus = IncompatibleInterfaceVersion;
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

QString Plugin::name() const
{
    // Return the plugin's name

    return mName;
}

PluginInfo Plugin::info() const
{
    // Return the plugin's information

    return mInfo;
}

QObject * Plugin::instance() const
{
    // Return the plugin's insance

    return mInstance;
}

Plugin::Status Plugin::status() const
{
    // Return the plugin's status

    return mStatus;
}

QString Plugin::statusErrors() const
{
    // Return the plugin's status errors

    return mStatusErrors;
}

int Plugin::nbOfStatusErrors() const
{
    // Return the number of plugin's status errors

    return mStatusErrors.count("\n")+1;
}

QString Plugin::name(const QString &pFileName)
{
    // Return the plugin's name based on its file name

    return QFileInfo(pFileName).baseName().remove(0, PluginPrefix.length());
    // Note: we must remove the plugin prefix part from the plugin file name...
}

QString Plugin::fileName(const QString &pDir, const QString &pName)
{
    // Return the plugin's file name based on its name

    return pDir+QDir::separator()+PluginPrefix+pName+PluginExtension;
    // Note: we must add the plugin prefix part to the plugin file name...
}

PluginInfo Plugin::info(const QString &pFileName)
{
    // Return the plugin's information

    typedef PluginInfo (*PluginInfoFunc)();

    PluginInfoFunc pluginInfoFunc = (PluginInfoFunc) QLibrary::resolve(pFileName,
                                                                       QString(name(pFileName)+"PluginInfo").toLatin1().constData());

    // Check whether the plugin information function was found

    if (pluginInfoFunc)
        // The plugin information function was found, so we can extract the
        // information we are after

        return pluginInfoFunc();
    else
        // The plugin information couldn't be found, so...

        return PluginInfo();
}

static const QString SettingsLoad = "Load";

bool Plugin::load(QSettings *pSettings, const QString &pName)
{
    // Retrieve the plugin's loading requirement

    pSettings->beginGroup(pName);
        bool res = pSettings->value(SettingsLoad, true).toBool();
    pSettings->endGroup();

    return res;
}

void Plugin::setLoad(QSettings *pSettings, const QString &pName,
                     const bool &pToBeLoaded)
{
    // Keep track of the plugin's loading requirement

    pSettings->beginGroup(pName);
        pSettings->setValue(SettingsLoad, pToBeLoaded);
    pSettings->endGroup();
}

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

}
