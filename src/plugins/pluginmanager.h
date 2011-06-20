#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>

class QSettings;

namespace OpenCOR {

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager();
    ~PluginManager();

    void loadPlugins(QSettings *pSettings);

private:
    QString mPluginsDir;
};

}

#endif
