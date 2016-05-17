/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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

Menu::Menu(const Menu::Type &pType, QMenu *pMenu) :
    mType(pType),
    mAction(0),
    mMenu(pMenu)
{
}

//==============================================================================

Menu::Menu(const Menu::Type &pType, QAction *pAction, QMenu *pMenu) :
    mType(pType),
    mAction(pAction),
    mMenu(pMenu)
{
}

//==============================================================================

Menu::Type Menu::type() const
{
    // Return the menu's type

    return mType;
}

//==============================================================================

QAction * Menu::action() const
{
    // Return the menu's action

    return mAction;
}

//==============================================================================

QMenu * Menu::menu() const
{
    // Return the menu itsef

    return mMenu;
}

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
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
