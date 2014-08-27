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
// Spinner widget
//==============================================================================

#include "guiutils.h"
#include "spinnerwidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QColor>
#include <QPainter>
#include <QPaintEvent>
#include <QPoint>

//==============================================================================

namespace OpenCOR {
namespace Spinner {

//==============================================================================

SpinnerWidget::SpinnerWidget(QWidget *pParent) :
    QWidget(pParent)
{
    resize(100, 100);
}

//==============================================================================

void SpinnerWidget::paintEvent(QPaintEvent *pEvent)
{
    // Paint ourselves

    QPainter painter(this);

    QColor backgroundColor = QColor(0, 0, 0, 32);//Core::borderColor();

//    backgroundColor.setAlphaF(0.5);

    painter.fillRect(0, 0, width(), height(), backgroundColor);

    // Accept the event

    pEvent->accept();
}

//==============================================================================

void SpinnerWidget::setVisible(bool pVisible)
{
    // Default handling of the method

    QWidget::setVisible(pVisible);
}

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
