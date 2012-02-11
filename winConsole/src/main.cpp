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

using namespace OpenCOR;

//==============================================================================

int main(int pArgc, char *pArgv[])
{
    // Create the application

    QCoreApplication app(pArgc, pArgv);

    // Some general initialisations

    initApplication(app);

    // Try to run OpenCOR as a console application

    int res;

    if (consoleApplication(app, res)) {
        // OpenCOR was run as a proper console application, so...

        return res;
    } else {
        // OpenCOR wasn't run as a proper console application, so start its GUI
        // version instead

        static const QString dotExe = ".exe";
        static const int dotExeSize = dotExe.size();

        if (app.applicationFilePath().right(dotExeSize) == dotExe) {
            // This is a safeguard from accidentally running a non-renamed (to
            // '.com') console version of OpenCOR

            error(app, "the console version of "+app.applicationName()+" has the wrong extension ('.exe' instead of '.com').");

            return -1;
        } else {
            QString guiAppFilePath = app.applicationDirPath()+QDir::separator()+app.applicationName()+dotExe;

            if (!QFileInfo(guiAppFilePath).exists()) {
                // We can't find the GUI version of OpenCOR, so...

                error(app, "the GUI version of "+app.applicationName()+" cannot be found.");

                return -1;
            } else {
                QProcess().startDetached(guiAppFilePath, app.arguments(), QProcess().workingDirectory());

                return 0;
            }
        }
    }
}

//==============================================================================
// End of file
//==============================================================================
