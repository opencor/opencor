//==============================================================================
// CellML annotation view widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWWIDGET_H
#define CELLMLANNOTATIONVIEWWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QModelIndex>

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

class CellmlAnnotationViewDetailsWidget;
class CellmlElementItem;
class CellmlElementItemDelegate;

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

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);

private:
    Ui::CellmlAnnotationViewWidget *mGui;

    QSplitter *mHorizontalSplitter;
    QSplitter *mVerticalSplitter;

    Core::TreeView *mCellmlTreeView;
    QStandardItemModel *mCellmlDataModel;
    CellmlElementItemDelegate *mCellmlElementItemDelegate;

    Core::TreeView *mMetadataTreeView;
    QStandardItemModel *mMetadataDataModel;

    CellmlAnnotationViewDetailsWidget *mDetails;

    CellMLSupport::CellmlFile *mCellmlFile;

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
