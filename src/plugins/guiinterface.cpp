#include "guiinterface.h"

#include <QAction>
#include <QApplication>
#include <QMenu>

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

GuiActionSettings::GuiActionSettings(const GuiActionSettingsType &pType,
                                     QAction *pAction) :
    mType(pType),
    mAction(pAction)
{
}

GuiActionSettings::GuiActionSettingsType GuiActionSettings::type() const
{
    // Return the action's type

    return mType;
}

QAction * GuiActionSettings::action() const
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

    foreach (GuiActionSettings *actionSettings, mActions)
        delete actionSettings;

    foreach (GuiViewSettings *viewSettings, mViews)
        delete viewSettings;

    foreach (GuiWindowSettings *windowSettings, mWindows)
        delete windowSettings;
}

void GuiSettings::addMenu(const GuiMenuSettings::GuiMenuSettingsType &pType,
                          QMenu *pMenu)
{
    // Add a new menu to our list

    mMenus.prepend(new GuiMenuSettings(pType, pMenu));
}

void GuiSettings::addAction(const GuiActionSettings::GuiActionSettingsType &pType,
                            QAction *pAction)
{
    // Add a new action to our list
    // Note: a null pAction means that we want to add a separator

    mActions.prepend(new GuiActionSettings(pType, pAction));
}

void GuiSettings::addView(const GuiViewSettings::Mode &pMode)
{
    // Add a new view to our list

    mViews.prepend(new GuiViewSettings(pMode));
}

void GuiSettings::addWindow(const Qt::DockWidgetArea &pDefaultDockingArea,
                            Core::DockWidget *pWindow)
{
    // Add a new window to our list

    mWindows.prepend(new GuiWindowSettings(pDefaultDockingArea, pWindow));
}

QList<GuiMenuSettings *> GuiSettings::menus() const
{
    // Return our menus

    return mMenus;
}

QList<GuiActionSettings *> GuiSettings::actions() const
{
    // Return our actions

    return mActions;
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
    // Create our settings object

    mSettings = new GuiSettings;
}

GuiInterface::~GuiInterface()
{
    // Delete our settings object

    delete mSettings;
}

void GuiInterface::initialize(const QList<Plugin *> &, QMainWindow *)
{
    // Nothing to do by default...
}

GuiSettings * GuiInterface::settings() const
{
    // Return the plugin's settings

    return mSettings;
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
    // Update the plugin's translator

    qApp->removeTranslator(&mTranslator);
    mTranslator.load(QString(":%1_%2").arg(mPluginName, pLocale));
    qApp->installTranslator(&mTranslator);

    // Retranslate the plugin

    retranslateUi();
}

QMenu * GuiInterface::newMenu(QMainWindow *pMainWindow)
{
    // Create and return a menu

    return new QMenu(pMainWindow);
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
