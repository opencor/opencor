//==============================================================================
// GUI interface
//==============================================================================

#include "centralwidget.h"
#include "dockwidget.h"
#include "guiinterface.h"
#include "widget.h"

//==============================================================================

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QToolBar>

//==============================================================================

namespace OpenCOR {

//==============================================================================

GuiMenuSettings::GuiMenuSettings(const GuiMenuSettingsType &pType,
                                 QMenu *pMenu) :
    mType(pType),
    mMenu(pMenu)
{
}

//==============================================================================

GuiMenuSettings::GuiMenuSettingsType GuiMenuSettings::type() const
{
    // Return the menu's type

    return mType;
}

//==============================================================================

QMenu * GuiMenuSettings::menu() const
{
    // Return the menu itsef

    return mMenu;
}

//==============================================================================

GuiMenuActionSettings::GuiMenuActionSettings(const GuiMenuActionSettingsType &pType,
                                             QAction *pAction) :
    mType(pType),
    mAction(pAction)
{
}

//==============================================================================

GuiMenuActionSettings::GuiMenuActionSettingsType GuiMenuActionSettings::type() const
{
    // Return the action's type

    return mType;
}

//==============================================================================

QAction * GuiMenuActionSettings::action() const
{
    // Return the action itself

    return mAction;
}

//==============================================================================

GuiViewSettings::GuiViewSettings(const Mode &pMode, const int &pIndex) :
    mMode(pMode),
    mIndex(pIndex)
{
}

//==============================================================================

GuiViewSettings::Mode GuiViewSettings::mode() const
{
    // Return the view's mode

    return mMode;
}

//==============================================================================

int GuiViewSettings::index() const
{
    // Return the view's index

    return mIndex;
}

//==============================================================================

GuiWindowSettings::GuiWindowSettings(const Qt::DockWidgetArea &pDefaultDockingArea,
                                     Core::DockWidget *pWindow,
                                     const GuiWindowSettingsType &pType,
                                     QAction *pAction) :
    mDefaultDockingArea(pDefaultDockingArea),
    mWindow(pWindow),
    mType(pType),
    mAction(pAction)
{
}

//==============================================================================

Qt::DockWidgetArea GuiWindowSettings::defaultDockingArea() const
{
    // Return the window's default docking area

    return mDefaultDockingArea;
}

//==============================================================================

Core::DockWidget * GuiWindowSettings::window() const
{
    // Return the window itself

    return mWindow;
}

//==============================================================================

GuiWindowSettings::GuiWindowSettingsType GuiWindowSettings::type() const
{
    // Return the action's type

    return mType;
}

//==============================================================================

QAction * GuiWindowSettings::action() const
{
    // Return the action itself

    return mAction;
}

//==============================================================================

GuiSettings::GuiSettings() :
    mCentralWidget(0)
{
}

//==============================================================================

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

//==============================================================================

void GuiSettings::addMenu(const GuiMenuSettings::GuiMenuSettingsType &pType,
                          QMenu *pMenu)
{
    // Add a menu to our list

    mMenus << new GuiMenuSettings(pType, pMenu);
}

//==============================================================================

void GuiSettings::addMenuAction(const GuiMenuActionSettings::GuiMenuActionSettingsType &pType,
                                QAction *pAction)
{
    // Add a menu action to our list
    // Note: a null pAction means that we want to add a separator

    mMenuActions << new GuiMenuActionSettings(pType, pAction);
}

//==============================================================================

void GuiSettings::addCentralWidget(Core::CentralWidget *pCentralWidget)
{
    // Set the central widget to be used

    mCentralWidget = pCentralWidget;
}

//==============================================================================

void GuiSettings::addView(const GuiViewSettings::Mode &pMode, const int &pIndex)
{
    // Add a new view to our list

    mViews << new GuiViewSettings(pMode, pIndex);
}

//==============================================================================

void GuiSettings::addWindow(const Qt::DockWidgetArea &pDefaultDockingArea,
                            Core::DockWidget *pWindow,
                            const GuiWindowSettings::GuiWindowSettingsType &pType,
                            QAction *pAction)
{
    // Add a window to our list

    mWindows << new GuiWindowSettings(pDefaultDockingArea, pWindow, pType,
                                      pAction);
}

//==============================================================================

QList<GuiMenuSettings *> GuiSettings::menus() const
{
    // Return our menus

    return mMenus;
}

//==============================================================================

QList<GuiMenuActionSettings *> GuiSettings::menuActions() const
{
    // Return our menu actions

    return mMenuActions;
}

//==============================================================================

Core::CentralWidget * GuiSettings::centralWidget() const
{
    // Return the central widget

    return mCentralWidget;
}

//==============================================================================

QList<GuiViewSettings *> GuiSettings::views() const
{
    // Return our views

    return mViews;
}

//==============================================================================

QList<GuiWindowSettings *> GuiSettings::windows() const
{
    // Return our windows

    return mWindows;
}

//==============================================================================

GuiInterface::GuiInterface()
{
    // Create our GUI settings object

    mGuiSettings = new GuiSettings;
}

//==============================================================================

GuiInterface::~GuiInterface()
{
    // Delete our GUI settings object

    delete mGuiSettings;

    // Delete our lists of view widgets

    foreach (GuiViewWidgets *viewWidgets, mViewWidgets)
        delete viewWidgets;
}

//==============================================================================

void GuiInterface::loadWindowSettings(QSettings *pSettings,
                                      Core::DockWidget *pWindow)
{
    // Retrieve the window's settings

    pSettings->beginGroup(pWindow->objectName());
        pWindow->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void GuiInterface::loadViewSettings(QSettings *pSettings, Core::Widget *pView)
{
    // Retrieve the view's settings

    pSettings->beginGroup(pView->objectName());
        pView->loadSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void GuiInterface::loadSettings(QSettings *)
{
    // Nothing to do by default...
}

//==============================================================================

void GuiInterface::saveWindowSettings(QSettings *pSettings,
                                      Core::DockWidget *pWindow) const
{
    // Keep track of the window's settings

    pSettings->beginGroup(pWindow->objectName());
        pWindow->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void GuiInterface::saveViewSettings(QSettings *pSettings,
                                    Core::Widget *pView) const
{
    // Keep track of the view's settings

    pSettings->beginGroup(pView->objectName());
        pView->saveSettings(pSettings);
    pSettings->endGroup();
}

//==============================================================================

void GuiInterface::saveSettings(QSettings *) const
{
    // Nothing to do by default...
}

//==============================================================================

void GuiInterface::loadingOfSettingsDone(const Plugins &)
{
    // Nothing to do by default...
}

//==============================================================================

QWidget * GuiInterface::viewWidget(const QString &pFileName,
                                   const int &pViewIndex)
{
    // Return the view widget associated with the given file name

    // Retrieve the list of view widgets associated with the view index

    GuiViewWidgets *viewWidgets = mViewWidgets.value(pViewIndex);

    if (!viewWidgets) {
        // There is no list of view widgets associated with the view index, so
        // create one and keep track of it

        viewWidgets = new GuiViewWidgets;

        mViewWidgets.insert(pViewIndex, viewWidgets);
    }

    // Retrieve, from our list of view widgets, the view widget associated with
    // the file name

    QWidget *res = viewWidgets->value(pFileName);

    // Check whether we got an empty view widget or not, and if so then create a
    // view widget for the file and keep track of it

    if (!res) {
        res = newViewWidget(pFileName);

        if (res)
            // Only keep track of the view widget if it is a real one

            viewWidgets->insert(pFileName, res);
    }

    // Return the view widget

    return res;
}

//==============================================================================

QWidget * GuiInterface::newViewWidget(const QString &)
{
    // Create and return no widget by default...

    return 0;
}

//==============================================================================

QString GuiInterface::viewName(const int &)
{
    // Return an empty string by default...

    return QString();
}

//==============================================================================

GuiSettings * GuiInterface::guiSettings() const
{
    // Return the plugin's GUI settings

    return mGuiSettings;
}

//==============================================================================

void GuiInterface::setMainWindow(QMainWindow *pMainWindow)
{
    // Set the main window

    mMainWindow = pMainWindow;
}

//==============================================================================

QMenu * GuiInterface::newMenu(QMainWindow *pMainWindow, const QString &pName)
{
    // Create a new menu

    QMenu *res = new QMenu(pMainWindow);

    // Set the name of the menu

    res->setObjectName("menu"+pName.left(1).toUpper()+pName.right(pName.size()-1));
    // Note #1: the object name must also be set for the main window to be able
    //          to reconcile different menus with the same name (and therefore
    //          merge their contents, if needed)
    // Note #2: the naming is such to respect the one used in the main window

    // Return the new menu

    return res;
}

//==============================================================================

QToolBar * GuiInterface::newToolBar(QMainWindow *pMainWindow,
                                    const QString &pName)
{
    // Create a new toolbar

    QToolBar *res = new QToolBar(pMainWindow);

    // Set the name of the toolbar

    res->setObjectName(pName.left(1).toLower()+pName.right(pName.size()-1)+"Toolbar");
    // Note #1: the object name must be set for QMainWindow::saveState() to work
    //          properly...
    // Note #2: the object name must also be set for the main window to be able
    //          to reconcile different toolbars with the same name (and
    //          therefore merge their contents, if needed)
    // Note #3: the naming is such to respect the one used in the main window

    // Set the icon size to that used in the Help toolbar

    res->setIconSize(QSize(24, 24));

    // Return the new toolbar

    return res;
}

//==============================================================================

QAction * GuiInterface::newAction(QMainWindow *pMainWindow,
                                  const bool &pCheckable,
                                  const QString &pIconResource,
                                  const QList<QKeySequence> &pKeySequences,
                                  const bool &pVisible)
{
    // Create and return an action

    QAction *res = new QAction(pMainWindow);

    res->setCheckable(pCheckable);
    res->setIcon(QIcon(pIconResource));
    res->setShortcuts(pKeySequences);
    res->setVisible(pVisible);

    return res;
}

//==============================================================================

QAction * GuiInterface::newAction(QMainWindow *pMainWindow,
                                  const bool &pCheckable,
                                  const QString &pIconResource,
                                  const QKeySequence::StandardKey &pStandardKey,
                                  const bool &pVisible)
{
    // Create and return an action

    return newAction(pMainWindow, pCheckable, pIconResource,
                     QKeySequence::keyBindings(pStandardKey), pVisible);
}

//==============================================================================

QAction * GuiInterface::newAction(QMainWindow *pMainWindow,
                                  const bool &pCheckable,
                                  const QString &pIconResource,
                                  const QKeySequence &pKeySequence,
                                  const bool &pVisible)
{
    // Create and return an action

    return newAction(pMainWindow, pCheckable, pIconResource,
                     QList<QKeySequence>() << pKeySequence, pVisible);
}

//==============================================================================

void GuiInterface::connectToolBarToAction(QToolBar *pToolbar, QAction *pAction)
{
    // Setup the action for the toolbar, so we can show/hide it at will

    QObject::connect(pAction, SIGNAL(triggered(bool)),
                     pToolbar, SLOT(setVisible(bool)));
    QObject::connect(pToolbar->toggleViewAction(), SIGNAL(toggled(bool)),
                     pAction, SLOT(setChecked(bool)));
}

//==============================================================================

void GuiInterface::connectDockWidgetToAction(QDockWidget *pDockWidget,
                                             QAction *pAction)
{
    // Setup the action for the dock widget, so we can show/hide it at will

    QObject::connect(pAction, SIGNAL(triggered(bool)),
                     pDockWidget, SLOT(setVisible(bool)));
    QObject::connect(pDockWidget->toggleViewAction(), SIGNAL(toggled(bool)),
                     pAction, SLOT(setChecked(bool)));
}

//==============================================================================

void GuiInterface::retranslateMenu(QMenu *pMenu, const QString &pTitle)
{
    // Retranslate the menu, i.e. retranslate its title

    pMenu->setTitle(pTitle);
}

//==============================================================================

void GuiInterface::retranslateAction(QAction *pAction, const QString &pText,
                                     const QString &pStatusTip)
{
    // Retranslate the action, i.e. retranslate its text and status tip

    pAction->setText(pText);
    pAction->setStatusTip(pStatusTip);
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
