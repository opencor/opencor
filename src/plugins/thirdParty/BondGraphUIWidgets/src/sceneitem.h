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
#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QUuid>

#include "bgeditorscene.h"
#include "interactiveiteminterface.h"
//#include "property.h"
#include "utils.h"

namespace OpenCOR {
namespace BondGraphUI {

// class BGEditorScene;

enum SceneItemFlags {
  IF_FramelessSelection = 1,
  IF_DeleteAllowed = 2,
  IF_LastFlag = 4
};

enum SceneItemStateFlags {
  IS_Normal = 0,
  IS_Selected = 1,
  IS_Hover = 2,
  IS_Drag_Accepted = 4,
  IS_Drag_Rejected = 8,
  IS_Attribute_Changed = 16,
  IS_Need_Update = 32
};

class ControlPoint;

class SceneItem : public InteractiveItemInterface {
public:
  SceneItem();
  virtual ~SceneItem();

  int itemFlags() const { return m_itemFlags; }
  void setItemFlags(int f) { m_itemFlags = f; }
  void setItemFlag(int f) { m_itemFlags |= f; }
  void resetItemFlag(int f) { m_itemFlags &= ~f; }

  int itemStateFlags() const { return m_internalStateFlags; }
  void setItemStateFlag(int f) { m_internalStateFlags |= f; }
  void resetItemStateFlag(int f) { m_internalStateFlags &= ~f; }
  static QByteArray TYPE() { return "SceneItem"; }
  virtual QByteArray typeId() const { return "SceneItem"; }
  virtual QString createNewId() const { return QUuid::createUuid().toString(); }
  virtual bool setDefaultId();
  virtual QString getDisplayName() const { return mDisplayName; }
  virtual void setDisplayName(QString name) { mDisplayName = name; };
  template <class C> QString createUniqueId(const QString &Tmpl) const;

  // attributes
  virtual bool hasLocalAttribute(const QByteArray &attrId) const;
  const QMap<QByteArray, QVariant> &getLocalAttributes() const {
    return m_attributes;
  }

  virtual bool setAttribute(const QByteArray &attrId, const QVariant &v);
  virtual bool removeAttribute(const QByteArray &attrId);
  virtual QVariant getAttribute(const QByteArray &attrId) const;
  virtual void setClassType(QString classType);
  virtual QString getClassType();
  virtual bool junctionType() const;
  virtual QByteArray classId() const { return "item"; }
  virtual QByteArray superClassId() const { return QByteArray(); }

  QString getId() const { return m_id; }
  void setId(const QString &id) { setAttribute("id", id); }

  enum VisibleFlags { VF_ANY = 0, VF_LABEL = 1, VF_TOOLTIP = 2 };
  virtual QSet<QByteArray> getVisibleAttributeIds(int flags) const;

  // scene access
  QGraphicsItem *getSceneItem() const {
    return dynamic_cast<QGraphicsItem *>((SceneItem *)this);
  }

  BGEditorScene *getScene() const;

  void addUndoState();

  // labels
  virtual void updateLabelContent();
  virtual void updateLabelDecoration();
  virtual void updateLabelPosition() {}
  void setLabelText(const QString &text);
  QString getLabelText() { return m_labelItem->text(); };
  void showLabel(bool on);
  void setIcon(const QPixmap &icon);
  QPixmap &getIcon() { return m_icon; };
  QRectF getSceneLabelRect() const;
  virtual QPointF getLabelCenter() const;

  // transformations
  virtual void transform(const QRectF &oldRect, const QRectF &newRect,
                         double xc, double yc, bool changeSize,
                         bool changePos) {
    Q_UNUSED(oldRect);
    Q_UNUSED(newRect);
    Q_UNUSED(xc);
    Q_UNUSED(yc);
    Q_UNUSED(changeSize);
    Q_UNUSED(changePos);
  }

  // serialization
  virtual bool storeTo(QDataStream &out, quint64 version64) const;
  virtual bool restoreFrom(QDataStream &out, quint64 version64);
  friend void to_json(nlohmann::json &j, const SceneItem &p);
  friend void from_json(const nlohmann::json &j, SceneItem &p);

  typedef QMap<quint64, SceneItem *> CItemLinkMap;
  virtual bool linkAfterRestore(const CItemLinkMap & /*idToItem*/) {
    return true;
  }
  virtual bool linkAfterPaste(const CItemLinkMap &idToItem) {
    return linkAfterRestore(idToItem);
  }

  // default the same
  static void beginRestore() { s_duringRestore = true; }
  static void endRestore() { s_duringRestore = false; }

  // returns new item of this class
  virtual SceneItem *clone() = 0;
  virtual SceneItem *create() const = 0;

  // copy data from item
  virtual void copyDataFrom(SceneItem *from);

  // callbacks
  virtual void onItemRestored();
  virtual void onItemSelected(bool state);
  virtual void onHoverEnter(QGraphicsItem *sceneItem,
                            QGraphicsSceneHoverEvent *event);

  // call from control points
  virtual void onControlPointMoved(ControlPoint * /*controlPoint*/,
                                   const QPointF & /*pos*/) {}
  virtual void onControlPointDelete(ControlPoint * /*controlPoint*/) {}

  // call from drag event
  virtual ItemDragTestResult
  acceptDragFromItem(QGraphicsItem * /*draggedItem*/) {
    return Accepted;
  }

  // called after restoring data (reimplement to update cached attribute values)
  virtual void updateCachedItems();
  static int getItemCount() { return s_count; }

  static void setItemCount(int count) { s_count = count; }

protected:
  int m_itemFlags;
  int m_internalStateFlags;
  QMap<QByteArray, QVariant> m_attributes;
  QString m_id;
  QGraphicsSimpleTextItem *m_labelItem;
  QString mDisplayName;
  QPixmap m_icon;
  // restore optimization
  static bool s_duringRestore;
  static int s_count;
};

template <class C>
QString SceneItem::createUniqueId(const QString &tmpl) const {
  return tmpl.arg(++s_count);

  // auto editorScene = getScene();
  // if (editorScene == nullptr)
  // {
  //   return tmpl.arg(++s_count);
  // }
  // auto citems = editorScene->getItems<C>();
  // QSet<QString> ids;
  // int tcount = 0;
  // for (const auto &citem : citems)
  // {
  //   ids << citem->getId();
  //   ++tcount;
  // }
  // QString newId;
  // do
  //   newId = tmpl.arg(++tcount);
  // while (ids.contains(newId));
  // if (s_count < tcount)
  //   s_count = tcount;
  // return newId;
};

} // namespace BondGraphUI
} // namespace OpenCOR