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
#include "bgjsonmodels.h"
#include <QDebug>
#include <QFile>
#include <QFont>

namespace OpenCOR {
namespace BondGraphUI {

QJsonTreeItem::QJsonTreeItem(QJsonTreeItem *parent) { mParent = parent; }

QJsonTreeItem::~QJsonTreeItem() { qDeleteAll(mChilds); }

void QJsonTreeItem::appendChild(QJsonTreeItem *item) { mChilds.append(item); }

QJsonTreeItem *QJsonTreeItem::child(int row) { return mChilds.value(row); }

QJsonTreeItem *QJsonTreeItem::parent() { return mParent; }

int QJsonTreeItem::childCount() const { return mChilds.count(); }

int QJsonTreeItem::row() const {
  if (mParent)
    return mParent->mChilds.indexOf(const_cast<QJsonTreeItem *>(this));

  return 0;
}

void QJsonTreeItem::setKey(const QString &key) { mKey = key; }

void QJsonTreeItem::setValue(const QVariant &value) { mValue = value; }

void QJsonTreeItem::setType(const JSONType &type) { mType = type; }

QString QJsonTreeItem::key() const { return mKey; }

QVariant QJsonTreeItem::value() const { return mValue; }

JSONType QJsonTreeItem::type() const { return mType; }

QJsonTreeItem *QJsonTreeItem::load(const nlohmann::json &value,
                                   QJsonTreeItem *parent) {

  QJsonTreeItem *m_rootItem = new QJsonTreeItem(parent);
  m_rootItem->setKey("root");

  if (value.is_object()) {

    // Get all QJsonValue childs
    for (auto &el : value.items()) {
      QJsonTreeItem *child = load(el.value(), m_rootItem);
      child->setKey(QString(el.key().c_str()));
      child->setType(el.value().type());
      m_rootItem->appendChild(child);
    }
  } else if (value.is_array()) {
    // Get all QJsonValue childs
    int index = 0;
    for (auto v : value) {
      QJsonTreeItem *child = load(v, m_rootItem);
      child->setKey(QString::number(index));
      child->setType(v.type());
      m_rootItem->appendChild(child);
      ++index;
    }
  } else if (value.is_boolean()) {
    if (value == true)
      m_rootItem->setValue("True");
    else
      m_rootItem->setValue("False");
    m_rootItem->setType(value.type());
  } else if (value.is_number()) {
    if (value.is_number_integer()) {
      int v = value;
      m_rootItem->setValue("" + v);
      m_rootItem->setType(value.type());
    } else {
      double v = value;
      m_rootItem->setValue(QString::number(v));
      m_rootItem->setType(value.type());
    }
  } else {
    std::string v = value;
    m_rootItem->setValue(QString::fromStdString(v));
    m_rootItem->setType(value.type());
  }

  return m_rootItem;
}

//=========================================================================

QJsonTreeModel::QJsonTreeModel(QObject *parent, QString h1, QString h2)
    : QAbstractItemModel(parent), mRootItem{new QJsonTreeItem} {
  mHeaders.append(h1);
  mHeaders.append(h2);
}

QJsonTreeModel::QJsonTreeModel(const nlohmann::json &json, QObject *parent,
                               QString h1, QString h2)
    : QAbstractItemModel(parent), mRootItem{new QJsonTreeItem} {
  mHeaders.append(h1);
  mHeaders.append(h2);
  loadJson(json);
}

QJsonTreeModel::~QJsonTreeModel() { delete mRootItem; }

bool QJsonTreeModel::loadJson(const nlohmann::json &jdoc) {
  if (!jdoc.is_null()) {
    beginResetModel();
    delete mRootItem;
    mRootItem = QJsonTreeItem::load(jdoc);
    mRootItem->setType(jdoc.type());
    endResetModel();
    return true;
  }

  qDebug() << Q_FUNC_INFO << "cannot load json"
           << QString::fromStdString(jdoc.dump());
  return false;
}

QVariant QJsonTreeModel::data(const QModelIndex &index, int role) const {

  if (!index.isValid())
    return QVariant();

  QJsonTreeItem *item = static_cast<QJsonTreeItem *>(index.internalPointer());

  if (role == Qt::DisplayRole) {

    if (index.column() == 0)
      return QString("%1").arg(item->key());

    if (index.column() == 1)
      return item->value();
  } else if (Qt::EditRole == role) {
    if (index.column() == 1) {
      return item->value();
    }
  } else if (Qt::UserRole == role) {

    QPair<QString, QString> result;
    result.first = item->parent()->key();
    result.second = item->value().toString();
    return QVariant::fromValue(result);
  }

  return QVariant();
}

bool QJsonTreeModel::setData(const QModelIndex &index, const QVariant &value,
                             int role) {
  int col = index.column();
  if (Qt::EditRole == role) {
    if (col == 1) {
      QJsonTreeItem *item =
          static_cast<QJsonTreeItem *>(index.internalPointer());
      item->setValue(value);
      Q_EMIT dataChanged(index, index, {Qt::EditRole});
      return true;
    }
  }

  return false;
}

QVariant QJsonTreeModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {

    return mHeaders.value(section);
  } else
    return QVariant();
}

QModelIndex QJsonTreeModel::index(int row, int column,
                                  const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  QJsonTreeItem *parentItem;

  if (!parent.isValid())
    parentItem = mRootItem;
  else
    parentItem = static_cast<QJsonTreeItem *>(parent.internalPointer());

  QJsonTreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex QJsonTreeModel::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return QModelIndex();

  QJsonTreeItem *childItem =
      static_cast<QJsonTreeItem *>(index.internalPointer());
  QJsonTreeItem *parentItem = childItem->parent();

  if (parentItem == mRootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int QJsonTreeModel::rowCount(const QModelIndex &parent) const {
  QJsonTreeItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = mRootItem;
  else
    parentItem = static_cast<QJsonTreeItem *>(parent.internalPointer());

  return parentItem->childCount();
}

int QJsonTreeModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent)
  return 2;
}

Qt::ItemFlags QJsonTreeModel::flags(const QModelIndex &index) const {
  /*int col = index.column();
  auto item = static_cast<QJsonTreeItem *>(index.internalPointer());

  auto isArray = JSONType::array == item->type();
  auto isObject = JSONType::object == item->type();

  if ((col == 1) && !(isArray || isObject)) {
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
  } else {
    return QAbstractItemModel::flags(index);
  }*/
  // No editable items by default
  return QAbstractItemModel::flags(index);
}

nlohmann::json QJsonTreeModel::json() const {

  auto v = genJson(mRootItem);
  return v;
}

nlohmann::json QJsonTreeModel::genJson(QJsonTreeItem *item) const {
  auto type = item->type();
  int nchild = item->childCount();

  if (JSONType::object == type) {
    nlohmann::json jo = nlohmann::json::object();
    for (int i = 0; i < nchild; ++i) {
      auto ch = item->child(i);
      auto key = ch->key();
      jo[key.toStdString()] = genJson(ch);
    }
    return jo;
  } else if (JSONType::array == type) {
    nlohmann::json arr = nlohmann::json::array();
    for (int i = 0; i < nchild; ++i) {
      auto ch = item->child(i);
      arr.push_back(genJson(ch));
    }
    return arr;
  } else {
    nlohmann::json va;
    switch ((unsigned int)item->value().type()) {
    case (unsigned int)(JSONType::boolean): {
      va = item->value().toBool();
      break;
    }
    case (unsigned int)(JSONType::number_integer): {
      va = item->value().toInt();
      break;
    }
    case (unsigned int)(JSONType::number_float): {
      va = item->value().toFloat();
      break;
    }
    default:
      va = item->value().toString().toStdString();
      break;
    }
    return va;
  }
}

QJsonTableModel::QJsonTableModel(const QJsonTableModel::Header &header,
                                 QObject *parent)
    : QAbstractTableModel(parent), m_header(header) {}

bool QJsonTableModel::setJson(const nlohmann::json &array) {
  beginResetModel();
  m_json = array;
  endResetModel();
  return true;
}

QVariant QJsonTableModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  switch (orientation) {
  case Qt::Horizontal:
    return m_header[section]["title"];
  case Qt::Vertical:
    // return section + 1;
    return QVariant();
  default:
    return QVariant();
  }
}

