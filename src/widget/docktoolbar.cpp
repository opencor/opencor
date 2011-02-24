#include "docktoolbar.h"

DockToolBar::DockToolBar(QWidget *pParent) :
    QToolBar(pParent)
{
    // Remove the border that is normally drawn for a toolbar (indeed it doesn't
    // look great when on a dock window, so...)

    setStyleSheet("QToolBar { border: 0px }");

    // Set the dimension of the icons to 83% of their default size

    setIconSize(0.83*iconSize());
}
