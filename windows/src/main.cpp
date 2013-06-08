//==============================================================================
// Main source file
//==============================================================================

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

            OpenCOR::error(app, "the CLI version of "+app->applicationName()+" has the wrong extension ('.exe' instead of '.com').");

            res = -1;
        } else {
            QString guiAppFilePath = app->applicationDirPath()+QDir::separator()+app->applicationName()+dotExe;

            if (!QFileInfo(guiAppFilePath).exists()) {
                // We can't find the GUI version of OpenCOR, so...

                OpenCOR::error(app, "the GUI version of "+app->applicationName()+" cannot be found.");

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
