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
        None                  = QStandardItem::UserType,
        Error                 = QStandardItem::UserType+1,
        Warning               = QStandardItem::UserType+2,
        Model                 = QStandardItem::UserType+3,
        Import                = QStandardItem::UserType+4,
        ImportUnit            = QStandardItem::UserType+5,
        ImportComponent       = QStandardItem::UserType+6,
        Unit                  = QStandardItem::UserType+7,
        UnitElement           = QStandardItem::UserType+8,
        Component             = QStandardItem::UserType+9,
        Variable              = QStandardItem::UserType+10,
        Group                 = QStandardItem::UserType+11,
        RelationshipReference = QStandardItem::UserType+12,
        ComponentReference    = QStandardItem::UserType+13,
        Connection            = QStandardItem::UserType+14,
        ComponentMapping      = QStandardItem::UserType+15,
        VariableMapping       = QStandardItem::UserType+16
    };

    explicit CellmlElementItem(const bool &pError, const QString &pText);
    explicit CellmlElementItem(const Type &pType, const QString &pText);
    explicit CellmlElementItem(const Type &pType,
                               CellMLSupport::CellmlFileElement *pElement,
                               const QString &pText = QString());

    bool isCategory() const;
    virtual int type() const;

    CellMLSupport::CellmlFileElement * element() const;

private:
    bool mCategory;
    Type mType;

    CellMLSupport::CellmlFileElement *mElement;

    void setIcon(const Type &pType);
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
    Ui::CellmlAnnotationViewWidget *mGui;

    QSplitter *mHorizontalSplitter;
    QSplitter *mVerticalSplitter;

    Core::TreeView *mCellmlTreeView;
    QStandardItemModel *mCellmlDataModel;
    CellmlItemDelegate *mCellmlItemDelegate;

    Core::TreeView *mMetadataTreeView;
    QStandardItemModel *mMetadataDataModel;

    CellmlAnnotationViewDetailsWidget *mDetails;

    CellMLSupport::CellmlFile *mCellmlFile;

    void retranslateCellmlDataItem(CellmlElementItem *pCellmlElementItem);

    void initTreeView(Core::TreeView *pTreeView, QStandardItemModel *pDataModel,
                      CellmlItemDelegate *pItemDelegate = 0);

    void populateCellmlDataModel();
    void populateUnitsDataModel(CellmlElementItem *pCellmlElementItem,
                                const CellMLSupport::CellmlFileUnits pUnits);
    void populateComponentReferenceDataModel(CellmlElementItem *pCellmlElementItem,
                                             CellMLSupport::CellmlFileComponentReference *pComponentReference);

    void populateMetadataDataModel();

    void indexExpandAll(const QModelIndex &pIndex) const;
    void indexCollapseAll(const QModelIndex &pIndex) const;

    bool indexIsAllExpanded(const QModelIndex &pIndex) const;

    void addRdfTriple(CellMLSupport::CellmlFileRdfTriples &pRdfTriples,
                      CellMLSupport::CellmlFileRdfTriple *pRdfTriple);

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

    void cellmlTreeViewContextMenu(const QPoint &pPosition) const;

    void on_actionExpandAll_triggered();
    void on_actionCollapseAll_triggered();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
