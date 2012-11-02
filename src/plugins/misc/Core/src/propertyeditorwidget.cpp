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

//    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

//==============================================================================

void PropertyEditorWidget::initialize(QStandardItemModel *pModel)
{
    // Stop tracking the change of property, if needed

    if (selectionModel())
        disconnect(selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                   this, SLOT(editProperty(const QModelIndex &, const QModelIndex &)));

    // Update our model

    setModel(pModel);

    // Keep track of the change of property
    // Note: the idea is to automatically start the editing of a property...

    connect(selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(editProperty(const QModelIndex &, const QModelIndex &)));
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
    // Check some key combinations

    if (   (pEvent->modifiers() & Qt::ControlModifier)
        && (pEvent->key() == Qt::Key_A))
        // The user wants to select everything which we don't want to allow,
        // so...

        pEvent->ignore();
    else
        // Not a key combination we handle, so...

        TreeViewWidget::keyPressEvent(pEvent);
}

//==============================================================================

void PropertyEditorWidget::editProperty(const QModelIndex &pNewItem,
                                        const QModelIndex &pOldItem)
{
    Q_UNUSED(pOldItem);

    // Edit the current property (which value is always in column 1)

    edit(model()->index(pNewItem.row(), 1));
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
