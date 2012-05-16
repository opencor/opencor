//==============================================================================
// CellML element item delegate
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

class CellmlElementItemDelegate : public QStyledItemDelegate
{
public:
    explicit CellmlElementItemDelegate(QWidget *pParent,
                                       QStandardItemModel *pDataModel);

    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;

private:
    QStandardItemModel *mCellmlDataModel;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
