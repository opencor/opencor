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
#include <QDebug>
#include <QGuiApplication>
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>

#include "bgconnection.h"
#include "bgeditorsceneconstants.h"
#include "bgelement.h"

#define _USE_MATH_DEFINES
#include <math.h>
namespace OpenCOR {
namespace BondGraphEditorWindow {

BGConnection::BGConnection(QGraphicsItem *parent) : Shape(parent) {
  m_firstElement = m_lastElement = nullptr;

  setZValue(-1);

  // setBoundingRegionGranularity(1);

  // non-movable but selectable
  auto flags = ItemIsSelectable | ItemSendsGeometryChanges | ItemIsMovable |
               ItemUsesExtendedStyleOption;
  setFlags(flags);

  // no selection frame
  setItemFlag(IF_FramelessSelection);

  // accept hovers
  setAcceptHoverEvents(true);

  // Set Arrow shape
  arrowHead << QPointF(0, 0) << QPointF(-ARROW_SIZE / 2, ARROW_SIZE)
            << QPointF(ARROW_SIZE / 2, ARROW_SIZE) << QPointF(0, 0);

  // cache
  // setCacheMode(DeviceCoordinateCache);

  // label
  m_labelItem = new QGraphicsSimpleTextItem(this);
  m_labelItem->setFlags(0);
  m_labelItem->setCacheMode(DeviceCoordinateCache);
  m_labelItem->setPen(Qt::NoPen);
  m_labelItem->setAcceptedMouseButtons(Qt::NoButton);

  setItemFlag(CF_End_Arrow);
  resetItemFlag(CF_Start_Arrow);
}

BGConnection::~BGConnection() {
  if (m_firstElement)
    m_firstElement->onConnectionDeleted(this);

  if (m_lastElement && m_lastElement != m_firstElement)
    m_lastElement->onConnectionDeleted(this);
}

// attributes

bool BGConnection::hasLocalAttribute(const QByteArray &attrId) const {
  if (attrId == attr_edge_direction)
    return true;
  else
    return SceneItem::hasLocalAttribute(attrId);
}

bool BGConnection::setAttribute(const QByteArray &attrId, const QVariant &v) {
  if (attrId == attr_edge_direction) {
    updateArrowFlags(v.toString());
  }

  bool res = SceneItem::setAttribute(attrId, v);

  if (res)
    update();
  return res;
}

bool BGConnection::removeAttribute(const QByteArray &attrId) {
  bool res = SceneItem::removeAttribute(attrId);

  if (attrId == attr_edge_direction) {
    updateArrowFlags(getAttribute(attr_edge_direction).toString());
  }

  if (res)
    update();
  return res;
}

// cached attributes

void BGConnection::updateCachedItems() {
  SceneItem::updateCachedItems();

  updateArrowFlags(getAttribute(attr_edge_direction).toString());
}

void BGConnection::updateArrowFlags(const QString &direction) {
  if (direction == "fullhead") {
    arrowHead.clear();
    arrowHead << QPointF(0, 0) << QPointF(-ARROW_SIZE / 2, ARROW_SIZE)
              << QPointF(ARROW_SIZE / 2, ARROW_SIZE) << QPointF(0, 0);
  } else if (direction == "halfhead") {
    arrowHead.clear();
    arrowHead << QPointF(0, 0) << QPointF(-ARROW_SIZE / 2, ARROW_SIZE)
              << QPointF(0, ARROW_SIZE) << QPointF(0, 0);
  }
  setItemFlag(CF_End_Arrow);
  resetItemFlag(CF_Start_Arrow);

}

// override

QRectF BGConnection::boundingRect() const {
  return Shape::boundingRect().adjusted(-10, -10, 10, 10);
}

void BGConnection::setupPainter(QPainter *painter,
                                const QStyleOptionGraphicsItem * /*option*/,
                                QWidget * /*widget*/) {
  double weight = getVisibleWeight();

  Qt::PenStyle penStyle = (Qt::PenStyle)Utils::textToPenStyle(
      getAttribute(attr_style).toString(), Qt::SolidLine);

  // get color (to optimize!)
  QColor color = getAttribute(attr_color).value<QColor>();

  QPen p(color, weight, penStyle, Qt::FlatCap, Qt::RoundJoin);
  painter->setPen(p);

  painter->setOpacity(1.0);
}

void BGConnection::drawSelection(QPainter *painter,
                                 const QStyleOptionGraphicsItem *option) const {
  bool isSelected = (option->state & QStyle::State_Selected);
  if (isSelected) {
    double weight = getVisibleWeight();
    QPen p(QColor(Qt::darkCyan), weight * 2 + 2, Qt::SolidLine, Qt::FlatCap,
           Qt::RoundJoin);
    painter->setOpacity(0.3);
    painter->setPen(p);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(m_shapeCachePath);
  }
}

double BGConnection::getWeight() const {
  bool ok = false;
  double weight = getAttribute(attr_weight).toDouble(&ok);
  return ok ? weight : 1;
}

double BGConnection::getVisibleWeight() const {
  bool ok = false;
  double weight = qMax(0.1, getAttribute(attr_weight).toDouble(&ok));
  if (!ok)
    return 1;
  else if (weight > 10)
    weight = 10; // safety
  return weight;
}

QLineF BGConnection::calculateArrowLine(const QPainterPath &path, bool first,
                                        const QLineF &direction) const {
  //// optimization: disable during drag or pan
  // Qt::MouseButtons buttons = QGuiApplication::mouseButtons();
  // if ((buttons & Qt::LeftButton) || (buttons & Qt::RightButton))
  //	return direction;

  //// optimization: disable during zoom
  // Qt::KeyboardModifiers keys = QGuiApplication::keyboardModifiers();
  // if (keys & Qt::ControlModifier)
  //	return direction;

  if (first && m_firstElement) {
    qreal arrowStart = path.percentAtLength(ARROW_SIZE);
    return QLineF(path.pointAtPercent(arrowStart), direction.p2());
  } else if (!first && m_lastElement) {
    qreal len = path.length();
    qreal arrowStart = path.percentAtLength(len - ARROW_SIZE);
    return QLineF(path.pointAtPercent(arrowStart), direction.p2());
  }

  return direction;
}

void BGConnection::drawArrow(QPainter *painter,
                             const QStyleOptionGraphicsItem * /*option*/,
                             bool first, const QLineF &direction) const {
  if (first && m_firstElement) {
    drawArrow(painter, 0, direction);
  } else if (!first && m_lastElement) {
    drawArrow(painter, 0, direction);
  }
}

void BGConnection::drawArrow(QPainter *painter, qreal /*shift*/,
                             const QLineF &direction) const {
  QPen oldPen = painter->pen();
  painter->save();

  painter->setPen(QPen(oldPen.color(), oldPen.widthF(), Qt::SolidLine,
                       Qt::SquareCap, Qt::MiterJoin));
  painter->setBrush(oldPen.color());

  static QLineF hl(0, 0, 0, 200);
  qreal a = direction.angleTo(hl);

  painter->translate(direction.p2());
  painter->rotate(180 + a);
  painter->translate(QPointF(0, oldPen.widthF()));
  painter->drawPolygon(arrowHead);

  painter->restore();
}

// IO

bool BGConnection::storeTo(QDataStream &out, quint64 version64) const {
  SceneItem::storeTo(out, version64);

  out << quint64(m_firstElement) << quint64(m_lastElement);

  out << m_firstPortId << m_lastPortId;

  return true;
}

bool BGConnection::restoreFrom(QDataStream &out, quint64 version64) {
  if (SceneItem::restoreFrom(out, version64)) {
    // these are TEMP ids
    out >> m_tempFirstElementId >> m_tempLastElementId;

    out >> m_firstPortId >> m_lastPortId;

    return true;
  }

  return false;
}

bool BGConnection::linkAfterRestore(const CItemLinkMap &idToItem) {
  // qDebug() << m_tempFirstElementId << m_tempLastElementId;
  auto p1 = idToItem[m_tempFirstElementId];
  auto p2 = idToItem[m_tempLastElementId];
  BGElement *node1 = dynamic_cast<BGElement *>(p1);
  BGElement *node2 = dynamic_cast<BGElement *>(p2);

  m_firstElement = m_lastElement = nullptr;
  setFirstElement(node1, m_firstPortId);
  setLastElement(node2, m_lastPortId);
  m_firstElement->getPort(m_firstPortId)->setConnection(this);
  m_lastElement->getPort(m_lastPortId)->setConnection(this);

  return true;
}

bool BGConnection::linkAfterPaste(const CItemLinkMap &idToItem) {
  bool res = linkAfterRestore(idToItem);

  return res && isValid();
}

// impl

void BGConnection::setFirstElement(BGElement *element, const QByteArray &portId,
                                   bool updatePos) {

  if (m_firstElement && m_firstElement != element)
    m_firstElement->onConnectionDetach(this);

  m_firstElement = element;

  if (m_firstPortId != portId)
    m_firstPortId = portId;

  if (m_firstElement) {
    m_firstElement->onConnectionAttach(this);
    setZValue(m_firstElement->zValue() + 1.0);
  }
  if (updatePos)
    onParentGeometryChanged();
}

void BGConnection::setLastElement(BGElement *element, const QByteArray &portId,
                                  bool updatePos) {

  if (m_lastElement && m_lastElement != element) {
    m_lastElement->onConnectionDetach(this);
  }

  m_lastElement = element;

  if (m_lastPortId != portId) {
    m_lastPortId = portId;
  }

  if (m_lastElement) {
    m_lastElement->onConnectionAttach(this);
    setZValue(m_firstElement->zValue() + 1.0);
  }
  if (updatePos)
    onParentGeometryChanged();
}

bool BGConnection::reattach(BGElement *oldNode, BGElement *newNode,
                            const QByteArray &portId) {
  if (newNode && oldNode == newNode && !newNode->allowCircledConnection())
    return false;

  bool done = false;

  if (m_firstElement == oldNode) {
    setFirstElement(newNode, portId, false), done = true;
    auto pt = m_firstElement->scenePos();
    auto target = m_lastElement->scenePos();
    auto vec = target - pt;
    auto d = sqrt(vec.x() * vec.x() + vec.y() * vec.y());
    double sz = m_firstElement->boundingRect().width() / 2;
    auto uv = sz * vec / d;
    m_firstElement->getPort(portId)->setPos(uv.x(), uv.y());
    m_firstElement->getPort(portId)->setConnection(this);
  }

  if (m_lastElement == oldNode) {
    setLastElement(newNode, portId, false), done = true;
    auto pt = m_lastElement->scenePos();
    auto target = m_firstElement->scenePos();
    auto vec = target - pt;
    auto d = sqrt(vec.x() * vec.x() + vec.y() * vec.y());
    double sz = m_lastElement->boundingRect().width() / 2;
    auto uv = sz * vec / d;
    m_lastElement->getPort(portId)->setPos(uv.x(), uv.y());
    m_lastElement->getPort(portId)->setConnection(this);
  }
  // qDebug()<<Q_FUNC_INFO<<"\t1\t"<<m_firstElement->getDisplayName()
  // <<"\t"<<m_lastElement->getDisplayName()<<"\t"<<done;
  return done;
}

bool BGConnection::reattach(BGElement *element, const QByteArray &oldPortId,
                            const QByteArray &newPortId) {
  if (element == nullptr)
    return false;

  if (oldPortId == newPortId && !element->allowCircledConnection())
    return false;

  bool done = false;

  if (m_firstElement == element && m_firstPortId == oldPortId) {
    setFirstElement(element, newPortId, false), done = true;
    auto pt = element->scenePos();
    auto target = m_lastElement->scenePos();
    auto vec = target - pt;
    auto d = sqrt(vec.x() * vec.x() + vec.y() * vec.y());
    double sz = element->boundingRect().width() / 2;
    auto uv = sz * vec / d;
    m_firstElement->getPort(newPortId)->setPos(uv.x(), uv.y());
    m_firstElement->getPort(newPortId)->setConnection(this);
  }

  if (m_lastElement == element && m_lastPortId == oldPortId) {
    setLastElement(element, newPortId, false), done = true;
    auto pt = element->scenePos();
    auto target = m_firstElement->scenePos();
    auto vec = target - pt;
    auto d = sqrt(vec.x() * vec.x() + vec.y() * vec.y());
    double sz = element->boundingRect().width() / 2;
    auto uv = sz * vec / d;
    element->getPort(newPortId)->setPos(uv.x(), uv.y());
    element->getPort(newPortId)->setConnection(this);
  }

  // onParentGeometryChanged();
  // qDebug()<<Q_FUNC_INFO<<"\t2\t"<<m_firstElement->getDisplayName()
  // <<"\t"<<m_lastElement->getDisplayName()<<"\t"<<done;
  return done;
}

void BGConnection::reverse() {
  qSwap(m_firstElement, m_lastElement);
  qSwap(m_firstPortId, m_lastPortId);

  onParentGeometryChanged();
}

// override

QString BGConnection::createNewId() const {
  static const QString tmpl("BGCn%1");
  return createUniqueId<BGConnection>(tmpl);
}

// callbacks

void BGConnection::onElementMoved(BGElement *element) {
  Q_UNUSED(element);
  Q_ASSERT(element == m_firstElement || element == m_lastElement);
  Q_ASSERT(element != nullptr);

  onParentGeometryChanged();
}

void BGConnection::onElementDetached(BGElement *element) {
  if (element == m_firstElement) {
    m_firstElement = nullptr;
  }

  if (element == m_lastElement) {
    m_lastElement = nullptr;
  }
}

void BGConnection::onElementDeleted(BGElement *element) {
  onElementDetached(element);

  delete this; // die as well
}

void BGConnection::onElementPortDeleted(BGElement *element,
                                        const QByteArray &portId) {
  Q_UNUSED(element);
  Q_UNUSED(portId);
  // Only port to port connection allowed
  // reattach(element, portId, "");
}

void BGConnection::onElementPortRenamed(BGElement *element,
                                        const QByteArray &portId,
                                        const QByteArray &oldPortId) {
  if (m_firstElement == element && m_firstPortId == oldPortId)
    m_firstPortId = portId;

  if (m_lastElement == element && m_lastPortId == oldPortId)
    m_lastPortId = portId;
}

// override

void BGConnection::onItemRestored() {
  updateCachedItems();

  onParentGeometryChanged();
}

QVariant BGConnection::itemChange(QGraphicsItem::GraphicsItemChange change,
                                  const QVariant &value) {
  if (change == ItemSceneHasChanged) {
    // set default ID
    setDefaultId();

    onItemRestored();

    return value;
  }

  if (change == ItemPositionChange) {
    // discard any movement
    return QVariant();
  }

  if (change == ItemPositionHasChanged) {
    // discard any movement
    return QVariant();
  }

  if (change == ItemSelectedHasChanged) {
    onItemSelected(value.toBool());

    return value;
  }

  return value;
}

void BGConnection::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
  onHoverEnter(this, event);
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR