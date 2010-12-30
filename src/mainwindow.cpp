#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef Q_WS_WIN
    #include <windows.h>
#endif

#include <QDesktopServices>
#include <QMessageBox>
#include <QSettings>
#include <QUrl>

#define OPENCOR_HOMEPAGE "http://opencor.sourceforge.net/"

#define SETTINGS_INSTITUTION "World"
#define SETTINGS_LOCALE "Locale"
#define SETTINGS_GEOMETRY "Geometry"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    // Set up the GUI

    ui->setupUi(this);

    // Retrieve our default settings

    loadSettings();
}

MainWindow::~MainWindow()
{
    // Keep track of our default settings

    saveSettings();

    // Delete the GUI

    delete ui;
}

void MainWindow::singleAppMsgRcvd(const QString&)
{
    // We have just received a message from another instance of OpenCOR, so
    // bring ourselves to the foreground
    // Note: one would normally use activateWindow(), but depending on the
    //       operating system it may or not bring OpenCOR to the foreground,
    //       so... instead we do what follows, depending on the operating
    //       system...

#ifdef Q_WS_WIN
    // Retrieve OpenCOR's window Id

    WId mwWinId = winId();

    // Restore OpenCOR, should it be minimized

    if (IsIconic(mwWinId))
        SendMessage(mwWinId, WM_SYSCOMMAND, SC_RESTORE, 0);

    // Bring OpenCOR to the foreground

    DWORD foregroundThreadPId = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    DWORD mwThreadPId         = GetWindowThreadProcessId(mwWinId, NULL);

    if (foregroundThreadPId != mwThreadPId)
    {
        // OpenCOR's thread process Id is not that of the foreground window, so
        // attach the foreground thread to OpenCOR's, set OpenCOR to the
        // foreground, and detach the foreground thread from OpenCOR's

        AttachThreadInput(foregroundThreadPId, mwThreadPId, true);

        SetForegroundWindow(mwWinId);

        AttachThreadInput(foregroundThreadPId, mwThreadPId, false);
    }
    else
        // OpenCOR's thread process Id is that of the foreground window, so
        // just set OpenCOR to the foreground

        SetForegroundWindow(mwWinId);

    // Note: under Windows, to use activateWindow() will only highlight the
    //       application in the taskbar, since under Windows no application
    //       should be allowed to bring itself to the foreground when another
    //       application is already in the foreground. Fair enough, but it
    //       happens that, here, the user wants OpenCOR to be brought to the
    //       foreground, hence the above code to get the effect we are after...
#else
    // Do what one should normally do and which works fine under Mac OS X

    activateWindow();

    #ifdef Q_WS_X11
        raise();
        // Note: under Linux, to use activateWindow() may or not give the
        //       expected result. The above code is supposed to make sure that
        //       OpenCOR gets brought to the foreground, but that itsn't the
        //       case under Ubuntu at least (it works when you want to open a
        //       second instance of OpenCOR, but not thereafter!)...
    #endif
#endif

    // Now, we must handle the arguments that were passed to us

    // TODO: handle the arguments passed to the 'official' instance of OpenCOR
}

void MainWindow::loadSettings()
{
    QSettings settings(SETTINGS_INSTITUTION, qApp->applicationName());

    // Retrieve the geometry of the main window

    restoreGeometry(settings.value(SETTINGS_GEOMETRY).toByteArray());

    // Retrieve the language to be used by OpenCOR, which by default is based
    // on the system's locale

    setLocale(settings.value(SETTINGS_LOCALE, QLocale::system().name()).toString());
}

void MainWindow::saveSettings()
{
    QSettings settings(SETTINGS_INSTITUTION, qApp->applicationName());

    // Keep track of the geometry of the main window

    settings.setValue(SETTINGS_GEOMETRY, saveGeometry());

    // Keep track of the language to be used by OpenCOR

    settings.setValue(SETTINGS_LOCALE, locale);
}

void MainWindow::setLocale(const QString& newLocale)
{
    if (newLocale != locale)
    {
        locale = newLocale;

        // Specify the language to be used by OpenCOR

//        qApp->removeTranslator(qtTranslator);
//        qtTranslator->load("qt_"+newLocale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
//        qApp->installTranslator(qtTranslator);

//        qApp->removeTranslator(appTranslator);
//        appTranslator->load(":app_"+newLocale);
//        qApp->installTranslator(appTranslator);
    }

    // Update the checked menu item
    // Note: it has to be done every single time, since selecting a menu item
    //       will automatically toggle its checked status, so...

    ui->actionEnglish->setChecked(newLocale.startsWith("en"));
    ui->actionFrench->setChecked(newLocale.startsWith("fr"));
}

void MainWindow::notYetImplemented(const QString& message)
{
    // Display a warning message about a particular feature having not yet been
    // implemented

    QMessageBox::warning(this, "OpenCOR", message+tr(" has not yet been implemented..."),
                         QMessageBox::Ok, QMessageBox::Ok);
}

void MainWindow::on_actionExit_triggered()
{
    // Exit OpenCOR

    close();
}

void MainWindow::on_actionEnglish_triggered()
{
    // Select English as the language used by OpenCOR

    setLocale("en");
}

void MainWindow::on_actionFrench_triggered()
{
    // Select French as the language used by OpenCOR

    setLocale("fr");
}

void MainWindow::on_actionHelp_triggered()
{
    notYetImplemented("MainWindow::on_actionHelp_triggered");
}

void MainWindow::on_actionHomepage_triggered()
{
    // Look up the OpenCOR home page

    QDesktopServices::openUrl(QUrl(OPENCOR_HOMEPAGE));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, qApp->applicationName(),
                       "<A HREF = \""+QString(OPENCOR_HOMEPAGE)+"\">OpenCOR</A> "+tr("is a cross-platform <A HREF = \"http://www.cellml.org/\">CellML</A>-based modelling environment written in C++, using the <A HREF = \"http://qt.nokia.com/\">Qt framework</A>. It can be used to organise, edit, simulate and analyse CellML files."));
}
