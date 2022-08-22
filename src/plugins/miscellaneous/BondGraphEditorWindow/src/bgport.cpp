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

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wdouble-promotion"
    #pragma clang diagnostic ignored "-Wfloat-conversion"
    #pragma clang diagnostic ignored "-Wold-style-cast"
    #pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#endif 

#include "bgelement.h"
#include "bgport.h"

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGPort::BGPort(BGElement *element, const QByteArray &port_id, QPointF pos) :
    QGraphicsEllipseItem(dynamic_cast<QGraphicsItem *>(element)),
    m_Element(element), m_id(port_id), m_position(pos), m_direction(0), m_canDelete(true)
{
    Q_ASSERT(m_Element != nullptr);
    uid = QUuid::createUuid();
    if (!m_canDelete) {
        setRect(-6, -6, 14, 14);
    } else {
        setRect(-8, -8, 16, 16);
        setToolTip(port_id); // No tool tip for junction port
    }
    setBrush(Qt::gray);
    setPen(QPen(Qt::black, 1));
    setPos(m_position);

    m_align = -1; // Qt::AlignBottom + Qt::AlignHCenter;
    QRectF r = boundingRect();
    float rx = r.center().x();
    float ry = r.center().y();

    // Diamond
    m_shapeCache << QPointF(rx, ry - r.height() / 2)
                 << QPointF(rx + r.width() / 2, ry)
                 << QPointF(rx, ry + r.height() / 2)
                 << QPointF(rx - r.width() / 2, ry)
                 << QPointF(rx, ry - r.height() / 2);

    auto flags = ItemIsMovable | ItemIsSelectable | ItemIgnoresParentOpacity | ItemSendsGeometryChanges;
    setFlags(flags);
}

BGPort::~BGPort()
{
    if (m_Element != nullptr) {
        m_Element->onPortDeleted(this);
    }
}

BGElement *BGPort::getElement() const
{
    return m_Element;
}
QUuid BGPort::getUid() const
{
    return uid;
}
const QByteArray &BGPort::getId() const
{
    return m_id;
}
bool BGPort::getDeleteFlag() const
{
    return m_canDelete;
}
double BGPort::getX() const
{
    return m_position.x();
}
double BGPort::getY() const
{
    return m_position.y();
}

int BGPort::getAlign() const
{
    return m_align;
}
int BGPort::getDirection() const
{
    return m_direction;
}
void BGPort::setDirection(int dir)
{
    m_direction = dir;
}

void BGPort::setConnection(BGConnection *conn)
{
    m_Connection = conn;
}
BGConnection *BGPort::getConnection() const
{
    return m_Connection;
}

void BGPort::setDeleteFlag(const bool flag)
{
    m_canDelete = flag;
    if (!m_canDelete) {
        setRect(-6, -6, 14, 14);
        setToolTip(""); // No tool tip for junction port
    } else {
        setRect(-8, -8, 16, 16);
    }
}

void BGPort::onParentDeleted()
{
    // clear m_Element if it is removed already
    m_Element = nullptr;
}

void BGPort::setAlignment(int align)
{
    m_align = align;
    QRectF nodeBox = m_Element->Shape::boundingRect();
    if (align != -1) {
        int x = 0;
        int y = 0;
        if (nodeBox.isValid()) {
            if (m_align & Qt::AlignLeft) {
                x -= nodeBox.width() / 2;
            } else if (m_align & Qt::AlignRight) {
                x += nodeBox.width() / 2;
            }
            if (m_align & Qt::AlignTop) {
                y -= nodeBox.height() / 2;
            } else if (m_align & Qt::AlignBottom) {
                y += nodeBox.height() / 2;
            }
        } else {
            int dim = m_Element->getMinDim();
            if (m_align & Qt::AlignLeft) {
                x -= dim / 2;
            } else if (m_align & Qt::AlignRight) {
                x += dim / 2;
            }
            if (m_align & Qt::AlignTop) {
                y -= dim / 2;
            } else if (m_align & Qt::AlignBottom) {
                y += dim / 2;
            }
        }

        setX(x);
        setY(y);
        m_position = QPointF(x, y);
    }
}

