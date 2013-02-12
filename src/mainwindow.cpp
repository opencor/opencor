//==============================================================================
// Main window
//==============================================================================

#include "common.h"
#include "fileinterface.h"
#include "i18ninterface.h"
#include "mainwindow.h"
#include "plugin.h"
#include "pluginmanager.h"
#include "pluginswindow.h"
#include "solverinterface.h"
#include "utils.h"

//==============================================================================

#include "ui_mainwindow.h"

//==============================================================================

#ifdef Q_OS_WIN
    #include <windows.h>
#endif

//==============================================================================

#include <QCloseEvent>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#ifdef Q_OS_MAC
    #include <QShortcut>
#endif
#include <QUrl>

//==============================================================================

namespace OpenCOR {

//==============================================================================

static const QString CorePlugin = "Core";
static const QString HelpPlugin = "Help";

//==============================================================================

MainWindow::MainWindow() :
    QMainWindow(),
    mGui(new Ui::MainWindow),
    mLocale(QString()),
    mFileNewMenu(0),
    mViewOrganisationMenu(0),
    mViewSeparator(0)
{
    // Create our settings object

    mSettings = new QSettings(SettingsApplicationName);

    // Create our plugin manager (which will automatically load our various
    // plugins)

    mPluginManager = new PluginManager(mSettings, PluginInfo::Gui);

    // Specify some general docking settings

    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    // Set up the GUI
    // Note: the application icon (which needs to be set for Linux, but not for
    //       Windows or OS X, since it's set through CMake in those cases (see
    //       CMakeLists.txt)) is set within the GUI file. This being said, it's
    //       good to have it set for all three platforms, since it can then be
    //       used in, for example, the about box...

    mGui->setupUi(this);

    // Title of our main window
    // Note: we don't set it in our .ui file since this will require
    //       'translating' it in other languages...

    setWindowTitle(qApp->applicationName());

    // A connection to handle the status bar

    connect(mGui->actionStatusBar, SIGNAL(triggered(bool)),
            statusBar(), SLOT(setVisible(bool)));

    // Some connections to handle our various menu items

    connect(mGui->actionExit, SIGNAL(triggered(bool)),
            this, SLOT(close()));
    connect(mGui->actionResetAll, SIGNAL(triggered(bool)),
            this, SLOT(resetAll()));

    // Set the shortcuts of some actions
    // Note: we do it here, so that we can use standard shortcuts (whenever
    //       possible)...

#if defined(Q_OS_WIN)
    // Note: QKeySequence::Quit corresponds to Alt+F4 on Windows, but it doesn't
    //       get shown in the menu item, not to mention that we would also like
    //       to support Ctrl+Q, so...

    mGui->actionExit->setShortcuts(QList<QKeySequence>()
                                       << QKeySequence(Qt::ALT|Qt::Key_F4)
                                       << QKeySequence(Qt::CTRL|Qt::Key_Q));
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    mGui->actionExit->setShortcut(QKeySequence::Quit);
#else
    #error Unsupported platform
#endif

#ifdef Q_OS_MAC
    // A special shortcut to have OpenCOR minimised on OS X when pressing Cmd+M
    // Note: indeed, when pressing Cmd+M on OS X, the active application is
    //       expected to minimise itself, so...

    new QShortcut(QKeySequence("Ctrl+M"),
                  this, SLOT(showMinimized()));
#endif

    mGui->actionFullScreen->setShortcut(QKeySequence::FullScreen);

    // Initialise our various plugins

    Plugins loadedPlugins = mPluginManager->loadedPlugins();

    foreach (Plugin *plugin, loadedPlugins) {
        // Do various things that are related to our differennt plugin
        // interfaces
        // Note: the order in which we do those things is important since, for
        //       example, the call to CoreInterface::initialize() may need some
        //       information which has been set as part of the GUI interface
        //       (e.g. the pointer to the main window)...

        // GUI interface

        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

        if (guiInterface)
            // Keep track of some information

            guiInterface->setMainWindow(this);

        // Internationalisation interface

        I18nInterface *i18nInterface = qobject_cast<I18nInterface *>(plugin->instance());

        if (i18nInterface)
            // Keep track of some information

            i18nInterface->setPluginName(plugin->name());

        // Core interface

        CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());

        if (coreInterface)
            // Initialise the plugin

            coreInterface->initialize();

        // Back to the GUI interface

        if (guiInterface)
            // Initialise the plugin further (i.e. do things which can only be
            // done by OpenCOR itself)

            initializeGuiPlugin(plugin->name(), guiInterface->guiSettings());
    }

