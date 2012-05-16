//==============================================================================
// CellML annotation view lists widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWLISTSWIDGET_H
#define CELLMLANNOTATIONVIEWLISTSWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QModelIndex>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewListsWidget;
}

//==============================================================================

class QSplitter;
class QStandardItemModel;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class TreeView;
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewWidget;
class CellmlElementItem;
class CellmlElementItemDelegate;

//==============================================================================

class CellmlAnnotationViewListsWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewListsWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewListsWidget();

    virtual void retranslateUi();

    QList<int> verticalSplitterSizes() const;

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewListsWidget *mGui;

    QSplitter *mVerticalSplitter;

    Core::TreeView *mCellmlTreeView;
    QStandardItemModel *mCellmlDataModel;
    CellmlElementItemDelegate *mCellmlElementItemDelegate;

    Core::TreeView *mMetadataTreeView;
    QStandardItemModel *mMetadataDataModel;

    void retranslateCellmlDataItem(CellmlElementItem *pCellmlElementItem);

    void initTreeView(Core::TreeView *pTreeView, QStandardItemModel *pDataModel,
                      CellmlElementItemDelegate *pItemDelegate = 0);

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
    void verticalSplitterMoved(const QList<int> &pVerticalSizes);

public Q_SLOTS:
    void updateVerticalSplitter(const QList<int> &pVerticalSizes);

private Q_SLOTS:
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
