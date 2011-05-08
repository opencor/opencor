#include "docktoolbar.h"

namespace OpenCOR {

DockToolBar::DockToolBar(QWidget *pParent) :
    QToolBar(pParent)
{
    // Remove the border that is normally drawn for a toolbar (indeed it doesn't
    // look great when on a dock window, so...)

    setStyleSheet("QToolBar { border: 0px; }");

    // Set the size of the icons to 20 by 20 pixels

    setIconSize(QSize(20, 20));
}

}
