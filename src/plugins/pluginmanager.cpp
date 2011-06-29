#include "plugin.h"
#include "pluginmanager.h"

#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>

#include <QMessageBox>

namespace OpenCOR {

PluginManager::PluginManager(QSettings *pSettings) :
    mSettings(pSettings)
{
#ifndef Q_WS_MAC
    const QString pluginsDir = "plugins";
#else
    const QString pluginsDir = "PlugIns";
#endif

    mPluginsDir =  QDir(qApp->applicationDirPath()).canonicalPath()
                  +QDir::separator()+QString("..")
                  +QDir::separator()+pluginsDir
                  +QDir::separator()+qApp->applicationName();

#ifndef Q_WS_MAC
    // The plugins directory should be correct, but in case we try to run
    // OpenCOR on Windows or Linux AND from within Qt Creator, then the binary
    // will be running [OpenCOR]/build/OpenCOR[.exe] rather than
    // [OpenCOR]/build/bin/OpenCOR[.exe] as we should if we were to mimic the
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

    foreach (Plugin *plugin, mPlugins)
        delete plugin;
}

void PluginManager::loadPlugin(const QString &pPluginFileName)
{
    // Check what type of plugin we are dealing with and what its dependencies
    // are, if any

    void *info = QLibrary::resolve(pPluginFileName,
                                   QString(QFileInfo(pPluginFileName).baseName()+"Info").toLatin1().constData());

    if (info)
        QMessageBox::information(0, "Info", QString("Found '%1' in '%2'...").arg(QFileInfo(pPluginFileName).baseName()+"Info", pPluginFileName));
    else
        QMessageBox::information(0, "Info", QString("Did NOT find '%1' in '%2'...").arg(QFileInfo(pPluginFileName).baseName()+"Info", pPluginFileName));
}

void PluginManager::loadPlugins()
{
    // Try to load all the plugins

#ifdef Q_WS_WIN
    const QString extension = ".dll";
#elif defined(Q_WS_MAC)
    const QString extension = ".dylib";
#else
    const QString extension = ".so";
#endif

    QFileInfoList files = QDir(mPluginsDir).entryInfoList(QStringList("*"+extension), QDir::Files);

    foreach (const QFileInfo &file, files)
        loadPlugin(file.canonicalFilePath());
}

}
