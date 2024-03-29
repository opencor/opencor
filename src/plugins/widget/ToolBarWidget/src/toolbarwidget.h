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

#pragma once

//==============================================================================

#include "toolbarwidgetglobal.h"

//==============================================================================

#include <QToolBar>

//==============================================================================

namespace OpenCOR {
namespace ToolBarWidget {

//==============================================================================

class ToolBarWidgetDropDownListWidgetAction;
class ToolBarWidgetLabelWidgetAction;
class ToolBarWidgetLineEditWidgetAction;
class ToolBarWidgetWheelWidgetAction;

//==============================================================================

class TOOLBARWIDGET_EXPORT ToolBarWidget : public QToolBar
{
    Q_OBJECT

public:
    explicit ToolBarWidget(QWidget *pParent);

    QAction * addSpacerWidgetAction(QSizePolicy::Policy pHorizontalSizePolicy,
                                    QSizePolicy::Policy pVerticalSizePolicy);
    ToolBarWidgetDropDownListWidgetAction * addDropDownListWidgetAction(QAction *pDefaultAction,
                                                                        QMenu *pDropDownMenu);
    ToolBarWidgetLabelWidgetAction * addLabelWidgetAction();
    ToolBarWidgetLineEditWidgetAction * addLineEditWidgetAction();
    ToolBarWidgetWheelWidgetAction * addWheelWidgetAction();

protected:
    void changeEvent(QEvent *pEvent) override;

private:
    void updateStyleSheet();
};

//==============================================================================

} // namespace ToolBarWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
