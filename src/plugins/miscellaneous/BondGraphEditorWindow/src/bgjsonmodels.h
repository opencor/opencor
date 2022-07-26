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
#pragma once

#include <QAbstractItemModel>
#include <QIcon>

#include "thirdparty/json.hpp"

namespace OpenCOR {
namespace BondGraphEditorWindow {

using JSONType = nlohmann::detail::value_t;

class QJsonTreeItem
{
public:
    QJsonTreeItem(QJsonTreeItem *parent = nullptr);
    ~QJsonTreeItem();
    void appendChild(QJsonTreeItem *item);
    QJsonTreeItem *child(int row);
    QJsonTreeItem *parent();
    int childCount() const;
    int row() const;
    void setKey(const QString &key);
    void setValue(const QVariant &value);
    void setType(const JSONType &type);
    QString key() const;
    QVariant value() const;
    JSONType type() const;

    static QJsonTreeItem *load(const nlohmann::json &value,
                               QJsonTreeItem *parent = nullptr);

protected:
private:
    QString mKey;
    QVariant mValue;
    JSONType mType;
    QList<QJsonTreeItem *> mChilds;
    QJsonTreeItem *mParent;
};

class QJsonTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QJsonTreeModel(QObject *parent = nullptr, QString header1 = "Property",
                            QString header2 = "value(s)");
    QJsonTreeModel(const nlohmann::json &json, QObject *parent = nullptr,
                   QString header1 = "key", QString header2 = "value");
    ~QJsonTreeModel() override;
    bool loadJson(const nlohmann::json &json);
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const Q_DECL_OVERRIDE;
    QModelIndex
    index(int row, int column,
          const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const
        Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    nlohmann::json json() const;

private:
    nlohmann::json genJson(QJsonTreeItem *) const;
    QJsonTreeItem *mRootItem;
    QStringList mHeaders;
};

class QJsonTableModel : public QAbstractTableModel
{
public:
    using Heading = QMap<QString, QString>;
    using Header = QVector<Heading>;
    QJsonTableModel(const Header &header, QObject *parent = nullptr);

    bool setJson(const nlohmann::json &json);
    virtual nlohmann::json json() const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    int
    rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    int
    columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) Q_DECL_OVERRIDE;

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    Header m_header;
    nlohmann::json m_json;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR