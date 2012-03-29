//==============================================================================
// Common widget
//==============================================================================

#include "commonwidget.h"
#include "dockwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QFrame>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CommonWidget::CommonWidget(QWidget *pParent) :
#ifndef OpenCOR_MAIN
    mParent(pParent),
    mBorderColor(QColor())
#else
    mParent(pParent)
#endif
{
#ifndef OpenCOR_MAIN
    updateBorderColor();
#endif
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

void CommonWidget::retranslateUi()
{
    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::loadSettings(QSettings *)
{
    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::saveSettings(QSettings *) const
{
    // Nothing to do by default...
}

//==============================================================================

void CommonWidget::loadingOfSettingsDone(const Plugins &)
{
    // Nothing to do by default...
}

//==============================================================================

#ifndef OpenCOR_MAIN
void CommonWidget::updateBorderColor()
{
    // We want the border to be of the same colour as the one used by Qt when
    // rendering the border of a frame with QFrame::StyledPanel as a shape. Now,
    // the problem is that this colour isn't defined anywhere, so we create a
    // frame, render it to an image and manually retrieve the colour of the
    // frame's border...

    QFrame frame;
    QImage image = QImage(frame.size(), QImage::Format_ARGB32_Premultiplied);

    frame.setFrameShape(QFrame::StyledPanel);
    frame.render(&image);

    mBorderColor = QColor(image.pixel(0, 0));
}
#endif

//==============================================================================

#ifndef OpenCOR_MAIN
void CommonWidget::drawBorderIfDocked(const bool &pForceDrawing,
                                      const bool &pTop, const bool &pLeft,
                                      const bool &pBottom, const bool &pRight)
{
#ifdef Q_WS_MAC
    // There is no need to draw a border on Mac OS X if we are docked, so
    // unless we want to force the drawing we just return

    if (!pForceDrawing)
        return;
#endif

    // Draw a border, but only if we are docked

    DockWidget *dockWidget = qobject_cast<DockWidget *>(mParent);
    QWidget *widget = dynamic_cast<QWidget *>(this);

#ifdef Q_WS_MAC
    if (dockWidget && widget) {
#else
    if (dockWidget && widget && (pForceDrawing || !dockWidget->isFloating())) {
#endif
        // The QObject/dynamic castings were successful, so our parent is really
        // a DockWidget, we are really a QWidget, and to top it all our parent
        // is floating (or we want to force the drawing), so let's go ahead...

        QRect border = widget->rect();

        QPainter painter(widget);

        QPen pen = painter.pen();

        pen.setColor(mBorderColor);

        painter.setPen(pen);

#ifdef Q_WS_MAC
        if (pTop)
#else
        if (pTop || !dockWidget->isFloating())
#endif
            painter.drawLine(border.topLeft(), border.topRight());

#ifdef Q_WS_MAC
        if (pLeft)
#else
        if (pLeft || !dockWidget->isFloating())
#endif
            painter.drawLine(border.topLeft(), border.bottomLeft());

#ifdef Q_WS_MAC
        if (pBottom)
#else
        if (pBottom || !dockWidget->isFloating())
#endif
            painter.drawLine(border.bottomLeft(), border.bottomRight());

#ifdef Q_WS_MAC
        if (pRight)
#else
        if (pRight || !dockWidget->isFloating())
#endif
            painter.drawLine(border.topRight(), border.bottomRight());
    }
}
#endif

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
