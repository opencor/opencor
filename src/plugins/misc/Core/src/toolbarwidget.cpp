//==============================================================================
// Tool bar widget
//==============================================================================

#include "toolbarwidget.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

ToolBarWidget::ToolBarWidget(QWidget *pParent) :
    QToolBar(pParent)
{
    // Remove the border which is normally drawn for a tool bar widget (indeed,
    // it doesn't look great when on a docked window, so...)

    setStyleSheet("QToolBar {"
                  "    border: 0px;"
                  "}");

    // Force the size of the icons to be 20 by 20 pixels

    setIconSize(QSize(20, 20));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
