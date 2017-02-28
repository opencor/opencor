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
// Zinc window widget
//==============================================================================

#include "zincwindowwidget.h"

//==============================================================================

#include <QPainter>
#include <QPaintEvent>

//==============================================================================

namespace OpenCOR {
namespace ZincWindow {

//==============================================================================

ZincWindowWidget::ZincWindowWidget(QWidget *pParent) :
    QOpenGLWidget(pParent),
    Core::CommonWidget(this)
{
}

//==============================================================================

void ZincWindowWidget::paintEvent(QPaintEvent *pEvent)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(pEvent->rect(), QBrush(QColor(64, 32, 64)));
    painter.translate(100, 100);

    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));

    painter.save();
    painter.setBrush(QBrush(gradient));
    painter.setPen(Qt::black);
//    painter.rotate(elapsed * 0.030);

    qreal r = 0;//elapsed / 1000.0;
    int n = 30;
    for (int i = 0; i < n; ++i) {
        painter.rotate(30);
        qreal factor = (i + r) / n;
        qreal radius = 0 + 120.0 * factor;
        qreal circleRadius = 1 + factor * 20;
        painter.drawEllipse(QRectF(radius, -circleRadius,
                                    circleRadius * 2, circleRadius * 2));
    }
    painter.restore();

    QFont textFont;
    textFont.setPixelSize(50);

    painter.setPen(Qt::white);
    painter.setFont(textFont);
    painter.drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Qt"));
    painter.end();

    // Accept the event

    pEvent->accept();
}

//==============================================================================

QSize ZincWindowWidget::sizeHint() const
{
    // Suggest a default size for the Zinc widget
    // Note: this is critical if we want a docked widget, with a Zinc widget on
    //       it, to have a decent size when docked to the main window...

    return defaultSize(0.15);
}

//==============================================================================

}   // namespace ZincWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
