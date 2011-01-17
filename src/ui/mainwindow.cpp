#include "mainwindow.h"
#include "helpwindow.h"
#include "utils.h"

#include "ui_mainwindow.h"

#ifdef Q_WS_WIN
    #include <windows.h>
#endif

#include <QApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QHelpEngine>
#include <QMessageBox>
#include <QSettings>
#include <QTemporaryFile>
#include <QUrl>

#define OPENCOR_HOMEPAGE "http://opencor.sourceforge.net/"

#define OPENCOR_HELP_HOMEPAGE QUrl("qthelp://world.opencor/doc/index.html")

#define SETTINGS_INSTITUTION "World"
#define SETTINGS_GENERAL_LOCALE "General_Locale"
#define SETTINGS_GENERAL_GEOMETRY "General_Geometry"
#define SETTINGS_GENERAL_STATE "General_State"
#define SETTINGS_HELPWINDOW_ZOOMLEVEL "HelpWindow_ZoomLevel"

MainWindow::MainWindow(bool *pRestart, QWidget *pParent) :
    QMainWindow(pParent),
    mUi(new Ui::MainWindow),
    mRestart(pRestart)
{
    // Set up the GUI

    mUi->setupUi(this);

    // By default, we don't want OpenCOR to restart automatically upon exiting
    // it

    *mRestart = false;

    // Set the name of the main window to that of the application

    setWindowTitle(qApp->applicationName());

    // Some basic signals/events for some actions

    connect(mUi->actionExit, SIGNAL(triggered(bool)),
            this, SLOT(close()));
    connect(mUi->actionResetAll, SIGNAL(triggered(bool)),
            this, SLOT(resetAll()));

    // Signals/events for showing/hiding the various toolbars

    connect(mUi->actionHelpToolbar, SIGNAL(triggered(bool)),
            mUi->helpToolbar, SLOT(setVisible(bool)));
    connect(mUi->helpToolbar->toggleViewAction(), SIGNAL(toggled(bool)),
            mUi->actionHelpToolbar, SLOT(setChecked(bool)));

    // Extract the help files

    QTemporaryFile tempDir;

    tempDir.open();    // Note: this is required to get a 'valid' temporary
    tempDir.close();   //       directory name...

    mTempDirName = tempDir.fileName().append("."+qApp->applicationName());

    QDir().mkdir(mTempDirName);

    QString applicationBaseName(QFileInfo(qApp->applicationFilePath()).baseName());

    mQchFileName = mTempDirName+QDir::separator()+applicationBaseName+".qch";
    mQhcFileName = mTempDirName+QDir::separator()+applicationBaseName+".qhc";

    saveResourceAs(":qchFile", mQchFileName);
    saveResourceAs(":qhcFile", mQhcFileName);

    // Set up the help engine

    mHelpEngine = new QHelpEngine(mQhcFileName);

    mHelpEngine->setupData();

    // Help window

    mHelpWindow = new HelpWindow(mHelpEngine, OPENCOR_HELP_HOMEPAGE);

    connect(mUi->actionHelp, SIGNAL(triggered(bool)),
            mHelpWindow, SLOT(setVisible(bool)));
    connect(mHelpWindow, SIGNAL(visibilityChanged(bool)),
            mUi->actionHelp, SLOT(setChecked(bool)));

    // Default user settings (useful the very first time we start OpenCOR or
    // after a reset all)

    defaultSettings();

    // Retrieve our default settings

    loadSettings();

    // Update the GUI, which may have changed (e.g. hidden toolbar) as a result
    // of loading OpenCOR's settings

    updateGUI();
}

