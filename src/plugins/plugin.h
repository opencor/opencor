#ifndef PLUGIN_H
#define PLUGIN_H

#include "plugininterface.h"

#include <QObject>

namespace OpenCOR {

class Plugin : public QObject
{
    Q_OBJECT

    friend class PluginManager;

public:
    enum PluginStatus
    {
        Undefined,
        NotFound,
        NotSuitable,
        Loaded,
        NotLoaded,
        NotPluginOrMissingDependencies
    };

    explicit Plugin(const QString &pFileName,
                    const PluginInfo::PluginType &pGuiOrConsoleType,
                    const bool &pForceLoading);

    QString name();
    PluginInfo info();
    QObject * instance();
    PluginStatus status();

    static QString name(const QString &pFileName);
    static QString fileName(const QString &pDir, const QString &pName);
    static PluginInfo info(const QString &pFileName);

private:
    QString mName;
    PluginInfo mInfo;
    QObject *mInstance;
    PluginStatus mStatus;
};

}

#endif
