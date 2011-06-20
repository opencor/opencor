#include "pluginmanager.h"

#include <QApplication>
#include <QDir>
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

    QFileInfoList files = QDir(mPluginsDir).entryInfoList(QStringList("*"+extension), QDir::Files);
    QStringList pluginFiles;

    foreach (const QFileInfo &file, files)
{
        pluginFiles << file.canonicalFilePath();
qDebug() << "Plugin #" << pluginFiles.count() << ":" << pluginFiles.last();
}

//---GRY--- TO BE DONE...
}

}
