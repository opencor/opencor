//==============================================================================
// CellML annotation view widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWWIDGET_H
#define CELLMLANNOTATIONVIEWWIDGET_H

//==============================================================================

#include "cellmlfilecomponentref.h"
#include "cellmlfileunit.h"
#include "widget.h"

//==============================================================================

#include <QStandardItem>
#include <QStyledItemDelegate>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewWidget;
}

//==============================================================================

class QSplitter;
class QStandardItemModel;
class QTextEdit;
class QTreeView;

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

class CellmlElementDelegate : public QStyledItemDelegate
{
public:
    explicit CellmlElementDelegate(QStandardItemModel *pDataModel);

    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;

private:
    QStandardItemModel *mDataModel;
};

//==============================================================================

class CellmlElementItem : public QStandardItem
{
public:
    enum Type
    {
        Error = QStandardItem::UserType,
        Category = QStandardItem::UserType+1,
        Item = QStandardItem::UserType+2
    };

    explicit CellmlElementItem(const Type &pType, const QString &pText);

    virtual int type() const;

private:
    Type mType;
};

//==============================================================================

class CellmlAnnotationViewWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewWidget(QWidget *pParent,
                                        const QString &pFileName);
    ~CellmlAnnotationViewWidget();

    QList<int> horizontalSplitterSizes() const;

private:
    Ui::CellmlAnnotationViewWidget *mUi;

    QSplitter *mHorizontalSplitter;

    QTreeView *mTreeView;
    QStandardItemModel *mDataModel;
    CellmlElementDelegate *mCellmlElementDelegate;

    QTextEdit *mDebugOutput;

    void initTreeView(const QString &pFileName);
    void initUnitsTreeView(const QString &pLeadingSpace,
                           const CellMLSupport::CellmlFileUnits pUnits);
    void initComponentRefTreeView(const QString &pLeadingSpace,
                                  CellMLSupport::CellmlFileComponentRef *pComponentRef);

Q_SIGNALS:
    void horizontalSplitterMoved(const QList<int> &pSizes);

public Q_SLOTS:
    void updateHorizontalSplitter(const QList<int> &pSizes);

private Q_SLOTS:
    void emitHorizontalSplitterMoved();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
