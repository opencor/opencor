//==============================================================================
// GUI interface
//==============================================================================

#include "centralwidget.h"
#include "dockwidget.h"
#include "guiinterface.h"

//==============================================================================

#include <QAction>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
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

GuiViewSettings::GuiViewSettings(const Mode &pMode) :
    mMode(pMode)
{
}

//==============================================================================

GuiViewSettings::Mode GuiViewSettings::mode() const
{
    // Return the view's mode

    return mMode;
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
    mMenus(QList<GuiMenuSettings *>()),
    mMenuActions(QList<GuiMenuActionSettings *>()),
    mCentralWidget(0),
    mView(0),
    mWindows(QList<GuiWindowSettings *>())
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

    delete mView;

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

void GuiSettings::setCentralWidget(Core::CentralWidget *pCentralWidget)
{
    // Set the central widget to be used

    mCentralWidget = pCentralWidget;
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

void GuiSettings::setView(const GuiViewSettings::Mode &pMode)
{
    // Add and set the view

    if (mView)
        // There is already a view, so delete it

        delete mView;

    mView = new GuiViewSettings(pMode);
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

QList<GuiWindowSettings *> GuiSettings::windows() const
{
    // Return our windows

    return mWindows;
}

//==============================================================================

GuiViewSettings * GuiSettings::view() const
{
    // Return our view

    return mView;
}

//==============================================================================

GuiInterface::GuiInterface() :
    mMainWindow(0),
    mModeViewWidgets(GuiViewWidgets())
{
    // Create our GUI settings object

    mGuiSettings = new GuiSettings();
}

//==============================================================================

void GuiInterface::destroy()
{
    // Delete our GUI settings object

    delete mGuiSettings;

    // Delete our view widgets

    foreach (QWidget *viewWidget, mModeViewWidgets)
        delete viewWidget;
}

//==============================================================================

QWidget * GuiInterface::viewWidget(const QString &pFileName)
{
    // Return the view widget associated with the given file name

    // Retrieve, from our list of view widgets, the view widget associated with
    // the file name

    QWidget *res = mModeViewWidgets.value(pFileName);

    // Check whether we got an empty view widget or not, and if so then create a
    // view widget for the file and keep track of it

    if (!res) {
        res = newViewWidget(pFileName);

        // Only keep track of the view widget if it is a real one

        if (res)
            mModeViewWidgets.insert(pFileName, res);
    }

    // Return the view widget

    return res;
}

//==============================================================================

QWidget * GuiInterface::newViewWidget(const QString &pFileName)
{
    // Create and return no widget by default...

    Q_UNUSED(pFileName);

    return 0;
}

//==============================================================================

QString GuiInterface::viewName()
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