    // Let our various plugins know that all of them have been initialised
    // Note: this is important to do, since the initialisation of a plugin is
    //       something which is done without knowing anything about other
    //       plugins. However, there may be things that require knowledge of
    //       what one or several other plugins are about, and this is something
    //       that can only be done once all the plugins have been initialised
    //       (e.g. the SingleCellSimulationView plugin needs to know which
    //       solvers, if any, are available to it)...

    foreach (Plugin *plugin, loadedPlugins) {
        CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());

        if (coreInterface)
            coreInterface->initializationsDone(loadedPlugins);
    }

#ifdef QT_DEBUG
    // Display our solvers' properties

    foreach (Plugin *plugin, loadedPlugins) {
        SolverInterface *solverInterface = qobject_cast<SolverInterface *>(plugin->instance());

        if (solverInterface) {
            qDebug("---------------------------------------");
            qDebug("'%s' solver:", qPrintable(solverInterface->name()));
            qDebug(" - Type: %s", qPrintable(solverInterface->typeAsString()));

            Solver::Properties properties = solverInterface->properties();

            if (properties.count()) {
                qDebug(" - Properties:");

                for (int i = 0, iMax = properties.count(); i < iMax; ++i) {
                    Solver::Property property = properties[i];
                    QString type;

                    switch (property.type()) {
                    case Solver::Double:
                        type = "Double";

                        break;
                    case Solver::Integer:
                        type = "Integer";

                        break;
                    default:
                        type = "???";
                    }

                    qDebug("    - %s: %s", qPrintable(property.name()), qPrintable(type));
                }
            } else {
                qDebug(" - Properties: none");
            }
        }
    }
#endif

    // Retrieve the user settings from the previous session, if any

    loadSettings();

    // Initialise the checked state of our full screen action, since OpenCOR may
    // (re)start in full screen mode

    mGui->actionFullScreen->setChecked(isFullScreen());
}

//==============================================================================

MainWindow::~MainWindow()
{
    // Finalise our various plugins
    // Note: we only need to test for our default interface since we want to
    //       call the finalize method and this method is not overriden by any
    //       other interface, so...

    Plugins loadedPlugins = mPluginManager->loadedPlugins();

    foreach (Plugin *plugin, loadedPlugins) {
        CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());

        if (coreInterface)
            coreInterface->finalize();
    }

    // Now that all of our plugins have been finalised, we can detroy their
    // respective interfaces
    // Note: not all of them may have things to destroy, but we better assume
    //       they all do in case they were ever to be modified...

    foreach (Plugin *plugin, loadedPlugins) {
        CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());
        FileInterface *fileInterface = qobject_cast<FileInterface *>(plugin->instance());
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());
        I18nInterface *i18nInterface = qobject_cast<I18nInterface *>(plugin->instance());
        SolverInterface *solverInterface = qobject_cast<SolverInterface *>(plugin->instance());

        if (coreInterface)
            coreInterface->destroy();

        if (fileInterface)
            fileInterface->destroy();

        if (guiInterface)
            guiInterface->destroy();

        if (i18nInterface)
            i18nInterface->destroy();

        if (solverInterface)
            solverInterface->destroy();
    }

    // Delete our central widget
    // Note: if we don't have one, then nothing will happen, so...

    delete centralWidget();

    // Delete some internal objects

    delete mPluginManager;
    delete mSettings;

    // Delete the GUI

    delete mGui;
}

//==============================================================================

void MainWindow::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    QMainWindow::changeEvent(pEvent);

    // Let our plugins know that something changed

    foreach (Plugin *plugin, mPluginManager->loadedPlugins()) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

        if (guiInterface)
            guiInterface->changeEvent(pEvent);
    }

    // Do a few more things for some changes

    if (   (pEvent->type() == QEvent::LocaleChange)
        && (mGui->actionSystem->isChecked()))
        // The system's locale has changed, so update OpenCOR's locale in case
        // the user wants to use the system's locale

        setLocale(SystemLocale);
