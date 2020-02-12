/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Plugin item model
//==============================================================================

#include "pluginitemmodel.h"

//==============================================================================

namespace OpenCOR {

//==============================================================================

PluginItem::PluginItem(const QIcon &pIcon, const QString &pName)
    : mIcon(pIcon),
      mName(pName)
{
}

//==============================================================================

PluginItem::PluginItem(const QString &pName)
    : PluginItem(QIcon(), pName)
{
}

//==============================================================================

PluginItem::~PluginItem()
{
    // Delete some internal objects

    qDeleteAll(mChildren);
}

//==============================================================================

void PluginItem::setParentAndModel(PluginItem *pParent, PluginItemModel *pModel)
{
    // Set our parent and our model

    mParent = pParent;
    mModel = pModel;
}

//==============================================================================

PluginItem * PluginItem::parent() const
{
    // Return our parent

    return mParent;
}

//==============================================================================

PluginItem * PluginItem::child(int pIndex) const
{
    // Return the requested child

    return mChildren.value(pIndex);
}

//==============================================================================

int PluginItem::childCount() const
{
    // Return our number of children

    return mChildren.count();
}

//==============================================================================

bool PluginItem::hasChildren() const
{
    // Return whether we have children

    return !mChildren.isEmpty();
}

//==============================================================================

int PluginItem::index() const
{
    // Return our index

    if (mParent != nullptr) {
        return mParent->mChildren.indexOf(const_cast<PluginItem *>(this));
    }

    return -1;
}

//==============================================================================

QModelIndex PluginItem::modelIndex() const
{
    // Return our model index

    return mModel->createIndex(index(), mParent);
}

//==============================================================================

bool PluginItem::hasCheckBox() const
{
    // Return whether we have a check box

    return mHasCheckBox;
}

//==============================================================================

void PluginItem::setHasCheckBox(bool pHasCheckBox)
{
    // Specify whether we have a check box

    mHasCheckBox = pHasCheckBox;
}

//==============================================================================

bool PluginItem::isCheckable() const
{
    // Return whether we are checkable

    return mCheckable;
}

//==============================================================================

void PluginItem::setCheckable(bool pCheckable)
{
    // Specify whether we are checkable

    mCheckable = pCheckable;
}

//==============================================================================

Qt::CheckState PluginItem::checkState() const
{
    // Return our check state

    return mCheckState;
}

//==============================================================================

void PluginItem::setCheckState(Qt::CheckState pCheckState)
{
    // Set our check state

    mCheckState = pCheckState;
}

//==============================================================================

QIcon PluginItem::icon() const
{
    // Return our icon

    return mIcon;
}

//==============================================================================

QString PluginItem::name() const
{
    // Return our name

    return mName;
}

//==============================================================================

void PluginItem::insert(int pIndex, PluginItem *pItem)
{
    // Insert the given item before the given index in our list of children

    pItem->setParentAndModel(this, mModel);

    mChildren.insert(pIndex, pItem);
}

//==============================================================================

void PluginItem::append(PluginItem *pItem)
{
    // Append the given item to our list of children

    pItem->setParentAndModel(this, mModel);

    mChildren << pItem;
}

//==============================================================================

PluginItemModel::PluginItemModel(QObject *pParent)
    : QAbstractItemModel(pParent),
      mRootItem(new PluginItem())
{
    mRootItem->setParentAndModel(nullptr, this);
}

//==============================================================================

PluginItemModel::~PluginItemModel()
{
    // Delete some internal objects

    delete mRootItem;
}

//==============================================================================

QModelIndex PluginItemModel::index(int pRow, int pColumn,
                                   const QModelIndex &pParent) const
{
    Q_UNUSED(pColumn)

    // Return the index of the item at the given row in the given parent

    if ((pRow < 0) || (pRow >= rowCount(pParent))) {
        return {};
    }

    PluginItem *childItem = (pParent.isValid()?
                                 static_cast<PluginItem*>(pParent.internalPointer()):
                                 mRootItem)->child(pRow);

    if (childItem != nullptr) {
        return createIndex(pRow, childItem);
    }

    return {};
}

//==============================================================================

QModelIndex PluginItemModel::parent(const QModelIndex &pChild) const
{
    // Return the parent of the given child

    if (pChild.isValid()) {
        PluginItem *parentItem = static_cast<PluginItem *>(pChild.internalPointer())->parent();

        if ((parentItem != nullptr) && (parentItem != mRootItem)) {
            return createIndex(parentItem->index(), parentItem);
        }
    }

    return {};
}

//==============================================================================

int PluginItemModel::rowCount(const QModelIndex &pParent) const
{
    // Return the number of rows for the given parent

    if (pParent.isValid()) {
        return static_cast<PluginItem *>(pParent.internalPointer())->childCount();
    }

    return mRootItem->childCount();
}

//==============================================================================

int PluginItemModel::columnCount(const QModelIndex &pParent) const
{
    Q_UNUSED(pParent)

    // Return the number of columns for the given parent, i.e. always one

    return 1;
}

//==============================================================================

QVariant PluginItemModel::data(const QModelIndex &pIndex, int pRole) const
{
    // Return the data for the given index

    if (pIndex.isValid()) {
        if (pRole == Qt::DisplayRole) {
            return static_cast<PluginItem *>(pIndex.internalPointer())->name();
        }

        if (pRole == Qt::CheckStateRole) {
            PluginItem *item = static_cast<PluginItem *>(pIndex.internalPointer());

            if (item->hasCheckBox()) {
                return static_cast<PluginItem *>(pIndex.internalPointer())->checkState();
            }
        }

        if (pRole == Qt::DecorationRole) {
            return static_cast<PluginItem *>(pIndex.internalPointer())->icon();
        }
    }

    return {};
}

//==============================================================================

PluginItem * PluginItemModel::invisibleRootItem() const
{
    // Return our invisible root item

    return mRootItem;
}

//==============================================================================

PluginItem * PluginItemModel::itemFromIndex(const QModelIndex &pIndex) const
{
    // Return the item at the given index

    if (pIndex.isValid()) {
        return static_cast<PluginItem *>(pIndex.internalPointer());
    }

    return nullptr;
}

//==============================================================================

QModelIndex PluginItemModel::createIndex(int pRow, PluginItem *pParent) const
{
    // Create and return the inde for the given row and parent

    return QAbstractItemModel::createIndex(pRow, 0, pParent);
}

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
