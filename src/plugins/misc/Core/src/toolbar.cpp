//==============================================================================
// Toolbar
//==============================================================================

#include "toolbar.h"

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

ToolBar::ToolBar(QWidget *pParent) :
    QToolBar(pParent)
{
    // Remove the border which is normally drawn for a toolbar (indeed, it
    // doesn't look great when on a dock window, so...)

    setStyleSheet("QToolBar { border: 0px; }");

    // Force the size of the icons to be 20 by 20 pixels

    setIconSize(QSize(20, 20));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
