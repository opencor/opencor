#include "centralwidget.h"
#include "viewerwindow.h"
#include "commonwidget.h"
#include "mainwindow.h"
#include "filebrowserwindow.h"
#include "fileorganiserwindow.h"
#include "helpwindow.h"
#include "pmrexplorerwindow.h"
#include "utils.h"

#include "ui_mainwindow.h"

#ifdef Q_WS_WIN
    #include <windows.h>
#endif

#include <QCoreApplication>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QHelpEngine>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QShowEvent>

#include <QxtTemporaryDir>

#define OPENCOR_HOMEPAGE "http://opencor.sourceforge.net/"
#define OPENCOR_HELP_HOMEPAGE "qthelp://opencor/doc/userIndex.html"

#define SETTINGS_GENERAL_LOCALE "General_Locale"
#define SETTINGS_GENERAL_GEOMETRY "General_Geometry"
#define SETTINGS_GENERAL_STATE "General_State"

#define SETTINGS_MAINWINDOW_STATUSBARVISIBILITY "MainWindow_StatusBarVisibility"

#define SETTINGS_NONE ""

#define SYSTEM_LOCALE ""
#define ENGLISH_LOCALE "en"
#define FRENCH_LOCALE  "fr"

MainWindow::MainWindow(QWidget *pParent) :
    QMainWindow(pParent),
    mUi(new Ui::MainWindow),
    mLocale(SYSTEM_LOCALE)
{
    // Set the name of the main window to that of the application

    setWindowTitle(qApp->applicationName());

    // Set up the UI

    mUi->setupUi(this);
    // Note: the application icon (which needs to be set in the case of Linux,
    //       unlike in the case of Windows and Mac OS X, since it's set through
    //       CMake in those cases (see CMakeLists.txt)) is set within the UI
    //       file. Otherwise, it's good to have it set for all three platforms,
    //       since it can then be used in, for example, the about box...

    // Set some styles various parts of the application

    setStyleSheet(resourceAsByteArray(":appStyleSheet").constData());

    // Set the New toolbar button with its dropdown menu using our custom-made
    // action (actionNew)
    // Note: ideally, this would be done using the GUI designer in QtCreator,
    //       but it's not quite clear to me how this can be achieved, so...

    QMenu *actionNewMenu = new QMenu(this);

    mUi->actionNew->setMenu(actionNewMenu);

    actionNewMenu->addAction(mUi->actionCellML10File);
    actionNewMenu->addAction(mUi->actionCellML11File);

    mUi->fileToolbar->insertAction(mUi->actionOpen, mUi->actionNew);
    mUi->fileToolbar->insertSeparator(mUi->actionOpen);

    // Set the central widget

    mCentralWidget = new CentralWidget(this);

    setCentralWidget(mCentralWidget);

    // Allow for things to be dropped on us

    setAcceptDrops(true);

    // Create a temporary directory where to put OpenCOR's resources

    mTempDir = new QxtTemporaryDir();

    // Extract the help files

    QString applicationBaseName = QFileInfo(qApp->applicationFilePath()).baseName();

    mQchFileName = mTempDir->path()+QDir::separator()+applicationBaseName+".qch";
    mQhcFileName = mTempDir->path()+QDir::separator()+applicationBaseName+".qhc";

    saveResourceAs(":qchFile", mQchFileName);
    saveResourceAs(":qhcFile", mQhcFileName);

    // Set up the help engine

    mHelpEngine = new QHelpEngine(mQhcFileName);

    mHelpEngine->setupData();

    // Specify some general docking settings

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    // Create the various dock windows

    mPmrExplorerWindow = new PmrExplorerWindow(this);
    mFileBrowserWindow = new FileBrowserWindow(this);
    mFileOrganiserWindow = new FileOrganiserWindow(this);

    mViewerWindow = new ViewerWindow(this);

    mHelpWindow = new HelpWindow(mHelpEngine, QUrl(OPENCOR_HELP_HOMEPAGE), this);

    // Some connections to handle the various toolbars

    connect(mUi->actionFileToolbar, SIGNAL(triggered(bool)),
            mUi->fileToolbar, SLOT(setVisible(bool)));
    connect(mUi->fileToolbar->toggleViewAction(), SIGNAL(toggled(bool)),
            mUi->actionFileToolbar, SLOT(setChecked(bool)));

    connect(mUi->actionHelpToolbar, SIGNAL(triggered(bool)),
            mUi->helpToolbar, SLOT(setVisible(bool)));
    connect(mUi->helpToolbar->toggleViewAction(), SIGNAL(toggled(bool)),
            mUi->actionHelpToolbar, SLOT(setChecked(bool)));

    // A connection to handle the status bar

    connect(mUi->actionStatusBar, SIGNAL(triggered(bool)),
            statusBar(), SLOT(setVisible(bool)));

    // Some connections to handle various menu items

    connect(mUi->actionExit, SIGNAL(triggered(bool)),
            this, SLOT(close()));
    connect(mUi->actionResetAll, SIGNAL(triggered(bool)),
            this, SLOT(resetAll()));

    // Some connections to handle various dock windows

    connect(mUi->actionPMRExplorer, SIGNAL(triggered(bool)),
            mPmrExplorerWindow, SLOT(setVisible(bool)));
    connect(mPmrExplorerWindow, SIGNAL(visibilityChanged(bool)),
            mUi->actionPMRExplorer, SLOT(setChecked(bool)));

    connect(mUi->actionFileBrowser, SIGNAL(triggered(bool)),
            mFileBrowserWindow, SLOT(setVisible(bool)));
    connect(mFileBrowserWindow, SIGNAL(visibilityChanged(bool)),
            mUi->actionFileBrowser, SLOT(setChecked(bool)));

    connect(mUi->actionFileOrganiser, SIGNAL(triggered(bool)),
            mFileOrganiserWindow, SLOT(setVisible(bool)));
    connect(mFileOrganiserWindow, SIGNAL(visibilityChanged(bool)),
            mUi->actionFileOrganiser, SLOT(setChecked(bool)));

    connect(mUi->actionViewer, SIGNAL(triggered(bool)),
            mViewerWindow, SLOT(setVisible(bool)));
    connect(mViewerWindow, SIGNAL(visibilityChanged(bool)),
            mUi->actionViewer, SLOT(setChecked(bool)));

    connect(mUi->actionHelp, SIGNAL(triggered(bool)),
            mHelpWindow, SLOT(setVisible(bool)));
    connect(mHelpWindow, SIGNAL(visibilityChanged(bool)),
            mUi->actionHelp, SLOT(setChecked(bool)));

    // Default user settings (useful the very first time we start OpenCOR or
    // after a reset all)

    defaultSettings();

    // Retrieve the user settings from the previous session

    loadSettings();

    // Bring ourselves to the foreground. Indeed, when restarting OpenCOR (as a
    // result of a reset all), OpenCOR will on Mac OS X be behind other
    // applications. This behaviour has, on occasions, also been observed on
    // Windows, so... rather than making the following code Mac OS X specific,
    // we may as well make it general, since it doesn't have any adverse effect
    // on either Windows or Linux

    activateWindow();

    raise();   // Just to be on the safe side
}

