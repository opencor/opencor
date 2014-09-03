/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Main window
//==============================================================================

#include "centralwidget.h"
#include "cliutils.h"
#include "common.h"
#include "coreinterface.h"
#include "coresettings.h"
#include "guiinterface.h"
#include "guiutils.h"
#include "i18ninterface.h"
#include "mainwindow.h"
#include "plugininterface.h"
#include "pluginmanager.h"
#include "pluginswindow.h"
#include "preferenceswindow.h"
#include "settings.h"
#include "viewinterface.h"
#include "windowinterface.h"
#include "windowwidget.h"

//==============================================================================

#include "ui_mainwindow.h"

//==============================================================================

#ifdef Q_OS_WIN
    #include <windows.h>
#endif

//==============================================================================

#include <Qt>

//==============================================================================

#include <QAction>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QRect>
#include <QSettings>
#ifdef Q_OS_MAC
    #include <QShortcut>
#endif
#include <QUrl>

//==============================================================================

#include <QtSingleApplication>

//==============================================================================

namespace OpenCOR {

//==============================================================================

static const auto SystemLocale  = QStringLiteral("");
static const auto EnglishLocale = QStringLiteral("en");
static const auto FrenchLocale  = QStringLiteral("fr");

//==============================================================================

MainWindow::MainWindow(SharedTools::QtSingleApplication *pApp) :
    QMainWindow(),
    mGui(new Ui::MainWindow),
    mShuttingDown(false),
    mLoadedPluginPlugins(Plugins()),
    mLoadedI18nPlugins(Plugins()),
    mLoadedGuiPlugins(Plugins()),
    mLoadedWindowPlugins(Plugins()),
    mLocale(QString()),
    mMenus(QMap<QString, QMenu *>()),
    mFileNewMenu(0),
    mViewWindowsMenu(0),
    mViewSeparator(0),
    mViewPlugin(0),
    mDockedWindowsVisible(true),
    mDockedWindowsState(QByteArray()),
    mStatusBarVisible(false)
{
    // Make sure that OpenCOR can handle a file opening request (from the
    // operating system), as well as a message sent by another instance of
    // itself

    QObject::connect(pApp, SIGNAL(fileOpenRequest(const QString &)),
                     this, SLOT(fileOpenRequest(const QString &)));
    QObject::connect(pApp, SIGNAL(messageReceived(const QString &, QObject *)),
                     this, SLOT(messageReceived(const QString &, QObject *)));

    // Create our settings object

    mSettings = new QSettings(SettingsOrganization, SettingsApplication);

    // Create our plugin manager (which will automatically load our various
    // plugins)

    mPluginManager = new PluginManager(qApp);

    // Retrieve some categories of plugins

    foreach (Plugin *plugin, mPluginManager->loadedPlugins()) {
        if (qobject_cast<PluginInterface *>(plugin->instance()))
            mLoadedPluginPlugins << plugin;

        if (qobject_cast<I18nInterface *>(plugin->instance()))
            mLoadedI18nPlugins << plugin;

        if (qobject_cast<GuiInterface *>(plugin->instance()))
            mLoadedGuiPlugins << plugin;

        if (qobject_cast<WindowInterface *>(plugin->instance()))
            mLoadedWindowPlugins << plugin;
    }

    // Set up the GUI
    // Note: the application icon (which needs to be set for Linux, but not for
    //       Windows or OS X, since it's set through CMake in those cases (see
    //       CMakeLists.txt)) is set within the GUI file. This being said, it's
    //       good to have it set for all three platforms, since it can then be
    //       used in, for example, the about box...

    mGui->setupUi(this);
//---GRY--- DISABLED UNTIL WE ACTUALLY SUPPORT USER PREFERENCES...
Core::showEnableAction(mGui->actionPreferences, false);

    // Set the role of some of our menu items, so that OS X can move them into
    // the application menu

    mGui->actionQuit->setMenuRole(QAction::QuitRole);
    mGui->actionPreferences->setMenuRole(QAction::PreferencesRole);
    mGui->actionAbout->setMenuRole(QAction::AboutRole);

    // Title of our main window
    // Note: we don't set it in our .ui file since this will require
    //       'translating' it in other languages...

    setWindowTitle(qApp->applicationName());

    // Customise our docked windows action and handle it through a connection
    // Note: Ctrl+Space doesn't work on OS X, so ideally we would add the Alt
    //       key (since it's next to it, but it doesn't work either), so in the
    //       we add the Meta key...

#if defined(Q_OS_MAC)
    mGui->actionDockedWindows->setShortcut(QKeySequence(Qt::CTRL|Qt::META|Qt::Key_Space));
#else
    mGui->actionDockedWindows->setShortcut(QKeySequence(Qt::CTRL|Qt::Key_Space));
#endif

    connect(mGui->actionDockedWindows, SIGNAL(triggered(bool)),
            this, SLOT(showDockedWindows(const bool &)));

    // A connection to handle the status bar

    connect(mGui->actionStatusBar, SIGNAL(triggered(bool)),
            statusBar(), SLOT(setVisible(bool)));

    // Some connections to handle our various menu items

    connect(mGui->actionQuit, SIGNAL(triggered()),
            this, SLOT(close()));
    connect(mGui->actionResetAll, SIGNAL(triggered()),
            this, SLOT(resetAll()));

    // Set the shortcuts of some actions
    // Note: we do it here, so that we can use standard shortcuts (whenever
    //       possible)...

#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    // Note: normally, we would be using QKeySequence::Quit, but we want to
    //       support both Alt+F4 and Ctrl+Q on Windows and Linux, and the
    //       default key sequence doesn't, so...

    mGui->actionQuit->setShortcuts(QList<QKeySequence>()
                                       << QKeySequence(Qt::ALT|Qt::Key_F4)
                                       << QKeySequence(Qt::CTRL|Qt::Key_Q));
#elif defined(Q_OS_MAC)
    mGui->actionQuit->setShortcut(QKeySequence::Quit);
#else
    #error Unsupported platform
#endif

#ifdef Q_OS_MAC
    // A special shortcut to have OpenCOR minimised on OS X when pressing Cmd+M
    // Note: indeed, when pressing Cmd+M on OS X, the active application is
    //       expected to minimise itself, so...

    new QShortcut(QKeySequence("Ctrl+M"),
                  this, SLOT(showMinimized()));

    // Note: we used to have a shortcut (the Escape key) to have OpenCOR resume
    //       from full screen mode, but this conflicted with EditorWidget where
    //       the Escape key is used to hide the find/replace widget. So, we
    //       decided to remove the shortcut. In the end, we are actually being
    //       consistent with other editing tools such as Qt Creator and
    //       TextWrangler...
#endif

    mGui->actionFullScreen->setShortcut(QKeySequence::FullScreen);

    // Initialise the plugin itself

    foreach (Plugin *plugin, mLoadedPluginPlugins)
        qobject_cast<PluginInterface *>(plugin->instance())->initializePlugin(this);

    // Initialise the plugin further by doing things that can only be done by
    // OpenCOR itself (e.g. set the central widget, create some menus)

    foreach (Plugin *plugin, mPluginManager->loadedPlugins())
        initializeGuiPlugin(plugin);

    // Keep track of the plugin's name in case we support
    // internationalisation

    foreach (Plugin *plugin, mLoadedI18nPlugins)
        qobject_cast<I18nInterface *>(plugin->instance())->setPluginName(plugin->name());

    // Let our various plugins know that all of them have been initialised
    // Note: this is important to do since the initialisation of a plugin is
    //       something that is done without knowing anything about other
    //       plugins. However, there may be things that require knowledge of
    //       what one or several other plugins are about, and this is something
    //       that can only be done once all the plugins have been initialised
    //       (e.g. the SingleCellSimulationView plugin needs to know which
    //       solvers, if any, are available to it)...

    foreach (Plugin *plugin, mLoadedPluginPlugins)
        qobject_cast<PluginInterface *>(plugin->instance())->pluginsInitialized(mPluginManager->loadedPlugins());

    // Keep track of the showing/hiding of the different window widgets

    foreach (Plugin *plugin, mLoadedWindowPlugins)
        connect(qobject_cast<WindowInterface *>(plugin->instance())->windowWidget(), SIGNAL(visibilityChanged(bool)),
                this, SLOT(updateDockWidgetsVisibility()));

    // Show/hide and enable/disable the windows action depending on whether
    // there are window widgets

    Core::showEnableAction(mGui->actionDockedWindows, mLoadedWindowPlugins.count());

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

    foreach (Plugin *plugin, mLoadedPluginPlugins)
        qobject_cast<PluginInterface *>(plugin->instance())->finalizePlugin();

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

    // Do a few more things for some changes

    if (   (pEvent->type() == QEvent::LocaleChange)
        && (mGui->actionSystem->isChecked())) {
        // The system's locale has changed, so update OpenCOR's locale in case
        // the user wants to use the system's locale

        setLocale(SystemLocale);
#ifdef Q_OS_MAC
    } else if (pEvent->type() == QEvent::WindowStateChange) {
        // The window state has changed, so update the checked state of our full
        // screen action
        // Note: useful on OS X since there is a special full screen button in
        //       the main window's title bar...

        mGui->actionFullScreen->setChecked(isFullScreen());
#endif
    } else if (pEvent->type() == QEvent::PaletteChange) {
        // The palette has changed, so update our colours

        Core::updateColors();
    }
}

//==============================================================================

void MainWindow::closeEvent(QCloseEvent *pEvent)
{
    // Check with our Core plugin, if it has been loaded, that it's OK to close

    bool canClose = true;

    if (mPluginManager->corePlugin())
        canClose = qobject_cast<CoreInterface *>(mPluginManager->corePlugin()->instance())->canClose();
        // Note: if the Core plugin is loaded, then it means it supports the
        //       Core interface, so no need to check anything...

    // Close ourselves, if possible

    if (canClose) {
        mShuttingDown = true;

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

    // Things that need to be done and can only be done once the main window is
    // fully initialised

    static bool firstTime = true;

    if (firstTime) {
        firstTime = false;

        // The first time we show OpenCOR, we want to make sure that its menu is
        // fine (i.e. it respects OpenCOR's settings that were loaded in the
        // constructor). Various connections (set in the constructor) take care
        // of this, but there is still one menu item (that tells us whether the
        // status bar is to be shown) for which no connection can be set. So, we
        // have to 'manually' set the status of that menu item here (as opposed
        // to, say, the constructor), since we may need (on Windows at least)
        // all of OpenCOR to be visible in order to be able to determine whether
        // the status bar is visible...

        statusBar()->setVisible(mStatusBarVisible);

        mGui->actionStatusBar->setChecked(statusBar()->isVisible());
    }
}

//==============================================================================

void MainWindow::initializeGuiPlugin(Plugin *pPlugin)
{
    // Retrieve and apply the plugin's GUI settings, if any

    GuiInterface *guiInterface = qobject_cast<GuiInterface *>(pPlugin->instance());

    if (guiInterface) {
        // Add the menus to our menu bar or merge them to existing menus, if
        // needed
        // Note: we must do that in reverse order since we are inserting menus,
        //       as opposed to appending some...

        Gui::MenuIterator menuIter(guiInterface->guiMenus());

        menuIter.toBack();

        while (menuIter.hasPrevious()) {
            // Insert the menu in the right place

            Gui::Menu menu = menuIter.previous();

            QMenu *newMenu = menu.menu();
            QString newMenuName = newMenu->objectName();

            QMenu *oldMenu = mMenus.value(newMenuName);

            if (oldMenu && !menu.action()) {
                // A menu with the same name already exists, so add the contents
                // of the new menu to the existing one

                oldMenu->addSeparator();
                oldMenu->addActions(newMenu->actions());

                // Delete the new menu since we don't need it anymore

                delete newMenu;
            } else {
                // No menu with the same name already exists (or the menu
                // doesn't have a name), so add the new menu to our menu bar

                switch (menu.type()) {
                case Gui::Menu::View:
                    mGui->menuBar->insertAction(mGui->menuView->menuAction(),
                                                newMenu->menuAction());

                    break;
                default:
                    // Not a type in which we are interested, so do nothing...

                    ;
                }

                // Keep track of the new menu, but only if it has a name

                if (newMenuName.size())
                    mMenus.insert(newMenuName, newMenu);
            }
        }

        // Add the actions/separators to our different menus
        // Note: as for the menus above, we must do it in reverse order since we
        //       are inserting actions, as opposed to appending some...

        Gui::MenuActionIterator menuActionIter(guiInterface->guiMenuActions());

        menuActionIter.toBack();

        while (menuActionIter.hasPrevious()) {
            // Insert the action/separator to the right menu

            Gui::MenuAction menuAction = menuActionIter.previous();

            switch (menuAction.type()) {
            case Gui::MenuAction::File: {
                QAction *action = menuAction.action();

                if (action)
                    mGui->menuFile->insertAction(mGui->menuFile->actions().first(), action);
                else
                    mGui->menuFile->insertSeparator(mGui->menuFile->actions().first());

                break;
            }
            case Gui::MenuAction::Tools: {
                QAction *action = menuAction.action();

                if (action)
                    mGui->menuTools->insertAction(mGui->menuTools->actions().first(), action);
                else
                    mGui->menuTools->insertSeparator(mGui->menuTools->actions().first());

                break;
            }
            default:
                // Not a type in which we are interested, so do nothing...

                ;
            }
        }

        // Add some sub-menus before some menu items

        foreach (const Gui::Menu &menu, guiInterface->guiMenus()) {
            // Insert the menu before a menu item / separator

            if (menu.action())
                switch (menu.type()) {
                case Gui::Menu::File:
                    mGui->menuFile->insertMenu(menu.action(), menu.menu());

                    break;
                default:
                    // Not a type in which we are interested, so do nothing...

                    ;
                }
        }

        // Add some actions to some sub-menus and keep track of them

        static QString pluginForFileNewMenu = QString();

        foreach (const Gui::MenuAction &menuAction, guiInterface->guiMenuActions()) {
            // Insert the action to the right menu

            switch (menuAction.type()) {
            case Gui::MenuAction::FileNew:
                // Check whether the File|New menu has been created and if not,
                // then create it

                if (!mFileNewMenu) {
                    // The menu doesn't already exist, so create it

                    mFileNewMenu = new QMenu(this);

                    mFileNewMenu->menuAction()->setIcon(QIcon(":/oxygen/mimetypes/application-x-zerosize.png"));

                    // Add the New menu to our File menu and add a separator
                    // after it

                    mGui->menuFile->insertMenu(mGui->menuFile->actions().first(),
                                               mFileNewMenu);
                    mGui->menuFile->insertSeparator(mGui->menuFile->actions()[1]);

                    pluginForFileNewMenu = pPlugin->name();
                } else if (pluginForFileNewMenu.compare(pPlugin->name())) {
                    // The File|New menu already exists, so add a separator to
                    // it so that previous menu items (from a different plugin)
                    // don't get mixed up with the new one

                    mFileNewMenu->addSeparator();

                    pluginForFileNewMenu = pPlugin->name();
                }

                mFileNewMenu->addAction(menuAction.action());

                break;
            default:
                // Not a type in which we are interested, so do nothing...

                ;
            }
        }
    }

    // Set our central widget, in case we are dealing with the Core plugin

    if (pPlugin == mPluginManager->corePlugin()) {
        // We are dealing with the Core plugin, so set our central widget

        setCentralWidget(qobject_cast<CoreInterface *>(pPlugin->instance())->centralWidget());
        // Note: if the Core plugin is loaded, then it means it supports the
        //       Core interface, so no need to check anything...

        // Also keep track of GUI updates in our central widget

        connect(static_cast<Core::CentralWidget *>(centralWidget()), SIGNAL(guiUpdated(Plugin *, const QString &)),
                this, SLOT(updateGui(Plugin *, const QString &)));
    }

    // Add the plugin's window, in case we are dealing with a window plugin

    WindowInterface *windowInterface = qobject_cast<WindowInterface *>(pPlugin->instance());

    if (windowInterface) {
        // Dock the window to its default dock area

        addDockWidget(windowInterface->windowDefaultDockArea(), windowInterface->windowWidget());

        // Add an action to our menu to show/hide the window

        if (!pPlugin->name().compare("HelpWindow")) {
            // Special case of the help window

            mGui->menuHelp->insertAction(mGui->actionHomePage,
                                         windowInterface->windowAction());
            mGui->menuHelp->insertSeparator(mGui->actionHomePage);
        } else {
            // Update the View menu by adding the action to the
            // View|Windows menu

            updateViewWindowsMenu(windowInterface->windowAction());
        }

        // Connect the action to the window

        connect(windowInterface->windowAction(), SIGNAL(triggered(bool)),
                windowInterface->windowWidget(), SLOT(setVisible(bool)));
        connect(windowInterface->windowWidget()->toggleViewAction(), SIGNAL(toggled(bool)),
                windowInterface->windowAction(), SLOT(setChecked(bool)));
    }
}

//==============================================================================

static const auto SettingsGeometry             = QStringLiteral("Geometry");
static const auto SettingsState                = QStringLiteral("State");
static const auto SettingsDockedWindowsVisible = QStringLiteral("DockedWindowsVisible");
static const auto SettingsDockedWindowsState   = QStringLiteral("DockedWindowsState");
static const auto SettingsStatusBarVisible     = QStringLiteral("StatusBarVisible");

//==============================================================================

void MainWindow::loadSettings()
{
    // Retrieve the geometry and state of the main window

    if (   !restoreGeometry(mSettings->value(SettingsGeometry).toByteArray())
        || !restoreState(mSettings->value(SettingsState).toByteArray())) {
        // The geometry and/or state of the main window couldn't be retrieved,
        // so go with some default settings

        // Default size and position of the main window

        QRect desktopGeometry = qApp->desktop()->availableGeometry();
        int horizSpace = desktopGeometry.width()/13;
        int vertSpace  = desktopGeometry.height()/13;

        setGeometry(desktopGeometry.left()+horizSpace,
                    desktopGeometry.top()+vertSpace,
                    desktopGeometry.width()-2*horizSpace,
                    desktopGeometry.height()-2*vertSpace);
    }

    // Retrieve whether the docked windows are to be shown

    showDockedWindows(mSettings->value(SettingsDockedWindowsVisible, true).toBool(), true);

    // Retrieve the state of the docked windows

    mDockedWindowsState = mSettings->value(SettingsDockedWindowsState, QByteArray()).toByteArray();

    // Retrieve whether the status bar is to be shown

    mStatusBarVisible = mSettings->value(SettingsStatusBarVisible, true).toBool();

    // Retrieve the settings of our various plugins

    foreach (Plugin *plugin, mLoadedPluginPlugins) {
        mSettings->beginGroup(SettingsPlugins);
            mSettings->beginGroup(plugin->name());
                qobject_cast<PluginInterface *>(plugin->instance())->loadSettings(mSettings);
            mSettings->endGroup();
        mSettings->endGroup();
    }

    // Let our core plugin know that all of the plugins have loaded their
    // settings
    // Note: this is similar to initializePlugin() vs. pluginsInitialized()...

    if (mPluginManager->corePlugin())
        qobject_cast<CoreInterface *>(mPluginManager->corePlugin()->instance())->settingsLoaded(mPluginManager->loadedPlugins());
        // Note: if the Core plugin is loaded, then it means it supports the
        //       Core interface, so no need to check anything...

    // Remove the File menu when on OS X, should no plugins be loaded
    // Note: our File menu should only contain the Exit menu item, but on OS X
    //       that menu item gets automatically moved to the application menu,
    //       so...

#ifdef Q_OS_MAC
    mGui->menuFile->menuAction()->setVisible(mPluginManager->loadedPlugins().count());
#endif

    // Retrieve and set the language to be used by OpenCOR
    // Note #1: the setting is forced in order to account for locale-dependent
    //          initialisations (e.g. see CentralWidget::retranslateUi())...
    // Note #2: this must be done once all the settings have been loaded since
    //          some plugins may, as a result of the loading of settings, create
    //          widgets that need translating (e.g. graph panels get created in
    //          the SingleCellView plugin)...

    setLocale(mSettings->value(SettingsLocale, SystemLocale).toString(), true);
}

//==============================================================================

void MainWindow::saveSettings() const
{
    // Keep track of the language to be used by OpenCOR

    mSettings->setValue(SettingsLocale, mLocale);

    // Keep track of the geometry and state of the main window

    mSettings->setValue(SettingsGeometry, saveGeometry());
    mSettings->setValue(SettingsState, saveState());

    // Keep track of whether the docked windows are to be shown

    mSettings->setValue(SettingsDockedWindowsVisible, mDockedWindowsVisible);

    // Keep track of the state of the docked windows

    mSettings->setValue(SettingsDockedWindowsState, mDockedWindowsState);

    // Keep track of whether the status bar is to be shown

    mSettings->setValue(SettingsStatusBarVisible,
                        mGui->statusBar->isVisible());

    // Keep track of the settings of our various plugins

    foreach (Plugin *plugin, mLoadedPluginPlugins) {
        mSettings->beginGroup(SettingsPlugins);
            mSettings->beginGroup(plugin->name());
                qobject_cast<PluginInterface *>(plugin->instance())->saveSettings(mSettings);
            mSettings->endGroup();
        mSettings->endGroup();
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

    if (pLocale.compare(mLocale) || pForceSetting) {
        mLocale = pLocale;

        // Also keep a copy of the new locale in our global settings (so that it
        // can be retrieved from any plugin)

        QSettings settings(SettingsOrganization, SettingsApplication);

        settings.beginGroup(SettingsGlobal);
            settings.setValue(SettingsLocale, locale());
        settings.endGroup();
    }

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
            I18nInterface::retranslateMenu(mFileNewMenu, tr("New"));

        if (mViewWindowsMenu)
            I18nInterface::retranslateMenu(mViewWindowsMenu, tr("Windows"));

        // Update the locale of our various loaded plugins
        // Note: we must set the locale of all the plugins before we can safely
        //       retranslate them since a plugin may require another plugin to
        //       work properly...

        foreach (Plugin *plugin, mLoadedI18nPlugins)
            qobject_cast<I18nInterface *>(plugin->instance())->setLocale(newLocale);

        // Retranslate our various plugins

        foreach (Plugin *plugin, mLoadedI18nPlugins)
            qobject_cast<I18nInterface *>(plugin->instance())->retranslateUi();

        // Reorder our various View|Windows menu items, just in case

        reorderViewWindowsMenu();
    }

    // Update the checked menu item
    // Note: it has to be done every single time, since selecting a menu item
    //       will automatically toggle its checked status, so...

    mGui->actionSystem->setChecked(!pLocale.compare(SystemLocale));

    mGui->actionEnglish->setChecked(!pLocale.compare(EnglishLocale));
    mGui->actionFrench->setChecked(!pLocale.compare(FrenchLocale));
}

//==============================================================================

void MainWindow::reorderViewWindowsMenu()
{
    // Reorder the View|Windows menu, should it exist
    // Note: this is useful after having added a new menu item or after having
    //       changed the locale

    if (mViewWindowsMenu) {
        // Retrieve the title of the menu items and keep track of their actions

        QStringList menuItemTitles;
        QMap<QString, QAction *> menuItemActions;

        foreach (QAction *menuItemAction, mViewWindowsMenu->actions()) {
            // Remove any "&" present in the menu item title, as well as replace
            // accentuated characters by non-accentuated ones, making the
            // sorting sensible

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
        // Note: to use addAction will effectively 'move' the menu items to the
        //       end of the menu, so since we do it in the right order, we end
        //       up with the menu items being properly ordered...

        foreach (const QString &menuItemTitle, menuItemTitles)
            mViewWindowsMenu->addAction(menuItemActions.value(menuItemTitle));
    }
}

//==============================================================================

void MainWindow::updateViewWindowsMenu(QAction *pAction)
{
    // Check whether we need to insert a separator before the docked windows
    // menu item

    if (!mViewSeparator)
        mViewSeparator = mGui->menuView->insertSeparator(mGui->actionDockedWindows);

    // Check whether the View|Windows menu already exists and create it if not

    if (!mViewWindowsMenu) {
        // The View|Windows menu doesn't already exist, so create it

        mViewWindowsMenu = new QMenu(this);

        // Add the View|Windows menu to our View menu

        mGui->menuView->insertMenu(mViewSeparator, mViewWindowsMenu);
    }

    // At this stage, the View|Windows menu exist, so add the given action to
    // it

    mViewWindowsMenu->addAction(pAction);
}

//==============================================================================

bool MainWindow::shuttingDown() const
{
    // Return whether we are shutting down

    return mShuttingDown;
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
    // ourselves

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

    if (mPluginManager->corePlugin())
        qobject_cast<CoreInterface *>(mPluginManager->corePlugin()->instance())->handleArguments(pArguments.split("|"));
        // Note: if the Core plugin is loaded, then it means it supports the
        //       Core interface, so no need to check anything...
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

    if (!authority.compare("openPluginsDialogBox", Qt::CaseInsensitive)) {
        // We want to open the Plugins dialog box

        on_actionPlugins_triggered();
    } else if (!authority.compare("openAboutDialogBox", Qt::CaseInsensitive)) {
        // We want to open the About dialog box

        on_actionAbout_triggered();
    } else if (!authority.compare("openFile", Qt::CaseInsensitive)) {
        // We want to open a file, so handle it as an argument that is passed to
        // OpenCOR
        // Note: the file name is contained in the path of the URL minus the
        //       leading forward slash. Indeed, an open file request will look
        //       something like gui://openFiles//home/user/file, so...

        handleArguments(pUrl.path().remove(0, 1));
    } else if (!authority.compare("openFiles", Qt::CaseInsensitive)) {
        // We want to open some files, so handle them as a series of arguments
        // that were passed to OpenCOR
        // Note: the file names are contained in the path of the URL minus the
        //       leading forward slash. Indeed, an open files request  will look
        //       something like
        //           gui://openFiles//home/user/file1|/home/user/file2
        //       so...

        handleArguments(pUrl.path().remove(0, 1));
    } else {
        // We are dealing with an action that OpenCOR itself can't handle, but
        // maybe one of its loaded plugins can

        QString host = pUrl.host();

        foreach (Plugin *plugin, mLoadedPluginPlugins)
            if (!plugin->name().toLower().compare(host)) {
                // This is an action for the current plugin, so forward the
                // action to it, should it support the Plugin interface

                qobject_cast<PluginInterface *>(plugin->instance())->handleAction(pUrl);

                break;
            }
    }
}

//==============================================================================

void MainWindow::messageReceived(const QString &pMessage, QObject *pSocket)
{
    Q_UNUSED(pSocket);

    // We have just received a message, which can be one of two things:
    //  1) The user tried to run another instance of OpenCOR, which sent a
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
    // Note: some black magic is needed for OS X. Indeed, on that platform,
    //       OpenCOR can be switched to / back from full screen mode either
    //       through its menu or using its full screen mode button (located in
    //       the top right of its title bar). If we use only method, then to
    //       simply use showFullScreen() and showNormal() is fine, but if for
    //       some reasons the user decides to mix both methods, then our black
    //       magic is needed...

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
    if (mPluginManager->plugins().count()) {
        // There are some plugins, so we can show the plugins window

        PluginsWindow pluginsWindow(mPluginManager, this);

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

void MainWindow::on_actionPreferences_triggered()
{
    // Show the preferences window

    PreferencesWindow preferencesWindow(this);

    preferencesWindow.exec();
}

//==============================================================================

static const auto OpencorHomePageUrl = QStringLiteral("http://www.opencor.ws/");

//==============================================================================

void MainWindow::on_actionHomePage_triggered()
{
    // Look up OpenCOR's home page

    QDesktopServices::openUrl(OpencorHomePageUrl);
}

//==============================================================================

void MainWindow::on_actionAbout_triggered()
{
    // Display some information about OpenCOR

    QMessageBox::about(this, tr("About"),
                        "<h1 align=center><strong>"+version(qApp)+"</strong></h1>"
                       +"<h3 align=center><em>"+Core::osName()+"</em></h3>"
                       +"<p align=center><em>"+Core::copyright()+"</em></p>"
                       +"<a href=\""+QString(OpencorHomePageUrl)+"\"><strong>"+qApp->applicationName()+"</strong></a> "+tr("is a cross-platform modelling environment, which can be used to organise, edit, simulate and analyse <a href=\"http://www.cellml.org/\">CellML</a> files."));
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

    qApp->exit(pSaveSettings?NormalRestart:CleanRestart);
}

//==============================================================================

void MainWindow::showEnableActions(const QList<QAction *> &pActions)
{
    // Show/enable or hide/disable the given actions, depending on whether they
    // correspond to a menu with visible/enabled or hidden/disabled actions,
    // respectively

    foreach (QAction *action, pActions) {
        QMenu *actionMenu = action->menu();

        if (actionMenu) {
            QList<QAction *> actionMenuActions = actionMenu->actions();

            showEnableActions(actionMenuActions);

            bool showEnable = false;

            foreach (QAction *actionMenuAction, actionMenuActions)
                if (   !actionMenuAction->isSeparator()
                    &&  actionMenuAction->isVisible()) {
                    showEnable = true;

                    break;
                }

            Core::showEnableAction(action, showEnable);
        }
    }
}

//==============================================================================

void MainWindow::updateGui(Plugin *pViewPlugin, const QString &pFileName)
{
    // We come here as a result of our central widget having updated its GUI,
    // meaning that a new view or file has been selected, so we may need to
    // enable/disable and/or show/hide some menus/actions/etc.

    // Keep track of our view plugin

    mViewPlugin = pViewPlugin;

    // Let our different plugins know that the GUI has been updated
    // Note: this can be useful when a plugin (e.g. CellMLTools) offers some
    //       tools that may need to be enabled/disabled and shown/hidden,
    //       depending on which view plugin and/or file are currently active...

    foreach (Plugin *plugin, mLoadedGuiPlugins)
        qobject_cast<GuiInterface *>(plugin->instance())->updateGui(mViewPlugin, pFileName);

    // Go through our different menus and show/hide them, depending on whether
    // they have visible items

    showEnableActions(mGui->menuBar->actions());
}

//==============================================================================

void MainWindow::showDockedWindows(const bool &pShow,
                                   const bool &pInitialisation)
{
    // Show/hide the docked windows

    if (!pInitialisation) {
        if (!pShow)
            mDockedWindowsState = saveState();

        foreach (Plugin *plugin, mLoadedWindowPlugins) {
            WindowInterface *windowInterface = qobject_cast<WindowInterface *>(plugin->instance());

            if (!windowInterface->windowWidget()->isFloating())
                windowInterface->windowWidget()->setVisible(pShow);
        }

        if (pShow && !mDockedWindowsState.isEmpty())
            restoreState(mDockedWindowsState);
    }

    // Keep track of the docked windows visible state

    mDockedWindowsVisible = pShow;

    // Update the checked state of our docked windows action

    mGui->actionDockedWindows->setChecked(pShow);
}

//==============================================================================

void MainWindow::updateDockWidgetsVisibility()
{
    // Check whether at least one dockable window is visible

    mDockedWindowsVisible = false;

    foreach (Plugin *plugin, mLoadedWindowPlugins) {
        QDockWidget *dockWidget = qobject_cast<WindowInterface *>(plugin->instance())->windowWidget();

        if (!dockWidget->isFloating() && dockWidget->isVisible()) {
            mDockedWindowsVisible = true;

            break;
        }
    }

    // Update the checked state of our docked windows action

    mGui->actionDockedWindows->setChecked(mDockedWindowsVisible);
}

//==============================================================================

void MainWindow::resetAll()
{
    if (QMessageBox::question(this, qApp->applicationName(),
                              tr("You are about to reset <strong>all</strong> of your settings. Do you wish to proceed?"),
                              QMessageBox::Yes|QMessageBox::No,
                              QMessageBox::Yes) == QMessageBox::Yes ) {
        // Restart OpenCOR without first saving its settings

        restart(false);
    }
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
