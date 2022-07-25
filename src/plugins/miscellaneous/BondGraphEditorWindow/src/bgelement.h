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
#include "bgport.h"
#include "sceneitem.h"
#include "thirdparty/json.hpp"
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QPixmap>
#include <QSet>

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGConnection;

enum NodeFlags {
  NF_OrphanAllowed = 1 // allow element to have no connections
};

class BGElement : public SceneItem, public QGraphicsRectItem {
public:
  typedef QGraphicsRectItem Shape;

  BGElement(QGraphicsItem *parent = nullptr);
  virtual ~BGElement();
  static QByteArray TYPE() { return "BGElement"; }
  virtual QByteArray typeId() const { return "BGElement"; }
  virtual QString createNewId() const;

  int bgElementFlags() const { return m_Flags; }
  void setBGElementFlags(int f) { m_Flags = f; }
  void setBGElementFlag(int f) { m_Flags |= f; }
  void resetBGElementFlag(int f) { m_Flags &= ~f; }
  nlohmann::json setBGElementDefinitions(QString json);
  nlohmann::json &getStateParameterJson();
  void setStateParameterJson(nlohmann::json &json);

  nlohmann::json &getJson();
  void setJson(nlohmann::json &json);
  QPixmap getRenderedLatex(int fontSize = 12);

  QString getDisplayName() const { return mDisplayName; }
  void setDisplayName(QString name);
  QString getConstiutiveRelation();
  bool isProxy();
  bool canBeProxy();
  void setAsProxy();

  // override
  virtual SceneItem *create() const { return new BGElement(parentItem()); }
  virtual SceneItem *clone();
  virtual void copyDataFrom(SceneItem *from);

  // transformations
  virtual void transform(const QRectF &oldRect, const QRectF &newRect,
                         double xc, double yc, bool changeSize,
                         bool changePos) override;

  // attributes
  virtual QSizeF getSize() const { return rect().size(); }
  virtual int getMinDim() const { return minDim; }
  virtual void setSize(float w, float h);

  virtual bool hasLocalAttribute(const QByteArray &attrId) const;
  virtual bool setAttribute(const QByteArray &attrId, const QVariant &v);
  virtual bool removeAttribute(const QByteArray &attrId);
  virtual QVariant getAttribute(const QByteArray &attrId) const;
  virtual QByteArray classId() const { return "element"; }
  virtual QByteArray superClassId() const { return SceneItem::classId(); }

  // ports
  BGPort *addPort(const QByteArray &portId = "",
                  QPointF pos = QPointF(-1000, -1000));
  bool removePort(const QByteArray &portId);
  void reorientPort(int dir);
  bool movePort(const QByteArray &portId, QPointF pos);
  bool renamePort(const QByteArray &portId, const QByteArray &newId);
  BGPort *getPort(const QByteArray &portId) const;
  int getMaxPorts() const { return maxPorts; };
  QByteArrayList getPortIds() const;

  // serialization
  virtual bool storeTo(QDataStream &out, quint64 version64) const;
  virtual bool restoreFrom(QDataStream &out, quint64 version64);
  friend void to_json(nlohmann::json &j, const BGElement &p);
  friend void from_json(const nlohmann::json &j, BGElement &p);

  // merges element with the current one.
  // element will be deleted afterwards if no circled connection allowed.
  virtual bool merge(BGElement *element, const QByteArray &portId = "");

  // splits all the connections from this element.
  // result is the list of the newly created nodes (or empty list if
  // connections < 2).
  virtual QList<BGElement *> unlink();

  // returns all nodes colliding with this one.
  QList<BGElement *> getCollidingElements() const;

  // returns all connections of the element.
  QSet<BGConnection *> getConnections() const { return m_connections; }

  // returns all incoming connections of the element.
  QSet<BGConnection *> getInConnections() const;

  // returns all outgoing connections of the element.
  QSet<BGConnection *> getOutConnections() const;

  // returns true if new connection from this element is allowed.
  virtual bool allowStartConnection() const;

  // returns true if a connection from this element to itself is allowed.
  virtual bool allowCircledConnection() const { return false; }

  // calculates point on the element's outline intersecting with line.
  virtual QPointF getIntersectionPoint(const QLineF &line,
                                       const QByteArray &portId,
                                       const double shadow = 1.0) const;

  // callbacks
  virtual void onConnectionAttach(BGConnection *conn);
  virtual void onConnectionDetach(BGConnection *conn);
  virtual void onConnectionDeleted(BGConnection *conn);

  virtual void onPortDeleted(BGPort *port);
  virtual void onPortRenamed(BGPort *port, const QByteArray &oldId);

  virtual void onItemMoved(const QPointF &delta);
  virtual void onItemRestored();
  virtual void
  onDroppedOn(const QSet<InteractiveItemInterface *> &acceptedItems,
              const QSet<InteractiveItemInterface *> &rejectedItems);
  virtual ItemDragTestResult acceptDragFromItem(QGraphicsItem *draggedItem);
  virtual void updatePortsLayout();
  virtual void pauseCacheUpdates();
  virtual void restartCacheUpdates();

  // override
  virtual QRectF boundingRect() const;
  friend void from_json(const nlohmann::json &j, BGEditorScene &p);

protected:
  // override
  virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
                              const QVariant &value);
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget = Q_NULLPTR);
  virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
  virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

  virtual void updateLabelPosition();
  virtual void updateCachedItems();

private:
  void recalculateShape();
  void updateConnections();
  void removeOrphanPorts();
  void resize(float size) { setRect(-size / 2, -size / 2, size, size); }
  void resize(float w, float h) { setRect(-w / 2, -h / 2, w, h); }
  void resize(const QSizeF &size) { resize(size.width(), size.height()); }

protected:
  QSet<BGConnection *> m_connections;
  bool m_canUpdateCache = true;
  int m_proxy = 0;
  int m_Flags = 0;
  int iconTop = 0;
  int iconLeft = 0;
  int maxPorts = 1;
  int minDim = 50;
  nlohmann::json m_annotation;
  QByteArray m_annotationCache;
  QMap<QByteArray, BGPort *> m_ports;
  QPixmap m_iconCache;
  QPolygonF m_shapeCache;
  QRectF m_sizeCache;
  QString m_shortname = "BGEl_%1";
  QString m_type;
  QMap<QString, QColor> itemColours;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR