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

BGConnection::BGConnection(QGraphicsItem *parent) :
    Shape(parent)
{
    m_firstElement = m_lastElement = nullptr;

    setZValue(-1);

    // setBoundingRegionGranularity(1);

    // non-movable but selectable
    auto flags = ItemIsSelectable | ItemSendsGeometryChanges | ItemIsMovable | ItemUsesExtendedStyleOption;
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

BGConnection::~BGConnection()
{
    if (m_firstElement)
        m_firstElement->onConnectionDeleted(this);

    if (m_lastElement && m_lastElement != m_firstElement)
        m_lastElement->onConnectionDeleted(this);
}

// attributes

bool BGConnection::hasLocalAttribute(const QByteArray &attr_id) const
{
    if (attr_id == attr_edge_direction)
        return true;
    else
        return SceneItem::hasLocalAttribute(attr_id);
}

BGElement *BGConnection::firstElement() const
{
    return m_firstElement;
}
BGElement *BGConnection::lastElement() const
{
    return m_lastElement;
}

const QByteArray &BGConnection::firstPortId() const
{
    return m_firstPortId;
}
const QByteArray &BGConnection::lastPortId() const
{
    return m_lastPortId;
}

bool BGConnection::isValid() const
{
    return m_firstElement != nullptr && m_lastElement != nullptr;
}
bool BGConnection::isCircled() const
{
    return isValid() && m_firstElement == m_lastElement;
}
ItemDragTestResult
BGConnection::acceptDragFromItem(QGraphicsItem * /*draggedItem*/)
{
    return Ignored;
}

QPainterPath BGConnection::shape() const
{
    return m_selectionShapePath;
}

bool BGConnection::setAttribute(const QByteArray &attr_id, const QVariant &v)
{
    if (attr_id == attr_edge_direction) {
        updateArrowFlags(v.toString());
    }

    bool res = SceneItem::setAttribute(attr_id, v);

    if (res)
        update();
    return res;
}

bool BGConnection::removeAttribute(const QByteArray &attr_id)
{
    bool res = SceneItem::removeAttribute(attr_id);

    if (attr_id == attr_edge_direction) {
        updateArrowFlags(getAttribute(attr_edge_direction).toString());
    }

    if (res)
        update();
    return res;
}

// cached attributes

void BGConnection::updateCachedItems()
{
    SceneItem::updateCachedItems();

    updateArrowFlags(getAttribute(attr_edge_direction).toString());
}

void BGConnection::updateArrowFlags(const QString &direction)
{
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

QRectF BGConnection::boundingRect() const
{
    return Shape::boundingRect().adjusted(-10, -10, 10, 10);
}

void BGConnection::setupPainter(QPainter *painter,
                                const QStyleOptionGraphicsItem * /*option*/,
                                QWidget * /*widget*/)
{
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
                                 const QStyleOptionGraphicsItem *option) const
{
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

double BGConnection::getWeight() const
{
    bool ok = false;
    double weight = getAttribute(attr_weight).toDouble(&ok);
    return ok ? weight : 1;
}

double BGConnection::getVisibleWeight() const
{
    bool ok = false;
    double weight = qMax(0.1, getAttribute(attr_weight).toDouble(&ok));
    if (!ok)
        return 1;
    else if (weight > 10)
        weight = 10; // safety
    return weight;
}

QLineF BGConnection::calculateArrowLine(const QPainterPath &path, bool first,
                                        const QLineF &direction) const
{
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
                             bool first, const QLineF &direction) const
{
    if (first && m_firstElement) {
        drawArrow(painter, 0, direction);
    } else if (!first && m_lastElement) {
        drawArrow(painter, 0, direction);
    }
}

void BGConnection::drawArrow(QPainter *painter, qreal /*shift*/,
                             const QLineF &direction) const
{
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

bool BGConnection::storeTo(QDataStream &out, quint64 version64) const
{
    SceneItem::storeTo(out, version64);

    out << quint64(m_firstElement) << quint64(m_lastElement);

    out << m_firstPortId << m_lastPortId;

    return true;
}

bool BGConnection::restoreFrom(QDataStream &out, quint64 version64)
{
    if (SceneItem::restoreFrom(out, version64)) {
        // these are TEMP ids
        out >> m_tempFirstElementId >> m_tempLastElementId;

        out >> m_firstPortId >> m_lastPortId;

        return true;
    }

    return false;
}

bool BGConnection::linkAfterRestore(const CItemLinkMap &id_to_item)
{
    auto p1 = id_to_item[m_tempFirstElementId];
    auto p2 = id_to_item[m_tempLastElementId];
    BGElement *node1 = dynamic_cast<BGElement *>(p1);
    BGElement *node2 = dynamic_cast<BGElement *>(p2);

    m_firstElement = m_lastElement = nullptr;
    setFirstElement(node1, m_firstPortId);
    setLastElement(node2, m_lastPortId);
    m_firstElement->getPort(m_firstPortId)->setConnection(this);
    m_lastElement->getPort(m_lastPortId)->setConnection(this);

    return true;
}

bool BGConnection::linkAfterPaste(const CItemLinkMap &id_to_item)
{
    bool res = linkAfterRestore(id_to_item);

    return res && isValid();
}

// impl

void BGConnection::setFirstElement(BGElement *element, const QByteArray &port_id,
                                   bool update_pos)
{
    if (m_firstElement && m_firstElement != element)
        m_firstElement->onConnectionDetach(this);

    m_firstElement = element;

    if (m_firstPortId != port_id)
        m_firstPortId = port_id;

    if (m_firstElement) {
        m_firstElement->onConnectionAttach(this);
        setZValue(m_firstElement->zValue() + 1.0);
    }
    if (update_pos)
        onParentGeometryChanged();
}

void BGConnection::setLastElement(BGElement *element, const QByteArray &port_id,
                                  bool update_pos)
{
    if (m_lastElement && m_lastElement != element) {
        m_lastElement->onConnectionDetach(this);
    }

    m_lastElement = element;

    if (m_lastPortId != port_id) {
        m_lastPortId = port_id;
    }

    if (m_lastElement) {
        m_lastElement->onConnectionAttach(this);
        setZValue(m_firstElement->zValue() + 1.0);
    }
    if (update_pos)
        onParentGeometryChanged();
}

bool BGConnection::reattach(BGElement *old_node, BGElement *new_node,
                            const QByteArray &port_id)
{
    if (new_node && old_node == new_node && !new_node->allowCircledConnection())
        return false;

    bool done = false;

    if (m_firstElement == old_node) {
        setFirstElement(new_node, port_id, false), done = true;
        auto pt = m_firstElement->scenePos();
        auto target = m_lastElement->scenePos();
        auto vec = target - pt;
        auto d = sqrt(vec.x() * vec.x() + vec.y() * vec.y());
        double sz = m_firstElement->boundingRect().width() / 2;
        auto uv = sz * vec / d;
        m_firstElement->getPort(port_id)->setPos(uv.x(), uv.y());
        m_firstElement->getPort(port_id)->setConnection(this);
    }

    if (m_lastElement == old_node) {
        setLastElement(new_node, port_id, false), done = true;
        auto pt = m_lastElement->scenePos();
        auto target = m_firstElement->scenePos();
        auto vec = target - pt;
        auto d = sqrt(vec.x() * vec.x() + vec.y() * vec.y());
        double sz = m_lastElement->boundingRect().width() / 2;
        auto uv = sz * vec / d;
        m_lastElement->getPort(port_id)->setPos(uv.x(), uv.y());
        m_lastElement->getPort(port_id)->setConnection(this);
    }

    return done;
}

bool BGConnection::reattach(BGElement *element, const QByteArray &old_port_id,
                            const QByteArray &new_port_id)
{
    if (element == nullptr)
        return false;

    if (old_port_id == new_port_id && !element->allowCircledConnection())
        return false;

    bool done = false;

    if (m_firstElement == element && m_firstPortId == old_port_id) {
        setFirstElement(element, new_port_id, false), done = true;
        auto pt = element->scenePos();
        auto target = m_lastElement->scenePos();
        auto vec = target - pt;
        auto d = sqrt(vec.x() * vec.x() + vec.y() * vec.y());
        double sz = element->boundingRect().width() / 2;
        auto uv = sz * vec / d;
        m_firstElement->getPort(new_port_id)->setPos(uv.x(), uv.y());
        m_firstElement->getPort(new_port_id)->setConnection(this);
    }

    if (m_lastElement == element && m_lastPortId == old_port_id) {
        setLastElement(element, new_port_id, false), done = true;
        auto pt = element->scenePos();
        auto target = m_firstElement->scenePos();
        auto vec = target - pt;
        auto d = sqrt(vec.x() * vec.x() + vec.y() * vec.y());
        double sz = element->boundingRect().width() / 2;
        auto uv = sz * vec / d;
        element->getPort(new_port_id)->setPos(uv.x(), uv.y());
        element->getPort(new_port_id)->setConnection(this);
    }

    return done;
}

void BGConnection::reverse()
{
    qSwap(m_firstElement, m_lastElement);
    qSwap(m_firstPortId, m_lastPortId);

    onParentGeometryChanged();
}

// override

QString BGConnection::createNewId() const
{
    static const QString tmpl("BGCn%1");
    return createUniqueId<BGConnection>(tmpl);
}

// callbacks

void BGConnection::onElementMoved(BGElement *element)
{
    Q_UNUSED(element);
    Q_ASSERT(element == m_firstElement || element == m_lastElement);
    Q_ASSERT(element != nullptr);

    onParentGeometryChanged();
}

void BGConnection::onElementDetached(BGElement *element)
{
    if (element == m_firstElement) {
        m_firstElement = nullptr;
    }

    if (element == m_lastElement) {
        m_lastElement = nullptr;
    }
}

void BGConnection::onElementDeleted(BGElement *element)
{
    onElementDetached(element);

    delete this; // die as well
}

void BGConnection::onElementPortDeleted(BGElement *element,
                                        const QByteArray &port_id)
{
    Q_UNUSED(element);
    Q_UNUSED(port_id);
    // Only port to port connection allowed
    // reattach(element, portId, "");
}

void BGConnection::onElementPortRenamed(BGElement *element,
                                        const QByteArray &port_id,
                                        const QByteArray &old_port_id)
{
    if (m_firstElement == element && m_firstPortId == old_port_id)
        m_firstPortId = port_id;

    if (m_lastElement == element && m_lastPortId == old_port_id)
        m_lastPortId = port_id;
}

// override

void BGConnection::onItemRestored()
{
    updateCachedItems();

    onParentGeometryChanged();
}

QVariant BGConnection::itemChange(QGraphicsItem::GraphicsItemChange change,
                                  const QVariant &value)
{
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

void BGConnection::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    onHoverEnter(this, event);
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR