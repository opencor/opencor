/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Plugin
//==============================================================================

#ifndef PLUGIN_H
#define PLUGIN_H

//==============================================================================

#include <QList>
#include <QObject>
#include <QString>
#include <QStringList>

//==============================================================================

#include "disableunusedvariablewarning.inl"
    namespace OpenCOR {
        static const char *SettingsOrganization = "Physiome";
        static const char *SettingsApplication  = "OpenCOR";
        static const char *SettingsPlugins      = "Plugins";

        #if defined(Q_OS_WIN)
            static const char *PluginPrefix    = "";
            static const char *PluginExtension = ".dll";
        #elif defined(Q_OS_LINUX)
            static const char *PluginPrefix    = "lib";
            static const char *PluginExtension = ".so";
        #elif defined(Q_OS_MAC)
            static const char *PluginPrefix    = "lib";
            static const char *PluginExtension = ".dylib";
        #else
            #error Unsupported platform
        #endif
    }   // namespace OpenCOR
#include "enableunusedvariablewarning.inl"

//==============================================================================

namespace OpenCOR {

//==============================================================================

class Plugin;
class PluginInfo;
class PluginManager;

//==============================================================================

typedef QList<Plugin *> Plugins;

//==============================================================================

class Plugin : public QObject
{
    Q_OBJECT

public:
    enum Status {
        UndefinedStatus,
        NotWanted,
        NotNeeded,
        Loaded,
        NotLoaded,
        NotPlugin,
        MissingOrInvalidDependencies
    };

    explicit Plugin(const QString &pFileName, PluginInfo *pInfo,
                    const bool &pLoad, PluginManager *pPluginManager);
    ~Plugin();

    QString name() const;
    PluginInfo * info() const;
    QObject * instance() const;
    Status status() const;
    QString statusErrors() const;
    int statusErrorsCount() const;

    static QString name(const QString &pFileName);
    static QString fileName(const QString &pPluginsDir, const QString &pName);
    static PluginInfo * info(const QString &pFileName);

    static bool load(const QString &pName);
    static void setLoad(const QString &pName, const bool &pToBeLoaded);

    static QStringList fullDependencies(const QString &pPluginsDir,
                                        const QString &pName,
                                        const int &pLevel = 0);

private:
    QString mName;
    PluginInfo *mInfo;
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