void BGPort::onParentGeometryChanged()
{
    Q_ASSERT(m_Element != nullptr);

    QRectF rect = m_Element->Shape::boundingRect();

    int x = m_position.x(), y = m_position.y();
    if (m_align != -1) {
        if (m_align & Qt::AlignLeft)
            x -= rect.width() / 2;
        else if (m_align & Qt::AlignRight)
            x += rect.width() / 2;

        if (m_align & Qt::AlignTop)
            y -= rect.height() / 2;
        else if (m_align & Qt::AlignBottom)
            y += rect.height() / 2;
    }
    if (m_anchorSize.isValid()) {
        auto previous = m_anchorSize;
        auto xScale = rect.width() / previous.width();
        auto yScale = rect.height() / previous.height();
        x *= xScale;
        y *= yScale;
    }
    m_position.setX(x);
    m_position.setY(y);
    if (!rect.contains(m_position)) {
        // Keep the item inside the scene rect.
        m_position.setX(qMin(rect.right(), qMax(m_position.x(), rect.left())));
        m_position.setY(qMin(rect.bottom(), qMax(m_position.y(), rect.top())));
    }
    setPos(m_position);
    m_anchorSize = rect;
}

void BGPort::setId(const QByteArray &port_id)
{
    if (m_id == port_id)
        return;

    auto oldId = m_id;
    m_id = port_id;
    if (m_canDelete) {
        setToolTip(port_id);
    }

    if (m_Element)
        m_Element->onPortRenamed(this, oldId);
}

void BGPort::setOffset(QPointF pos)
{
    // m_align = -1;
    m_position = pos;
}

QColor BGPort::getColor() const
{
    return brush().color();
}

void BGPort::setColor(const QColor &color)
{
    setBrush(color);
}

QVariant BGPort::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene() != nullptr && !m_canDelete) { // Do not allow junction ports to move
        return QPointF(0, 0);
    }
    if (change == ItemPositionChange && scene() != nullptr) {
        // value is the new position.
        QPointF newPos = value.toPointF();
        QRectF rect = m_Element->Shape::boundingRect();
        if (!rect.contains(newPos)) {
            // Keep the item inside the scene rect.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
            return newPos;
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void BGPort::copyDataFrom(const BGPort &port)
{
    m_id = port.m_id;
    m_align = port.m_align;
    m_position = port.m_position;
    m_anchorSize = port.m_anchorSize;
    m_canDelete = port.m_canDelete;
    m_direction = port.m_direction;

    setBrush(port.brush());
    setPen(port.pen());
    setRect(port.rect());
}

ItemDragTestResult BGPort::acceptDragFromItem(QGraphicsItem *dragged_item)
{
    if (dynamic_cast<BGElement *>(dragged_item)) {
        setOpacity(0.5);
        return Accepted;
    } else {
        setOpacity(1);
        return Ignored;
    }
}

void BGPort::leaveDragFromItem(QGraphicsItem * /*draggedItem*/)
{
    setOpacity(1);
}

void BGPort::onClick(QGraphicsSceneMouseEvent * /*mouseEvent*/)
{
    if (m_canDelete)
        setSelected(true);
    else
        m_Element->setSelected(true);
}

// serialization

bool BGPort::storeTo(QDataStream &out, quint64 /*version64*/) const
{
    out << m_id;
    out << m_position;
    out << m_align;
    out << m_canDelete;
    out << m_direction;
    // color etc. since v12
    out << brush() << pen() << rect();
    return true;
}

void BGPort::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    Q_UNUSED(widget);
    if (!m_canDelete)
        return;
    bool isSelected = (option->state & QStyle::State_Selected);

    painter->setClipRect(boundingRect());

    // get color (to optimize!)
    QColor color = getColor();
    if (color.isValid())
        painter->setBrush(color);
    else
        painter->setBrush(Qt::NoBrush);

    qreal strokeSize = 0.05;

    QColor strokeColor("black");
    // if (!m_canDelete) {
    //   strokeColor = QColor(Qt::transparent);
    // }
    int strokeStyle = Qt::SolidLine;

    // selection background outline
    if (isSelected) {
        painter->setPen(QPen(Qt::darkCyan, strokeSize + 5, Qt::SolidLine,
                             Qt::FlatCap, Qt::RoundJoin));
        painter->setOpacity(0.3);
        painter->drawPolygon(m_shapeCache);
    } else {
        painter->setPen(QPen(strokeColor, strokeSize, (Qt::PenStyle)strokeStyle));
        painter->drawPolygon(m_shapeCache);
    }
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
