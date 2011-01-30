#include <QCoreApplication>
#include <QFileInfo>
#include <QProcess>

#include "common.h"

#include <iostream>

int main(int pArgc, char *pArgv[])
{
    QCoreApplication app(pArgc, pArgv);

    int res;

    // Some general initialisations

    initApplication(app);

    // Try to run OpenCOR as a console application

    if (consoleApplication(app, res))
        // OpenCOR was run as a proper console application, so...

        return res;
    else
    {
        // OpenCOR wasn't run as a proper console application, so start the GUI
        // version of OpenCOR

		QString dotExe = ".exe";

        if (app.applicationFilePath().right(4) == dotExe)
        {
            // This is a safeguard from accidentally running a non-renamed (to
            // '.com') console version of OpenCOR

            error(app, "the console version of "+app.applicationName()+" has the wrong extension ('.exe' instead of '.com').");

            return -1;
        }
        else
        {
            QString guiAppFilePath = app.applicationDirPath()+"/"+app.applicationName()+dotExe;

            if (!QFileInfo(guiAppFilePath).exists())
            {
                // We can't find the GUI version of OpenCOR, so...

                error(app, "the GUI version of "+app.applicationName()+" cannot be found.");

                return -1;
            }
            else
            {
                QProcess().startDetached(guiAppFilePath, app.arguments(), QProcess().workingDirectory());

                return 0;
            }
        }
    }
}
