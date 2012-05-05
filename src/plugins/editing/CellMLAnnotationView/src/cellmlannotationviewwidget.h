//==============================================================================
// CellML annotation view widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWWIDGET_H
#define CELLMLANNOTATIONVIEWWIDGET_H

//==============================================================================

#include "cellmlfile.h"
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
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlItemDelegate : public QStyledItemDelegate
{
public:
    explicit CellmlItemDelegate(QWidget *pParent,
                                QStandardItemModel *pDataModel);

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
        None            = QStandardItem::UserType,
        Error           = QStandardItem::UserType+1,
        Warning         = QStandardItem::UserType+2,
        Category        = QStandardItem::UserType+3,
        Model           = QStandardItem::UserType+4,
        Import          = QStandardItem::UserType+5,
        Unit            = QStandardItem::UserType+6,
        UnitElement     = QStandardItem::UserType+7,
        Component       = QStandardItem::UserType+8,
        Variable        = QStandardItem::UserType+9,
        MathmlElement   = QStandardItem::UserType+10,
        Group           = QStandardItem::UserType+11,
        RelationshipRef = QStandardItem::UserType+12,
        ComponentRef    = QStandardItem::UserType+13,
        Connection      = QStandardItem::UserType+14,
        Metadata        = QStandardItem::UserType+15
    };

    explicit CellmlElementItem(CellMLSupport::CellmlFileModel *pModel);
    explicit CellmlElementItem(CellMLSupport::CellmlFileImport *pImport);
    explicit CellmlElementItem(CellMLSupport::CellmlFileUnit *pUnit);
    explicit CellmlElementItem(CellMLSupport::CellmlFileUnitElement *pUnitElement);
    explicit CellmlElementItem(CellMLSupport::CellmlFileComponent *pComponent);
    explicit CellmlElementItem(const Type &pType, const QString &pText);
    explicit CellmlElementItem(const Type &pType, const Type &pSubType,
                               const QString &pText);

    virtual int type() const;
    int subType() const;

    CellMLSupport::CellmlFileElement * element() const;

private:
    Type mType;
    Type mSubType;

    CellMLSupport::CellmlFileElement *mElement;

    void initialize(const Type &pType, const Type &pSubType,
                    const QString &pText,
                    CellMLSupport::CellmlFileElement *pElement = 0);
};

//==============================================================================

class CellmlAnnotationViewDetailsWidget;

//==============================================================================

class CellmlAnnotationViewWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewWidget(QWidget *pParent,
                                        const QString &pFileName);
    ~CellmlAnnotationViewWidget();

    virtual void retranslateUi();

    QList<int> horizontalSplitterSizes() const;
    QList<int> verticalSplitterSizes() const;

private:
    Ui::CellmlAnnotationViewWidget *mUi;

    QSplitter *mHorizontalSplitter;
    QSplitter *mVerticalSplitter;

    Core::TreeView *mCellmlTreeView;
    QStandardItemModel *mCellmlDataModel;
    CellmlItemDelegate *mCellmlItemDelegate;

    Core::TreeView *mMetadataTreeView;
    QStandardItemModel *mMetadataDataModel;

    CellmlAnnotationViewDetailsWidget *mDetails;

    QChar mRightArrow;

    void retranslateCellmlDataItem(CellmlElementItem *pCellmlElementItem);

    void initTreeView(Core::TreeView *pTreeView);

    void populateCellmlDataModel(const QString &pFileName);
    void populateUnitsDataModel(CellmlElementItem *pCellmlElementItem,
                                const CellMLSupport::CellmlFileUnits pUnits);
    void populateComponentRefDataModel(CellmlElementItem *pCellmlElementItem,
                                       CellMLSupport::CellmlFileComponentRef *pComponentRef);

    void populateMetadataDataModel(const QString &pFileName);

Q_SIGNALS:
    void horizontalSplitterMoved(const QList<int> &pHorizontalSizes);
    void verticalSplitterMoved(const QList<int> &pVerticalSizes);

public Q_SLOTS:
    void updateHorizontalSplitter(const QList<int> &pHorizontalSizes);
    void updateVerticalSplitter(const QList<int> &pVerticalSizes);

private Q_SLOTS:
    void emitHorizontalSplitterMoved();
    void emitVerticalSplitterMoved();

    void resizeCellmlTreeViewToContents();

    void updateCellmlNode(const QModelIndex &pNewIndex,
                          const QModelIndex &pOldIndex);
    void updateMetadataNode(const QModelIndex &pNewIndex,
                            const QModelIndex &pOldIndex);
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
