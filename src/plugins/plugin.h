#ifndef PLUGIN_H
#define PLUGIN_H

#include "coreinterface.h"

#include <QObject>

class QSettings;

namespace OpenCOR {

#ifdef Q_WS_WIN
    static const QString PluginPrefix = "";
    static const QString PluginExtension = ".dll";
#else
    static const QString PluginPrefix = "lib";

    #ifdef Q_WS_MAC
        static const QString PluginExtension = ".dylib";
    #else
        static const QString PluginExtension = ".so";
    #endif
#endif

class Plugin : public QObject
{
    Q_OBJECT

public:
    enum Status
    {
        Undefined,
        NotFound,
        IncompatibleVersion,
        NotSuitable,
        NotWanted,
        NotNeeded,
        Loaded,
        NotLoaded,
        NotPlugin,
        MissingDependencies,
        NotPluginOrMissingDependencies
    };

    explicit Plugin(const QString &pFileName,
                    const PluginInfo::Type &pGuiOrConsoleType,
                    const bool &pForceLoading,
                    const PluginInfo::Version &pExpectedVersion,
                    QSettings *pSettings, const QString &pPluginsDir,
                    const QMap<QString, Plugin *> &pMappedPlugins);

    QString name() const;
    PluginInfo info() const;
    QObject * instance() const;
    Status status() const;
    QString statusErrors() const;
    int nbOfStatusErrors() const;

    static QString name(const QString &pFileName);
    static QString fileName(const QString &pDir, const QString &pName);
    static PluginInfo info(const QString &pFileName);

    static bool load(QSettings *pSettings, const QString &pName);
    static void setLoad(QSettings *pSettings, const QString &pName,
                        const bool &pToBeLoaded);

    static QStringList requiredPlugins(const QString &pPluginsDir,
                                       const QString &pName,
                                       const int &pLevel = 0);

private:
    QString mName;
    PluginInfo mInfo;
    QObject *mInstance;
    Status mStatus;
    QString mStatusErrors;
};

}

#endif
