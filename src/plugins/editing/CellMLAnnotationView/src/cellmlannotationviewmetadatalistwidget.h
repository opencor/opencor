//==============================================================================
// CellML annotation view metadata list widget
//==============================================================================

#ifndef CELLMLANNOTATIONVIEWMETADATALISTWIDGET_H
#define CELLMLANNOTATIONVIEWMETADATALISTWIDGET_H

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QModelIndex>
#include <QSplitter>

//==============================================================================

namespace Ui {
    class CellmlAnnotationViewMetadataListWidget;
}

//==============================================================================

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

class CellmlAnnotationViewMetadataListWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewMetadataListWidget(CellmlAnnotationViewWidget *pParent);
    ~CellmlAnnotationViewMetadataListWidget();

    virtual void retranslateUi();

    Core::TreeView * treeView() const;

    QStringList ids() const;

protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);

private:
    CellmlAnnotationViewWidget *mParent;

    Ui::CellmlAnnotationViewMetadataListWidget *mGui;

    Core::TreeView *mTreeView;
    QStandardItemModel *mDataModel;

    QList<QModelIndex> mIndexes;

    void populateDataModel();

private Q_SLOTS:
    void updateActions();
    void updateNode(const QModelIndex &pNewIndex, const QModelIndex &pOldIndex);

    void on_actionAddMetadata_triggered();

    void on_actionRemoveMetadata_triggered();
    void on_actionRemoveCurrentMetadata_triggered();
    void on_actionRemoveAllMetadata_triggered();

    void on_actionClearMetadata_triggered();
    void on_actionClearCurrentMetadata_triggered();
    void on_actionClearAllMetadata_triggered();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
