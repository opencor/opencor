#include "checkforupdateswindow.h"
#include "common.h"
#include "guiinterface.h"
#include "mainwindow.h"
#include "plugin.h"
#include "pluginmanager.h"
#include "pluginswindow.h"
#include "preferenceswindow.h"
#include "utils.h"

#ifdef Q_WS_WIN
    #include <windows.h>
#endif

#include <QApplication>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#ifdef Q_WS_MAC
    #include <QShortcut>
#endif
#include <QUrl>

namespace OpenCOR {

static const QString SystemLocale  = "";
static const QString EnglishLocale = "en";
static const QString FrenchLocale  = "fr";

void MainWindowUi::setupUi(MainWindow *pMainWindow)
{
    if (pMainWindow->objectName().isEmpty())
        pMainWindow->setObjectName(QString::fromUtf8("MainWindow"));
    pMainWindow->resize(800, 600);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/appIcon"), QSize(), QIcon::Normal, QIcon::Off);
    pMainWindow->setWindowIcon(icon);
    actionExit = new QAction(pMainWindow);
    actionExit->setObjectName(QString::fromUtf8("actionExit"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/oxygen/actions/application-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionExit->setIcon(icon1);
    actionExit->setIconVisibleInMenu(true);
    actionEnglish = new QAction(pMainWindow);
    actionEnglish->setObjectName(QString::fromUtf8("actionEnglish"));
    actionEnglish->setCheckable(true);
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/flags/en.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionEnglish->setIcon(icon2);
    actionEnglish->setIconVisibleInMenu(true);
    actionFrench = new QAction(pMainWindow);
    actionFrench->setObjectName(QString::fromUtf8("actionFrench"));
    actionFrench->setCheckable(true);
    QIcon icon3;
    icon3.addFile(QString::fromUtf8(":/flags/fr.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionFrench->setIcon(icon3);
    actionFrench->setIconVisibleInMenu(true);
    actionHomePage = new QAction(pMainWindow);
    actionHomePage->setObjectName(QString::fromUtf8("actionHomePage"));
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/oxygen/categories/applications-internet.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionHomePage->setIcon(icon4);
    actionHomePage->setIconVisibleInMenu(true);
    actionAbout = new QAction(pMainWindow);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/oxygen/actions/help-about.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionAbout->setIcon(icon5);
    actionAbout->setIconVisibleInMenu(true);
    actionResetAll = new QAction(pMainWindow);
    actionResetAll->setObjectName(QString::fromUtf8("actionResetAll"));
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/oxygen/actions/system-reboot.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionResetAll->setIcon(icon6);
    actionResetAll->setIconVisibleInMenu(true);
    actionHelpToolbar = new QAction(pMainWindow);
    actionHelpToolbar->setObjectName(QString::fromUtf8("actionHelpToolbar"));
    actionHelpToolbar->setCheckable(true);
    actionSystem = new QAction(pMainWindow);
    actionSystem->setObjectName(QString::fromUtf8("actionSystem"));
    actionSystem->setCheckable(true);
    actionFullScreen = new QAction(pMainWindow);
    actionFullScreen->setObjectName(QString::fromUtf8("actionFullScreen"));
    actionFullScreen->setCheckable(true);
    QIcon icon7;
    icon7.addFile(QString::fromUtf8(":/oxygen/actions/view-fullscreen.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionFullScreen->setIcon(icon7);
    actionStatusBar = new QAction(pMainWindow);
    actionStatusBar->setObjectName(QString::fromUtf8("actionStatusBar"));
    actionStatusBar->setCheckable(true);
    actionCheckForUpdates = new QAction(pMainWindow);
    actionCheckForUpdates->setObjectName(QString::fromUtf8("actionCheckForUpdates"));
    QIcon icon8;
    icon8.addFile(QString::fromUtf8(":/oxygen/apps/system-software-update.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionCheckForUpdates->setIcon(icon8);
    actionCheckForUpdates->setIconVisibleInMenu(true);
    actionPreferences = new QAction(pMainWindow);
    actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
    QIcon icon9;
    icon9.addFile(QString::fromUtf8(":/oxygen/actions/configure.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionPreferences->setIcon(icon9);
    actionPlugins = new QAction(pMainWindow);
    actionPlugins->setObjectName(QString::fromUtf8("actionPlugins"));
    QIcon icon10;
    icon10.addFile(QString::fromUtf8(":/oxygen/apps/preferences-plugin.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionPlugins->setIcon(icon10);
    centralWidget = new QWidget(pMainWindow);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    pMainWindow->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(pMainWindow);
    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 800, 21));
    menuFile = new QMenu(menuBar);
    menuFile->setObjectName(QString::fromUtf8("menuFile"));
    menuTools = new QMenu(menuBar);
    menuTools->setObjectName(QString::fromUtf8("menuTools"));
    menuLanguage = new QMenu(menuTools);
    menuLanguage->setObjectName(QString::fromUtf8("menuLanguage"));
    QIcon icon11;
    icon11.addFile(QString::fromUtf8(":/oxygen/categories/applications-education-language.png"), QSize(), QIcon::Normal, QIcon::Off);
    menuLanguage->setIcon(icon11);
    menuHelp = new QMenu(menuBar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    menuView = new QMenu(menuBar);
    menuView->setObjectName(QString::fromUtf8("menuView"));
    menuToolbars = new QMenu(menuView);
    menuToolbars->setObjectName(QString::fromUtf8("menuToolbars"));
    QIcon icon12;
    icon12.addFile(QString::fromUtf8(":/oxygen/actions/configure-toolbars.png"), QSize(), QIcon::Normal, QIcon::Off);
    menuToolbars->setIcon(icon12);
    pMainWindow->setMenuBar(menuBar);
    helpToolbar = new QToolBar(pMainWindow);
    helpToolbar->setObjectName(QString::fromUtf8("helpToolbar"));
    helpToolbar->setIconSize(QSize(24, 24));
    pMainWindow->addToolBar(Qt::TopToolBarArea, helpToolbar);
    statusBar = new QStatusBar(pMainWindow);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    pMainWindow->setStatusBar(statusBar);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuView->menuAction());
    menuBar->addAction(menuTools->menuAction());
    menuBar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionExit);
    menuTools->addAction(menuLanguage->menuAction());
    menuTools->addSeparator();
    menuTools->addAction(actionPlugins);
    menuTools->addSeparator();
    menuTools->addAction(actionPreferences);
    menuTools->addSeparator();
    menuTools->addAction(actionResetAll);
    menuLanguage->addAction(actionSystem);
    menuLanguage->addSeparator();
    menuLanguage->addAction(actionEnglish);
    menuLanguage->addAction(actionFrench);
    menuHelp->addAction(actionHomePage);
    menuHelp->addSeparator();
    menuHelp->addAction(actionCheckForUpdates);
    menuHelp->addSeparator();
    menuHelp->addAction(actionAbout);
    menuView->addAction(menuToolbars->menuAction());
    menuView->addAction(actionStatusBar);
    menuView->addSeparator();
    menuView->addAction(actionFullScreen);
    menuToolbars->addAction(actionHelpToolbar);
    helpToolbar->addAction(actionHomePage);
    helpToolbar->addSeparator();
    helpToolbar->addAction(actionAbout);

    retranslateUi(pMainWindow);

    QMetaObject::connectSlotsByName(pMainWindow);
}

void MainWindowUi::retranslateUi(MainWindow *pMainWindow)
{
    actionExit->setText(tr("&Exit"));
#ifndef QT_NO_STATUSTIP
    actionExit->setStatusTip(tr("Exit OpenCOR"));
#endif // QT_NO_STATUSTIP
    actionExit->setShortcut(tr("Ctrl+Q"));
    actionEnglish->setText(tr("&English"));
#ifndef QT_NO_STATUSTIP
    actionEnglish->setStatusTip(tr("Select English as the language used by OpenCOR"));
#endif // QT_NO_STATUSTIP
    actionFrench->setText(tr("&French"));
#ifndef QT_NO_STATUSTIP
    actionFrench->setStatusTip(tr("Select French as the language used by OpenCOR"));
#endif // QT_NO_STATUSTIP
    actionHomePage->setText(tr("Home &Page"));
#ifndef QT_NO_STATUSTIP
    actionHomePage->setStatusTip(tr("Look up the OpenCOR home page"));
#endif // QT_NO_STATUSTIP
    actionAbout->setText(tr("&About..."));
#ifndef QT_NO_STATUSTIP
    actionAbout->setStatusTip(tr("Some general information about OpenCOR"));
#endif // QT_NO_STATUSTIP
    actionResetAll->setText(tr("&Reset All"));
#ifndef QT_NO_STATUSTIP
    actionResetAll->setStatusTip(tr("Reset all the user settings"));
#endif // QT_NO_STATUSTIP
    actionHelpToolbar->setText(tr("&Help"));
#ifndef QT_NO_STATUSTIP
    actionHelpToolbar->setStatusTip(tr("Show/hide the Help toolbar"));
#endif // QT_NO_STATUSTIP
    actionSystem->setText(tr("&System"));
#ifndef QT_NO_STATUSTIP
    actionSystem->setStatusTip(tr("Select the system's language as the language used by OpenCOR"));
#endif // QT_NO_STATUSTIP
    actionFullScreen->setText(tr("&Full Screen"));
#ifndef QT_NO_STATUSTIP
    actionFullScreen->setStatusTip(tr("Switch to / back from full screen mode"));
#endif // QT_NO_STATUSTIP
    actionFullScreen->setShortcut(tr("F11"));
    actionStatusBar->setText(tr("Status &Bar"));
#ifndef QT_NO_STATUSTIP
    actionStatusBar->setStatusTip(tr("Show/hide the status bar"));
#endif // QT_NO_STATUSTIP
    actionCheckForUpdates->setText(tr("Check for &Updates..."));
#ifndef QT_NO_STATUSTIP
    actionCheckForUpdates->setStatusTip(tr("Check for updates"));
#endif // QT_NO_STATUSTIP
    actionPreferences->setText(tr("&Preferences..."));
#ifndef QT_NO_STATUSTIP
    actionPreferences->setStatusTip(tr("Preferences for OpenCOR"));
#endif // QT_NO_STATUSTIP
    actionPlugins->setText(tr("Plu&gins..."));
    menuFile->setTitle(tr("&File"));
    menuTools->setTitle(tr("&Tools"));
    menuLanguage->setTitle(tr("&Language"));
    menuHelp->setTitle(tr("&Help"));
    menuView->setTitle(tr("&View"));
    menuToolbars->setTitle(tr("&Toolbars"));
    helpToolbar->setWindowTitle(tr("Help Toolbar"));
    Q_UNUSED(pMainWindow);
} // retranslateUi

MainWindow::MainWindow(QWidget *pParent) :
    QMainWindow(pParent),
    mUi(new MainWindowUi)
{
    // Create our settings object

    mSettings = new QSettings(qApp->applicationName());

    // Create our plugin manager (which will automatically load our various
    // plugins)

    mPluginManager = new PluginManager(mSettings, PluginInfo::Gui);

    // Specify some general docking settings

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    // Set up the UI

    mUi->setupUi(this);
    // Note: the application icon (which needs to be set for Linux, but neither
    //       for Windows nor Mac OS X, since it's set through CMake in those
    //       cases (see CMakeLists.txt)) is set within the UI file. This being
    //       said, it's good to have it set for all three platforms, since it
    //       can then be used in, for example, the about box, so...

//---GRY--- THE BELOW CODE WOULD BE GOOD TO HAVE, BUT THERE IS A KNOWN BUG (SEE
//          http://bugreports.qt.nokia.com/browse/QTBUG-3116) WHICH MAKES THAT
//          CODE TO CAUSE PROBLEMS WITH THE LOADING/RESTORING OF THE APPLICATION
//          GEOMETRY, SO...

//#ifdef Q_WS_MAC
//    // Make the application look more like a Mac OS X application

//    setUnifiedTitleAndToolBarOnMac(true);
//#endif

    // Some connections to handle our Help toolbar

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

    // Initialise our various plugins

    foreach(Plugin *plugin, mPluginManager->loadedPlugins()) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

        if (guiInterface) {
            // The plugin implements our GUI interface, so...

            guiInterface->initialize(this);
        } else {
            // The plugin doesn't implement our GUI interface, so let's see
            // whether it implements our default interface

            PluginInterface *pluginInterface = qobject_cast<PluginInterface *>(plugin->instance());

            if (pluginInterface)
                // The plugin implements our default interface, so...

                pluginInterface->initialize();
        }
    }

#ifdef Q_WS_MAC
    // A special shortcut to have OpenCOR minimised on Mac OS X when pressing
    // Cmd+M
    // Note: indeed, when pressing Cmd+M on Mac OS X, the active application
    //       is expected to minimise itself, so...

    new QShortcut(QKeySequence("Ctrl+M"),
                  this, SLOT(showMinimized()));
#endif

    // Retrieve the user settings from the previous session, if any

    loadSettings();

    // Bring ourselves to the foreground
    // Note: indeed, when starting/restarting OpenCOR (as a result of a Reset
    //       All in the case of a restart), OpenCOR will on Mac OS X be behind
    //       any other application. Now, because it doesn't harm to bring
    //       ourselves to the foreground when on Windows or Linux, we may as
    //       well do it on those platforms too...

    raise();
}

MainWindow::~MainWindow()
{
    // Finalize our various plugins

    foreach(Plugin *plugin, mPluginManager->loadedPlugins()) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

        if (guiInterface) {
            // The plugin implements our GUI interface, so...

            guiInterface->finalize();
        } else {
            // The plugin doesn't implement our GUI interface, so let's see
            // whether it implements our default interface

            PluginInterface *pluginInterface = qobject_cast<PluginInterface *>(plugin->instance());

            if (pluginInterface)
                // The plugin implements our default interface, so...

                pluginInterface->finalize();
        }
    }

    // Delete some internal objects

    delete mPluginManager;
    delete mSettings;
    delete mUi;
}

void MainWindow::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    QMainWindow::changeEvent(pEvent);

    // If the system's locale has changed, then update OpenCOR's locale in case
    // the user wants to use the system's locale

    if (   (pEvent->type() == QEvent::LocaleChange)
        && (mUi->actionSystem->isChecked()))
        setLocale(SystemLocale);
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
}

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    // Keep track of our default settings
    // Note: it must be done here, as opposed to the destructor, otherwise some
    //       settings (e.g. dock windows) won't be properly saved

    saveSettings();

    // Default handling of the event

    QMainWindow::closeEvent(pEvent);
}

static const QString SettingsLocale              = "Locale";
static const QString SettingsGeometry            = "Geometry";
static const QString SettingsState               = "State";
static const QString SettingsStatusBarVisibility = "StatusBarVisibility";

void MainWindow::loadSettings()
{
    mSettings->beginGroup(objectName());
        // Retrieve the language to be used by OpenCOR

        setLocale(mSettings->value(SettingsLocale, SystemLocale).toString());

        // Retrieve the geometry and state of the main window

        bool needDefaultSettings =    !restoreGeometry(mSettings->value(SettingsGeometry).toByteArray())
                                   || !restoreState(mSettings->value(SettingsState).toByteArray());

        if (needDefaultSettings) {
            // The geometry and/or state of the main window couldn't be
            // retrieved, so go with some default settins

            // Default size and position of the main window

            double ratio = 1.0/13.0;
            QRect desktopGeometry = qApp->desktop()->availableGeometry();
            int horizSpace = ratio*desktopGeometry.width();
            int vertSpace  = ratio*desktopGeometry.height();

            setGeometry(desktopGeometry.left()+horizSpace,
                        desktopGeometry.top()+vertSpace,
                        desktopGeometry.width()-2*horizSpace,
                        desktopGeometry.height()-2*vertSpace);
        } else {
            // The geometry and state of the main window could be retrieved, so
            // carry on with the loading of the settings

            // Retrieve whether the status bar is to be shown

            mUi->statusBar->setVisible(mSettings->value(SettingsStatusBarVisibility,
                                                        true).toBool());
        }

        // Retrieve the settings of our various plugins

//---GRY--- TO BE DONE (FOR OUR VARIOUS PLUGINS)...
    mSettings->endGroup();
}

void MainWindow::saveSettings()
{
    mSettings->beginGroup(objectName());
        // Keep track of the language to be used by OpenCOR

        mSettings->setValue(SettingsLocale, mLocale);

        // Keep track of the geometry of the main window

        mSettings->setValue(SettingsGeometry, saveGeometry());

        // Keep track of the state of the main window

        mSettings->setValue(SettingsState, saveState());

        // Keep track of whether the status bar is to be shown

        mSettings->setValue(SettingsStatusBarVisibility,
                            mUi->statusBar->isVisible());

        // Keep track of the settings of our various plugins

//---GRY--- TO BE DONE (FOR OUR VARIOUS PLUGINS)...
    mSettings->endGroup();
}

QString MainWindow::locale()
{
    // Return the current locale

    const QString systemLocale = QLocale::system().name().left(2);

    return (mLocale == SystemLocale)?systemLocale:mLocale;
}

void MainWindow::setLocale(const QString &pLocale)
{
    if ((pLocale != mLocale) || (pLocale == SystemLocale)) {
        // The new locale is different from the existing one, so we need to
        // translate everything

        const QString systemLocale = QLocale::system().name().left(2);

        QString realLocale = (pLocale == SystemLocale)?systemLocale:pLocale;

        mLocale = pLocale;

        // Specify the language to be used by OpenCOR and our various plugins

        qApp->removeTranslator(&mQtTranslator);
        mQtTranslator.load(":qt_"+realLocale);
        qApp->installTranslator(&mQtTranslator);

        qApp->removeTranslator(&mAppTranslator);
        mAppTranslator.load(":app_"+realLocale);
        qApp->installTranslator(&mAppTranslator);

//---GRY--- TO BE DONE (FOR OUR VARIOUS PLUGINS)...

        // Translate the whole GUI (including our various plugins) and update
        // the actions just to be on the safe side

        mUi->retranslateUi(this);

//---GRY--- TO BE DONE (FOR OUR VARIOUS PLUGINS)...
    }

    // Update the checked menu item
    // Note: it has to be done every single time, since selecting a menu item
    //       will automatically toggle its checked status, so...

    mUi->actionSystem->setChecked(mLocale == SystemLocale);

    mUi->actionEnglish->setChecked(mLocale == EnglishLocale);
    mUi->actionFrench->setChecked(mLocale == FrenchLocale);
}

void MainWindow::singleAppMsgRcvd(const QString &)
{
    // We have just received a message from another instance of OpenCOR, so
    // bring ourselves to the foreground
    // Note: one would normally use activateWindow(), but depending on the
    //       operating system it may or not bring OpenCOR to the foreground,
    //       so... instead we do what follows, depending on the operating
    //       system...

#ifdef Q_WS_WIN
    // Retrieve OpenCOR's window Id

    WId mainWinId = winId();

    // Bring OpenCOR to the foreground

    DWORD foregroundThreadProcId = GetWindowThreadProcessId(GetForegroundWindow(),
                                                            NULL);
    DWORD mainThreadProcId = GetWindowThreadProcessId(mainWinId, NULL);

    if (foregroundThreadProcId != mainThreadProcId) {
        // OpenCOR's thread process Id is not that of the foreground window, so
        // attach the foreground thread to OpenCOR's, set OpenCOR to the
        // foreground, and detach the foreground thread from OpenCOR's

        AttachThreadInput(foregroundThreadProcId, mainThreadProcId, true);

        SetForegroundWindow(mainWinId);
        SetFocus(mainWinId);

        AttachThreadInput(foregroundThreadProcId, mainThreadProcId, false);
    } else {
        // OpenCOR's thread process Id is that of the foreground window, so
        // just set OpenCOR to the foreground

        SetForegroundWindow(mainWinId);
    }

    // Show/restore OpenCOR, depending on its current state

    if (IsIconic(mainWinId))
        ShowWindow(mainWinId, SW_RESTORE);
    else
        ShowWindow(mainWinId, SW_SHOW);

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

void MainWindow::on_actionFullScreen_triggered()
{
    // Switch to / back from full screen mode

    if (mUi->actionFullScreen->isChecked())
        showFullScreen();
    else
        showNormal();
}

void MainWindow::on_actionSystem_triggered()
{
    // Select the system's language as the language used by OpenCOR

    setLocale(SystemLocale);
}

void MainWindow::on_actionEnglish_triggered()
{
    // Select English as the language used by OpenCOR

    setLocale(EnglishLocale);
}

void MainWindow::on_actionFrench_triggered()
{
    // Select French as the language used by OpenCOR

    setLocale(FrenchLocale);
}

void MainWindow::on_actionPlugins_triggered()
{
    // Plugins' preferences

    OpenCOR::PluginsWindow pluginsWindow(mPluginManager, this);

    pluginsWindow.exec();
}

void MainWindow::on_actionPreferences_triggered()
{
    // User's preferences

    OpenCOR::PreferencesWindow preferencesWindow(this);

    preferencesWindow.exec();
}

void MainWindow::on_actionHomePage_triggered()
{
    // Look up OpenCOR home page

    QDesktopServices::openUrl(QUrl(OpencorHomepageUrl));
}

void MainWindow::on_actionCheckForUpdates_triggered()
{
    // Check for updates

    OpenCOR::CheckForUpdatesWindow checkForUpdatesWindow(this);

    checkForUpdatesWindow.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    // Display some information about OpenCOR

    QMessageBox::about(this, tr("About"),
                        "<h1 align=center><b>"+getAppVersion(qApp)+"</b></h1>"
                       +"<h3 align=center><em>"+getOsName()+"</em></h3>"
                       +"<p align=center><em>"+getAppCopyright(true)+"</em></p>"
                       +"<a href=\""+QString(OpencorHomepageUrl)+"\">"+qApp->applicationName()+"</a> "+tr("is a cross-platform <a href=\"http://www.cellml.org/\">CellML</a>-based modelling environment which can be used to organise, edit, simulate and analyse CellML files."));
}

void MainWindow::resetAll()
{
    if( QMessageBox::question(this, qApp->applicationName(),
                              tr("You are about to reset <strong>all</strong> of your user settings. Are you sure that this is what you want?"),
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::Yes) == QMessageBox::Yes ) {
        // We want to reset everything, so clear all the user settings and
        // restart OpenCOR (indeed, a restart will ensure that the various dock
        // windows are, for instance, properly reset with regards to their
        // dimensions)

        QSettings(qApp->applicationName()).clear();

        // Ask for OpenCOR to be restarted
        // Note: the closeEvent method won't get called and this is exactly what
        //       we want, since we don't want to save OpenCOR's settings

        qApp->exit(MainWindow::NeedRestart);
    }
}

}
