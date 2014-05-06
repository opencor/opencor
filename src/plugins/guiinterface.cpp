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
#include <QMenu>
#include <QToolBar>

//==============================================================================

namespace OpenCOR {
namespace Gui {

//==============================================================================

MenuAction::MenuAction(const MenuAction::Type &pType, QAction *pAction) :
    mType(pType),
    mAction(pAction)
{
}

//==============================================================================

MenuAction::Type MenuAction::type() const
{
    // Return the action's type

    return mType;
}

//==============================================================================

QAction * MenuAction::action() const
{
    // Return the action itself

    return mAction;
}

//==============================================================================

}   // namespace Gui

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

GuiSettings::GuiSettings() :
    mMenus(QList<GuiMenuSettings *>())
{
}

//==============================================================================

GuiSettings::~GuiSettings()
{
    // Delete the contents of our various lists

    foreach (GuiMenuSettings *menuSettings, mMenus)
        delete menuSettings;
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

QList<GuiMenuSettings *> GuiSettings::menus() const
{
    // Return our menus

    return mMenus;
}

//==============================================================================

GuiInterface::GuiInterface()
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

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
