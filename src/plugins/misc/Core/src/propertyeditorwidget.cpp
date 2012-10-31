//==============================================================================
// Property editor widget
//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

#include <QKeyEvent>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(QWidget *pParent) :
    TreeViewWidget(pParent)
{
}

//==============================================================================

void PropertyEditorWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Check for some key combinations and ignore them, if needed

    if (   (pEvent->modifiers() & Qt::ControlModifier)
        && (pEvent->key() == Qt::Key_A))
        // The user wants to select everything which we don't want to allow,
        // so...

        pEvent->ignore();
    else
        // Not a key combination which we don't want, so...

        TreeViewWidget::keyPressEvent(pEvent);
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
