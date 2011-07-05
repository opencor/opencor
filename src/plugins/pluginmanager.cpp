#include "plugin.h"
#include "plugininterface.h"
#include "pluginmanager.h"

#include <QApplication>
#include <QDir>
#include <QPluginLoader>

#include <QMessageBox>

namespace OpenCOR {

PluginManager::PluginManager(const PluginInfo::PluginType &pGuiOrConsoleType) :
    mGuiOrConsoleType(pGuiOrConsoleType)
{
    QString pluginsDir =  QDir(qApp->applicationDirPath()).canonicalPath()
                         +QDir::separator()+QString("..")
#ifndef Q_WS_MAC
                         +QDir::separator()+"plugins"
#else
                         +QDir::separator()+"PlugIns"
#endif
                         +QDir::separator()+qApp->applicationName();

#ifndef Q_WS_MAC
    // The plugins directory should be correct, but in case we try to run
    // OpenCOR on Windows or Linux AND from within Qt Creator, then the binary
    // will be running from [OpenCOR]/build/OpenCOR[.exe] rather than
    // [OpenCOR]/build/bin/OpenCOR[.exe] as it should if we were to mimic the
    // case where OpenCOR has been deployed. Then, because the plugins are in
    // [OpenCOR]/build/plugins/OpenCOR, we must skip the "../" bit. Yes, it's
    // not neat, but... is there another solution?...

    if (!QDir(pluginsDir).exists())
        pluginsDir =  QDir(qApp->applicationDirPath()).canonicalPath()
                     +QDir::separator()+"plugins"
                     +QDir::separator()+qApp->applicationName();
#endif

    // Try to load all the plugins we can find, but only if nothing has been
    // done about plugins before

    foreach (const QFileInfo &file,
             QDir(pluginsDir).entryInfoList(QStringList("*"+PluginExtension), QDir::Files))
        plugin(QDir::toNativeSeparators(file.canonicalFilePath()));

// Quick report on the loading of the plugins

QString report;

QMap<QString, Plugin *>::const_iterator iter = mPlugins.begin();

while (iter != mPlugins.constEnd()) {
    QString status;

    switch (plugin(iter.key())->status()) {
    case Plugin::NotFound:
        status = "NOT FOUND...";

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
    case Plugin::NotPluginOrMissingDependencies:
        status = "Not a plugin or missing dependencies...";

        break;
    default:
        status = "Undefined...";

        break;
    }

    report += plugin(iter.key())->name()+"\n   ---> "+status+"\n";

    ++iter;
}

report.chop(1);

QMessageBox::information(0, QString("Nb of plugins: %1...").arg(mPlugins.count()), report);
}

PluginManager::~PluginManager()
{
    // Delete the plugins

    while (!mPlugins.isEmpty()) {
        delete mPlugins.begin().value();

        mPlugins.erase(mPlugins.begin());
    }
}

QList<Plugin *> PluginManager::loadedPlugins()
{
    // Return the list of loaded plugins

    QList<Plugin *> res;

    QMap<QString, Plugin *>::const_iterator iter = mPlugins.begin();

    while (iter != mPlugins.constEnd()) {
        if (iter.value()->status() == Plugin::Loaded)
            res << iter.value();

        ++iter;
    }

    return res;
}

Plugin * PluginManager::plugin(const QString &pFileName)
{
    // Check whether the plugin exists

    Plugin *res = mPlugins.value(pFileName);

    if (!res) {
        // The plugin doesn't exist, so create it

        res = new Plugin(pFileName, mGuiOrConsoleType);

        mPlugins.insert(pFileName, res);
    }

    // Return the plugin

    return res;
}

}
