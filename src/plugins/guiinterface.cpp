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
    mAction(0),
    mMenu(pMenu)
{
}

//==============================================================================

GuiMenuSettings::GuiMenuSettings(const GuiMenuSettingsType &pType,
                                 QAction *pAction, QMenu *pMenu) :
    mType(pType),
    mAction(pAction),
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

QAction * GuiMenuSettings::action() const
{
    // Return the menu's action

    return mAction;
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

GuiViewSettings::GuiViewSettings(const Mode &pMode,
                                 const QStringList &pMimeTypes) :
    mMode(pMode),
    mMimeTypes(pMimeTypes)
{
}

//==============================================================================

GuiViewSettings::Mode GuiViewSettings::mode() const
{
    // Return the view's mode

    return mMode;
}

//==============================================================================

QStringList GuiViewSettings::mimeTypes() const
{
    // Return the view's MIME types

    return mMimeTypes;
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

void GuiSettings::addMenu(const GuiMenuSettings::GuiMenuSettingsType &pType,
                          QAction *pAction, QMenu *pMenu)
{
    // Add a menu to our list

    mMenus << new GuiMenuSettings(pType, pAction, pMenu);
}

//==============================================================================

void GuiSettings::addMenuAction(const GuiMenuActionSettings::GuiMenuActionSettingsType &pType,
                                QAction *pAction)
{
    // Add a menu action to our list
    // Note: a null pAction means that we want to add a separator...

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

void GuiSettings::setView(const GuiViewSettings::Mode &pMode,
                          const QStringList &pMimeTypes)
{
    // Add and set the view

    if (mView)
        // There is already a view, so delete it

        delete mView;

    mView = new GuiViewSettings(pMode, pMimeTypes);
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
    mMainWindow(0)
{
    // Create our GUI settings object

    mGuiSettings = new GuiSettings();
}

//==============================================================================

void GuiInterface::destroy()
{
    // Delete our GUI settings object

    delete mGuiSettings;
}

//==============================================================================

QWidget * GuiInterface::viewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // Create and return no view widget by default...

    return 0;
}

//==============================================================================

bool GuiInterface::hasViewWidget(const QString &pFileName) const
{
    Q_UNUSED(pFileName);

    // There is no view widget by default...

    return false;
}

//==============================================================================

void GuiInterface::deleteViewWidget(const QString &pFileName)
{
    Q_UNUSED(pFileName);

    // No view widget to delete by default...
}

//==============================================================================

QString GuiInterface::viewName()
{
    // Return an empty string by default...

    return QString();
}

//==============================================================================

bool GuiInterface::saveFile(const QString &pOldFileName,
                            const QString &pNewFileName)
{
    Q_UNUSED(pOldFileName);
    Q_UNUSED(pNewFileName);

    // The file is not saved by default...

    return false;
}

//==============================================================================

bool GuiInterface::canClose() const
{
    // We can close by default...

    return true;
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

QMenu * GuiInterface::newMenu(QMainWindow *pMainWindow,
                              const QString &pNameOrIconResource,
                              const bool &pProvidedName)
{
    // Create and return a menu

    QMenu *res = new QMenu(pMainWindow);

    if (pProvidedName)
        res->setObjectName("menu"+pNameOrIconResource.left(1).toUpper()+pNameOrIconResource.right(pNameOrIconResource.size()-1));
        // Note #1: the object name must also be set for the main window to be
        //          able to reconcile different menus with the same name (and
        //          therefore merge their contents, if needed)...
        // Note #2: the naming is such to respect the one used in the main
        //          window...
    else
        res->menuAction()->setIcon(QIcon(pNameOrIconResource));

    return res;
}

//==============================================================================

QAction * GuiInterface::newAction(QMainWindow *pMainWindow,
                                  const bool &pCheckable,
                                  const QString &pIconResource,
                                  const QList<QKeySequence> &pKeySequences)
{
    // Create and return an action

    QAction *res = new QAction(pMainWindow);

    res->setCheckable(pCheckable);
    res->setIcon(QIcon(pIconResource));
    res->setShortcuts(pKeySequences);

    return res;
}

//==============================================================================

QAction * GuiInterface::newAction(QMainWindow *pMainWindow,
                                  const bool &pCheckable,
                                  const QString &pIconResource,
                                  const QKeySequence::StandardKey &pStandardKey)
{
    // Create and return an action

    return newAction(pMainWindow, pCheckable, pIconResource,
                     QKeySequence::keyBindings(pStandardKey));
}

//==============================================================================

QAction * GuiInterface::newAction(QMainWindow *pMainWindow,
                                  const bool &pCheckable,
                                  const QString &pIconResource,
                                  const QKeySequence &pKeySequence)
{
    // Create and return an action

    return newAction(pMainWindow, pCheckable, pIconResource,
                     QList<QKeySequence>() << pKeySequence);
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
