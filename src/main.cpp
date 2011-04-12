#include "mainwindow.h"
#include "common.h"

#include <QProcess>
#include <QSettings>

#include <QtSingleApplication>

int main(int pArgc, char *pArgv[])
{
    QtSingleApplication app(pArgc, pArgv);

    // Some general initialisations

    initApplication(app);

#ifndef Q_WS_WIN
    // Try to run OpenCOR as a console application
    // Note: in the case of Windows, we have two binaries (.com and .exe which
    //       are for the pure console and GUI versions of OpenCOR, resp.). This
    //       means that when a console window is open, to enter something like:
    //           C:\>OpenCOR
    //       will effectively call OpenCOR.com. From there, should there be no
    //       argument that requires console treatment, the GUI version of
    //       OpenCOR will then be launched. This is, unfortunately, the only way
    //       to have OpenCOR behave as both a console and GUI application on
    //       Windows, hence the ../winConsole/main.cpp file which is used to
    //       generate the console version of OpenCOR...

    int res;

    if (consoleApplication(app, res))
        // OpenCOR was run as a proper console application, so...

        return res;
#endif

    // Send a message (containing the arguments that were passed to this
    // instance of OpenCOR) to the 'official' instance of OpenCOR, should there
    // be one. If there is no 'official' instance of OpenCOR, then just carry
    // on as normal, otherwise exit since we only want one instance of OpenCOR
    // at any given time

    if (app.isRunning()) {
        app.sendMessage(app.arguments().join(" "));

        return 0;
    }

    // Create the main window

    MainWindow win;

    // Keep track of the main window (useful for QtSingleApplication)

    app.setActivationWindow(&win);

    // Make sure that OpenCOR can handle the message sent by another
    // instance of itself

    QObject::connect(&app, SIGNAL(messageReceived(const QString &)),
                     &win, SLOT(singleAppMsgRcvd(const QString &)));

    // Show the main window

    win.show();

    // Execute the application

#ifdef Q_WS_WIN
    int res;
#endif

    res = app.exec();

    // We are done with the execution of the application, so now the question is
    // whether we need to reset everything
    // Note: we do this here rather than 'within' the GUI because once we have
    //       launched a new instance of OpenCOR, we want this instance of
    //       OpenCOR to finish as soon as possible which will be the case here
    //       since all that remains to be done is to return the result of the
    //       execution of the application...

    if (win.needResetAll()) {
        // Clear all the user settings and restart OpenCOR (indeed, a restart
        // will ensure that the various dock windows are, for instance, properly
        // reset with regards to their dimensions)

        QSettings(app.applicationName()).clear();

        // Restart OpenCOR, but without providing any of the argument with which
        // OpenCOR was originally started, since we indeed want to reset
        // everything

        QProcess::startDetached(app.applicationFilePath(), QStringList(),
                                app.applicationDirPath());
    }

    return res;
}
