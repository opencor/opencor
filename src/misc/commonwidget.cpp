#include "commonwidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QPainter>
#include <QPen>
#include <QWidget>

CommonWidget::CommonWidget(QWidget *pParent) :
    mParent(pParent)
{
}

QSize CommonWidget::defaultSize(const double &pRatio) const
{
    // The default size of a widget is one fifth of the available geometry

    QRect desktopGeometry = qApp->desktop()->availableGeometry();

    return QSize(pRatio*desktopGeometry.width(), pRatio*desktopGeometry.height());
}

void CommonWidget::drawBorderIfDocked()
{
#ifndef Q_WS_MAC
    // Draw a border, but only if we are docked
    // Note: no need for that on Mac OS X, since it already looks OK as it is

    QDockWidget *dockWidget = dynamic_cast<QDockWidget *>(mParent);
    QWidget *widget = dynamic_cast<QWidget *>(this);

    if (dockWidget && widget && !dockWidget->isFloating()) {
        // The dynamic castings were successful, so our parent is really a
        // QDockWidget, we are really a QWidget, and to top it all our parent
        // is floating, so we can draw a border

        QRect border = widget->rect();

        border.adjust(0, 0, -1, -1);

        QPainter painter(widget);

        QPen pen = painter.pen();

        pen.setColor(Qt::lightGray);

        painter.setPen(pen);
        painter.drawRect(border);
    }
#endif
}