#ifdef Q_OS_MAC
    else if (pEvent->type() == QEvent::WindowStateChange)
        // The window state has changed, so update the checked state of our full
        // screen action
        // Note: useful on OS X since there is a special full screen button in
        //       the main window's title bar...

        mGui->actionFullScreen->setChecked(isFullScreen());
#endif
}

//==============================================================================

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    // Check with our plugins that it's OK to close

    bool canClose = true;

    foreach (Plugin *plugin, mPluginManager->loadedPlugins()) {
        GuiInterface *guiInterface = qobject_cast<GuiInterface *>(plugin->instance());

        if (guiInterface)
            canClose = guiInterface->canClose() && canClose;
            // Note: we want to ask all the plugins whether we can close, hence
            //       the order of the above test...
    }

    // Close ourselves

    if (canClose) {
        // Keep track of our default settings
        // Note: it must be done here, as opposed to the destructor, otherwise
        //       some settings (e.g. docked windows) won't be properly saved...

        saveSettings();

        // Accept the event

        pEvent->accept();
    } else {
        // Ignore the event, i.e. don't close ourselves

        pEvent->ignore();
    }
}

//==============================================================================

void MainWindow::showEvent(QShowEvent *pEvent)
{
    // Default handling of the event

    QMainWindow::showEvent(pEvent);

    // Things which need to be done and can only be done once the main window is
    // fully created

    static bool firstTime = true;

    if (firstTime) {
        firstTime = false;

        // The first time we show OpenCOR, we want to make sure that its menu is
        // fine (i.e. it respects OpenCOR's settings that were loaded in the
        // constructor). Various connections (set in the constructor) take care
        // of this, but there is still one menu item (which tells us whether the
        // status bar is to be shown) for which no connection can be set. So, we
        // have to 'manually' set the status of that menu item here (as opposed
        // to, say, the constructor), since we may need (on Windows at least)
        // all of OpenCOR to be visible in order to be able to determine whether
        // the status bar is visible...

        mGui->actionStatusBar->setChecked(statusBar()->isVisible());
    }
}

//==============================================================================

