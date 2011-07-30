#include "guiinterface.h"

#include <QAction>
#include <QApplication>
#include <QMenu>

namespace OpenCOR {

GuiSettingsCoreMenu::GuiSettingsCoreMenu(const GuiSettingsCoreMenuType &pType,
                                         QMenu *pMenu) :
    mType(pType),
    mMenu(pMenu)
{
}

GuiSettingsCoreMenu::GuiSettingsCoreMenuType GuiSettingsCoreMenu::type() const
{
    // Return the menu's type

    return mType;
}

QMenu * GuiSettingsCoreMenu::menu() const
{
    // Return the menu itsef

    return mMenu;
}

GuiSettingsCoreAction::GuiSettingsCoreAction(const GuiSettingsCoreActionType &pType,
                                             QAction *pAction) :
    mType(pType),
    mAction(pAction)
{
}

GuiSettingsCoreAction::GuiSettingsCoreActionType GuiSettingsCoreAction::type() const
{
    // Return the action's type

    return mType;
}

QAction * GuiSettingsCoreAction::action() const
{
    // Return the action itself

    return mAction;
}

GuiSettingsCorePlugin::GuiSettingsCorePlugin(Core::CentralWidget *pCentralWidget) :
    mCentralWidget(pCentralWidget)
{
}

void GuiSettingsCorePlugin::addMenu(const GuiSettingsCoreMenu::GuiSettingsCoreMenuType &pType,
                                    QMenu *pMenu)
{
    // Add a new menu to our list

    mMenus << GuiSettingsCoreMenu(pType, pMenu);
}

void GuiSettingsCorePlugin::addAction(const GuiSettingsCoreAction::GuiSettingsCoreActionType &pType,
                                      QAction *pAction)
{
    // Add a new action to our list

    mActions << GuiSettingsCoreAction(pType, pAction);
}

QList<GuiSettingsCoreMenu> GuiSettingsCorePlugin::menus() const
{
    // Return our menus

    return mMenus;
}

QList<GuiSettingsCoreAction> GuiSettingsCorePlugin::actions() const
{
    // Return our actions

    return mActions;
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

GuiSettingsCentralWidget::GuiSettingsCentralWidget(Core::CentralWidget *pCentralWidget) :
    mCentralWidget(pCentralWidget)
{
}

Core::CentralWidget * GuiSettingsCentralWidget::centralWidget() const
{
    // Return the central widget

    return mCentralWidget;
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

void GuiSettings::addCentralWidget(Core::CentralWidget *pCentralWidget)
{
    // Add the central widget
    // Note: we can have only one central widget, so...

    if (mCentralWidget.isEmpty())
        mCentralWidget << GuiSettingsCentralWidget(pCentralWidget);
}

void GuiSettings::addAction(const GuiSettingsAction::GuiSettingsActionType &pType,
                            QAction *pAction)
{
    // Add a new action to our list

    mActions << GuiSettingsAction(pType, pAction);
}

void GuiSettings::addWindow(const Qt::DockWidgetArea &pDefaultDockingArea,
                            Core::DockWidget *pWindow)
{
    // Add a new dock widget to our list

    mWindows << GuiSettingsWindow(pDefaultDockingArea, pWindow);
}

QList<GuiSettingsCentralWidget> GuiSettings::centralWidget() const
{
    // Return our central widget

    return mCentralWidget;
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
