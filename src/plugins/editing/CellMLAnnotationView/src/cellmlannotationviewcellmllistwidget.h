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
#include <QStandardItem>
#include <QStyledItemDelegate>

//==============================================================================

namespace Ui {
    class CellMLAnnotationViewCellMLListWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class TreeViewWidget;
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellMLAnnotationViewWidget;

//==============================================================================

class CellMLAnnotationViewCellMLElementItemDelegate : public QStyledItemDelegate
{
public:
    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;
};

//==============================================================================

class CellMLAnnotationViewCellMLElementItem : public QStandardItem
{
public:
    enum Type {
        Error                 = QStandardItem::UserType,
        Warning               = QStandardItem::UserType+1,
        Model                 = QStandardItem::UserType+2,
        Import                = QStandardItem::UserType+3,
        ImportUnit            = QStandardItem::UserType+4,
        ImportComponent       = QStandardItem::UserType+5,
        Unit                  = QStandardItem::UserType+6,
        UnitElement           = QStandardItem::UserType+7,
        Component             = QStandardItem::UserType+8,
        Variable              = QStandardItem::UserType+9,
        Group                 = QStandardItem::UserType+10,
        RelationshipReference = QStandardItem::UserType+11,
        ComponentReference    = QStandardItem::UserType+12,
        Connection            = QStandardItem::UserType+13,
        ComponentMapping      = QStandardItem::UserType+14,
        VariableMapping       = QStandardItem::UserType+15
    };

    explicit CellMLAnnotationViewCellMLElementItem(const bool &pError, const QString &pText);
    explicit CellMLAnnotationViewCellMLElementItem(const Type &pType, const QString &pText);
    explicit CellMLAnnotationViewCellMLElementItem(const Type &pType,
                                                   iface::cellml_api::CellMLElement *pElement,
                                                   const int pNumber = -1);

    bool isCategory() const;
    virtual int type() const;
    int number() const;

    iface::cellml_api::CellMLElement * element() const;

private:
    bool mCategory;
    Type mType;

    ObjRef<iface::cellml_api::CellMLElement> mElement;

    int mNumber;

    void constructor(const bool &pCategory, const Type &pType,
                     iface::cellml_api::CellMLElement *pElement,
                     const int &pNumber);

    void setIcon(const Type &pType);
};

//==============================================================================

class CellMLAnnotationViewCellMLListWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellMLAnnotationViewCellMLListWidget(CellMLAnnotationViewWidget *pParent);
    ~CellMLAnnotationViewCellMLListWidget();

    virtual void retranslateUi();

    Core::TreeViewWidget * treeViewWidget() const;

    CellMLAnnotationViewCellMLElementItem * currentCellMLElementItem() const;

private:
    CellMLSupport::CellMLFile *mCellMLFile;

    Ui::CellMLAnnotationViewCellMLListWidget *mGui;

    Core::TreeViewWidget *mTreeViewWidget;
    QStandardItemModel *mModel;
    CellMLAnnotationViewCellMLElementItemDelegate *mItemDelegate;

    QList<QModelIndex> mIndexes;

    void retranslateDataItem(CellMLAnnotationViewCellMLElementItem *pCellMLElementItem);

    void populateModel();
    void populateUnitsModel(CellMLAnnotationViewCellMLElementItem *pCellMLElementItem,
                            iface::cellml_api::UnitsSet *pUnits);
    void populateGroupComponentReferenceModel(CellMLAnnotationViewCellMLElementItem *pCellMLElementItem,
                                              iface::cellml_api::ComponentRef *pGroupComponentReference);

    void indexExpandAll(const QModelIndex &pIndex) const;
    void indexCollapseAll(const QModelIndex &pIndex) const;

    bool indexIsAllExpanded(const QModelIndex &pIndex) const;

Q_SIGNALS:
    void metadataDetailsRequested(iface::cellml_api::CellMLElement *pCellFileElement);

private Q_SLOTS:
    void resizeTreeViewToContents();

    void updateMetadataDetails(const QModelIndex &pNewIndex,
                               const QModelIndex &pOldIndex);

    void showCustomContextMenu(const QPoint &pPosition) const;

    void on_actionExpandAll_triggered();
    void on_actionCollapseAll_triggered();

    void on_actionRemoveCurrentMetadata_triggered();
    void on_actionRemoveAllMetadata_triggered();

    void on_actionOpenImport_triggered();
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