void MainWindow::initializeGuiPlugin(const QString &pPluginName,
                                     GuiSettings *pGuiSettings)
{
    // Add the menus to our menu bar or merge them to existing menus, if needed
    // Note: we must do that in reverse order since we are inserting menus,
    //       as opposed to appending them...

    QListIterator<GuiMenuSettings *> menuIter(pGuiSettings->menus());

    menuIter.toBack();

    while (menuIter.hasPrevious()) {
        // Insert the menu in the right place

        GuiMenuSettings *menuSettings = menuIter.previous();

        QMenu *newMenu = menuSettings->menu();
        QString newMenuName = newMenu->objectName();

        QMenu *oldMenu = mMenus.value(newMenuName);

        if (oldMenu && !menuSettings->action()) {
            // A menu with the same name already exists, so add the contents of
            // the new menu to the existing one

            oldMenu->addSeparator();
            oldMenu->addActions(newMenu->actions());

            // Delete the new menu, since we don't need it...
            // Note: it's not critical since the menu never gets shown, but it
            //       doesn't harm either, so...

            delete newMenu;
        } else {
            // No menu with the same name already exists, so add the new menu to
            // our menu bar

            switch (menuSettings->type()) {
            case GuiMenuSettings::View:
                mGui->menuBar->insertAction(mGui->menuView->menuAction(),
                                            newMenu->menuAction());

                break;
            default:
                // Nothing to be done...

                ;
            }

            // Keep track of the new menu

            mMenus.insert(newMenuName, newMenu);
        }
    }

    // Add the actions/separators to our different menus
    // Note: as for the menus above, we must do that in reverse order since we
    //       are inserting actions, as opposed to appending them...

    QListIterator<GuiMenuActionSettings *> menuActionIter(pGuiSettings->menuActions());

    menuActionIter.toBack();

    while (menuActionIter.hasPrevious()) {
        // Insert the action/separator to the right menu

        GuiMenuActionSettings *menuActionSettings = menuActionIter.previous();

        switch (menuActionSettings->type()) {
        case GuiMenuActionSettings::File:
            if(menuActionSettings->action())
                mGui->menuFile->insertAction(mGui->menuFile->actions().first(),
                                             menuActionSettings->action());
            else
                mGui->menuFile->insertSeparator(mGui->menuFile->actions().first());

            break;
        default:
            // Not a type of interest, so do nothing...

            ;
        }
    }

    // Add some sub-menus before some menu items

    foreach (GuiMenuSettings *menuSettings, pGuiSettings->menus())
        // Insert the menu before a menu item / separator

        if (menuSettings->action())
            switch (menuSettings->type()) {
            case GuiMenuActionSettings::File:
                mGui->menuFile->insertMenu(menuSettings->action(),
                                           menuSettings->menu());

                break;
            default:
                // Not a type of interest, so do nothing...

                ;
            }

    // Add some actions to some sub-menus

    static QString pluginForFileNewMenu = QString();

    foreach (GuiMenuActionSettings *menuActionSettings,
             pGuiSettings->menuActions())
        // Insert the action to the right menu

        switch (menuActionSettings->type()) {
        case GuiMenuActionSettings::FileNew:
            // Check whether the File|New menu has been created and if not, then
            // create it

            if (!mFileNewMenu) {
                // The menu doesn't already exist, so create it

                mFileNewMenu = new QMenu(this);

                mFileNewMenu->menuAction()->setIcon(QIcon(":/oxygen/mimetypes/application-x-zerosize.png"));

                // Add the New menu to our File menu and add a separator after
                // it

                mGui->menuFile->insertMenu(mGui->menuFile->actions().first(),
                                           mFileNewMenu);
                mGui->menuFile->insertSeparator(mGui->menuFile->actions()[1]);

                pluginForFileNewMenu = pPluginName;
            } else if (pluginForFileNewMenu.compare(pPluginName)) {
                // The File|New menu already exists, so add a separator to it so
                // that previous menu items (from a different plugin) don't get
                // mixed up with the new one

                mFileNewMenu->addSeparator();

                pluginForFileNewMenu = pPluginName;
            }

            mFileNewMenu->addAction(menuActionSettings->action());

            break;
        default:
            // Not a type of interest, so do nothing...

            ;
        }

    // Set the central widget, but only if we are dealing with the Core plugin

    if (!pPluginName.compare(CorePlugin))
        if (pGuiSettings->centralWidget())
            setCentralWidget((QWidget *) pGuiSettings->centralWidget());

    // Add the windows (including to the corresponding menu)

    foreach (GuiWindowSettings *windowSettings, pGuiSettings->windows()) {
        // Dock the window to its default docking area

        QDockWidget *dockWidget = (QDockWidget *) windowSettings->window();

        addDockWidget(windowSettings->defaultDockingArea(), dockWidget);

        // Add an action to our menu to show/hide the menu

        bool doConnectDockWidgetToAction = true;

        switch (windowSettings->type()) {
        case GuiWindowSettings::Organisation:
            // Update the View menu by adding the action to the
            // View|Organisation menu

            updateViewMenu(GuiWindowSettings::Organisation,
                           windowSettings->action());

            break;
        case GuiWindowSettings::Help:
            if (!pPluginName.compare(HelpPlugin)) {
                // We only want to add the action if we are coming here from the
                // Help plugin

                mGui->menuHelp->insertAction(mGui->actionHomePage,
                                             windowSettings->action());
                mGui->menuHelp->insertSeparator(mGui->actionHomePage);
            } else {
                doConnectDockWidgetToAction = false;
            }

            break;
        default:
            // Unknown type, so...

            doConnectDockWidgetToAction = false;
        }

        // Connect the action to the window

        if (doConnectDockWidgetToAction)
            GuiInterface::connectDockWidgetToAction(dockWidget,
                                                    windowSettings->action());
    }

    // Reorder our various View menus, just in case

    reorderViewMenus();
}

//==============================================================================

static const QString SettingsLocale              = "Locale";
static const QString SettingsGeometry            = "Geometry";
static const QString SettingsState               = "State";
static const QString SettingsStatusBarVisibility = "StatusBarVisibility";

