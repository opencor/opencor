/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Tool bar widget drop-down list widget action
//==============================================================================

#include "toolbarwidgetdropdownlistwidgetaction.h"

//==============================================================================

#include <QMenu>
#include <QTimer>
#include <QToolButton>

//==============================================================================

namespace OpenCOR {
namespace ToolBarWidget {

//==============================================================================

ToolBarWidgetDropDownListWidgetAction::ToolBarWidgetDropDownListWidgetAction(QAction *pDefaultAction,
                                                                             QMenu *pDropDownMenu,
                                                                             QWidget *pParent)
    : QWidgetAction(pParent),
      mDefaultAction(pDefaultAction),
      mDropDownMenu(pDropDownMenu)
{
    // Unhover our drop-down lists when about to hide our drop-down menu

    connect(mDropDownMenu, &QMenu::aboutToHide,
            this, &ToolBarWidgetDropDownListWidgetAction::unhoverDropDownLists);
}

//==============================================================================

QWidget * ToolBarWidgetDropDownListWidgetAction::createWidget(QWidget *pParent)
{
    // Create and return a drop-down list widget
    // Note: in some cases, to emit the created() signal directly after creating
    //       the drop-down list widget may result in the signal being emitted
    //       before a caller gets a chance to create a connection for it, hence
    //       we emit the signal through a single shot...

    auto res = new QToolButton(pParent);

    res->setDefaultAction(mDefaultAction);
    res->setMenu(mDropDownMenu);
    res->setPopupMode(QToolButton::MenuButtonPopup);

    QTimer::singleShot(0, this, std::bind(&ToolBarWidgetDropDownListWidgetAction::emitCreated,
                                          this, res));

    return res;
}

//==============================================================================

QList<QToolButton *> ToolBarWidgetDropDownListWidgetAction::dropDownLists() const
{
    // Return our created drop-down lists

    QList<QToolButton *> res;
    const QList<QWidget *> dropDownLists = createdWidgets();

    for (const auto &dropDownList : dropDownLists) {
        res << static_cast<QToolButton *>(dropDownList);
    }

    return res;
}

//==============================================================================

bool ToolBarWidgetDropDownListWidgetAction::validDropDownList(QToolButton *pDropDownList) const
{
    // Return whether the given drop-down list is (still) valid
    // Note: this method is needed so that people who handle the created()
    //       signal can ensure that the drop-down list is still valid since
    //       QWidgetAction is in charge of creating/destroying them...

    return createdWidgets().contains(pDropDownList);
}

//==============================================================================

void ToolBarWidgetDropDownListWidgetAction::emitCreated(QToolButton *pDropDownList)
{
    // Let people know that a drop-down list widget has been created

    emit created(pDropDownList);
}

//==============================================================================

void ToolBarWidgetDropDownListWidgetAction::unhoverDropDownLists()
{
    // Unhover all our drop-down lists
    // Note: indeed, say that you show the drop-down menu but then decide not to
    //       click on any of the menu items then, on macOS (and this because of
    //       our custom styling), the drop-down list will still look hovered,
    //       something that we don't want...

    const QList<QToolButton *> dropDownLists = ToolBarWidgetDropDownListWidgetAction::dropDownLists();

    for (const auto &dropDownList : dropDownLists) {
        dropDownList->setAttribute(Qt::WA_UnderMouse, false);
    }
}

//==============================================================================

} // namespace ToolBarWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
