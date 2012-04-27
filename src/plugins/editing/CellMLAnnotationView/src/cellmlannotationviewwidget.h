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

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class TreeView;
}

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlItemDelegate : public QStyledItemDelegate
{
public:
    explicit CellmlItemDelegate(QStandardItemModel *pDataModel);

    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;

private:
    QStandardItemModel *mCellmlDataModel;
};

//==============================================================================

class CellmlElementItem : public QStandardItem
{
public:
    enum Type
    {
        Error           = QStandardItem::UserType,
        Warning         = QStandardItem::UserType+1,
        Category        = QStandardItem::UserType+2,
        Model           = QStandardItem::UserType+3,
        Import          = QStandardItem::UserType+4,
        Unit            = QStandardItem::UserType+5,
        UnitElement     = QStandardItem::UserType+6,
        Component       = QStandardItem::UserType+7,
        Variable        = QStandardItem::UserType+8,
        MathmlElement   = QStandardItem::UserType+9,
        Group           = QStandardItem::UserType+10,
        RelationshipRef = QStandardItem::UserType+11,
        ComponentRef    = QStandardItem::UserType+12,
        Connection      = QStandardItem::UserType+13
    };

    explicit CellmlElementItem(const Type &pType, const QString &pText);
    explicit CellmlElementItem(const Type &pType, const Type &pSubType,
                               const QString &pText);

    virtual int type() const;

private:
    Type mType;

    void initialize(const Type &pType, const Type &pSubType,
                    const QString &pText);
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
    QSplitter *mVerticalSplitter;

    Core::TreeView *mCellmlTreeView;
    QStandardItemModel *mCellmlDataModel;
    CellmlItemDelegate *mCellmlItemDelegate;

    Core::TreeView *mMetadataTreeView;

    QTextEdit *mDebugOutput;

    QChar mRightArrow;

    void initTreeView(Core::TreeView *pTreeView);

    void populateCellmlTreeView(const QString &pFileName);
    void populateUnitsTreeView(CellmlElementItem *pCellmlElementItem,
                               const CellMLSupport::CellmlFileUnits pUnits);
    void populateComponentRefTreeView(CellmlElementItem *pCellmlElementItem,
                                      CellMLSupport::CellmlFileComponentRef *pComponentRef);

Q_SIGNALS:
    void horizontalSplitterMoved(const QList<int> &pSizes);

public Q_SLOTS:
    void updateHorizontalSplitter(const QList<int> &pSizes);

private Q_SLOTS:
    void emitHorizontalSplitterMoved();

    void resizeToContents();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