//==============================================================================

void MainWindow::loadSettings()
{
    // Retrieve and set the language to be used by OpenCOR
    // Note: the setting is forced in order to account for locale-dependent
    //       initialisations (e.g. see CentralWidget::retranslateUi())...

    setLocale(mSettings->value(SettingsLocale, SystemLocale).toString(), true);

    // Retrieve the geometry and state of the main window

    if (   !restoreGeometry(mSettings->value(SettingsGeometry).toByteArray())
        || !restoreState(mSettings->value(SettingsState).toByteArray())) {
        // The geometry and/or state of the main window couldn't be retrieved,
        // so go with some default settings

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

        mGui->statusBar->setVisible(mSettings->value(SettingsStatusBarVisibility,
                                                     true).toBool());
    }

    // Retrieve the settings of our various plugins

    Plugins loadedPlugins = mPluginManager->loadedPlugins();

    foreach (Plugin *plugin, loadedPlugins) {
        CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());

        if (coreInterface) {
            mSettings->beginGroup(SettingsPlugins);
                mSettings->beginGroup(plugin->name());
                    coreInterface->loadSettings(mSettings);
                mSettings->endGroup();
            mSettings->endGroup();
        }
    }

    // Let our various plugins know that all of them have loaded their settings
    // Note: this is similar to initialize() vs. initializationsDone()...

    foreach (Plugin *plugin, loadedPlugins) {
        CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());

        if (coreInterface)
            coreInterface->loadingOfSettingsDone(loadedPlugins);
    }
}

//==============================================================================

void MainWindow::saveSettings() const
{
    // Keep track of the language to be used by OpenCOR

    mSettings->setValue(SettingsLocale, mLocale);

    // Keep track of the geometry of the main window

    mSettings->setValue(SettingsGeometry, saveGeometry());

    // Keep track of the state of the main window

    mSettings->setValue(SettingsState, saveState());

    // Keep track of whether the status bar is to be shown

    mSettings->setValue(SettingsStatusBarVisibility,
                        mGui->statusBar->isVisible());

    // Keep track of the settings of our various plugins

    foreach (Plugin *plugin, mPluginManager->loadedPlugins()) {
        CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());

        if (coreInterface) {
            mSettings->beginGroup(SettingsPlugins);
                mSettings->beginGroup(plugin->name());
                    coreInterface->saveSettings(mSettings);
                mSettings->endGroup();
            mSettings->endGroup();
        }
    }
}

//==============================================================================

QString MainWindow::locale() const
{
    // Return the current locale

    return !mLocale.compare(SystemLocale)?
               QLocale::system().name().left(2):
               mLocale;
}

//==============================================================================