MainWindow::~MainWindow()
{
    // Delete some internal objects

    delete mHelpEngine;
    delete mUi;

    // Delete the help files

    QFile(mQchFileName).remove();
    QFile(mQhcFileName).remove();

    QDir().rmdir(mTempDirName);
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    // Keep track of our default settings, but only if we don't want to restart
    // OpenCOR (i.e. as a result of a reset all)
    // Note: it must be done here, as opposed to the destructor, otherwise some
    //       settings (e.g. docked windows) won't be properly saved

    if (!*mRestart)
    {
        saveSettings();

        pEvent->accept();
    }
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

void MainWindow::resetAll()
{
    // Clear all the user settings and asked for OpenCOR to be restarted
    // (indeed, a restart will ensure that all the docked windows are properly
    // reset with regards to their dimensions)

    QSettings(SETTINGS_INSTITUTION, qApp->applicationName()).clear();

    *mRestart = true;

    close();
}

void MainWindow::defaultSettings()
{
    // Default language to be used by OpenCOR

    setLocale(QLocale::system().name());

    // Default size and position of the main window

    const double spaceRatio = 1.0/13.0;
    const QRect desktopGeometry = qApp->desktop()->availableGeometry();
    const int horizSpace = spaceRatio*desktopGeometry.width();
    const int vertSpace  = spaceRatio*desktopGeometry.height();

    setGeometry(desktopGeometry.left()+horizSpace, desktopGeometry.top()+vertSpace, desktopGeometry.width()-2*horizSpace, desktopGeometry.height()-2*vertSpace);

    // Default size and position of the help window

    mHelpWindow->setVisible(false);   // So we can set things up without having
                                      // the screen flashing

    addDockWidget(Qt::RightDockWidgetArea, mHelpWindow);

    mHelpWindow->defaultSettings();

    mHelpWindow->setVisible(true);

    // Default visibility and location of the various toolbars

    addToolBar(Qt::TopToolBarArea, mUi->helpToolbar);

    mUi->helpToolbar->setVisible(true);
}

void MainWindow::loadSettings()
{
    QSettings settings(SETTINGS_INSTITUTION, qApp->applicationName());

    // Retrieve the language to be used by OpenCOR, with a default just in case

    setLocale(settings.value(SETTINGS_GENERAL_LOCALE, QLocale::system().name()).toString());

    // Retrieve the geometry of the main window

    restoreGeometry(settings.value(SETTINGS_GENERAL_GEOMETRY).toByteArray());

    // Retrieve the state of the main window

    restoreState(settings.value(SETTINGS_GENERAL_STATE).toByteArray());

    // Retrieve the zoom level for the help widget, with a default value just
    // in case

    mHelpWindow->setZoomLevel(settings.value(SETTINGS_HELPWINDOW_ZOOMLEVEL, mHelpWindow->defaultZoomLevel()).toInt());
}

void MainWindow::saveSettings()
{
    QSettings settings(SETTINGS_INSTITUTION, qApp->applicationName());

    // Keep track of the language to be used by OpenCOR

    settings.setValue(SETTINGS_GENERAL_LOCALE, mLocale);

    // Keep track of the geometry of the main window

    settings.setValue(SETTINGS_GENERAL_GEOMETRY, saveGeometry());

    // Keep track of the state of the main window

    settings.setValue(SETTINGS_GENERAL_STATE, saveState());

    // Keep track of the text size multiplier for the help widget

    settings.setValue(SETTINGS_HELPWINDOW_ZOOMLEVEL, mHelpWindow->zoomLevel());
}

void MainWindow::setLocale(const QString& pLocale)
{
    if (pLocale != mLocale)
    {
        mLocale = pLocale;

        // Specify the language to be used by OpenCOR

//        qApp->removeTranslator(qtTranslator);
//        qtTranslator->load("qt_"+pLocale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
//        qApp->installTranslator(qtTranslator);

//        qApp->removeTranslator(appTranslator);
//        appTranslator->load(":app_"+pLocale);
//        qApp->installTranslator(appTranslator);
    }

    // Update the checked menu item
    // Note: it has to be done every single time, since selecting a menu item
    //       will automatically toggle its checked status, so...

    mUi->actionEnglish->setChecked(pLocale.startsWith("en"));
    mUi->actionFrench->setChecked(pLocale.startsWith("fr"));
}

void MainWindow::notYetImplemented(const QString& pMsg)
{
    // Display a warning message about a particular feature having not yet been
    // implemented

    QMessageBox::warning(this, qApp->applicationName(), pMsg+tr(" has not yet been implemented..."),
                         QMessageBox::Ok, QMessageBox::Ok);
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

void MainWindow::updateGUI()
{
    // Update the checked status of the toolbars menu items

    mUi->actionHelpToolbar->setChecked(mUi->helpToolbar->isVisible());
}

void MainWindow::on_actionHomepage_triggered()
{
    // Look up the OpenCOR home page

    QDesktopServices::openUrl(QUrl(OPENCOR_HOMEPAGE));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, qApp->applicationName(),
                       QString("")+
                       "<CENTER>"+
                           "<H1><B>"+qApp->applicationName()+" "+qApp->applicationVersion()+"</B></H1>"+
                           "<H3><I>"+getOsName()+"</I></H3>"+
                       "</CENTER>"+
                       "<BR>"+
                       "<A HREF = \""+QString(OPENCOR_HOMEPAGE)+"\">"+qApp->applicationName()+"</A> "+tr("is a cross-platform <A HREF = \"http://www.cellml.org/\">CellML</A>-based modelling environment which can be used to organise, edit, simulate and analyse CellML files."));
}
