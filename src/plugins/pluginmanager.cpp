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

    // Retrieve the list of plugins available for loading

    QFileInfoList fileInfoList = QDir(pluginsDir).entryInfoList(QStringList("*"+PluginExtension),
                                                                QDir::Files);

    // Self-contained plugins (e.g. the Core plugin) don't, by default, get
    // loaded, but the situation is obviously different if such a plugin is
    // required by another plugin (e.g. the Help plugin requires the Core
    // plugin), in which case the self-contained plugin should be loaded. So, we
    // must here determine which of those plugins need to be loaded...

    QStringList requiredSelfContainedPlugins;

//---GRY--- TO BE DONE...

    requiredSelfContainedPlugins.removeDuplicates();

    // Try to load all the plugins we can find, but only if nothing has been
    // done about plugins before

    foreach (const QFileInfo &file, fileInfoList) {
        QString fileName = QDir::toNativeSeparators(file.canonicalFilePath());

        mPlugins.insert(fileName, new Plugin(fileName,
                                             mGuiOrConsoleType,
                                             false));
    }
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

}
