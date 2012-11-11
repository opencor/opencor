//==============================================================================
// Plugin
//==============================================================================

#ifndef PLUGIN_H
#define PLUGIN_H

//==============================================================================

#include "plugininfo.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {

//==============================================================================

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

//==============================================================================

static const QString SettingsPlugins = "Plugins";

//==============================================================================

class Plugin;
class PluginManager;

//==============================================================================

typedef QList<Plugin *> Plugins;

//==============================================================================

class Plugin : public QObject
{
    Q_OBJECT

public:
    enum Status
    {
        UndefinedStatus,
        NotFound,
        InvalidFormatVersion,
        NotSuitable,
        NotWanted,
        NotNeeded,
        Loaded,
        NotLoaded,
        NotPlugin,
        MissingOrInvalidDependencies
    };

    explicit Plugin(const QString &pFileName,
                    const PluginInfo::Type &pGuiOrConsoleType,
                    const bool &pForceLoading,
                    const PluginInfo::FormatVersion &pExpectedFormatVersion,
                    const QString &pPluginsDir, PluginManager *pPluginManager
                   );

    QString name() const;
    PluginInfo info() const;
    QObject * instance() const;
    Status status() const;
    QString statusErrors() const;
    int statusErrorsCount() const;

    static QString name(const QString &pFileName);
    static QString fileName(const QString &pPluginsDir, const QString &pName);
    static PluginInfo info(const QString &pFileName);

    static bool load(const QString &pName);
    static void setLoad(const QString &pName, const bool &pToBeLoaded);

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

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