void MainWindow::setLocale(const QString &pLocale, const bool &pForceSetting)
{
    const QString systemLocale = QLocale::system().name().left(2);

    QString oldLocale = !mLocale.compare(SystemLocale)?systemLocale:mLocale;
    QString newLocale = !pLocale.compare(SystemLocale)?systemLocale:pLocale;

    // Keep track of the new locale, if needed

    if (pLocale.compare(mLocale))
        mLocale = pLocale;

    // Check whether the new locale is different from the old one and if so,
    // then retranslate everything

    if (oldLocale.compare(newLocale) || pForceSetting) {
        // Specify the language to be used by OpenCOR

        qApp->removeTranslator(&mQtTranslator);
        mQtTranslator.load(":qt_"+newLocale);
        qApp->installTranslator(&mQtTranslator);

        qApp->removeTranslator(&mAppTranslator);
        mAppTranslator.load(":app_"+newLocale);
        qApp->installTranslator(&mAppTranslator);

        // Retranslate OpenCOR

        mGui->retranslateUi(this);

        // Retranslate some widgets that are not originally part of our user
        // interface

        if (mFileNewMenu)
            GuiInterface::retranslateMenu(mFileNewMenu, tr("New"));

        if (mViewOrganisationMenu)
            GuiInterface::retranslateMenu(mViewOrganisationMenu,
                                          tr("Organisation"));

        // Update the locale of our various loaded plugins
        // Note: we must set the locale of all the plugins before we can safely
        //       retranslate them. Indeed, a plugin may require another plugin
        //       (which was loaded either before or after) to work properly. For
        //       example, the QtPropertyBrowserSupport plugin is loaded before
        //       the SingleCellSimulation plugin. Yet, the latter plugin needs
        //       the former plugin to be translated before the it can be used
        //       properly. Conversely, the Core plugin is loaded before the
        //       RawView plugin. Yet, the former plugin needs the latter plugin
        //       to be translated before it can be used properly (indeed, the
        //       Core plugin needs to know the name of the view that was created
        //       by the RawView plugin). So...

        Plugins loadedPlugins = mPluginManager->loadedPlugins();
        QList<I18nInterface *> i18nInterfaces = QList<I18nInterface *>();

        for (int i = 0, iMax = loadedPlugins.count(); i < iMax; ++i) {
            I18nInterface *i18nInterface = qobject_cast<I18nInterface *>(loadedPlugins[i]->instance());

            if (i18nInterface) {
                i18nInterface->setLocale(newLocale);

                i18nInterfaces << i18nInterface;
            }
        }

        // Retranslate our various plugins

        foreach (I18nInterface *i18nInterface, i18nInterfaces)
            i18nInterface->retranslateUi();

        // Reorder our various View menus, just in case

        reorderViewMenus();
    }

    // Update the checked menu item
    // Note: it has to be done every single time, since selecting a menu item
    //       will automatically toggle its checked status, so...

    mGui->actionSystem->setChecked(!pLocale.compare(SystemLocale));

    mGui->actionEnglish->setChecked(!pLocale.compare(EnglishLocale));
    mGui->actionFrench->setChecked(!pLocale.compare(FrenchLocale));
}

//==============================================================================

void MainWindow::reorderViewMenu(QMenu *pViewMenu)
{
    // Reorder the required View menu

    QStringList menuItemTitles;
    QMap<QString, QAction *> menuItemActions;

    // Retrieve the title of the menu items and keep track of their actions

    foreach (QAction *menuItemAction, pViewMenu->actions()) {
        // Remove any '&' present in the menu item title, as well as replace
        // accentuated characters by non-accentuated ones, making the sorting
        // sensible

        QString menuItemTitle = menuItemAction->text().remove("&").normalized(QString::NormalizationForm_KD);

        for (int i = menuItemTitle.length()-1; i >= 0; --i)
            if (menuItemTitle[i].category() == QChar::Mark_NonSpacing)
                menuItemTitle.remove(i, 1);

        // Keep track of the menu item title and the action to which it is
        // associated

        menuItemTitles << menuItemTitle;
        menuItemActions.insert(menuItemTitle, menuItemAction);
    }

    // Sort the menu items

    menuItemTitles.sort();

    // Add the menu items actions in the new order
    // Note: to use addAction will effectively 'move' the menu items to the end
    //       of the menu, so since we do it in the right order, we end up with
    //       the menu items being properly ordered...

    foreach (const QString menuItemTitle, menuItemTitles)
        pViewMenu->addAction(menuItemActions.value(menuItemTitle));
}

//==============================================================================

void MainWindow::reorderViewMenus()
{
    // Reorder the View|Organisation men, should it exist
    // Note: this is useful after having added a new menu item or after having
    //       changed the locale

    if (mViewOrganisationMenu)
        reorderViewMenu(mViewOrganisationMenu);
}

//==============================================================================

void MainWindow::updateViewMenu(const GuiWindowSettings::GuiWindowSettingsType &pMenuType,
                                QAction *pAction)
{
    // Check whether we need to insert a separator before the status bar menu
    // item

    if ((pMenuType != GuiWindowSettings::Help) && !mViewSeparator)
        // None of the menus have already been inserted which means that we need
        // to insert a separator before the Full Screen menu item

        mViewSeparator =  mGui->menuView->insertSeparator(mGui->actionStatusBar);

    // Determine the menu that is to be inserted, should this be required, and
    // the action before which it is to be inserted

    QMenu **menu;
    QAction *action;

    switch (pMenuType) {
    case GuiWindowSettings::Organisation:
        menu   = &mViewOrganisationMenu;
        action = mViewSeparator;

        break;
    default:
        // Unknown type, so just leave...

        return;
    }

    // Check whether the menu already exists and create it if not

    if (!*menu) {
        // The menu doesn't already exist, so create it

        *menu = new QMenu(this);

        // Add the menu to our View menu

        mGui->menuView->insertMenu(action, *menu);
    }

    // At this stage, the menu to which we want to add an action has been
    // created, so we can just add the action to it

    (*menu)->addAction(pAction);
}

