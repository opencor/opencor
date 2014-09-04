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
// Widget
//==============================================================================

#include "widget.h"

//==============================================================================

#include <QResizeEvent>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

Widget::Widget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent),
    mSizeHint(defaultSize(0.15))
{
}

//==============================================================================

Widget::Widget(const QSize &pSizeHint, QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent),
    mSizeHint(pSizeHint)
{
}

//==============================================================================

QSize Widget::sizeHint() const
{
    // Suggest our default size for the widget

    if (mSizeHint.isValid())
        return mSizeHint;
    else
        return QWidget::sizeHint();
}

//==============================================================================

void Widget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    QWidget::resizeEvent(pEvent);

    // Let people know that we have been resized

    emit resized(pEvent->size(), pEvent->oldSize());
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
