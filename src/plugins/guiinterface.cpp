/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// GUI interface
//==============================================================================

#include "guiinterface.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int guiInterfaceVersion()
{
    // Version of the GUI interface

    return 1;
}

//==============================================================================

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
