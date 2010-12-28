#include <QtGui/QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create the main window

    MainWindow win;

    // Set the application icon, but only for Linux, since in the case of
    // Windows and Mac, it's done through CMake (see CMakeLists.txt)

#ifdef Q_WS_X11
    win.setWindowIcon(QIcon(QString(":appIcon")));
#endif

    // Set the name of the main window to OpenCOR

    win.setWindowTitle("OpenCOR");

    // Show the main window

    win.show();

    // Execute the application

    return app.exec();
}
