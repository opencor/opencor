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
#include "cliutils.h"
#include "common.h"
#include "plugin.h"

//==============================================================================

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QSettings>
#include <QString>

//==============================================================================

namespace OpenCOR {

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
    // Create our CLI application object

    CliApplication *cliApp = new CliApplication(pApp);

    // Run our CLI application

    return cliApp->run(pRes);
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
