#include "commonwidget.h"

#include <QApplication>
#include <QDesktopWidget>

QSize CommonWidget::defaultSize(const double &pRatio) const
{
    // The default size of a widget is one fifth of the available geometry

    QRect desktopGeometry = qApp->desktop()->availableGeometry();

    return QSize(pRatio*desktopGeometry.width(), pRatio*desktopGeometry.height());
}