MainWindow::~MainWindow()
{
    // Delete some internal objects

    delete mHelpEngine;
    delete mUi;

    // Delete the help files and then temporary directory by deleting the
    // temporary directory object

    QFile(mQchFileName).remove();
    QFile(mQhcFileName).remove();

    delete mTempDir;
}

void MainWindow::notYetImplemented(const QString &method)
{
    QMessageBox::information(this, qApp->applicationName()+" Information",
                             "Sorry, but the '"+method+"' method has not yet been implemented.");
}

void MainWindow::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    QMainWindow::changeEvent(pEvent);

    // If the system's locale has changed, then update OpenCOR's locale in case
    // the user wants to use the system's locale

    if (   (pEvent->type() == QEvent::LocaleChange)
        && (mUi->actionSystem->isChecked()))
        setLocale(SYSTEM_LOCALE);

    // Accept the event

    pEvent->accept();
}

void MainWindow::showEvent(QShowEvent *pEvent)
{
    // Default handling of the event

    QMainWindow::showEvent(pEvent);

    // The first time we show OpenCOR, we want to make sure that its menu is
    // fine (i.e. it respects OpenCOR's settings that were loaded in the
    // constructor). Various connections (set in the constructor) take care of
    // this, but there still remains one menu item (which specifies whether the
    // status bar is to be shown or not) for which no connection can be set. So,
    // we have to 'manually' set the status of that menu item here (as opposed
    // to, say, the constructor), since we may need (on Windows at least) all of
    // OpenCOR to be visible in order to be able to determine whether the status
    // bar is visible or not

    static bool firstTime = true;

    if (firstTime) {
        firstTime = false;

        mUi->actionStatusBar->setChecked(statusBar()->isVisible());
    }

    // Accept the event

    pEvent->accept();
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    // Keep track of our default settings
    // Note: it must be done here, as opposed to the destructor, otherwise some
    //       settings (e.g. dock windows) won't be properly saved

    saveSettings();

    // Accept the event

    pEvent->accept();

    // Default handling of the event

    QMainWindow::closeEvent(pEvent);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if we are dropping
    // URLs

    if (pEvent->mimeData()->hasUrls())
        pEvent->acceptProposedAction();
    else
        pEvent->ignore();
}

