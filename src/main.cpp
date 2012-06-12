//==============================================================================
// Main
//==============================================================================

#include "mainwindow.h"
#include "common.h"

//==============================================================================

#include <QDir>
#include <QPointer>
#include <QProcess>
#include <QSettings>

//==============================================================================

#include <QtSingleApplication>

//==============================================================================

using namespace OpenCOR;

//==============================================================================

void removeInstances()
{
    // Remove all 'global' instances

    QSettings(qApp->applicationName()).remove("Instances");
}

//==============================================================================

int main(int pArgc, char *pArgv[])
{
    int res;

    // Create the application

    SharedTools::QtSingleApplication *app = new SharedTools::QtSingleApplication(pArgc, pArgv);

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

    if (consoleApplication(app, &res)) {
        // OpenCOR was run as a proper console application, so...

        delete app;

        return res;
    }
#endif

    // Send a message (containing the arguments that were passed to this
    // instance of OpenCOR minus the first argument since it corresponds to the
    // full path to the executable which we are not interested in) to the
    // 'official' instance of OpenCOR, should there be one. If there is no
    // 'official' instance of OpenCOR, then just carry on as normal, otherwise
    // exit since we only want one instance of OpenCOR at any given time

    QStringList appArguments = app->arguments();

    appArguments.removeFirst();

    QString arguments = appArguments.join("|");

    if (app->isRunning()) {
        app->sendMessage(arguments);

        delete app;

        return 0;
    }

    // Specify where to find non-OpenCOR plugins (only required on Windows)

#ifdef Q_WS_WIN
    app->addLibraryPath( QDir(app->applicationDirPath()).canonicalPath()
                        +QDir::separator()+QString("..")
                        +QDir::separator()+"plugins");
#endif

    // Remove all 'global' instances, in case OpenCOR previously crashed or
    // something (and therefore didn't remove all of them before quitting)

    removeInstances();

    // Create the main window

    MainWindow *win = new MainWindow();

    // Keep track of the main window (required by QtSingleApplication so that it
    // can do what it's supposed to be doing)

    app->setActivationWindow(win);

    // Make sure that OpenCOR can handle the message sent by another instance of
    // itself

    QObject::connect(app, SIGNAL(messageReceived(const QString &)),
                     win, SLOT(singleAppMsgRcvd(const QString &)));

    // Handle the arguments

    win->handleArguments(arguments);

    // Show the main window

    win->show();

    // Execute the application

    res = app->exec();

    // Keep track of the application file and directory paths (in case we need
    // to restart OpenCOR)

    QString appFilePath = app->applicationFilePath();
    QString appDirPath  = app->applicationDirPath();

    // Delete the main window

    delete win;

    // Remove all 'global' instances that were created and used during this
    // session

    removeInstances();

    // Delete the application

    delete app;

    // We are done with the execution of the application, so now the question is
    // whether we need to restart or not
    // Note: we do this here rather than 'within' the GUI because once we have
    //       launched a new instance of OpenCOR, we want this instance of
    //       OpenCOR to finish as soon as possible which will be the case here
    //       since all that remains to be done is to return the result of the
    //       execution of the application...

    if (res == OpenCOR::NeedRestart)
        // Restart OpenCOR, but without providing any of the argument with which
        // OpenCOR was originally started, since we want to reset everything

        QProcess::startDetached(appFilePath, QStringList(), appDirPath);

    // We are done, so...

    return res;
}

//==============================================================================
// End of file
//==============================================================================
