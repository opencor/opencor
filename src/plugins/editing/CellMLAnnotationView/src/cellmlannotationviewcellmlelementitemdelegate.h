//==============================================================================
// CellML annotation view CellML element item delegate
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWCELLMLELEMENTITEMDELEGATE_H
#define CELLMLANNOTATIONVIEWCELLMLELEMENTITEMDELEGATE_H

//==============================================================================

#include <QStyledItemDelegate>

//==============================================================================

class QStandardItemModel;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewCellmlElementItemDelegate : public QStyledItemDelegate
{
public:
    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
