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
// Main
//==============================================================================

#include "cliapplication.h"
#include "cliutils.h"

//==============================================================================

#include <iostream>

//==============================================================================

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QPointer>
#include <QProcess>
#include <QSettings>

//==============================================================================

void error(const QString &pMsg)
{
    std::cout << qPrintable(OpenCOR::version()) << std::endl;
    std::cout << std::endl;
    std::cout << "Error: " << qPrintable(pMsg) << std::endl;
}

//==============================================================================

int main(int pArgC, char *pArgV[])
{
    // Initialise Qt's message pattern

    OpenCOR::initQtMessagePattern();

    // Initialise the plugins path

    OpenCOR::initPluginsPath(pArgV[0]);

    // Create our application

    OpenCOR::CliApplication *cliApp = new OpenCOR::CliApplication(pArgC, pArgV);

    // Some general initialisations

    OpenCOR::initApplication();

    // Try to run OpenCOR as a CLI application

    int res;

    if (!cliApp->run(&res)) {
        // OpenCOR isn't meant to be run as a CLI application, so start its GUI
        // version instead

        static const QString DotExe = ".exe";

        if (cliApp->applicationFilePath().right(DotExe.size()) == DotExe) {
            // This is a safeguard from accidentally running a non-renamed (to
            // '.com') CLI version of OpenCOR

            error("the CLI version of "+qAppName()+" has the wrong extension ('.exe' instead of '.com').");

            res = -1;
        } else {
            QString guiAppFilePath = cliApp->applicationDirPath()+QDir::separator()+qAppName()+DotExe;

            if (!QFile::exists(guiAppFilePath)) {
                // We can't find the GUI version of OpenCOR, so...

                error("the GUI version of "+qAppName()+" cannot be found.");

                res = -1;
            } else {
                // We found the GUI version of OpenCOR, so run it with our
                // arguments, minus the first one since it corresponds to the
                // full path to our executable, which we are not interested in

                QStringList appArguments = cliApp->arguments();

                appArguments.removeFirst();

                QProcess().startDetached(guiAppFilePath, appArguments, QProcess().workingDirectory());

                res = 0;
            }
        }
    }

    // Release some memory

    delete cliApp;

    // We are done, so...

    return res;
}

//==============================================================================
// End of file
//==============================================================================
