#ifndef PLUGIN_H
#define PLUGIN_H

#include "plugininterface.h"

#include <QObject>

class QSettings;

namespace OpenCOR {

class PluginManager;

class Plugin : public QObject
{
    Q_OBJECT

public:
    enum PluginStatus
    {
        Undefined,
        NotFound,
        NotSuitable,
        NotWanted,
        NotNeeded,
        Loaded,
        NotLoaded,
        NotPlugin,
        MissingDependencies,
        NotPluginOrMissingDependencies
    };

    explicit Plugin(PluginManager *pPluginManager, const QString &pFileName,
                    const PluginInfo::PluginType &pGuiOrConsoleType,
                    const bool &pForceLoading);

    QString name();
    PluginInfo info();
    QObject * instance();
    PluginStatus status();
    QString statusDescription();

    static QString name(const QString &pFileName);
    static QString fileName(const QString &pDir, const QString &pName);
    static PluginInfo info(const QString &pFileName);

    static bool load(QSettings *pSettings, const QString &pName);
    static void setLoad(QSettings *pSettings, const QString &pName,
                        const bool &pToBeLoaded);

private:
    QString mName;
    PluginInfo mInfo;
    QObject *mInstance;
    PluginStatus mStatus;
    QString mStatusError;
};

}

#endif
