#include "ui/mainwindow.h"

#include <QFileInfo>
#include <QProcess>

#include <QtSingleApplication>

int main(int pArgc, char *pArgv[])
{
    QtSingleApplication app(pArgc, pArgv);

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

    bool restart = false;

    MainWindow win(&restart);
    // Note: the application icon (which is useful for Linux, since in the case
    //       of Windows and Mac, it's set through CMake (see CMakeLists.txt))
    //       is set within the UI file. Otherwise, it's good to have it set for
    //       all three platforms, since it can then be used in, for example,
    //       the about box...

    // Keep track of the main window (useful for QtSingleApplication)

    app.setActivationWindow(&win);

    // Make sure that OpenCOR can handle the message sent by another
    // instance of itself

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                     &win, SLOT(singleAppMsgRcvd(const QString&)));

    // Show the main window

    win.show();

    // Execute the application

    int res = app.exec();

    if (restart)
        QProcess::startDetached(app.applicationFilePath(), QStringList(), app.applicationDirPath());
        // Note: because we are restarting as a result of a result all, we must
        //       not provide OpenCOR with any argument, even if it was provided
        //       with some in the first place

    return res;
}