//==============================================================================

void MainWindow::showSelf()
{
    // Note: to show ourselves, one would normally use activateWindow() (and
    //       possibly raise()), but depending on the operating system it may or
    //       not bring OpenCOR to the foreground, so... instead we do what
    //       follows, depending on the operating system...

#if defined(Q_OS_WIN)
    // Show ourselves the Windows way

    // Retrieve OpenCOR's window Id

    HWND mainWinId = reinterpret_cast<HWND>(winId());

    // Bring OpenCOR to the foreground

    DWORD foregroundThreadProcId = GetWindowThreadProcessId(GetForegroundWindow(), 0);
    DWORD mainThreadProcId = GetWindowThreadProcessId(mainWinId, 0);

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
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    // We are on Linux or OS X, so we can simply activate the window and raise
//---GRY--- THERE ARE A COUPLE OF ISSUES WITH raise(), SEE
//          https://bugreports.qt-project.org/browse/QTBUG-29087
//          https://bugreports.qt-project.org/browse/QTBUG-29107

    activateWindow();
    raise();
#else
    #error Unsupported platform
#endif
}

//==============================================================================

void MainWindow::handleArguments(const QString &pArguments)
{
    // Handle the arguments that were passed to OpenCOR by passing them to the
    // Core plugin, should it be loaded

    foreach (Plugin *plugin, mPluginManager->loadedPlugins())
        if (!plugin->name().compare(CorePlugin)) {
            CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());

            if (coreInterface)
                coreInterface->handleArguments(pArguments.split("|"));

            break;
        }
}

//==============================================================================

void MainWindow::fileOpenRequest(const QString &pFileName)
{
    // We have received a request to open a file, so handle it as an argument
    // that was passed to OpenCOR

    handleArguments(pFileName);
}

//==============================================================================

void MainWindow::handleAction(const QUrl &pUrl)
{
    // Handle the action that was passed to OpenCOR

    QString authority = pUrl.authority();

    if (!authority.compare("actionplugins")) {
        // We want to open the Plugins dialog box

        on_actionPlugins_triggered();
    } else if (!authority.compare("actionabout")) {
        // We want to open the About box

        on_actionAbout_triggered();
    } else {
        // We are dealing with an action which OpenCOR itself can't handle, but
        // maybe one of its loaded plugins can

        QString host = pUrl.host();

        foreach (Plugin *plugin, mPluginManager->loadedPlugins())
            if (!plugin->name().toLower().compare(host)) {
                // This is an action for the current plugin, so forward the
                // action to it, should it support the Core interface

                CoreInterface *coreInterface = qobject_cast<CoreInterface *>(plugin->instance());

                if (coreInterface)
                    // The plugin supports the Core interface, so ask it to
                    // handle the action

                    coreInterface->handleAction(pUrl);

                break;
            }
    }
}

//==============================================================================

void MainWindow::messageReceived(const QString &pMessage)
{
    // We have just received a message which can be one of two things:
    //  1) The user tried to run another instance of OpenCOR which sent a
    //     message to this instance, asking it to bring itself to the foreground
    //     and handling all the arguments passed in the message; or
    //  2) A GUI action was sent to us, so we need to handle it.

    // Check whether the passed message corresponds to a GUI action

    QUrl url = pMessage;

    if (!url.scheme().compare("gui")) {
        // We are dealing with a GUI COR action, so handle it

        handleAction(url);
    } else {
        // It's not a GUI action, so bring ourselves to the foreground

        showSelf();

        // Now, we must handle the arguments that were passed to us

        handleArguments(pMessage);
    }
}

//==============================================================================