void MainWindow::dropEvent(QDropEvent *pEvent)
{
    QList<QUrl> urlList = pEvent->mimeData()->urls();
    QString urls;

    for (int i = 0; i < urlList.size(); ++i)
        urls += "<li>"+urlList.at(i).path()+"</li>";

    QMessageBox::information(this, qApp->applicationName(),
                             "You have just dropped the following files: <ul>"+urls+"</ul>");

    // Accept the proposed action for the event

    pEvent->acceptProposedAction();
}

void MainWindow::defaultSettingsForDockWindow(QDockWidget *pDockWindow,
                                              const Qt::DockWidgetArea &pDockArea,
                                              QDockWidget *pDockWidget)
{
    CommonWidget *commonWidget = dynamic_cast<CommonWidget *>(pDockWindow);

    if (commonWidget) {
        // The dynamic casting was successful, so we are really dealing with
        // the right kind of dock window and can therefore go ahead

        // Hide the dock window, so that we can set things up without having the
        // screen flashing

        pDockWindow->setVisible(false);

        // Position the dock window to its default location

        if (pDockWidget)
            tabifyDockWidget(pDockWidget, pDockWindow);
        else
            addDockWidget(pDockArea, pDockWindow);

        // Apply the dock window's default settings

        commonWidget->defaultSettings();

        // Make the dock window visible

        pDockWindow->setVisible(true);
    }
}

void MainWindow::defaultSettings()
{
    // Default language to be used by OpenCOR

    setLocale(SYSTEM_LOCALE);

    // Default size and position of the main window

    double ratio = 1.0/13.0;
    QRect desktopGeometry = qApp->desktop()->availableGeometry();
    int horizSpace = ratio*desktopGeometry.width();
    int vertSpace  = ratio*desktopGeometry.height();

    setGeometry(desktopGeometry.left()+horizSpace,
                desktopGeometry.top()+vertSpace,
                desktopGeometry.width()-2*horizSpace,
                desktopGeometry.height()-2*vertSpace);

    // Default visibility and location of the various toolbars

    addToolBar(Qt::TopToolBarArea, mUi->fileToolbar);
    addToolBar(Qt::TopToolBarArea, mUi->helpToolbar);

    mUi->fileToolbar->setVisible(true);
    mUi->helpToolbar->setVisible(true);

    // Default size and position of the various dock windows

    defaultSettingsForDockWindow(mPmrExplorerWindow, Qt::LeftDockWidgetArea);
    defaultSettingsForDockWindow(mFileBrowserWindow, Qt::LeftDockWidgetArea);
    defaultSettingsForDockWindow(mFileOrganiserWindow, Qt::NoDockWidgetArea, mFileBrowserWindow);

    defaultSettingsForDockWindow(mViewerWindow, Qt::TopDockWidgetArea);

    defaultSettingsForDockWindow(mHelpWindow, Qt::RightDockWidgetArea);
}

