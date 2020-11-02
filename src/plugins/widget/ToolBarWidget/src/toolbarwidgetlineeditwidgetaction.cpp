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
// Tool bar widget line edit widget action
//==============================================================================

#include "toolbarwidgetlineeditwidgetaction.h"

//==============================================================================

#include <QLineEdit>
#include <QTimer>

//==============================================================================

namespace OpenCOR {
namespace ToolBarWidget {

//==============================================================================

ToolBarWidgetLineEditWidgetAction::ToolBarWidgetLineEditWidgetAction(QWidget *pParent)
    : QWidgetAction(pParent)
{
}

//==============================================================================

QWidget * ToolBarWidgetLineEditWidgetAction::createWidget(QWidget *pParent)
{
    // Create and return a line edit widget
    // Note: in some cases, to emit the created() signal directly after creating
    //       the line edit widget may result in the signal being emitted before
    //       a caller gets a chance to create a connection for it, hence we emit
    //       the signal through a single shot...

    auto res = new QLineEdit(pParent);

    connect(res, &QLineEdit::textChanged,
            this, &ToolBarWidgetLineEditWidgetAction::emitTextChanged);
    connect(res, &QLineEdit::returnPressed,
            this, &ToolBarWidgetLineEditWidgetAction::emitReturnPressed);

    QTimer::singleShot(0, this, std::bind(&ToolBarWidgetLineEditWidgetAction::emitCreated,
                                          this, res));

    return res;
}

//==============================================================================

QList<QLineEdit *> ToolBarWidgetLineEditWidgetAction::lineEdits() const
{
    // Return our created line edits

    QList<QLineEdit *> res;

    for (const auto &lineEdit : createdWidgets()) {
        res << static_cast<QLineEdit *>(lineEdit);
    }

    return res;
}

//==============================================================================

bool ToolBarWidgetLineEditWidgetAction::validLineEdit(QLineEdit *pLineEdit) const
{
    // Return whether the given line edit is (still) valid
    // Note: this method is needed so that people who handle the created()
    //       signal can ensure that the line edit is still valid since
    //       QWidgetAction is in charge of creating/destroying them...

    for (const auto &lineEdit : createdWidgets()) {
        if (pLineEdit == lineEdit) {
            return true;
        }
    }

    return false;
}

//==============================================================================

void ToolBarWidgetLineEditWidgetAction::setText(const QString &pText)
{
    // Set the text of all our created line edits

    for (const auto &lineEdit : lineEdits()) {
        lineEdit->setText(pText);
    }
}

//==============================================================================

void ToolBarWidgetLineEditWidgetAction::emitCreated(QLineEdit *pLineEdit)
{
    // Let people know that a line edit widget has been created

    emit created(pLineEdit);
}

//==============================================================================

void ToolBarWidgetLineEditWidgetAction::emitTextChanged()
{
    // Let people know that the text has changed

    emit textChanged(qobject_cast<QLineEdit *>(sender())->text());
}

//==============================================================================

void ToolBarWidgetLineEditWidgetAction::emitReturnPressed()
{
    // Let people know that return was pressed

    emit returnPressed(qobject_cast<QLineEdit *>(sender())->text());
}

//==============================================================================

} // namespace ToolBarWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
