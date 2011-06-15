#include "cellml.h"
#include "centralwidget.h"
#include "checkforupdateswindow.h"
#include "common.h"
#include "mainwindow.h"
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
#include <QUrl>

namespace OpenCOR {

static const QString SystemLocale  = "";
static const QString EnglishLocale = "en";
static const QString FrenchLocale  = "fr";

MainWindow::MainWindow(QWidget *pParent) :
    QMainWindow(pParent),
    mUi(new Ui::MainWindow)
{
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

    // Set the New toolbar button with its dropdown menu using our custom-made
    // action (actionNew)
    // Note: ideally, this would be done using the GUI designer in QtCreator,
    //       but it's not quite clear to me how this can be achieved, so...

    QMenu *actionNewMenu = new QMenu(this);

    mUi->actionNew->setMenu(actionNewMenu);

    actionNewMenu->addAction(mUi->actionCellML10File);
    actionNewMenu->addAction(mUi->actionCellML11File);

    mUi->fileToolbar->insertAction(mUi->actionSave, mUi->actionNew);
    mUi->fileToolbar->insertSeparator(mUi->actionSave);

    // Set the Reopen menu

    mActionReopenMenu = new QMenu(this);

    mUi->actionReopen->setMenu(mActionReopenMenu);

    mUi->menuFile->insertAction(mUi->actionSave, mUi->actionReopen);
    mUi->menuFile->insertSeparator(mUi->actionSave);

    // Set the Open/Reopen toolbar button with its dropdown menu

    mActionOpenReopenMenu = new QMenu(this);

    mUi->actionOpenReopen->setMenu(mActionOpenReopenMenu);

    mUi->fileToolbar->insertAction(mUi->actionSave, mUi->actionOpenReopen);
    mUi->fileToolbar->insertSeparator(mUi->actionSave);

    // Set the central widget

    mCentralWidget = new CentralWidget(this);

    setCentralWidget(mCentralWidget);

    // Specify some general docking settings

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    // Load our various plugins

//---GRY--- TO BE DONE...

    // Some connections to handle the various toolbars

    connect(mUi->actionFileToolbar, SIGNAL(triggered(bool)),
            mUi->fileToolbar, SLOT(setVisible(bool)));
    connect(mUi->fileToolbar->toggleViewAction(), SIGNAL(toggled(bool)),
            mUi->actionFileToolbar, SLOT(setChecked(bool)));

    connect(mUi->actionEditToolbar, SIGNAL(triggered(bool)),
            mUi->editToolbar, SLOT(setVisible(bool)));
    connect(mUi->editToolbar->toggleViewAction(), SIGNAL(toggled(bool)),
            mUi->actionEditToolbar, SLOT(setChecked(bool)));

    connect(mUi->actionRunToolbar, SIGNAL(triggered(bool)),
            mUi->runToolbar, SLOT(setVisible(bool)));
    connect(mUi->runToolbar->toggleViewAction(), SIGNAL(toggled(bool)),
            mUi->actionRunToolbar, SLOT(setChecked(bool)));

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

    // Some connections to handle the File menu

    connect(mUi->actionClose, SIGNAL(triggered(bool)),
            mCentralWidget, SLOT(closeFile()));
    connect(mUi->actionCloseAll, SIGNAL(triggered(bool)),
            mCentralWidget, SLOT(closeFiles()));

    // Some connections to handle the central widget

    connect(mCentralWidget, SIGNAL(fileOpened(const QString &)),
            this, SLOT(fileOpened(const QString &)));
    connect(mCentralWidget, SIGNAL(fileClosed(const QString &)),
            this, SLOT(fileClosed(const QString &)));
    connect(mCentralWidget, SIGNAL(fileActivated(const QString &)),
            this, SLOT(updateWindowTitle()));

    // Some connections for the GUI side of our various plugins

//---GRY--- TO BE DONE...

#ifdef Q_WS_MAC
    // A special shortcut to have OpenCOR minimised on Mac OS X when pressing
    // Cmd+M
    // Note: indeed, when pressing Cmd+M on Mac OS X, the active application
    //       is expected to minimise itself, so...

    new QShortcut(QKeySequence("Ctrl+M"),
                  this, SLOT(showMinimized()));
#endif

    // Default title for the main window
    // Note: loadSettings may result in a new title (if a file is opened)

    updateWindowTitle();

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
    // Delete the UI

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

void MainWindow::updateWindowTitle()
{
    // Set the title of the main window to that of the application and add the
    // name of the active file, should there be one

    QString activeFileName = mCentralWidget->activeFileName();

    if (activeFileName.isEmpty())
        setWindowTitle(qApp->applicationName());
    else
        setWindowTitle(qApp->applicationName()+" - "+activeFileName);
}

static const QString SettingsLocale              = "Locale";
static const QString SettingsGeometry            = "Geometry";
static const QString SettingsState               = "State";
static const QString SettingsStatusBarVisibility = "StatusBarVisibility";
static const QString SettingsFileDialogDirectory = "FileDialogDirectory";
static const QString SettingsRecentlyOpenedFiles = "RecentlyOpenedFiles";
static const QString SettingsDebugModeEnabled    = "DebugModeEnabled";

void MainWindow::loadSettings()
{
    QSettings settings(qApp->applicationName());

    settings.beginGroup(objectName());
        // Retrieve the language to be used by OpenCOR

        setLocale(settings.value(SettingsLocale, SystemLocale).toString());

        // Retrieve the geometry and state of the main window

        bool needDefaultSettings =    !restoreGeometry(settings.value(SettingsGeometry).toByteArray())
                                   || !restoreState(settings.value(SettingsState).toByteArray());

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

            mUi->statusBar->setVisible(settings.value(SettingsStatusBarVisibility,
                                                      true).toBool());
        }

        // Retrieve the active directory

        mActiveDir.setPath(settings.value(SettingsFileDialogDirectory,
                                          QDir::currentPath()).toString());

        // Retrieve our recently opened files (and add them to our menus)

        QStringList recentlyOpenedFiles = settings.value(SettingsRecentlyOpenedFiles).toStringList();

        for (int i = recentlyOpenedFiles.count()-1; i >= 0; --i)
            addRecentlyOpenedFile(recentlyOpenedFiles.at(i));

        // Retrieve whether we are working in debug mode or not

        mUi->actionDebugMode->setChecked(settings.value(SettingsDebugModeEnabled,
                                                        false).toBool());

        // Retrieve the settings of the the central widget and of our various
        // plugins

        mCentralWidget->loadSettings(settings);

//---GRY--- TO BE DONE (FOR OUR VARIOUS PLUGINS)...
    settings.endGroup();
}

void MainWindow::saveSettings()
{
    QSettings settings(qApp->applicationName());

    settings.beginGroup(objectName());
        // Keep track of the language to be used by OpenCOR

        settings.setValue(SettingsLocale, mLocale);

        // Keep track of the geometry of the main window

        settings.setValue(SettingsGeometry, saveGeometry());

        // Keep track of the state of the main window

        settings.setValue(SettingsState, saveState());

        // Keep track of whether the status bar is to be shown

        settings.setValue(SettingsStatusBarVisibility,
                          mUi->statusBar->isVisible());

        // Keep track of the active directory

        settings.setValue(SettingsFileDialogDirectory, mActiveDir.path());

        // Keep track of our recently opened files

        QStringList recentlyOpenedFiles;

        for (int i = 0; i < mActionReopenMenu->actions().count(); ++i)
            recentlyOpenedFiles << mActionReopenMenu->actions().at(i)->text();

        settings.setValue(SettingsRecentlyOpenedFiles, recentlyOpenedFiles);

        // Keep track of whether we are working in debug mode or not

        settings.setValue(SettingsDebugModeEnabled,
                          mUi->actionDebugMode->isChecked());

        // Keep track of the settings of the central widget and of our various
        // plugins

        mCentralWidget->saveSettings(settings);

//---GRY--- TO BE DONE (FOR OUR VARIOUS PLUGINS)...
    settings.endGroup();
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

        updateActions();

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

void MainWindow::updateActions()
{
    // Make sure that our various actions are properly enabled/disabled

    // File actions

    mUi->actionOpen->setEnabled(true);
    mUi->actionReopen->setEnabled(mActionReopenMenu->actions().count());
    mUi->actionOpenReopen->setEnabled(true);

    mUi->actionSave->setEnabled(false);
    mUi->actionSaveAs->setEnabled(false);
    mUi->actionSaveAll->setEnabled(false);

    mUi->actionClose->setEnabled(mCentralWidget->nbOfFilesOpened());
    mUi->actionCloseAll->setEnabled(mCentralWidget->nbOfFilesOpened());

    mUi->actionPrint->setEnabled(false);

    // Edit actions

    mUi->actionUndo->setEnabled(false);
    mUi->actionRedo->setEnabled(false);

    mUi->actionCut->setEnabled(false);
    mUi->actionCopy->setEnabled(false);
    mUi->actionPaste->setEnabled(false);
    mUi->actionDelete->setEnabled(false);

    mUi->actionFind->setEnabled(false);
    mUi->actionFindNext->setEnabled(false);
    mUi->actionFindPrevious->setEnabled(false);
    mUi->actionReplace->setEnabled(false);

    mUi->actionSelectAll->setEnabled(false);

    // Run actions

    mUi->actionRun->setEnabled(false);
    mUi->actionStop->setEnabled(false);
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
    // Ask for the file(s) to be opened

    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      tr("Open File"),
                                                      mActiveDir.path(),
                                                      tr("All Files")
                                                      +" (*"
#ifdef Q_WS_WIN
                                                      +".*"
#endif
                                                      +");;"+tr("CellML File")+" (*.cellml)");

    if (files.count())
        // There is at least one file which is to be opened, so we can keep
        // track of the folder in which it is
        // Note #1: we use the last file to determine the folder that is to be
        //          remembered since on Windows 7, at least, it's possible to
        //          search for files from within the file dialog box, the last
        //          file should be the one we are 'interested' in...
        // Note #2: this doesn't, unfortunately, address the case where the user
        //          goes to a directory and then closes the file dialog box
        //          without selecting any file. There might be a way to get it
        //          to work, but that would involve using the exec method rather
        //          than the static getOpenFilesNames method, which would result
        //          in a non-native looking file dialog box (on Windows 7 at
        //          least), so it's not an option unfortunately...

        mActiveDir = QFileInfo(files.at(files.count()-1)).path();

    // Open the file(s)

    mCentralWidget->openFiles(files);
}

void MainWindow::on_actionOpenReopen_triggered()
{
    // Open one or several files

    on_actionOpen_triggered();
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

void MainWindow::on_actionPrint_triggered()
{
    notYetImplemented("void MainWindow::on_actionPrint_triggered()");
}

void MainWindow::on_actionUndo_triggered()
{
    notYetImplemented("void MainWindow::on_actionUndo_triggered()");
}

void MainWindow::on_actionRedo_triggered()
{
    notYetImplemented("void MainWindow::on_actionRedo_triggered()");
}

void MainWindow::on_actionCut_triggered()
{
    notYetImplemented("void MainWindow::on_actionCut_triggered()");
}

void MainWindow::on_actionCopy_triggered()
{
    notYetImplemented("void MainWindow::on_actionCopy_triggered()");
}

void MainWindow::on_actionPaste_triggered()
{
    notYetImplemented("void MainWindow::on_actionPaste_triggered()");
}

void MainWindow::on_actionDelete_triggered()
{
    notYetImplemented("void MainWindow::on_actionDelete_triggered()");
}

void MainWindow::on_actionFind_triggered()
{
    notYetImplemented("void MainWindow::on_actionFind_triggered()");
}

void MainWindow::on_actionFindNext_triggered()
{
    notYetImplemented("void MainWindow::on_actionFindNext_triggered()");
}

void MainWindow::on_actionFindPrevious_triggered()
{
    notYetImplemented("void MainWindow::on_actionFindPrevious_triggered()");
}

void MainWindow::on_actionReplace_triggered()
{
    notYetImplemented("void MainWindow::on_actionReplace_triggered()");
}

void MainWindow::on_actionSelectAll_triggered()
{
    notYetImplemented("void MainWindow::on_actionSelectAll_triggered()");
}

void MainWindow::on_actionFullScreen_triggered()
{
    // Switch to / back from full screen mode

    if (mUi->actionFullScreen->isChecked())
        showFullScreen();
    else
        showNormal();
}

void MainWindow::on_actionRun_triggered()
{
    notYetImplemented("void MainWindow::on_actionRun_triggered()");
}

void MainWindow::on_actionStop_triggered()
{
    notYetImplemented("void MainWindow::on_actionStop_triggered()");
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

    OpenCOR::PluginsWindow pluginsWindow(this);

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

void MainWindow::on_actionCellMLHomePage_triggered()
{
    // Look up CellML home page

    QDesktopServices::openUrl(QUrl(CellmlHomepageUrl));
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

    QMessageBox::about(this, qApp->applicationName(),
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

void MainWindow::fileOpened(const QString &pFileName)
{
    // Remove the file from our list of recently opened files, should it be in
    // it

    removeRecentlyOpenedFile(pFileName);

    // Update the main window's title

    updateWindowTitle();

    // Update the actions, just to be on the safe side

    updateActions();
}

void MainWindow::fileClosed(const QString &pFileName)
{
    // Add the file to our list of recently opened files, should it be in it

    addRecentlyOpenedFile(pFileName);

    // Update the main window's title

    updateWindowTitle();

    // Update the actions, just to be on the safe side

    updateActions();
}

int MainWindow::isRecentlyOpenedFile(const QString &pFileName)
{
    // Check whether the given file name is part of our list of recently opened
    // files

    for (int i = 0; i < mActionReopenMenu->actions().count(); ++i)
        if (!mActionReopenMenu->actions().at(i)->text().compare(pFileName))
            // The file name was found, so return its index in our list

            return i;

    // The file name couldn't be found, so...

    return -1;
}

void MainWindow::doUpdateRecentlyOpenedFiles(const QList<QAction *> pActions)
{
    // Sepcify within the actions which ones should be visible

    static const int TotalNbOfRecentlyOpenedFiles = 10;

    for (int i = 0; i < pActions.count(); ++i)
        pActions.at(i)->setVisible(i < TotalNbOfRecentlyOpenedFiles);
}

void MainWindow::updateRecentlyOpenedFiles()
{
    // Update the list of actions for our reopen menus

    doUpdateRecentlyOpenedFiles(mActionReopenMenu->actions());
    doUpdateRecentlyOpenedFiles(mActionOpenReopenMenu->actions());
}

void MainWindow::addRecentlyOpenedFile(const QString &pFileName)
{
    // Check whether the file is not already in our list of recently opened
    // files

    if (isRecentlyOpenedFile(pFileName) == -1) {
        // The file couldn't be found in our list, so create an entry for the
        // file in our menus

        QAction *action = new QAction(pFileName, this);

        connect(action, SIGNAL(triggered()),
                this, SLOT(reopenFile()));

        mActionReopenMenu->insertAction(mActionOpenReopenMenu->actions().count()?
                                            mActionOpenReopenMenu->actions().first():
                                            0,
                                        action);
        mActionOpenReopenMenu->insertAction(mActionOpenReopenMenu->actions().count()?
                                                mActionOpenReopenMenu->actions().first():
                                                0,
                                            action);

        // Update what should be shown in our list of recently opened files

        updateRecentlyOpenedFiles();
    }
}

void MainWindow::removeRecentlyOpenedFile(const QString &pFileName)
{
    // Search for the file in our list of recently opened files

    int index = isRecentlyOpenedFile(pFileName);

    if (index != -1) {
        // The file could be found in our list, so remove it from our menus

        QAction *action = mActionReopenMenu->actions().at(index);

        mActionReopenMenu->removeAction(action);
        mActionOpenReopenMenu->removeAction(action);

        delete action;

        // Update what should be shown in our list of recently opened files

        updateRecentlyOpenedFiles();
    }
}

void MainWindow::reopenFile()
{
    // Reopen one of our recently opened files

    QAction *action = qobject_cast<QAction *>(sender());

    if (action)
        mCentralWidget->openFile(action->text());
}

}