void MainWindow::loadSettings()
{
    QSettings settings(qApp->applicationName());

    // Retrieve the language to be used by OpenCOR

    setLocale(settings.value(SETTINGS_GENERAL_LOCALE, SYSTEM_LOCALE).toString());

    // Retrieve the geometry of the main window

    restoreGeometry(settings.value(SETTINGS_GENERAL_GEOMETRY).toByteArray());

    // Retrieve the state of the main window

    restoreState(settings.value(SETTINGS_GENERAL_STATE).toByteArray());

    // Retrieve whether the status bar is to be shown

    mUi->statusBar->setVisible(settings.value(SETTINGS_MAINWINDOW_STATUSBARVISIBILITY,
                                              true).toBool());

    // Retrieve the settings of the various dock windows

    mPmrExplorerWindow->loadSettings(settings, SETTINGS_NONE);
    mFileBrowserWindow->loadSettings(settings, SETTINGS_NONE);
    mFileOrganiserWindow->loadSettings(settings, SETTINGS_NONE);

    mViewerWindow->loadSettings(settings, SETTINGS_NONE);

    mHelpWindow->loadSettings(settings, SETTINGS_NONE);
}

void MainWindow::saveSettings()
{
    QSettings settings(qApp->applicationName());

    // Keep track of the language to be used by OpenCOR

    settings.setValue(SETTINGS_GENERAL_LOCALE, mLocale);

    // Keep track of the geometry of the main window

    settings.setValue(SETTINGS_GENERAL_GEOMETRY, saveGeometry());

    // Keep track of the state of the main window

    settings.setValue(SETTINGS_GENERAL_STATE, saveState());

    // Keep track of whether the status bar is to be shown

    settings.setValue(SETTINGS_MAINWINDOW_STATUSBARVISIBILITY,
                      mUi->statusBar->isVisible());

    // Keep track of the settings of the various dock windows

    mPmrExplorerWindow->saveSettings(settings, SETTINGS_NONE);
    mFileBrowserWindow->saveSettings(settings, SETTINGS_NONE);
    mFileOrganiserWindow->saveSettings(settings, SETTINGS_NONE);

    mViewerWindow->saveSettings(settings, SETTINGS_NONE);

    mHelpWindow->saveSettings(settings, SETTINGS_NONE);
}

void MainWindow::setLocale(const QString &pLocale)
{
    if ((pLocale != mLocale) || (pLocale == SYSTEM_LOCALE)) {
        QString realLocale = (pLocale == SYSTEM_LOCALE)?
                                 QLocale::system().name().left(2):
                                 pLocale;

        mLocale = pLocale;

        // Specify the language to be used by OpenCOR

        qApp->removeTranslator(&mQtTranslator);
        mQtTranslator.load(":qt_"+realLocale);
        qApp->installTranslator(&mQtTranslator);

        qApp->removeTranslator(&mAppTranslator);
        mAppTranslator.load(":app_"+realLocale);
        qApp->installTranslator(&mAppTranslator);

        // Translate the whole GUI (including any 'child' window), should the
        // language have changed

        mUi->retranslateUi(this);

        mPmrExplorerWindow->retranslateUi();
        mFileBrowserWindow->retranslateUi();
        mFileOrganiserWindow->retranslateUi();

        mViewerWindow->retranslateUi();

        mHelpWindow->retranslateUi();
    }

    // Update the checked menu item
    // Note: it has to be done every single time, since selecting a menu item
    //       will automatically toggle its checked status, so...

    mUi->actionSystem->setChecked(mLocale == SYSTEM_LOCALE);

    mUi->actionEnglish->setChecked(mLocale == ENGLISH_LOCALE);
    mUi->actionFrench->setChecked(mLocale == FRENCH_LOCALE);
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

    DWORD foregroundThreadPId = GetWindowThreadProcessId(GetForegroundWindow(),
                                                         NULL);
    DWORD mwThreadPId         = GetWindowThreadProcessId(mwWinId, NULL);

    if (foregroundThreadPId != mwThreadPId) {
        // OpenCOR's thread process Id is not that of the foreground window, so
        // attach the foreground thread to OpenCOR's, set OpenCOR to the
        // foreground, and detach the foreground thread from OpenCOR's

        AttachThreadInput(foregroundThreadPId, mwThreadPId, true);

        SetForegroundWindow(mwWinId);

        AttachThreadInput(foregroundThreadPId, mwThreadPId, false);
    } else {
        // OpenCOR's thread process Id is that of the foreground window, so
        // just set OpenCOR to the foreground

        SetForegroundWindow(mwWinId);
    }

    // Note: under Windows, to use activateWindow() will only highlight the
    //       application in the taskbar, since under Windows no application
    //       should be allowed to bring itself to the foreground when another
    //       application is already in the foreground. Fair enough, but it
    //       happens that, here, the user wants OpenCOR to be brought to the
    //       foreground, hence the above code to get the effect we are after...
#else
    // Do what one should normally do

    activateWindow();

    raise();   // Just to be on the safe side
    // Note: raise() never seems to be required on Mac OS X, but to use
    //       activateWindow() under Linux may or not give the expected result,
    //       so...
#endif

    // Now, we must handle the arguments that were passed to OpenCOR

    // TODO: handle the arguments passed to the 'official' instance of OpenCOR
}

