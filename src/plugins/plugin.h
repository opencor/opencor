/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Plugin
//==============================================================================

#pragma once

//==============================================================================

#include <QObject>
#include <QString>
#include <QStringList>

//==============================================================================

namespace OpenCOR {

//==============================================================================

#if defined(Q_OS_WIN)
    static const auto PluginPrefix    = QStringLiteral("");
    static const auto PluginExtension = QStringLiteral(".dll");
#elif defined(Q_OS_LINUX)
    static const auto PluginPrefix    = QStringLiteral("lib");
    static const auto PluginExtension = QStringLiteral(".so");
#else
    static const auto PluginPrefix    = QStringLiteral("lib");
    static const auto PluginExtension = QStringLiteral(".dylib");
#endif

//==============================================================================

static const auto SettingsPlugins = QStringLiteral("Plugins");

//==============================================================================

static const auto CorePluginName = QStringLiteral("Core");

//==============================================================================

class PluginInfo;
class PluginManager;

//==============================================================================

class Plugin : public QObject
{
    Q_OBJECT

public:
    enum class Status {
        NotWanted,
        NotNeeded,
        Loaded,
        NotLoaded,
        NotPlugin,
        OldPlugin,
        NotCorePlugin,
        InvalidCorePlugin,
        NotCliPluginNoCliSupport,
        NotCliPluginNoCliInterface,
        MissingOrInvalidDependencies
    };

    explicit Plugin(const QString &pFileName, PluginInfo *pInfo,
                    const QString &pErrorMessage, bool pLoad,
                    PluginManager *pPluginManager);
    ~Plugin() override;

    static bool compare(Plugin *pPlugin1, Plugin *pPlugin2);

    QString name() const;
    PluginInfo * info() const;
    QString errorMessage() const;
    QObject * instance() const;
    Status status() const;
    QString statusErrors() const;
    int statusErrorsCount() const;

    static int pluginInfoVersion(const QString &pFileName);
    static int interfaceVersion(const QString &pFileName,
                                const QString &pFunctionName);

    static QString name(const QString &pFileName);
    static QString fileName(const QString &pPluginsDir, const QString &pName);
    static PluginInfo * info(const QString &pFileName,
                             QString *pErrorMessage = nullptr);

    static bool load(const QString &pName);
    static void setLoad(const QString &pName, bool pToBeLoaded);

    static QStringList fullDependencies(const QString &pPluginsDir,
                                        const QString &pName, int pLevel = 0);

private:
    QString mName;
    PluginInfo *mInfo = nullptr;
    QString mErrorMessage;
    QObject *mInstance = nullptr;
    Status mStatus = Status::NotPlugin;
    QString mStatusErrors;
};

//==============================================================================

using Plugins = QList<Plugin *>;

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