void MainWindow::on_actionFullScreen_triggered()
{
    // Switch to / back from full screen mode
//---GRY--- SOME BLACK MAGIN IS NEEDED FOR OS X. INDEED, ON THAT PLATFORM,
//          OpenCOR CAN BE SWITCHED TO / BACK FROM FULL SCREEN MODE EITHER
//          THROUGH ITS MENU OR USING ITS FULL SCREEN MODE BUTTON (LOCATED IN
//          THE TOP RIGHT OF ITS TITLE BAR). IF ONE USES ONLY ONE METHOD, THEN
//          TO SIMPLY USE showFullScreen() AND showNormal() IS FINE, BUT IF FOR
//          SOME REASON THE USER DECIDES TO MIX BOTH METHODS THEN THE BLACK
//          MAGIC IS NEEDED...!?

    if (isFullScreen()) {
        showNormal();
#ifdef Q_OS_MAC
        showFullScreen();
#endif
    } else {
        showFullScreen();
#ifdef Q_OS_MAC
        showNormal();
#endif
    }
}

//==============================================================================

void MainWindow::on_actionSystem_triggered()
{
    // Select the system's language as the language used by OpenCOR

    setLocale(SystemLocale);
}

//==============================================================================

void MainWindow::on_actionEnglish_triggered()
{
    // Select English as the language used by OpenCOR

    setLocale(EnglishLocale);
}

//==============================================================================

void MainWindow::on_actionFrench_triggered()
{
    // Select French as the language used by OpenCOR

    setLocale(FrenchLocale);
}

//==============================================================================

void MainWindow::on_actionPlugins_triggered()
{
    // Plugins preferences

    if (mPluginManager->plugins().count()) {
        // There are some plugins, so we can show the plugins window

        OpenCOR::PluginsWindow pluginsWindow(mPluginManager, this);

        mSettings->beginGroup(pluginsWindow.objectName());
            pluginsWindow.loadSettings(mSettings);
        mSettings->endGroup();

        pluginsWindow.exec();
        // Note: the execution of the plugins window may result in the saving of
        //       the application's settings, so for this to work we must ensure
        //       that any opened settings group has first been closed...

        mSettings->beginGroup(pluginsWindow.objectName());
            pluginsWindow.saveSettings(mSettings);
        mSettings->endGroup();
    } else {
        // There are no plugins, so...

        QMessageBox::warning(this, tr("Plugins"),
                             tr("Sorry, but no plugin could be found."));
    }
}

//==============================================================================

void MainWindow::on_actionHomePage_triggered()
{
    // Look up OpenCOR's home page

    QDesktopServices::openUrl(QUrl(OpencorHomepageUrl));
}

//==============================================================================

void MainWindow::on_actionAbout_triggered()
{
    // Display some information about OpenCOR

    QMessageBox::about(this, tr("About"),
                        "<h1 align=center><b>"+getAppVersion(qApp)+"</b></h1>"
                       +"<h3 align=center><em>"+getOsName()+"</em></h3>"
                       +"<p align=center><em>"+getAppCopyright(true)+"</em></p>"
                       +"<a href=\""+QString(OpencorHomepageUrl)+"\">"+qApp->applicationName()+"</a> "+tr("is a cross-platform <a href=\"http://www.cellml.org/\">CellML</a>-based modelling environment which can be used to organise, edit, simulate and analyse CellML files."));
}

//==============================================================================

void MainWindow::restart(const bool &pSaveSettings) const
{
    // Restart OpenCOR after saving its settings, if required
    // Note: the closeEvent method won't get called when exiting OpenCOR and
    //       this is exactly what we want in case we don't want to save its
    //       settings (e.g. when resetting all)

    if (pSaveSettings)
        saveSettings();

    qApp->exit(NeedRestart);
}

//==============================================================================

void MainWindow::resetAll()
{
    if (QMessageBox::question(this, qApp->applicationName(),
                              tr("You are about to reset <strong>all</strong> of your settings. Do you wish to proceed?"),
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::Yes) == QMessageBox::Yes ) {
        // We want to reset everything, so clear all the user settings and
        // restart OpenCOR (indeed, a restart will ensure that the various dock
        // windows are, for instance, properly reset with regards to their
        // dimensions)

        mSettings->clear();

        // Restart OpenCOR without first saving its settings

        restart(false);
    }
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
