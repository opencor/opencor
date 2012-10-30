//==============================================================================
// CellML annotation view CellML list widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWCELLMLLISTWIDGET_H
#define CELLMLANNOTATIONVIEWCELLMLLISTWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QModelIndex>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewCellmlListWidget;
}

//==============================================================================

class QStandardItemModel;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class TreeViewWidget;
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewWidget;
class CellmlAnnotationViewCellmlElementItem;
class CellmlAnnotationViewCellmlElementItemDelegate;

//==============================================================================

class CellmlAnnotationViewCellmlListWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewCellmlListWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewCellmlListWidget();

    virtual void retranslateUi();

    Core::TreeViewWidget * treeViewWidget() const;

    CellmlAnnotationViewCellmlElementItem * currentCellmlElementItem() const;

private:
    CellMLSupport::CellmlFile *mCellmlFile;

    Ui::CellmlAnnotationViewCellmlListWidget *mGui;

    Core::TreeViewWidget *mTreeViewWidget;
    QStandardItemModel *mDataModel;
    CellmlAnnotationViewCellmlElementItemDelegate *mItemDelegate;

    QList<QModelIndex> mIndexes;

    void retranslateDataItem(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem);

    void populateDataModel();
    void populateUnitsDataModel(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem,
                                CellMLSupport::CellmlFileUnits *pUnits);
    void populateComponentReferenceDataModel(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem,
                                             CellMLSupport::CellmlFileComponentReference *pComponentReference);

    void indexExpandAll(const QModelIndex &pIndex) const;
    void indexCollapseAll(const QModelIndex &pIndex) const;

    bool indexIsAllExpanded(const QModelIndex &pIndex) const;

Q_SIGNALS:
    void metadataDetailsRequested(CellMLSupport::CellmlFileElement *pCellFileElement);

private Q_SLOTS:
    void resizeTreeViewToContents();

    void updateMetadataDetails(const QModelIndex &pNewIndex,
                               const QModelIndex &pOldIndex);

    void showCustomContextMenu(const QPoint &pPosition) const;

    void on_actionExpandAll_triggered();
    void on_actionCollapseAll_triggered();

    void on_actionRemoveCurrentMetadata_triggered();
    void on_actionRemoveAllMetadata_triggered();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
