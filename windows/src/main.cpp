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
// Main source file
//==============================================================================

#include "cliutils.h"
#include "common.h"

//==============================================================================

#include <iostream>

//==============================================================================

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QPointer>
#include <QProcess>

//==============================================================================

void error(QCoreApplication *pApp, const QString &pMsg)
{
    std::cout << qPrintable(OpenCOR::Core::version(pApp)) << std::endl;
    std::cout << std::endl;
    std::cout << "Error: " << qPrintable(pMsg) << std::endl;
}

//==============================================================================

int main(int pArgC, char *pArgV[])
{
    // Create the application

    QCoreApplication *app = new QCoreApplication(pArgC, pArgV);

    // Some general initialisations

    OpenCOR::initApplication(app);

    // Try to run OpenCOR as a CLI application

    int res;

    if (!OpenCOR::cliApplication(app, &res)) {
        // OpenCOR isn't meant to be run as a CLI application, so start its GUI
        // version instead

        static const QString dotExe = ".exe";

        if (app->applicationFilePath().right(dotExe.size()) == dotExe) {
            // This is a safeguard from accidentally running a non-renamed (to
            // '.com') CLI version of OpenCOR

            error(app, "the CLI version of "+app->applicationName()+" has the wrong extension ('.exe' instead of '.com').");

            res = -1;
        } else {
            QString guiAppFilePath = app->applicationDirPath()+QDir::separator()+app->applicationName()+dotExe;

            if (!QFileInfo(guiAppFilePath).exists()) {
                // We can't find the GUI version of OpenCOR, so...

                error(app, "the GUI version of "+app->applicationName()+" cannot be found.");

                res = -1;
            } else {
                QProcess().startDetached(guiAppFilePath, app->arguments(), QProcess().workingDirectory());

                res = 0;
            }
        }
    }

    // Release some memory

    delete app;

    // We are done, so...

    return res;
}

//==============================================================================
// End of file
//==============================================================================
