#include "mainwindow.h"
#include "cmdLine.h"

#include <QFileInfo>

#include <QtSingleApplication>

int main(int pArgc, char *pArgv[])
{
    QtSingleApplication app(pArgc, pArgv);

#ifndef Q_WS_WIN
    // Try to run OpenCOR as a command line application
    // Note: in the case of Windows, we have two binaries (.com and .exe which
    //       are for the pure command line and GUI versions of OpenCOR, resp.).
    //       This means that when a console window is open, to enter something
    //       like:
    //           C:\>OpenCOR
    //       will effectively call OpenCOR.com. From there, should there be no
    //       arguments that require a command line treatment, then the GUI
    //       version of OpenCOR will be launched. This is, unfortunately, the
    //       only way to have OpenCOR behave as both a command line and GUI
    //       application on Windows, hence the ../winCmdLine/main.cpp file
    //       which is used to generate the command line version of OpenCOR...

    int res;

    if (cmdLineApplication(app, res))
        // OpenCOR was run as a proper command line application, so...

        return res;
#endif

    // Send a message (containing the arguments that were passed to this
    // instance of OpenCOR) to the 'official' instance of OpenCOR, should there
    // be one. If there is no 'official' instance of OpenCOR, then just carry
    // on as normal, otherwise exit since we only want one instance of OpenCOR
    // at any given time

    if (app.isRunning())
    {
        app.sendMessage(app.arguments().join(" "));

        return 0;
    }

    // Set the name of the organization

    app.setOrganizationName("World");

    // Set the name of the application

    app.setApplicationName(QFileInfo(app.applicationFilePath()).baseName());

    // Retrieve and set the version of the application

    QFile versionFile(":version");

    versionFile.open(QIODevice::ReadOnly);

    QString version = QString(versionFile.readLine()).trimmed();

    if (version.endsWith(".0"))
        // There is no actual patch information, so trim it

        version.truncate(version.length()-2);

    versionFile.close();

    app.setApplicationVersion(version);

    // Create the main window

    MainWindow win;

    // Keep track of the main window (useful for QtSingleApplication)

    app.setActivationWindow(&win);

    // Make sure that OpenCOR can handle the message sent by another
    // instance of itself

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                     &win, SLOT(singleAppMsgRcvd(const QString&)));

    // Show the main window

    win.show();

    // Execute the application

    return app.exec();
}
