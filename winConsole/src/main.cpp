#include <QCoreApplication>
#include <QFileInfo>
#include <QPointer>
#include <QProcess>

#include "common.h"

#include <iostream>

int main(int pArgc, char *pArgv[])
{
    int res;
    QPointer<QCoreApplication> app;

    // Create the application

    app = new QCoreApplication(pArgc, pArgv);

    // Some general initialisations

    initApplication(app);

    // Try to run OpenCOR as a console application

    if (!consoleApplication(app, &res)) {
        // OpenCOR wasn't run as a proper console application, so start its GUI
        // version instead

		QString dotExe = ".exe";

        if (app->applicationFilePath().right(4) == dotExe) {
            // This is a safeguard from accidentally running a non-renamed (to
            // '.com') console version of OpenCOR

            error(app, "the console version of "+app->applicationName()+" has the wrong extension ('.exe' instead of '.com').");

            res = -1;
        } else {
            QString guiAppFilePath = app->applicationDirPath()+"/"+app->applicationName()+dotExe;

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
