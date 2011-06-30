#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QMap>
#include <QObject>

class Plugin;

class QSettings;

namespace OpenCOR {

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QSettings *pSettings);
    ~PluginManager();

    void loadPlugins();

private:
    QSettings *mSettings;

    QString mPluginsDir;

    QMap<QString, Plugin *> mPlugins;

    Plugin *plugin(const QString &pPluginName);

    void loadPlugin(const QString &pPluginFileName);
};

}

#endif
