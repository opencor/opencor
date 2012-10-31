//==============================================================================
// Property editor widget
//==============================================================================

#include "propertyeditorwidget.h"

//==============================================================================

#include <QKeyEvent>
#include <QStandardItem>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

PropertyEditorWidget::PropertyEditorWidget(QWidget *pParent) :
    TreeViewWidget(pParent)
{
    // Customise ourself

    setFrameShape(QFrame::NoFrame);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

//==============================================================================

QStandardItem * PropertyEditorWidget::newNonEditableItem()
{
    // Create and return a non-editable item

    QStandardItem *res = new QStandardItem();

    res->setFlags(res->flags() & ~Qt::ItemIsEditable);

    return res;
}

//==============================================================================

QStandardItem * PropertyEditorWidget::newEditableItem()
{
    // Create and return an editable item

    return new QStandardItem();
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
