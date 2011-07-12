#include "plugin.h"
#include "pluginmanager.h"

#include <QDir>
#include <QFileInfo>
#include <QPluginLoader>

namespace OpenCOR {

Plugin::Plugin(PluginManager *pPluginManager, const QString &pFileName,
               const PluginInfo::PluginType &pGuiOrConsoleType,
               const bool &pForceLoading) :
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

        // Try to load the plugin, but only if it is either a general plugin or
        // one of the type we are happy with, and has dependencies or is
        // required by another plugin

        if (   (   (mInfo.mType == PluginInfo::General)
                || (mInfo.mType == pGuiOrConsoleType))
            && (mInfo.dependencies().count() || pForceLoading)) {
            // We are dealing with the right kind of plugin, so check that all
            // of its dependencies, if any, are loaded
            // Note: the reason we only do this on non-Windows systems is that
            //       on Windows a shared library's dependencies must be loadable
            //       before the shared library itself can be loaded while on
            //       Linux / Mac OS X, it's possible to load a shared library
            //       even its dependencies are not loaded, so...

#ifndef Q_WS_WIN
            bool pluginDependenciesLoaded = true;

            mStatusError = "";

            foreach(const QString &dependency, mInfo.dependencies()) {
                Plugin *pluginDependency = pPluginManager->plugin(dependency);

                if (   !pluginDependency
                    || (   pluginDependency
                        && (pluginDependency->status() != Loaded))) {
                    // Either the plugin dependency couldn't be found or it
                    // could be found but it isn't loaded, so...

                    pluginDependenciesLoaded = false;

                    mStatus = MissingDependencies;

                    if (!mStatusError.isEmpty())
                        mStatusError += "\n";

                    mStatusError += " - "+dependency;
                }
            }
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
                    mStatusError = pluginLoader.errorString();
                }
#ifndef Q_WS_WIN
            }
#endif
        } else if (mInfo.mType == PluginInfo::Undefined) {
            // We couldn't retrieve the plugin information which means that we
            // are not dealing with an OpenCOR plugin or that one or several of
            // the plugin's dependencies weren't loaded, so...

#ifdef Q_WS_WIN
            mStatus = NotPluginOrMissingDependencies;
#else
            mStatus = NotPlugin;
#endif
        } else if (mInfo.mType != pGuiOrConsoleType){
            // We are dealing with a plugin which is not of the type we are
            // happy with (i.e. it's a console plugin but we are running the GUI
            // version of OpenCOR, or it's a GUI plugin but we are running the
            // console version of OpenCOR), so...

            mStatus = NotSuitable;
        } else {
            // If none of the above applies then it means we are dealing with a
            // plugin which is not needed, so...

            mStatus = NotNeeded;
        }
    } else {
        // The plugin doesn't exist, so...

        mStatus = NotFound;
    }
}

QString Plugin::name()
{
    // Return the plugin's name

    return mName;
}

PluginInfo Plugin::info()
{
    // Return the plugin's information

    return mInfo;
}

QObject * Plugin::instance()
{
    // Return the plugin's insance

    return mInstance;
}

Plugin::PluginStatus Plugin::status()
{
    // Return the plugin's status

    return mStatus;
}

QString Plugin::statusDescription()
{
    // Return the plugin's status' description, if any

    switch (mStatus) {
    case NotFound:
        return tr("The plugin could not be found");
    case NotSuitable:
        return tr("The plugin is not of the right type");
    case NotNeeded:
        return tr("The plugin is not needed");
    case Loaded:
        return tr("The plugin is loaded and is fully functional");
    case NotLoaded:
        return  tr("The plugin could not be loaded due to the following problem:")+"\n"
               +mStatusError;
    case NotPlugin:
        return tr("This is not a plugin");
    case MissingDependencies:
        return  tr("The plugin could not be loaded due to a/some missing dependency/ies:")+"\n"
               +mStatusError;
    case NotPluginOrMissingDependencies:
        return tr("This is not a plugin or some plugin dependencies are missing");
    default:
        return tr("The plugin's status is undefined");
    }
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
    // Note: in order to do this, we must, on Windows, keep track of the current
    //       directory and go to the directory where our plugin is, thus making
    //       sure that we can retrieve the plugin's information without any
    //       problem. Indeed, if we think of the CellMLModelRepository plugin,
    //       it requires the Core plugin to be loaded even if all we want is to
    //       retrieve its information, so...

    typedef PluginInfo (*PluginInfoFunc)();

#ifdef Q_WS_WIN
    QString origPath = QDir::currentPath();

    QDir::setCurrent(QFileInfo(pFileName).absolutePath());
#endif

    PluginInfoFunc pluginInfoFunc = (PluginInfoFunc) QLibrary::resolve(pFileName,
                                                                       QString(name(pFileName)+"PluginInfo").toLatin1().constData());

#ifdef Q_WS_WIN
    QDir::setCurrent(origPath);
#endif

    // Check whether the plugin information function was found

    if (pluginInfoFunc)
        // The plugin information function was found, so we can extract the
        // information we are after

        return pluginInfoFunc();
    else
        return PluginInfo();
}

}
