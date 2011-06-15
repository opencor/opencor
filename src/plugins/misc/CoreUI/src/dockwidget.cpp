#include "dockwidget.h"

namespace OpenCOR {

DockWidget::DockWidget(QWidget *pParent) :
    QDockWidget(pParent),
    CommonWidget(pParent)
{
#ifdef Q_WS_MAC
    // Remove the padding for the float and close buttons for Mac OS X (indeed
    // it makes the title bar of a docked (not floating) window insanely big,
    // so...)

    setStyleSheet("QDockWidget::float-button, QDockWidget::close-button { padding: 0px; }");
#endif
}

}
