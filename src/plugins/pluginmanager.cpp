#include "pluginmanager.h"

#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include <QSettings>

#include <QDebug>

namespace OpenCOR {

PluginManager::PluginManager()
{
#ifndef Q_WS_MAC
    QString pluginsDir = "plugins";
#else
    QString pluginsDir = QString("..")+QDir::separator()+"PlugIns";
#endif

    mPluginsDir =  QDir(qApp->applicationDirPath()).canonicalPath()
                  +QDir::separator()+pluginsDir;

#ifdef Q_WS_MAC
    mPluginsDir += QDir::separator()+qApp->applicationName();
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

    qDebug() << "Loading plugins from" << mPluginsDir << ":";

    QFileInfoList files = QDir(mPluginsDir).entryInfoList(QStringList("*"+extension), QDir::Files);
    QStringList pluginFiles;

    foreach (const QFileInfo &file, files) {
        QString pluginFileName = file.canonicalFilePath();

        pluginFiles << pluginFileName;

        // Try to load the plugin as a 'proper' plugin

        QPluginLoader loader(pluginFileName);

        if (loader.load())
            qDebug() << "Plugin #" << pluginFiles.count() << ":" << QFileInfo(pluginFileName).baseName() << "- Loaded...";
        else
            qDebug() << "Plugin #" << pluginFiles.count() << ":" << QFileInfo(pluginFileName).baseName() << "- NOT LOADED...";
    }

    //---GRY--- TO BE DONE...
}

}
