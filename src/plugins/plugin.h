#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>

namespace OpenCOR {

class PluginInfo;

class Plugin : public QObject
{
    Q_OBJECT

    friend class PluginManager;

public:
    enum PluginStatus
    {
        Undefined,
        NotFound,
        NotPlugin,
        NotSuitable,
        Loaded,
        NotLoaded,
        NotWanted,
        DependenciesNotLoaded
    };

    explicit Plugin(const QString &pName);
    ~Plugin();

    QString name();

    PluginStatus status();

    static QString name(const QString &pPluginFileName);
    static QString fileName(const QString &pPluginsDir,
                            const QString &pPluginName);

    static PluginInfo info(  const QString &pPluginFileName
#ifdef Q_WS_WIN
                           , const QStringList &pLoadedPlugins
#endif
                          );

private:
    QString mName;

    PluginStatus mStatus;

    void setStatus(const PluginStatus &pStatus);

#ifdef Q_WS_WIN
    static QStringList dependencies(const QString &pPluginFileName);
#endif
};

}

#endif
