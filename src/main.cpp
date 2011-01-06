#include "ui/mainwindow.h"

#include <QtSingleApplication>

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);

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

    // Create the main window

    MainWindow win;

    // Set the application icon
    // Note: normally, this would only be required for Linux, since in the case
    //       of Windows and Mac, it's done through CMake (see CMakeLists.txt).
    //       Yet, we do it for all three platforms, since it associates an icon
    //       to OpenCOR, which can then be used in, for example, the about box,
    //       so...

    win.setWindowIcon(QIcon(QString(":appIcon")));

    // Keep track of the main window (useful for QtSingleApplication)

    app.setActivationWindow(&win);

    // Make sure that OpenCOR can handle the message sent by another instance
    // of itself

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                     &win, SLOT(singleAppMsgRcvd(const QString&)));

    // Show the main window

    win.show();

    // Execute the application

    return app.exec();
}
