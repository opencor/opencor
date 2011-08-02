#include "guiinterface.h"

#include <QAction>
#include <QApplication>
#include <QMenu>

namespace OpenCOR {

GuiSettingsCorePlugin::GuiSettingsCorePlugin(Core::CentralWidget *pCentralWidget) :
    mCentralWidget(pCentralWidget)
{
}

Core::CentralWidget * GuiSettingsCorePlugin::centralWidget() const
{
    // Return the central widget

    return mCentralWidget;
}

GuiSettingsHelpPlugin::GuiSettingsHelpPlugin(QAction *pHelpAction,
                                             Core::DockWidget *pHelpWindow) :
    mHelpAction(pHelpAction),
    mHelpWindow(pHelpWindow)
{
}

QAction * GuiSettingsHelpPlugin::helpAction() const
{
    // Return the help action

    return mHelpAction;
}

Core::DockWidget * GuiSettingsHelpPlugin::helpWindow() const
{
    // Return the help window

    return mHelpWindow;
}

GuiSettingsMenu::GuiSettingsMenu(const GuiSettingsMenuType &pType,
                                 QMenu *pMenu) :
    mType(pType),
    mMenu(pMenu)
{
}

GuiSettingsMenu::GuiSettingsMenuType GuiSettingsMenu::type() const
{
    // Return the menu's type

    return mType;
}

QMenu * GuiSettingsMenu::menu() const
{
    // Return the menu itsef

    return mMenu;
}

GuiSettingsAction::GuiSettingsAction(const GuiSettingsActionType &pType,
                                     QAction *pAction) :
    mType(pType),
    mAction(pAction)
{
}

GuiSettingsAction::GuiSettingsActionType GuiSettingsAction::type() const
{
    // Return the action's type

    return mType;
}

QAction * GuiSettingsAction::action() const
{
    // Return the action itself

    return mAction;
}

GuiSettingsWindow::GuiSettingsWindow(const Qt::DockWidgetArea &pDefaultDockingArea,
                                     Core::DockWidget *pWindow) :
    mDefaultDockingArea(pDefaultDockingArea),
    mWindow(pWindow)
{
}

Qt::DockWidgetArea GuiSettingsWindow::defaultDockingArea() const
{
    // Return the window's default docking area

    return mDefaultDockingArea;
}

Core::DockWidget * GuiSettingsWindow::window() const
{
    // Return the window itself

    return mWindow;
}

void GuiSettings::addMenu(const GuiSettingsMenu::GuiSettingsMenuType &pType,
                          QMenu *pMenu)
{
    // Add a new menu to our list

    mMenus.prepend(GuiSettingsMenu(pType, pMenu));
}

void GuiSettings::addAction(const GuiSettingsAction::GuiSettingsActionType &pType,
                            QAction *pAction)
{
    // Add a new action to our list
    // Note: a null pAction means that we want to add a separator

    mActions.prepend(GuiSettingsAction(pType, pAction));
}

void GuiSettings::addWindow(const Qt::DockWidgetArea &pDefaultDockingArea,
                            Core::DockWidget *pWindow)
{
    // Add a new dock widget to our list

    mWindows.prepend(GuiSettingsWindow(pDefaultDockingArea, pWindow));
}

QList<GuiSettingsMenu> GuiSettings::menus() const
{
    // Return our menus

    return mMenus;
}

QList<GuiSettingsAction> GuiSettings::actions() const
{
    // Return our actions

    return mActions;
}

QList<GuiSettingsWindow> GuiSettings::windows() const
{
    // Return our windows

    return mWindows;
}

GuiInterface::GuiInterface(const QString &pPluginName) :
    mData(0),
    mPluginName(pPluginName)
{
}

void GuiInterface::initialize(QMainWindow *)
{
    // Nothing to do by default...
}

GuiSettings GuiInterface::settings() const
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
                                  const QString &pIconResource)
{
    // Create and return an action

    QAction *res = new QAction(pMainWindow);

    res->setCheckable(pCheckable);
    res->setIcon(QIcon(pIconResource));

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
