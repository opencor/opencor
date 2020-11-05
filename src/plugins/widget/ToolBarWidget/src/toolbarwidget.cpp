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
// Tool bar widget
//==============================================================================

#include "toolbarwidget.h"
#include "toolbarwidgetdropdownlistwidgetaction.h"
#include "toolbarwidgetlabelwidgetaction.h"
#include "toolbarwidgetlineeditwidgetaction.h"
#include "toolbarwidgetwheelwidgetaction.h"

//==============================================================================

//#include <QApplication>
//#include <QLabel>
//#include <QLineEdit>
//#include <QScreen>
//#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace ToolBarWidget {

//==============================================================================

ToolBarWidget::ToolBarWidget(QWidget *pParent)
    : QToolBar(pParent)
{
    // Remove the border which is normally drawn for a tool bar widget (indeed,
    // it doesn't look great when on a docked window) and make sure that we have
    // a spacing of 4 pixels (indeed, on Windows/Linux, the layout has no
    // spacing, which doesn't look great in some cases)

    setStyleSheet("QToolBar {"
                  "    border: none;"
                  "    spacing: 4px;"
                  "}");

    // Force the size of the icons to be 20 by 20 pixels
    // Note: this ensures that our icons have a decent size on HiDPI screens...

    setIconSize(QSize(20, 20));
}

//==============================================================================

QAction * ToolBarWidget::addSpacerWidgetAction(QSizePolicy::Policy pHorizontalSizePolicy,
                                               QSizePolicy::Policy pVerticalSizePolicy)
{
    // Add and return a spacer widget action

    auto spacer = new QWidget(this);

    spacer->setMinimumSize(0, 0);
    spacer->setSizePolicy(pHorizontalSizePolicy, pVerticalSizePolicy);

    return addWidget(spacer);
}

//==============================================================================

ToolBarWidgetDropDownListWidgetAction * ToolBarWidget::addDropDownListWidgetAction(QAction *pDefaultAction,
                                                                                   QMenu *pDropDownMenu)
{
    // Add and return a drop-down list widget action

    auto res = new ToolBarWidgetDropDownListWidgetAction(pDefaultAction, pDropDownMenu, this);

    addAction(res);

    return res;
}

//==============================================================================

ToolBarWidgetLabelWidgetAction * ToolBarWidget::addLabelWidgetAction()
{
    // Add and return a label widget action

    auto res = new ToolBarWidgetLabelWidgetAction(this);

    addAction(res);

    return res;
}

//==============================================================================

ToolBarWidgetLineEditWidgetAction * ToolBarWidget::addLineEditWidgetAction()
{
    // Add and return a line edit widget action

    auto res = new ToolBarWidgetLineEditWidgetAction(this);

    addAction(res);

    return res;
}

//==============================================================================

ToolBarWidgetWheelWidgetAction * ToolBarWidget::addWheelWidgetAction()
{
    // Add and return a wheel widget action

    auto res = new ToolBarWidgetWheelWidgetAction(this);

    addAction(res);

    return res;
}

//==============================================================================

} // namespace ToolBarWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
