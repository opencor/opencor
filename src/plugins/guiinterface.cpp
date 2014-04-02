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
// GUI interface
//==============================================================================

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

GuiMenuSettings::GuiMenuSettings(const GuiMenuSettings::Type &pType,
                                 QMenu *pMenu) :
    mType(pType),
    mAction(0),
    mMenu(pMenu)
{
}

//==============================================================================

GuiMenuSettings::GuiMenuSettings(const GuiMenuSettings::Type &pType,
                                 QAction *pAction, QMenu *pMenu) :
    mType(pType),
    mAction(pAction),
    mMenu(pMenu)
{
}

//==============================================================================

GuiMenuSettings::Type GuiMenuSettings::type() const
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

GuiMenuActionSettings::GuiMenuActionSettings(const GuiMenuActionSettings::Type &pType,
                                             QAction *pAction) :
    mType(pType),
    mAction(pAction)
{
}

//==============================================================================

GuiMenuActionSettings::Type GuiMenuActionSettings::type() const
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

QString GuiViewSettings::modeAsString(const GuiViewSettings::Mode &pMode)
{
    // Return the mode corresponding to the given mode string

    switch (pMode) {
    case GuiViewSettings::Editing:
        return ModeEditing;
    case GuiViewSettings::Simulation:
        return ModeSimulation;
    case GuiViewSettings::Analysis:
        return ModeAnalysis;
    default:   // GuiViewSettings::Unknown
        return ModeUnknown;
    }
}

//==============================================================================

GuiViewSettings::Mode GuiViewSettings::modeFromString(const QString &pMode)
{
    // Return the mode string corresponding to the given mode

    if (!pMode.compare(ModeEditing))
        return GuiViewSettings::Editing;
    else if (!pMode.compare(ModeSimulation))
        return GuiViewSettings::Simulation;
    else if (!pMode.compare(ModeAnalysis))
        return GuiViewSettings::Analysis;
    else
        return GuiViewSettings::Unknown;
}

//==============================================================================

GuiWindowSettings::GuiWindowSettings(const Qt::DockWidgetArea &pDefaultDockArea,
                                     QWidget *pWindow,
                                     const GuiWindowSettings::Type &pType,
                                     QAction *pAction) :
    mDefaultDockArea(pDefaultDockArea),
    mWindow(pWindow),
    mType(pType),
    mAction(pAction)
{
}

//==============================================================================

Qt::DockWidgetArea GuiWindowSettings::defaultDockArea() const
{
    // Return the window's default dock area

    return mDefaultDockArea;
}

//==============================================================================

QWidget * GuiWindowSettings::window() const
{
    // Return the window itself

    return mWindow;
}

//==============================================================================

GuiWindowSettings::Type GuiWindowSettings::type() const
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

void GuiSettings::addMenu(const GuiMenuSettings::Type &pType, QMenu *pMenu)
{
    // Add a menu to our list

    mMenus << new GuiMenuSettings(pType, pMenu);
}

//==============================================================================

void GuiSettings::addMenu(const GuiMenuSettings::Type &pType, QAction *pAction,
                          QMenu *pMenu)
{
    // Add a menu to our list

    mMenus << new GuiMenuSettings(pType, pAction, pMenu);
}

//==============================================================================

void GuiSettings::addMenuAction(const GuiMenuActionSettings::Type &pType,
                                QAction *pAction)
{
    // Add a menu action to our list
    // Note: a null pAction means that we want to add a separator...

    mMenuActions << new GuiMenuActionSettings(pType, pAction);
}

//==============================================================================

void GuiSettings::setCentralWidget(QWidget *pCentralWidget)
{
    // Set the central widget to be used

    mCentralWidget = pCentralWidget;
}

//==============================================================================

void GuiSettings::addWindow(const Qt::DockWidgetArea &pDefaultDockArea,
                            QWidget *pWindow,
                            const GuiWindowSettings::Type &pType,
                            QAction *pAction)
{
    // Add a window to our list

    mWindows << new GuiWindowSettings(pDefaultDockArea, pWindow, pType, pAction);
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

QWidget * GuiSettings::centralWidget() const
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

GuiInterface::~GuiInterface()
{
    // Delete our GUI settings object

    delete mGuiSettings;
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

QMenu * GuiInterface::newMenu(QWidget *pParent,
                              const QString &pNameOrIconResource,
                              const bool &pProvidedName)
{
    // Create and return a menu

    QMenu *res = new QMenu(pParent);

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

QAction * GuiInterface::newAction(QWidget *pParent, const bool &pCheckable,
                                  const QString &pIconResource,
                                  const QList<QKeySequence> &pKeySequences)
{
    // Create and return an action

    QAction *res = new QAction(pParent);

    res->setCheckable(pCheckable);
    res->setIcon(QIcon(pIconResource));
    res->setShortcuts(pKeySequences);

    return res;
}

//==============================================================================

QAction * GuiInterface::newAction(QWidget *pParent, const bool &pCheckable,
                                  const QString &pIconResource,
                                  const QKeySequence::StandardKey &pStandardKey)
{
    // Create and return an action

    return newAction(pParent, pCheckable, pIconResource,
                     QKeySequence::keyBindings(pStandardKey));
}

//==============================================================================

QAction * GuiInterface::newAction(QWidget *pParent, const bool &pCheckable,
                                  const QString &pIconResource,
                                  const QKeySequence &pKeySequence)
{
    // Create and return an action

    return newAction(pParent, pCheckable, pIconResource,
                     QList<QKeySequence>() << pKeySequence);
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
