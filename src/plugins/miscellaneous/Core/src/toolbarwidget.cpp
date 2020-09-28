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

//==============================================================================

#include <QLabel>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

ToolBarLabelWidgetAction::ToolBarLabelWidgetAction(QWidget *pParent)
    : QWidgetAction(pParent)
{
}

//==============================================================================

QWidget * ToolBarLabelWidgetAction::createWidget(QWidget *pParent)
{
    // Create and return a label widget
    // Note: in some cases, to emit the labelCreated() signal directly after
    //       creating the label may result in the signal being emitted before a
    //       caller gets a chance to create a connection for it, hence we emit
    //       the signal through a single shot...

    auto res = new QLabel(pParent);

    QTimer::singleShot(0, this, std::bind(&ToolBarLabelWidgetAction::emitLabelCreated,
                                          this, res));

    return res;
}

//==============================================================================

QList<QLabel *> ToolBarLabelWidgetAction::labels() const
{
    // Return our created labels

    QList<QLabel *> res;

    for (const auto &label : createdWidgets()) {
        res << static_cast<QLabel *>(label);
    }

    return res;
}

//==============================================================================

bool ToolBarLabelWidgetAction::validLabel(QLabel *pLabel) const
{
    // Return whether the given label is (still) valid
    // Note: this method is needed so that people who handle the labelCreated()
    //       signal can ensure that the label is still valid since QWidgetAction
    //       is in charge of creating/destroying them...

    for (const auto &label : createdWidgets()) {
        if (pLabel == label) {
            return true;
        }
    }

    return false;
}

//==============================================================================

void ToolBarLabelWidgetAction::emitLabelCreated(QLabel *pLabel)
{
    // Let people know that a label widget has been created

    emit labelCreated(pLabel);
}

//==============================================================================

ToolBarWidget::ToolBarWidget(QWidget *pParent)
    : QToolBar(pParent)
{
    // Note: we do NOT want a parent! Indeed, if we did have a parent, then to
    //       start OpenCOR using the Simulation Experiment view would, on macOS,
    //       result in the tool bar having the wrong style!? Yet, the tool bar
    //       would look fine for other files, just not the one with which
    //       OpenCOR started!? The only case where it works as expected is when
    //       we don't have a parent...

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

ToolBarLabelWidgetAction * ToolBarWidget::addLabelWidgetAction()
{
    // Add and return a label widget action

    auto res = new ToolBarLabelWidgetAction(this);

    addAction(res);

    return res;
}

//==============================================================================

QAction * ToolBarWidget::addWidgetAction(QWidget *pWidget)
{
    // Add the given widget to ourselves

    return addWidget(pWidget);
}

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
