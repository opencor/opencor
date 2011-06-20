#include "pluginmanager.h"

#include <QApplication>
#include <QDir>

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

}