int QJsonTableModel::rowCount(const QModelIndex &) const {
  return (int)m_json.size();
}

int QJsonTableModel::columnCount(const QModelIndex &) const {
  return (int)m_header.size();
}

nlohmann::json QJsonTableModel::json() const { return m_json; }

bool QJsonTableModel::setData(const QModelIndex &index, const QVariant &value,
                              int role) {

  if (Qt::EditRole == role) {
    auto &obj = m_json[index.row()];
    const auto &key = m_header[index.column()]["index"].toStdString();
    auto &v = obj[key];

    if (v.is_string() ||
        v.is_null()) { // When dimension is not provided, the value is null
      obj[key] = value.toString().toStdString();
    } else if (v.is_number()) {
      obj[key] = value.toDouble();
    } else {
      return false;
    }
    Q_EMIT dataChanged(index, index, {Qt::EditRole});
    return true;
  } else if (Qt::UserRole == role) {
    auto &obj = m_json[index.row()];
    const auto &key = m_header[index.column()]["index"].toStdString();

    obj[key] = nlohmann::json::parse(value.toString().toStdString());
    Q_EMIT dataChanged(index, index, {Qt::UserRole});
    return true;
  }
  return false;
}

Qt::ItemFlags QJsonTableModel::flags(const QModelIndex &index) const {
  int col = index.column();
  if (col > 0) {
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
  } else {
    return QAbstractItemModel::flags(index);
  }
}

QVariant QJsonTableModel::data(const QModelIndex &index, int role) const {
  switch (role) {
  case Qt::DisplayRole: {
    // First column is the parameter name
    auto &obj = m_json[index.row()];
    // qDebug() <<Q_FUNC_INFO<<"\t"<< index.row()
    // <<"\t"<<role<<"\n"<<QString::fromStdString(obj.dump());
    const auto &key = m_header[index.column()]["index"].toStdString();
    if (obj.contains(key)) {
      auto v = obj[key];
      if (v.is_object()) {
        v = v["value"];
      }
      if (v.is_string()) {
        return QString::fromStdString(v.get<std::string>());
      } else if (v.is_number()) {
        return QString::number(v.get<double>());
      } else {
        return QVariant();
      }
    } else {
      return QVariant();
    }
  }
  case Qt::ToolTipRole: {
    auto &obj = m_json[index.row()];
    const auto &key = "description";
    if (obj.contains(key)) {
      auto &v = obj[key];
      return QString::fromStdString(v.get<std::string>());
    }
    return QVariant();
  }
  case Qt::StatusTipRole: {
    return m_header[index.column()]["datatype"];
  }
  case Qt::UserRole: {
    auto &obj = m_json[index.row()];
    const auto &key = m_header[index.column()]["index"].toStdString();
    if (obj.contains(key)) {
      auto &v = obj[key];
      nlohmann::json udata;
      udata["value"] = v;
      udata["record"] = obj;
      return QString::fromStdString(udata.dump());
    }
    return QVariant();
  }
  default:
    return QVariant();
  }
}

} // namespace BondGraphUI
} // namespace OpenCOR