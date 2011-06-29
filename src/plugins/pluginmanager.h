#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QList>
#include <QObject>

class Plugin;

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

    QList<Plugin *> mPlugins;
};

}

#endif
