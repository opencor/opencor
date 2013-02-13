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

Plugin::Plugin(const QString &pFileName,
               const PluginInfo::Type &pGuiOrConsoleType,
               const bool &pForceLoading,
               const PluginInfo::FormatVersion &pExpectedFormatVersion,
               const QString &pPluginsDir, PluginManager *pPluginManager
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

        mInfo.setFullDependencies(requiredPlugins(pPluginsDir, mName));

        // Try to load the plugin, but only if it uses the right format version,
        // if it is either a general plugin or one of the type we are happy
        // with, and if it is manageable or is required by another plugin

        if (    (mInfo.formatVersion() == pExpectedFormatVersion)
            && (   (mInfo.type() == PluginInfo::General)
                || (mInfo.type() == pGuiOrConsoleType))
            && (   (mInfo.manageable() && load(mName))
                || pForceLoading)) {
            // We are dealing with the right kind of plugin, so check that all
            // of its dependencies, if any, are loaded
            // Note: normally, we would only do this on non-Windows systems
            //       since, on Windows, a shared library's dependencies must be
            //       loaded before the shared library itself can be loaded,
            //       while on Linux / OS X, it's possible to load a shared
            //       library even if its dependencies are not loaded. However,
            //       we want to check that the format version used by the plugin
            //       matches the one used by OpenCOR, so in the end we also do
            //       it on Windows. Indeed, it might very well be that a plugin
            //       can still be loaded fine, yet use an invalid format, so...

            bool pluginDependenciesLoaded = true;

            mStatusErrors = "";

            foreach (const QString &dependency, mInfo.dependencies()) {
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
                    // The plugin has been properly loaded, so...

                    mInstance = pluginLoader.instance();

                    mStatus = Loaded;
                } else {
                    // The plugin couldn't be loaded for some reason (surely,
                    // this should never happen...?!), so...

                    mStatus = NotLoaded;
                    mStatusErrors = pluginLoader.errorString();
                }
            }
        } else if (mInfo.type() == PluginInfo::UndefinedType) {
            // We couldn't retrieve the plugin information which means that we
            // are not dealing with an OpenCOR plugin or that one or several of
            // the plugin's dependencies weren't loaded, so...

            mStatus = NotPlugin;
        } else if (mInfo.formatVersion() != pExpectedFormatVersion) {
            // We are dealing with a plugin which relies on a different format
            // version, so...

            mStatus = InvalidFormatVersion;
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

    if (pluginInfoFunc) {
        // The plugin information function was found, so we can extract the
        // information we are after
        // Note: see the definition for PLUGININFO_FUNC in pluginfo.h for an
        //       explanation of the code below (i.e. retrieving a pointer to a
        //       PluginInfo object and then deleting it)...

        PluginInfo *pluginInfo = static_cast<PluginInfo *>(pluginInfoFunc());
        PluginInfo res;

        res.setFormatVersion(pluginInfo->formatVersion());
        res.setType(pluginInfo->type());
        res.setCategory(pluginInfo->category());
        res.setManageable(pluginInfo->manageable());
        res.setDependencies(pluginInfo->dependencies());
        res.setFullDependencies(QStringList());
        res.setDescriptions(pluginInfo->descriptions());

        delete pluginInfo;

        return res;
    } else {
        // The plugin information couldn't be found, so...

        return PluginInfo();
    }
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
