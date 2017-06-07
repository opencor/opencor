/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// CellML Annotation view CellML list widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmlfile.h"
#include "widget.h"

//==============================================================================

#include <QModelIndex>
#include <QStandardItem>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>

//==============================================================================

class QPainter;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class TreeViewWidget;
}   // namespace Core

//==============================================================================

namespace CellMLAnnotationView {

//==============================================================================

class CellmlAnnotationViewEditingWidget;

//==============================================================================

class CellmlAnnotationViewCellmlElementItemDelegate : public QStyledItemDelegate
{
public:
    explicit CellmlAnnotationViewCellmlElementItemDelegate(QObject *pParent);

    virtual void paint(QPainter *pPainter, const QStyleOptionViewItem &pOption,
                       const QModelIndex &pIndex) const;
};

//==============================================================================

class CellmlAnnotationViewCellmlElementItem : public QStandardItem
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

    explicit CellmlAnnotationViewCellmlElementItem(const bool &pError, const QString &pText);
    explicit CellmlAnnotationViewCellmlElementItem(const Type &pType, const QString &pText);
    explicit CellmlAnnotationViewCellmlElementItem(const Type &pType,
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

    void setIcon(const Type &pType);

    explicit CellmlAnnotationViewCellmlElementItem(const bool &pCategory,
                                                   const Type &pType,
                                                   const QString &pText,
                                                   iface::cellml_api::CellMLElement *pElement,
                                                   const int &pNumber);
};

//==============================================================================

class CellmlAnnotationViewCellmlListWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit CellmlAnnotationViewCellmlListWidget(CellmlAnnotationViewEditingWidget *pParent);

    virtual void retranslateUi();

    Core::TreeViewWidget * treeViewWidget() const;

private:
    CellMLSupport::CellmlFile *mCellmlFile;

    QStandardItemModel *mTreeViewModel;
    Core::TreeViewWidget *mTreeViewWidget;

    QAction *mExpandAllAction;
    QAction *mCollapseAllAction;
    QAction *mRemoveCurrentMetadataAction;
    QAction *mRemoveAllMetadataAction;
    QAction *mOpenImportAction;

    void retranslateDataItem(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem);

    void initializeTreeViewWidget(const bool &pSelectFirstItem = true);

    void populateModel();
    void populateUnitsModel(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem,
                            iface::cellml_api::UnitsSet *pUnitsSet);
    void populateGroupComponentReferenceModel(CellmlAnnotationViewCellmlElementItem *pCellmlElementItem,
                                              iface::cellml_api::ComponentRef *pGroupComponentReference);

    CellmlAnnotationViewCellmlElementItem * currentCellmlElementItem() const;

    void indexExpandAll(const QModelIndex &pIndex) const;
    void indexCollapseAll(const QModelIndex &pIndex) const;

    bool indexIsAllExpanded(const QModelIndex &pIndex) const;

signals:
    void metadataDetailsRequested(iface::cellml_api::CellMLElement *pCellElement);

private slots:
    void expandAll();
    void collapseAll();
    void removeCurrentMetadata();
    void removeAllMetadata();
    void openImport();

    void resizeTreeViewToContents();

    void updateMetadataDetails(const QModelIndex &pNewIndex,
                               const QModelIndex &pOldIndex);

    void showCustomContextMenu(const QPoint &pPosition) const;
};

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
