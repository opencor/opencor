#include "plugin.h"
#include "plugininterface.h"
#include "pluginmanager.h"

#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>

#include <QMessageBox>

namespace OpenCOR {

PluginManager::PluginManager(QSettings *pSettings,
                             const PluginInfo::PluginType &pGuiOrConsoleType) :
    mSettings(pSettings),
    mGuiOrConsoleType(pGuiOrConsoleType)
{
#ifndef Q_WS_MAC
    static const QString pluginsDir = "plugins";
#else
    static const QString pluginsDir = "PlugIns";
#endif

    mPluginsDir =  QDir(qApp->applicationDirPath()).canonicalPath()
                  +QDir::separator()+QString("..")
                  +QDir::separator()+pluginsDir
                  +QDir::separator()+qApp->applicationName();

#ifndef Q_WS_MAC
    // The plugins directory should be correct, but in case we try to run
    // OpenCOR on Windows or Linux AND from within Qt Creator, then the binary
    // will be running from [OpenCOR]/build/OpenCOR[.exe] rather than
    // [OpenCOR]/build/bin/OpenCOR[.exe] as it should if we were to mimic the
    // case where OpenCOR has been deployed. Then, because the plugins are in
    // [OpenCOR]/build/plugins/OpenCOR, we must skip the "../" bit. Yes, it's
    // not neat, but... is there another solution?...

    if (!QDir(mPluginsDir).exists())
        mPluginsDir =  QDir(qApp->applicationDirPath()).canonicalPath()
                      +QDir::separator()+pluginsDir
                      +QDir::separator()+qApp->applicationName();
#endif
}

PluginManager::~PluginManager()
{
    // Remove all the plugins

    while (!mPlugins.isEmpty()) {
        delete mPlugins.begin().value();

        mPlugins.erase(mPlugins.begin());
    }
}

Plugin * PluginManager::plugin(const QString &pPluginName)
{
    // Check whether the plugin exists

    Plugin *plugin = mPlugins.value(pPluginName);

    if (!plugin) {
        // The plugin doesn't exist, so create it

        plugin = new Plugin(pPluginName);

        mPlugins.insert(pPluginName, plugin);
    }

    // Return the plugin

    return plugin;
}

void PluginManager::loadPlugin(const QString &pPluginFileName)
{
    Plugin *pluginObject = plugin(Plugin::name(pPluginFileName));

    // Check whether the plugin has already been dealt with

    if (pluginObject->status() == Plugin::Undefined) {
        // The plugin hasn't already been dealt with, so check whether it
        // physically exists

        if (QFileInfo(pPluginFileName).exists()) {
            // The plugin exists, so retrieve its information and only try to
            // load it if it is either a general plugin or one of the type we
            // are happy with

            PluginInfo pluginInfo = Plugin::info(pPluginFileName);

            if (   (pluginInfo.type == PluginInfo::General)
                || (pluginInfo.type == mGuiOrConsoleType)) {
                // We are dealing with the right kind of plugin, so check
                // whether it has dependencies, and if so then load them and
                // make sure that they are properly loaded

                bool dependenciesLoaded = true;

                foreach(const QString &dependency, pluginInfo.dependencies) {
                    loadPlugin(Plugin::fileName(mPluginsDir, dependency));

                    Plugin *dependencyPlugin = plugin(dependency);

                    if (dependencyPlugin->status() != Plugin::Loaded) {
                        dependenciesLoaded = false;

                        break;
                    }
                }

                // At this stage, we try to load the plugin itself, but only if
                // all of its dependencies have been loaded

                if (dependenciesLoaded) {
                    // The plugin's dependencies have been loaded, so try to
                    // load the plugin
//---GRY--- WE SHOULD CHECK IN THE SETTINGS WHETHER THE USER ACTUALLY WANTs TO
//          LOAD THE PLUGIN OR NOT...

                    QPluginLoader pluginLoader(pPluginFileName);

                    if (pluginLoader.load())
                        // The plugin has been properly loaded, so...

                        pluginObject->setStatus(Plugin::Loaded);
                    else
                        // The plugin couldn't be loaded for some reason
                        // (surely, this should never happen...?!), so...

                        pluginObject->setStatus(Plugin::NotLoaded);
                } else {
                    // Some dependencies haven't been loaded, so...

                    pluginObject->setStatus(Plugin::DependenciesNotLoaded);
                }
            } else if (pluginInfo.type == PluginInfo::Undefined) {
                // We couldn't retrieve the plugin information which means we
                // are not dealing with an OpenCOR plugin, so...

                pluginObject->setStatus(Plugin::NotPlugin);
            } else {
                // We are dealing with a plugin that is not of the type we are
                // happy with (i.e. it's a console plugin but we are running the
                // GUI version of OpenCOR, or it's a GUI plugin but we are
                // running the console version of OpenCOR), so...

                pluginObject->setStatus(Plugin::NotSuitable);
            }
        } else {
            // The plugin doesn't exist, so...

            pluginObject->setStatus(Plugin::NotFound);
        }
    }
}

void PluginManager::loadPlugins()
{
    // Try to load all the plugins we can find

    QFileInfoList files = QDir(mPluginsDir).entryInfoList(QStringList("*"+PluginExtension), QDir::Files);

    foreach (const QFileInfo &file, files)
        loadPlugin(file.canonicalFilePath());

QString report;

QMap<QString, Plugin *>::const_iterator iter = mPlugins.begin();

while (iter != mPlugins.constEnd()) {
    QString status;

    switch (plugin(iter.key())->status()) {
    case Plugin::NotFound:
        status = "NOT FOUND...";

        break;
    case Plugin::NotPlugin:
        status = "Not plugin...";

        break;
    case Plugin::NotSuitable:
        status = "Not suitable...";

        break;
    case Plugin::Loaded:
        status = "Loaded...";

        break;
    case Plugin::NotLoaded:
        status = "NOT LOADED...";

        break;
    case Plugin::NotWanted:
        status = "Not wanted...";

        break;
    case Plugin::DependenciesNotLoaded:
        status = "Dependencies not loaded...";

        break;
    default:
        status = "Undefined...";

        break;
    }

    report += iter.key()+":\t\t"+status+"\n";

    ++iter;
}

report.chop(1);

QMessageBox::information(0, QString("Nb of plugins: %1...").arg(mPlugins.count()), report);
}

}
