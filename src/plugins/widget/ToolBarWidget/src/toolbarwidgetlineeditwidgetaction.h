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

#pragma once

//==============================================================================

#include "toolbarwidgetglobal.h"

//==============================================================================

#include <QWidgetAction>

//==============================================================================

class QLineEdit;

//==============================================================================

namespace OpenCOR {
namespace ToolBarWidget {

//==============================================================================

class TOOLBARWIDGET_EXPORT ToolBarWidgetLineEditWidgetAction : public QWidgetAction
{
    Q_OBJECT

public:
    explicit ToolBarWidgetLineEditWidgetAction(QWidget *pParent);

    QList<QLineEdit *> lineEdits() const;
    bool validLineEdit(QLineEdit *pLineEdit) const;

    void setText(const QString &pText);

protected:
    QWidget * createWidget(QWidget *pParent) override;

private:
    void emitCreated(QLineEdit *pLineEdit);

signals:
    void created(QLineEdit *pLineEdit);
    void textChanged(const QString &pText);
    void returnPressed();
};

//==============================================================================

} // namespace ToolBarWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
