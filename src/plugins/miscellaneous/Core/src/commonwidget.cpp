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
// Common widget
//==============================================================================

#include "commonwidget.h"
#include "cliutils.h"
#include "guiutils.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QPainter>
#include <QPen>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CommonWidget::CommonWidget(QWidget *pParent) :
    mParent(pParent)
{
}

//==============================================================================

QSize CommonWidget::defaultSize(const double &pRatio) const
{
    // The default size of a widget is a ratio of the available geometry

    QRect desktopGeometry = qApp->desktop()->availableGeometry();

    return QSize(pRatio*desktopGeometry.width(),
                 pRatio*desktopGeometry.height());
}

//==============================================================================

void CommonWidget::loadSettings(QSettings *pSettings)
{
    Q_UNUSED(pSettings);

    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);

    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::retranslateUi()
{
    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::drawBorder(const bool &pDockedTop, const bool &pDockedLeft,
                              const bool &pDockedBottom, const bool &pDockedRight,
                              const bool &pFloatingTop, const bool &pFloatingLeft,
                              const bool &pFloatingBottom, const bool &pFloatingRight)
{
    // Draw a border around the widget
    // Note: a qobject_cast is not good enough for mParent on OS X...

    QDockWidget *dockWidget = dynamic_cast<QDockWidget *>(mParent);
    QWidget *widget = dynamic_cast<QWidget *>(this);

    if (dockWidget && widget) {
        // The castings were successful, so our parent is really a QDockWidget,
        // we are really a QWidget, and to top it all our parent is floating (or
        // we want to force the drawing), so let's go ahead...

        QPainter painter(widget);

        QPen newPen = painter.pen();

        newPen.setColor(borderColor());

        painter.setPen(newPen);

        bool dockWidgetDocked = !dockWidget->isFloating();
        QRect border = widget->rect();

        if ((pDockedTop && dockWidgetDocked) || (pFloatingTop && !dockWidgetDocked))
            painter.drawLine(border.topLeft(), border.topRight());

        if ((pDockedLeft && dockWidgetDocked) || (pFloatingLeft && !dockWidgetDocked))
            painter.drawLine(border.topLeft(), border.bottomLeft());

        if ((pDockedBottom && dockWidgetDocked) || (pFloatingBottom && !dockWidgetDocked))
            painter.drawLine(border.bottomLeft(), border.bottomRight());

        if ((pDockedRight && dockWidgetDocked) || (pFloatingRight && !dockWidgetDocked))
            painter.drawLine(border.topRight(), border.bottomRight());
    }
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
