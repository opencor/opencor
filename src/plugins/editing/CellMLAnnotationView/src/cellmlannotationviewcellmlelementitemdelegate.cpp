//==============================================================================
// CellML annotation view CellML element item delegate
//==============================================================================

#include "cellmlannotationviewcellmlelementitem.h"
#include "cellmlannotationviewcellmlelementitemdelegate.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

void CellmlAnnotationViewCellmlElementItemDelegate::paint(QPainter *pPainter,
                                                          const QStyleOptionViewItem &pOption,
                                                          const QModelIndex &pIndex) const
{
    // Paint the item as normal, except for the items which are not checkable
    // (i.e. plugins which the user cannot decide whether to load) in which case
    // we paint them as if they were disabled

    CellmlAnnotationViewCellmlElementItem *cellmlElementItem = static_cast<CellmlAnnotationViewCellmlElementItem *>(static_cast<const QStandardItemModel *>(pIndex.model())->itemFromIndex(pIndex));

    QStyleOptionViewItemV4 option(pOption);

    initStyleOption(&option, pIndex);

    if (   (cellmlElementItem->type() == CellmlAnnotationViewCellmlElementItem::Error)
        || (cellmlElementItem->type() == CellmlAnnotationViewCellmlElementItem::Warning)
        || cellmlElementItem->isCategory()) {
        // This is an error/warning/category item, so prevent it from being
        // hoverable. Otherwise, show the item enabled since it's actually
        // disabled (so we can't select it), yet we want to see as if it was
        // enabled...

        option.state &= ~QStyle::State_MouseOver;
        option.state |=  QStyle::State_Enabled;

        // In the case of a category, we make the font bold

        option.font.setBold(cellmlElementItem->isCategory());
    }

    QStyledItemDelegate::paint(pPainter, option, pIndex);
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
