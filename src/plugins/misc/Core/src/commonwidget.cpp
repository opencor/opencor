//==============================================================================
// Common widget
//==============================================================================

#include "commonwidget.h"
#include "dockwidget.h"

//==============================================================================

#include <QApplication>
#include <QDesktopWidget>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QWidget>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

CommonWidget::CommonWidget(QWidget *pParent) :
    mParent(pParent)
{
}

//==============================================================================

QSize CommonWidget::sizeHint() const
{
    // The default size of a widget is 5% of the available geometry

    return 0.05*qApp->desktop()->availableGeometry().size();
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

        pen.setColor(qApp->palette().color(QPalette::Midlight));

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
