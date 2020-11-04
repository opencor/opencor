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

#pragma once

//==============================================================================

#include "toolbarwidgetglobal.h"

//==============================================================================

#include <QWidgetAction>

//==============================================================================

class QwtWheel;

//==============================================================================

namespace OpenCOR {
namespace ToolBarWidget {

//==============================================================================

class TOOLBARWIDGET_EXPORT ToolBarWidgetWheelWidgetAction : public QWidgetAction
{
    Q_OBJECT

public:
    ToolBarWidgetWheelWidgetAction(QWidget *pParent);

    QList<QwtWheel *> wheels() const;
    bool validWheel(QwtWheel *pWheel) const;

protected:
    QWidget * createWidget(QWidget *pParent) override;

private:
    void emitCreated(QwtWheel *pWheel);

signals:
    void created(QwtWheel *pWheel);
    void valueChanged(double pValue);
    void wheelReleased();
};

//==============================================================================

} // namespace ToolBarWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
