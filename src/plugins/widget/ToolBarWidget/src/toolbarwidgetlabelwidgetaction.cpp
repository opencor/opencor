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
// Tool bar widget label widget action
//==============================================================================

#include "toolbarwidgetlabelwidgetaction.h"

//==============================================================================

#include <QLabel>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace ToolBarWidget {

//==============================================================================

ToolBarWidgetLabelWidgetAction::ToolBarWidgetLabelWidgetAction(QWidget *pParent)
    : QWidgetAction(pParent)
{
}

//==============================================================================

QWidget * ToolBarWidgetLabelWidgetAction::createWidget(QWidget *pParent)
{
    // Create and return a label widget
    // Note: in some cases, to emit the created() signal directly after creating
    //       the label widget may result in the signal being emitted before a
    //       caller gets a chance to create a connection for it, hence we emit
    //       the signal through a single shot...

    auto res = new QLabel(pParent);

    QTimer::singleShot(0, this, std::bind(&ToolBarWidgetLabelWidgetAction::emitCreated,
                                          this, res));

    return res;
}

//==============================================================================

QList<QLabel *> ToolBarWidgetLabelWidgetAction::labels() const
{
    // Return our created labels

    QList<QLabel *> res;
    const QList<QWidget *> labels = createdWidgets();

    for (const auto &label : labels) {
        res << static_cast<QLabel *>(label);
    }

    return res;
}

//==============================================================================

bool ToolBarWidgetLabelWidgetAction::validLabel(QLabel *pLabel) const
{
    // Return whether the given label is (still) valid
    // Note: this method is needed so that people who handle the created()
    //       signal can ensure that the label is still valid since QWidgetAction
    //       is in charge of creating/destroying them...

    return createdWidgets().contains(pLabel);
}

//==============================================================================

void ToolBarWidgetLabelWidgetAction::emitCreated(QLabel *pLabel)
{
    // Let people know that a label widget has been created

    emit created(pLabel);
}

//==============================================================================

} // namespace ToolBarWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
