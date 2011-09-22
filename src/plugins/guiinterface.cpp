#include "centralwidget.h"
#include "dockwidget.h"
#include "guiinterface.h"

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QToolBar>

namespace OpenCOR {

GuiCoreSettings::GuiCoreSettings(Core::CentralWidget *pCentralWidget) :
    mCentralWidget(pCentralWidget)
{
}

Core::CentralWidget * GuiCoreSettings::centralWidget() const
{
    // Return the central widget

    return mCentralWidget;
}

GuiHelpSettings::GuiHelpSettings(QAction *pHelpAction,
                                 Core::DockWidget *pHelpWindow) :
    mHelpAction(pHelpAction),
    mHelpWindow(pHelpWindow)
{
}

QAction * GuiHelpSettings::helpAction() const
{
    // Return the help action

    return mHelpAction;
}

Core::DockWidget * GuiHelpSettings::helpWindow() const
{
    // Return the help window

    return mHelpWindow;
}

GuiMenuSettings::GuiMenuSettings(const GuiMenuSettingsType &pType,
                                 QMenu *pMenu) :
    mType(pType),
    mMenu(pMenu)
{
}

GuiMenuSettings::GuiMenuSettingsType GuiMenuSettings::type() const
{
    // Return the menu's type

    return mType;
}

QMenu * GuiMenuSettings::menu() const
{
    // Return the menu itsef

    return mMenu;
}

GuiMenuActionSettings::GuiMenuActionSettings(const GuiMenuActionSettingsType &pType,
                                             QAction *pAction) :
    mType(pType),
    mAction(pAction)
{
}

GuiMenuActionSettings::GuiMenuActionSettingsType GuiMenuActionSettings::type() const
{
    // Return the action's type

    return mType;
}

QAction * GuiMenuActionSettings::action() const
{
    // Return the action itself

    return mAction;
}

GuiViewSettings::GuiViewSettings(const Mode &pMode) :
    mMode(pMode),
    mTabBar(0),
    mTabIndex(-1)
{
}

GuiViewSettings::Mode GuiViewSettings::mode() const
{
    // Return the view's mode

    return mMode;
}

void GuiViewSettings::setTabBar(QTabBar *pTabBar)
{
    // Set the view's tab bar

    mTabBar = pTabBar;
}

void GuiViewSettings::setTabIndex(const int &pTabIndex)
{
    // Set the view's tab index

    mTabIndex = pTabIndex;
}

QTabBar * GuiViewSettings::tabBar() const
{
    // Return the view's tab bar

    return mTabBar;
}

int GuiViewSettings::tabIndex() const
{
    // Return the view's tab index

    return mTabIndex;
}

GuiWindowSettings::GuiWindowSettings(const Qt::DockWidgetArea &pDefaultDockingArea,
                                     Core::DockWidget *pWindow) :
    mDefaultDockingArea(pDefaultDockingArea),
    mWindow(pWindow)
{
}

Qt::DockWidgetArea GuiWindowSettings::defaultDockingArea() const
{
    // Return the window's default docking area

    return mDefaultDockingArea;
}

Core::DockWidget * GuiWindowSettings::window() const
{
    // Return the window itself

    return mWindow;
}

GuiSettings::~GuiSettings()
{
    // Delete the contents of our various lists

    foreach (GuiMenuSettings *menuSettings, mMenus)
        delete menuSettings;

    foreach (GuiMenuActionSettings *menuActionSettings, mMenuActions)
        delete menuActionSettings;

    foreach (GuiViewSettings *viewSettings, mViews)
        delete viewSettings;

    foreach (GuiWindowSettings *windowSettings, mWindows)
        delete windowSettings;
}

void GuiSettings::addMenu(const GuiMenuSettings::GuiMenuSettingsType &pType,
                          QMenu *pMenu)
{
    // Add a new menu to our list

    mMenus << new GuiMenuSettings(pType, pMenu);
}

void GuiSettings::addMenuAction(const GuiMenuActionSettings::GuiMenuActionSettingsType &pType,
                                QAction *pAction)
{
    // Add a new menu action to our list
    // Note: a null pAction means that we want to add a separator

    mMenuActions << new GuiMenuActionSettings(pType, pAction);
}

void GuiSettings::addView(const GuiViewSettings::Mode &pMode)
{
    // Add a new view to our list

    mViews << new GuiViewSettings(pMode);
}

void GuiSettings::addWindow(const Qt::DockWidgetArea &pDefaultDockingArea,
                            Core::DockWidget *pWindow)
{
    // Add a new window to our list

    mWindows << new GuiWindowSettings(pDefaultDockingArea, pWindow);
}

QList<GuiMenuSettings *> GuiSettings::menus() const
{
    // Return our menus

    return mMenus;
}

QList<GuiMenuActionSettings *> GuiSettings::menuActions() const
{
    // Return our menu actions

    return mMenuActions;
}

QList<GuiViewSettings *> GuiSettings::views() const
{
    // Return our views

    return mViews;
}

QList<GuiWindowSettings *> GuiSettings::windows() const
{
    // Return our windows

    return mWindows;
}

GuiInterface::GuiInterface(const QString &pPluginName) :
    mData(0),
    mPluginName(pPluginName)
{
    // Create our GUI settings object

    mGuiSettings = new GuiSettings;
}

GuiInterface::~GuiInterface()
{
    // Delete our GUI settings object

    delete mGuiSettings;
}

void GuiInterface::loadWindowSettings(QSettings *pSettings,
                                      const bool &pNeedDefaultSettings,
                                      const Qt::DockWidgetArea &pDefaultDockingArea,
                                      Core::DockWidget *pWindow)
{
    if (pNeedDefaultSettings) {
        // Hide the window, so that we can set things up without having the
        // screen flashing

        pWindow->setVisible(false);

        // Position the window to its default location

        mMainWindow->addDockWidget(pDefaultDockingArea, pWindow);
    }

    // Load the window's settings

    pSettings->beginGroup(pWindow->objectName());
        pWindow->loadSettings(pSettings);
    pSettings->endGroup();

    if (pNeedDefaultSettings)
        // Make the window visible

        pWindow->setVisible(true);
}

void GuiInterface::loadSettings(QSettings *pSettings,
                                const bool &pNeedDefaultSettings)
{
    if (!mPluginName.compare(CorePlugin)) {
        // We are dealing with our special Core plugin

        if (mData) {
            // Our special Core plugin has its data set, so retrieve its central
            // widget's settings

            Core::CentralWidget *centralWidget = ((GuiCoreSettings *) mData)->centralWidget();

            // Load the central widget's settings

            pSettings->beginGroup(centralWidget->objectName());
                centralWidget->loadSettings(pSettings);
            pSettings->endGroup();

            // Set the central widget

            mMainWindow->setCentralWidget(centralWidget);
        }
    } else if (!mPluginName.compare(HelpPlugin)) {
        // We are dealing with our special Help plugin

        if (mData)
            // Our special Help plugin has its data set, so retrieve its
            // window's settings

            loadWindowSettings(pSettings, pNeedDefaultSettings,
                               Qt::RightDockWidgetArea,
                               ((GuiHelpSettings *) mData)->helpWindow());
    } else {
        // Neither the Core nor the Help plugin, so retrieve all of the plugin's
        // windows' settings

        foreach (GuiWindowSettings *windowSettings,
                 mGuiSettings->windows())
            loadWindowSettings(pSettings, pNeedDefaultSettings,
                               windowSettings->defaultDockingArea(),
                               windowSettings->window());
    }
}

void GuiInterface::saveSettings(QSettings *pSettings) const
{
    if (!mPluginName.compare(CorePlugin)) {
        // We are dealing with our special Core plugin

        if (mData) {
            // Our special Core plugin has its data set, so keep track of its
            // central widget's settings

            Core::CentralWidget *centralWidget = ((GuiCoreSettings *) mData)->centralWidget();

            pSettings->beginGroup(centralWidget->objectName());
                centralWidget->saveSettings(pSettings);
            pSettings->endGroup();
        }
    } else if (!mPluginName.compare(HelpPlugin)) {
        // We are dealing with our special Help plugin

        if (mData) {
            // Our special Help plugin has its data set, so keep track of its
            // window's settings

            Core::DockWidget *helpWindow = ((GuiHelpSettings *) mData)->helpWindow();

            pSettings->beginGroup(helpWindow->objectName());
                helpWindow->saveSettings(pSettings);
            pSettings->endGroup();
        }
    } else {
        // Neither the Core nor the Help plugin, so keep track of all of the
        // plugin's windows' settings

        foreach (GuiWindowSettings *windowSettings,
                 mGuiSettings->windows()) {
            pSettings->beginGroup(windowSettings->window()->objectName());
                windowSettings->window()->saveSettings(pSettings);
            pSettings->endGroup();
        }
    }
}

GuiSettings * GuiInterface::guiSettings() const
{
    // Return the plugin's GUI settings

    return mGuiSettings;
}

void * GuiInterface::data() const
{
    // Return the plugin's data

    return mData;
}

QString GuiInterface::pluginName() const
{
    // Return the plugin's name

    return mPluginName;
}

void GuiInterface::setLocale(const QString &pLocale)
{
    // Keep track of the new locale

    mLocale = pLocale;

    // Update the plugin's translator

    qApp->removeTranslator(&mTranslator);
    mTranslator.load(QString(":%1_%2").arg(mPluginName, pLocale));
    qApp->installTranslator(&mTranslator);

    // Retranslate the plugin

    retranslateUi();
}

void GuiInterface::setParameters(const QList<Plugin *> &pLoadedPlugins,
                                 QMainWindow *pMainWindow)
{
    // Set the loaded plugins

    mLoadedPlugins = pLoadedPlugins;

    // Set the main window

    mMainWindow = pMainWindow;
}

QMenu * GuiInterface::newMenu(QMainWindow *pMainWindow, const QString &pName)
{
    // Return the required menu or a new one, if it doesn't exist

    QMenu * res = OpenCOR::menus.value(pName);

    if (!res) {
        // The menu doesn't already exist, so create a new one and keep track of
        // it

        res = new QMenu(pMainWindow);

        OpenCOR::menus.insert(pName, res);
    }

    // Return the new/found menu

    return res;
}

QToolBar * GuiInterface::newToolBar(QMainWindow *pMainWindow,
                                    const QString &pName)
{
    // Return the required toolbar or a new one, if it doesn't exist

    QToolBar * res = OpenCOR::toolbars.value(pName);

    if (!res) {
        // The toolbar doesn't already exist, so create a new one and keep track
        // of it

        res = new QToolBar(pMainWindow);

        OpenCOR::toolbars.insert(pName, res);
    }

    // Return the new/found menu

    return res;
}

QAction * GuiInterface::newAction(QMainWindow *pMainWindow,
                                  const bool &pCheckable,
                                  const QString &pIconResource,
                                  const bool &pVisible)
{
    // Create and return an action

    QAction *res = new QAction(pMainWindow);

    res->setCheckable(pCheckable);
    res->setIcon(QIcon(pIconResource));
    res->setVisible(pVisible);

    return res;
}

void GuiInterface::retranslateMenu(QMenu *pMenu, const QString &pTitle)
{
    // Retranslate the menu, i.e. retranslate its title

    pMenu->setTitle(pTitle);
}

void GuiInterface::retranslateAction(QAction *pAction, const QString &pText,
                                     const QString &pStatusTip,
                                     const QString &pShortcut)
{
    // Retranslate the action, i.e. retranslate its text, status tip and
    // shortcut

    pAction->setText(pText);
    pAction->setStatusTip(pStatusTip);
    pAction->setShortcut(pShortcut);
}

void GuiInterface::retranslateUi()
{
    // Nothing to do by default...
}

}
