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
// CLI utilities
//==============================================================================

#include "cliapplication.h"
#include "cliutils.h"
#include "coresettings.h"
#include "settings.h"

//==============================================================================

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QResource>
#include <QSettings>
#include <QTemporaryFile>

//==============================================================================

namespace OpenCOR {

//==============================================================================

#include "corecliutils.cpp.inl"

//==============================================================================

void initPluginsPath(const QString &pAppFileName)
{
    // Initialise the plugins path

    QFileInfo appFileInfo = pAppFileName;
    QString appDir;

#ifdef Q_OS_WIN
    if (appFileInfo.suffix().isEmpty())
        // If pAppFileName has no suffix, then it means we tried to run OpenCOR
        // using something like "[OpenCOR]/OpenCOR", in which case QFileInfo()
        // will get lost when trying to retrieve the canonical path for OpenCOR.
        // Now, when use something like "[OpenCOR]/OpenCOR", it's as if we were
        // to use something like "[OpenCOR]/OpenCOR.com", so update appFileInfo
        // accordingly

        appFileInfo = pAppFileName+".com";
#endif

    appDir = QDir::toNativeSeparators(appFileInfo.canonicalPath());

    QString pluginsDir = QString();

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    pluginsDir = appDir+QDir::separator()+QString("..")+QDir::separator()+"plugins";

    if (!QDir(pluginsDir).exists())
        // The plugins directory doesn't exist, which should only happen if we
        // are trying to run OpenCOR from within Qt Creator, in which case
        // OpenCOR's file name will be [OpenCOR]/build/OpenCOR.exe rather than
        // [OpenCOR]/build/bin/OpenCOR.exe as it should normally be if we were
        // to mimic the case where OpenCOR has been deployed. Then, because the
        // plugins are in [OpenCOR]/build/plugins/OpenCOR, we must skip the
        // "../" bit. So, yes, it's not neat, but is there another solution?...

        pluginsDir = appDir+QDir::separator()+"plugins";
#elif defined(Q_OS_MAC)
    pluginsDir = appDir+QDir::separator()+QString("..")+QDir::separator()+"PlugIns";
#else
    #error Unsupported platform
#endif

    pluginsDir = QDir::toNativeSeparators(QDir(pluginsDir).canonicalPath());

    QCoreApplication::setLibraryPaths(QStringList() << pluginsDir);
}

//==============================================================================

void initApplication(QCoreApplication *pApp, QString *pAppDate)
{
    // Remove all 'global' instances, in case OpenCOR previously crashed or
    // something (and therefore didn't remove all of them before quitting)

    OpenCOR::removeGlobalInstances();

    // Set the name of the application

    pApp->setApplicationName(QFileInfo(pApp->applicationFilePath()).baseName());

    // Retrieve and set the version of the application

    QString versionData;

    readTextFromFile(":app_versiondate", versionData);

    QStringList versionDataList = versionData.split(eolString());

    pApp->setApplicationVersion(versionDataList.first());

    if (pAppDate)
        *pAppDate = versionDataList.last();
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
    QString res = QString();
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
        bitVersion = QString();

    if (!appVersion.contains("-"))
        res += "Version ";
    else
        res += "Snapshot ";

    res += appVersion;

    if (!bitVersion.isEmpty())
        res += " ("+bitVersion+")";

    return res;
}

//==============================================================================

QString version(QCoreApplication *pApp)
{
    return  pApp->applicationName()+" "+shortVersion(pApp);
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
