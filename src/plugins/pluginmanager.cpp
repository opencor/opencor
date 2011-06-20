#include "pluginmanager.h"

#include <QApplication>
#include <QDir>
#include <QSettings>

namespace OpenCOR {

PluginManager::PluginManager()
{
#ifndef Q_WS_MAC
    mPluginsDir = QDir(qApp->applicationDirPath()).canonicalPath()+"/plugins";
#else
    mPluginsDir = QDir(qApp->applicationFilePath()).canonicalPath()+"/../PlugIns";
    //---GRY--- TO BE DOUBLE CHECKED...
#endif
}

PluginManager::~PluginManager()
{
}

void PluginManager::loadPlugins(QSettings *pSettings)
{
//---GRY--- TO BE DONE...
}

}
