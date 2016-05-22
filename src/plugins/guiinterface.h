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

#pragma once

//==============================================================================

#include <QIcon>
#include <QList>
#include <QKeySequence>
#include <QString>

//==============================================================================

class QAction;
class QMenu;
class QWidget;

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
    enum Type {
        File,
        View
    };

    explicit Menu(const Type &pType, QMenu *pMenu);
    explicit Menu(const Type &pType, QAction *pAction, QMenu *pMenu);

    Type type() const;
    QAction * action() const;
    QMenu * menu() const;

private:
    Type mType;
    QAction *mAction;
    QMenu *mMenu;
};

//==============================================================================

typedef QList<Menu> Menus;
typedef QListIterator<Menu> MenuIterator;

//==============================================================================

class MenuAction
{
public:
    enum Type {
        File,
        FileNew,
        Tools
    };

    explicit MenuAction(const Type &pType, QAction *pAction = 0);

    Type type() const;
    QAction * action() const;

private:
    Type mType;
    QAction *mAction;
};

//==============================================================================

typedef QList<MenuAction> MenuActions;
typedef QListIterator<MenuAction> MenuActionIterator;

//==============================================================================

}   // namespace Gui

//==============================================================================

class GuiInterface
{
public:
#define INTERFACE_DEFINITION
    #include "guiinterface.inl"
#undef INTERFACE_DEFINITION
};

//==============================================================================

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::GuiInterface, "OpenCOR::GuiInterface")

//==============================================================================
// End of file
//==============================================================================
