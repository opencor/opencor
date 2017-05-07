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

        if (qApp->applicationFilePath().right(DotExe.size()) == DotExe) {
            // This is a safeguard from accidentally running a non-renamed (to
            // '.com') CLI version of OpenCOR

            error("the CLI version of "+qAppName()+" has the wrong extension ('.exe' instead of '.com').");

            res = -1;
        } else {
            QString guiAppFilePath = qApp->applicationDirPath()+QDir::separator()+qAppName()+DotExe;

            if (!QFile::exists(guiAppFilePath)) {
                // We can't find the GUI version of OpenCOR, so...

                error("the GUI version of "+qAppName()+" cannot be found.");

                res = -1;
            } else {
                // We found the GUI version of OpenCOR, so run it with our
                // arguments, minus the first one since it corresponds to the
                // full path to our executable, which we are not interested in

                QStringList appArguments = qApp->arguments();

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