void MainWindow::on_actionSystem_triggered()
{
    // Select the system's language as the language used by OpenCOR

    setLocale(SYSTEM_LOCALE);
}

void MainWindow::on_actionEnglish_triggered()
{
    // Select English as the language used by OpenCOR

    setLocale(ENGLISH_LOCALE);
}

void MainWindow::on_actionFrench_triggered()
{
    // Select French as the language used by OpenCOR

    setLocale(FRENCH_LOCALE);
}

void MainWindow::on_actionHomePage_triggered()
{
    // Look up OpenCOR's home page

    QDesktopServices::openUrl(QUrl(OPENCOR_HOMEPAGE));
}

void MainWindow::on_actionAbout_triggered()
{
    // Display some information about OpenCOR

    QMessageBox::about(this, qApp->applicationName(),
                       QString("")+
                       "<center>"+
                           "<h1><b>"+qApp->applicationName()+" "+qApp->applicationVersion()+"</b></h1>"+
                           "<h3><em>"+getOsName()+"</em></h3>"+
                       "</center>"+
                       "<br />"+
                       "<a href=\""+QString(OPENCOR_HOMEPAGE)+"\">"+qApp->applicationName()+"</a> "+tr("is a cross-platform <a href=\"http://www.cellml.org/\">CellML</a>-based modelling environment which can be used to organise, edit, simulate and analyse CellML files."));
}

void MainWindow::resetAll()
{
    if( QMessageBox::question(this, qApp->applicationName(),
                              tr("You are about to reset <strong>all</strong> of your user settings. Are you sure that this is what you want?"),
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::Yes) == QMessageBox::Yes ) {
        // Clear all the user settings and restart OpenCOR (indeed, a restart
        // will ensure that the various dock windows are, for instance, properly
        // reset with regards to their dimensions)

        QSettings(qApp->applicationName()).clear();

        // Restart OpenCOR, but without providing any of the argument with which
        // OpenCOR was originally started, since we indeed want to reset
        // everything

        QProcess::startDetached(qApp->applicationFilePath(), QStringList(),
                                qApp->applicationDirPath());

        // Quit OpenCOR
        // Note: the closeEvent method won't get called and this is exactly what
        //       we want, since we don't want to save OpenCOR's settings

        qApp->quit();
    }
}

void MainWindow::on_actionNew_triggered()
{
    notYetImplemented("void MainWindow::on_actionNew_triggered()");
}

void MainWindow::on_actionCellML10File_triggered()
{
    notYetImplemented("void MainWindow::on_actionCellML10File_triggered()");
}

void MainWindow::on_actionCellML11File_triggered()
{
    notYetImplemented("void MainWindow::on_actionCellML11File_triggered()");
}

void MainWindow::on_actionOpen_triggered()
{
    notYetImplemented("void MainWindow::on_actionOpen_triggered()");
}

void MainWindow::on_actionReopen_triggered()
{
    notYetImplemented("void MainWindow::on_actionReopen_triggered()");
}

void MainWindow::on_actionClose_triggered()
{
    notYetImplemented("void MainWindow::on_actionClose_triggered()");
}

void MainWindow::on_actionCloseAll_triggered()
{
    notYetImplemented("void MainWindow::on_actionCloseAll_triggered()");
}

void MainWindow::on_actionSave_triggered()
{
    notYetImplemented("void MainWindow::on_actionSave_triggered()");
}

void MainWindow::on_actionSaveAs_triggered()
{
    notYetImplemented("void MainWindow::on_actionSaveAs_triggered()");
}

void MainWindow::on_actionSaveAll_triggered()
{
    notYetImplemented("void MainWindow::on_actionSaveAll_triggered()");
}

void MainWindow::on_actionFullScreen_triggered()
{
    // Switch to / back from full screen mode

    if (mUi->actionFullScreen->isChecked())
        showFullScreen();
    else
        showNormal();
}
