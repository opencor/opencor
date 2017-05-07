/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Progress bar widget
//==============================================================================

#include "coreguiutils.h"
#include "progressbarwidget.h"

//==============================================================================

#include <QPainter>
#include <QPaintEvent>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

ProgressBarWidget::ProgressBarWidget(QWidget *pParent) :
    Widget(QSize(), pParent),
    mValue(0.0)
{
}

//==============================================================================

void ProgressBarWidget::paintEvent(QPaintEvent *pEvent)
{
    // Paint ourselves

    QPainter painter(this);

    if (isEnabled()) {
        int value = mValue*width();

        if (value)
            painter.fillRect(0, 0, value, height(), highlightColor());

        if (value != width())
            painter.fillRect(value, 0, width()-value, height(), windowColor());
    } else {
        painter.fillRect(0, 0, width(), height(), windowColor());
    }

    // Accept the event

    pEvent->accept();
}

//==============================================================================

double ProgressBarWidget::value() const
{
    // Return our value

    return mValue;
}

//==============================================================================

void ProgressBarWidget::setValue(const double &pValue)
{
    // Update both our value and ourselves, if needed

    double value = qMin(1.0, qMax(pValue, 0.0));

    if (value != mValue) {
        bool needUpdate = int(mValue*width()) != int(value*width());

        mValue = value;

        if (needUpdate)
            update();
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
