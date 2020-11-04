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
// Tool bar widget wheel widget action
//==============================================================================

#include "toolbarwidgetwheelwidgetaction.h"

//==============================================================================

#include <QTimer>

//==============================================================================

#include "qwtbegin.h"
    #include "qwt_wheel.h"
#include "qwtend.h"

//==============================================================================

namespace OpenCOR {
namespace ToolBarWidget {

//==============================================================================

ToolBarWidgetWheelWidgetAction::ToolBarWidgetWheelWidgetAction(QWidget *pParent)
    : QWidgetAction(pParent)
{
}

//==============================================================================

QWidget * ToolBarWidgetWheelWidgetAction::createWidget(QWidget *pParent)
{
    // Create and return a wheel widget
    // Note: in some cases, to emit the created() signal directly after creating
    //       the wheel widget may result in the signal being emitted before a
    //       caller gets a chance to create a connection for it, hence we emit
    //       the signal through a single shot...

    auto res = new QwtWheel(pParent);

    res->setBorderWidth(0);
    res->setFocusPolicy(Qt::NoFocus);
    res->setWheelBorderWidth(0);

    connect(res, &QwtWheel::valueChanged,
            this, &ToolBarWidgetWheelWidgetAction::valueChanged);
    connect(res, &QwtWheel::wheelReleased,
            this, &ToolBarWidgetWheelWidgetAction::wheelReleased);

    QTimer::singleShot(0, this, std::bind(&ToolBarWidgetWheelWidgetAction::emitCreated,
                                          this, res));

    return res;
}

//==============================================================================

QList<QwtWheel *> ToolBarWidgetWheelWidgetAction::wheels() const
{
    // Return our created wheels

    QList<QwtWheel *> res;

    for (const auto &wheel : createdWidgets()) {
        res << static_cast<QwtWheel *>(wheel);
    }

    return res;
}

//==============================================================================

bool ToolBarWidgetWheelWidgetAction::validWheel(QwtWheel *pWheel) const
{
    // Return whether the given wheel is (still) valid
    // Note: this method is needed so that people who handle the created()
    //       signal can ensure that the wheel is still valid since QWidgetAction
    //       is in charge of creating/destroying them...

    return createdWidgets().contains(pWheel);
}

//==============================================================================

void ToolBarWidgetWheelWidgetAction::emitCreated(QwtWheel *pWheel)
{
    // Let people know that a wheel widget has been created

    emit created(pWheel);
}

//==============================================================================

} // namespace ToolBarWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
