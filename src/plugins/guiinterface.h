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

#pragma once

//==============================================================================

#include <QObject>

//==============================================================================

class QAction;
class QMenu;

//==============================================================================

namespace OpenCOR {

//==============================================================================

class Plugin;

//==============================================================================

namespace Gui {

//==============================================================================

class Menu
{
public:
    enum class Type {
        File,
        View
    };

    explicit Menu(Type pType, QMenu *pMenu);
    explicit Menu(Type pType, QAction *pAction, QMenu *pMenu);

    Type type() const;
    QAction * action() const;
    QMenu * menu() const;

private:
    Type mType;
    QAction *mAction = nullptr;
    QMenu *mMenu;
};

//==============================================================================

using Menus = QList<Menu>;

//==============================================================================

class MenuAction
{
public:
    enum class Type {
        File,
        FileNew,
        Tools
    };

    explicit MenuAction(Type pType, QAction *pAction);

    Type type() const;
    QAction * action() const;

private:
    Type mType;
    QAction *mAction;
};

//==============================================================================

using MenuActions = QList<MenuAction>;

//==============================================================================

} // namespace Gui

//==============================================================================

extern "C" Q_DECL_EXPORT int guiInterfaceVersion();

//==============================================================================

class GuiInterface
{
public:
    virtual ~GuiInterface();

#define INTERFACE_DEFINITION
    #include "guiinterface.inl"
#undef INTERFACE_DEFINITION
};

//==============================================================================

} // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR::GuiInterface")

//==============================================================================
// End of file
//==============================================================================
