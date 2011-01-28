#include <QCoreApplication>
#include <QFileInfo>
#include <QProcess>

#include "cmdLine.h"

int main(int pArgc, char *pArgv[])
{
    QCoreApplication app(pArgc, pArgv);

    int res;

    if (cmdLineApplication(app, res))
        // OpenCOR was run as a proper command line application, so...

        return res;
    else
    {
        // OpenCOR wasn't run as a proper command line application, so start
        // the GUI version of OpenCOR

        QProcess().startDetached(app.applicationDirPath()+"/"+QFileInfo(app.applicationFilePath()).baseName()+".exe",
                                 app.arguments(), QProcess().workingDirectory());

        return 0;
    }
}
