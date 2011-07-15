#include "checkforupdateswindow.h"
#include "common.h"
#include "guiinterface.h"
#include "mainwindow.h"
#include "plugin.h"
#include "pluginmanager.h"
#include "pluginswindow.h"
#include "preferenceswindow.h"
#include "utils.h"

#include "ui_mainwindow.h"

#ifdef Q_WS_WIN
    #include <windows.h>
#endif

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

MainWindow::MainWindow(QWidget *pParent) :
    QMainWindow(pParent),
    mUi(new Ui::MainWindow)
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

    foreach (Plugin *plugin, mPluginManager->loadedPlugins()) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

        if (guiInterface) {
            // The plugin implements our GUI interface, so...

            guiInterface->initialize(this);

            // Handle the GUI settings of the plugin

            handlePluginGuiSettings(guiInterface->settings());
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

    foreach (Plugin *plugin, mPluginManager->loadedPlugins()) {
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

void MainWindow::handlePluginGuiSettings(const GuiSettings &pGuiSettings) const
{
    // Handle the plugin's actions

    foreach (const GuiSettingsAction &guiSettingsAction, pGuiSettings.actions())
        // Handle the action depending on its type

        switch (guiSettingsAction.type()) {
        case GuiSettingsAction::Help:
            // We are dealing with a help type action, so we need to add it to
            // our help menu

            mUi->menuHelp->insertAction(mUi->actionHomePage,
                                        guiSettingsAction.action());
            mUi->menuHelp->insertSeparator(mUi->actionHomePage);

            // As well as to our help tool bar

            mUi->helpToolbar->insertAction(mUi->actionHomePage,
                                           guiSettingsAction.action());
            mUi->helpToolbar->insertSeparator(mUi->actionHomePage);

            break;
        }
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

void MainWindow::saveSettings() const
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

QString MainWindow::locale() const
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

        // Specify the language to be used by OpenCOR

        qApp->removeTranslator(&mQtTranslator);
        mQtTranslator.load(":qt_"+realLocale);
        qApp->installTranslator(&mQtTranslator);

        qApp->removeTranslator(&mAppTranslator);
        mAppTranslator.load(":app_"+realLocale);
        qApp->installTranslator(&mAppTranslator);

        // Retranslate OpenCOR

        mUi->retranslateUi(this);

        // Update the locale of our various plugins

        foreach (Plugin *plugin, mPluginManager->loadedPlugins()) {
            GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

            if (guiInterface)
                // The plugin implements our GUI interface, so...

                guiInterface->setLocale(realLocale);
        }
    }

    // Update the checked menu item
    // Note: it has to be done every single time, since selecting a menu item
    //       will automatically toggle its checked status, so...

    mUi->actionSystem->setChecked(mLocale == SystemLocale);

    mUi->actionEnglish->setChecked(mLocale == EnglishLocale);
    mUi->actionFrench->setChecked(mLocale == FrenchLocale);
}

#ifdef Q_WS_WIN
void MainWindow::singleAppMsgRcvd(const QString &) const
#else
void MainWindow::singleAppMsgRcvd(const QString &)
#endif
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
    // Do what one would normally do

    activateWindow();

    raise();   // Just to be on the safe side
    // Note: raise() never seems to be required on Mac OS X, but to use
    //       activateWindow() on its own under Linux may or not give the
    //       expected result, so...
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
