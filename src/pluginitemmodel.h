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

#pragma once

//==============================================================================

#include <QAbstractItemModel>
#include <QIcon>

//==============================================================================

namespace OpenCOR {

//==============================================================================

class PluginItemModel;

//==============================================================================

class PluginItem
{
public:
    explicit PluginItem(const QIcon &pIcon, const QString &pName);
    explicit PluginItem(const QString &pName = QString());
    ~PluginItem();

    void setParentAndModel(PluginItem *pParent, PluginItemModel *pModel);

    PluginItem * parent() const;
    PluginItem * child(int pIndex) const;

    int childCount() const;
    bool hasChildren() const;

    int index() const;

    QModelIndex modelIndex() const;

    bool hasCheckBox() const;
    void setHasCheckBox(bool pHasCheckBox);

    bool isCheckable() const;
    void setCheckable(bool pCheckable);

    Qt::CheckState checkState() const;
    void setCheckState(Qt::CheckState pCheckState);

    QIcon icon() const;
    QString name() const;

    void insert(int pIndex, PluginItem *pItem);
    void append(PluginItem *pItem);

private:
    PluginItemModel *mModel = nullptr;

    PluginItem *mParent = nullptr;
    QVector<PluginItem *> mChildren;

    bool mHasCheckBox = false;
    bool mCheckable = false;
    Qt::CheckState mCheckState;

    QIcon mIcon;
    QString mName;
};

//==============================================================================

class PluginItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit PluginItemModel(QObject *pParent);
    ~PluginItemModel() override;

    QModelIndex index(int pRow, int pColumn,
                      const QModelIndex &pParent = {}) const override;
    QModelIndex parent(const QModelIndex &pChild) const override;
    int rowCount(const QModelIndex &pParent = {}) const override;
    int columnCount(const QModelIndex &pParent = {}) const override;
    QVariant data(const QModelIndex &pIndex,
                  int pRole = Qt::DisplayRole) const override;

    PluginItem * invisibleRootItem() const;

    PluginItem * itemFromIndex(const QModelIndex &pIndex) const;

    QModelIndex createIndex(int pRow, PluginItem *pParent) const;

private:
    PluginItem *mRootItem;
};

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
