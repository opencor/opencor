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
                    const PluginInfo::PluginType &pGuiOrConsoleType);

    QString name();

    PluginInfo info();

    PluginStatus status();

private:
    QString mName;

    PluginInfo mInfo;

    PluginStatus mStatus;
};

}

#endif
