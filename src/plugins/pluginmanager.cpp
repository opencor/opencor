#include "pluginmanager.h"

#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>

#include <QMessageBox>

namespace OpenCOR {

PluginManager::PluginManager()
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
}

void PluginManager::loadPlugins(QSettings *pSettings)
{
    // Determine which plugins are available for loading

#ifdef Q_WS_WIN
    const QString extension = ".dll";
#elif defined(Q_WS_MAC)
    const QString extension = ".dylib";
#else
    const QString extension = ".so";
#endif

    QString feedback = QString("Loading plugins from '%1':").arg(QDir::toNativeSeparators(mPluginsDir));

    QFileInfoList files = QDir(mPluginsDir).entryInfoList(QStringList("*"+extension), QDir::Files);
    QStringList pluginFiles;

    foreach (const QFileInfo &file, files) {
        QString pluginFileName = file.canonicalFilePath();

        pluginFiles << pluginFileName;

        // Try to load the plugin as a 'proper' plugin

        QPluginLoader loader(pluginFileName);

        if (loader.load())
            feedback += QString("\nPlugin #%1: '%2' - Loaded...").arg(QString::number(pluginFiles.count()), QFileInfo(pluginFileName).baseName());
        else
            feedback += QString("\nPlugin #%1: '%2' - NOT LOADED [%3]...").arg(QString::number(pluginFiles.count()), QFileInfo(pluginFileName).baseName(), loader.errorString());
    }

    QMessageBox::information(0, "Plugins...", feedback);

    //---GRY--- TO BE DONE...
}

}
