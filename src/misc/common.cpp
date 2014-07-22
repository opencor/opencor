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
// Some common methods between the CLI and GUI versions of OpenCOR
//==============================================================================

#include "cliapplication.h"
#include "coresettings.h"
#include "common.h"
#include "plugin.h"
#include "settings.h"

//==============================================================================

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QRegularExpression>
#include <QSettings>
#include <QString>

//==============================================================================

namespace OpenCOR {

//==============================================================================

void initPluginsPath(const QString &pAppFileName)
{
    // Initialise the plugins path
    // Note: pAppFileName contains OpenCOR's file name. Using this, we want to
    //       retrieve OpenCOR's file path. Normally, we would be using
    //       QFileInfo(pAppFileName).canonicalPath(), but this may not always
    //       work. Indeed, say that you want to run OpenCOR from the command
    //       line by simply entering something like "[OpenCOR]/OpenCOR", then
    //       QFileInfo() will get lost since "[OpenCOR]/OpenCOR" doesn't refer
    //       to a physical file (it would need either the .exe or .com extension
    //       for this). So, instead, we manually retrieve OpenCOR's file path...

    QString appFilePath = pAppFileName;
    QString pluginsDir = QString();

    if (appFilePath.contains(QRegularExpression("[/\\\\]")))
        appFilePath.remove(QRegularExpression("[/\\\\][^/\\\\]*$"));
    else
        appFilePath = ".";

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    pluginsDir = appFilePath+QDir::separator()+QString("..")+QDir::separator()+"plugins";

    if (!QDir(pluginsDir).exists())
        // The plugins directory doesn't exist, which should only happen if we
        // are trying to run OpenCOR from within Qt Creator, in which case
        // OpenCOR's file name will be [OpenCOR]/build/OpenCOR.exe rather than
        // [OpenCOR]/build/bin/OpenCOR.exe as it should normally be if we were
        // to mimic the case where OpenCOR has been deployed. Then, because the
        // plugins are in [OpenCOR]/build/plugins/OpenCOR, we must skip the
        // "../" bit. So, yes, it's not neat, but is there another solution?...

        pluginsDir = appFilePath+QDir::separator()+"plugins";
#elif defined(Q_OS_MAC)
    pluginsDir = appFilePath+QDir::separator()+QString("..")+QDir::separator()+"PlugIns";
#else
    #error Unsupported platform
#endif

    pluginsDir = QDir::toNativeSeparators(QDir(pluginsDir).canonicalPath());

    QCoreApplication::setLibraryPaths(QStringList() <<  pluginsDir);
}

//==============================================================================

void initApplication(QCoreApplication *pApp)
{
    // Set the name of the application

    pApp->setApplicationName(QFileInfo(pApp->applicationFilePath()).baseName());

    // Retrieve and set the version of the application

    QFile versionFile(":app_version");

    versionFile.open(QIODevice::ReadOnly);

    pApp->setApplicationVersion(QString(versionFile.readLine()).trimmed());

    versionFile.close();
}

//==============================================================================

bool cliApplication(QCoreApplication *pApp, int *pRes)
{
    // Create and run our CLI application object

    CliApplication *cliApp = new CliApplication(pApp);

    bool res = cliApp->run(pRes);

    delete cliApp;

    return res;
}

//==============================================================================

void removeGlobalInstances()
{
    // Remove all the 'global' information shared between OpenCOR and its
    // different plugins

    QSettings(SettingsOrganization, SettingsApplication).remove(SettingsGlobal);
}

//==============================================================================

QString shortVersion(QCoreApplication *pApp)
{
    QString res;
    QString appVersion = pApp->applicationVersion();

    if (!appVersion.contains("-"))
        res += "Version ";
    else
        res += "Snapshot ";

    res += appVersion;

    return res;
}

//==============================================================================

QString version(QCoreApplication *pApp)
{
    QString appVersion = pApp->applicationVersion();
    QString bitVersion;

    enum {
        SizeOfPointer = sizeof(void *)
    };

    if (SizeOfPointer == 4)
        bitVersion = "32-bit";
    else if (SizeOfPointer == 8)
        bitVersion = "64-bit";
    else
        // Not a size that we could recognise, so...

        bitVersion = "";

    bool snapshot = appVersion.contains("-");
    QString res = pApp->applicationName()+" ";

    if (snapshot)
        res += "[";

    res += appVersion;

    if (snapshot)
        res += "]";

    if (!bitVersion.isEmpty())
        res += " ("+bitVersion+")";

    return res;
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
