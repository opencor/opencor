#include <QCoreApplication>
#include <QFileInfo>
#include <QProcess>

#include "common.h"

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

        QProcess().startDetached(app.applicationDirPath()+"/"+QFileInfo(app.applicationFilePath()).baseName()+".exe",
                                 app.arguments(), QProcess().workingDirectory());

        return 0;
    }
}
