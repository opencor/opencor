/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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
