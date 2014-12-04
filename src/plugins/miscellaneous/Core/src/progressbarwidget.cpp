/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

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
    mWidth(0),
    mOldValue(0.0),
    mValue(0.0)
{
}

//==============================================================================

void ProgressBarWidget::paintEvent(QPaintEvent *pEvent)
{
    // Paint ourselves

    QPainter painter(this);

    int value = mValue*mWidth;

    if (value)
        painter.fillRect(0, 0, value, height(), highlightColor());

    if (value != mWidth)
        painter.fillRect(value, 0, mWidth-value, height(), windowColor());

    // Accept the event

    pEvent->accept();
}

//==============================================================================

void ProgressBarWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    Widget::resizeEvent(pEvent);

    // Keep track of our new width

    mWidth = pEvent->size().width();
}

//==============================================================================

void ProgressBarWidget::setValue(const double &pValue)
{
    // Update both our value and ourselves, if needed

    double value = qMin(1.0, qMax(pValue, 0.0));

    if (value != mValue) {
        mValue = value;

        // Update ourselves, but only if necessary

        if (int(mOldValue*mWidth) != int(mValue*mWidth)) {
            mOldValue = mValue;

            update();
        }
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
