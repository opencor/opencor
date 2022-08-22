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

#include "bgdconnection.h"
#include "bgeditorsceneconstants.h"
#include "bgelement.h"

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wold-style-cast"
#endif

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGDConnection::BGDConnection(QGraphicsItem *parent) :
    BGConnection(parent)
{
    m_bendFactor = 0;
}

void BGDConnection::setBendFactor(int bf)
{
    if (bf != m_bendFactor) {
        m_bendFactor = bf;
        setAttribute("bendfactor", QVariant(bf));
        onParentGeometryChanged();
    }
}

QByteArray BGDConnection::TYPE()
{
    return "BGDConnection";
}

QByteArray BGDConnection::typeId() const
{
    return "BGDConnection";
}

QByteArray BGDConnection::classId() const
{
    return "connection";
}

QByteArray BGDConnection::superClassId() const
{
    return BGConnection::classId();
}

SceneItem *BGDConnection::create() const
{
    return new BGDConnection(parentItem());
}

QPointF BGDConnection::getLabelCenter() const
{
    return m_controlPoint;
}

int BGDConnection::getBendFactor() const
{
    return m_bendFactor;
}

void BGDConnection::transform(const QRectF &old_rect, const QRectF &new_rect,
                              double xc, double yc, bool change_size,
                              bool change_pos)
{
    Q_UNUSED(old_rect);
    Q_UNUSED(new_rect);
    Q_UNUSED(xc);
    Q_UNUSED(yc);
    Q_UNUSED(change_size);
    Q_UNUSED(change_pos);
}

// serialization

bool BGDConnection::storeTo(QDataStream &out, quint64 version64) const
{
    SceneItem::storeTo(out, version64);

    out << quint64(m_firstElement) << quint64(m_lastElement);

    out << m_firstPortId << m_lastPortId;

    out << m_bendFactor;

    out << m_controlPos << m_controlPoint;

    return true;
}

bool BGDConnection::restoreFrom(QDataStream &out, quint64 version64)
{
    if (SceneItem::restoreFrom(out, version64)) {
        // these are TEMP ids
        out >> m_tempFirstElementId >> m_tempLastElementId;

        out >> m_firstPortId >> m_lastPortId;

        out >> m_bendFactor;

        out >> m_controlPos >> m_controlPoint;

        return true;
    }

    return false;
}

// override

BGConnection *BGDConnection::clone()
{
    BGDConnection *c = new BGDConnection(parentItem());

    // assign directly!
    c->m_firstElement = m_firstElement;
    c->m_firstPortId = m_firstPortId;
    c->m_lastElement = m_lastElement;
    c->m_lastPortId = m_lastPortId;
    // c->m_controlPos = m_controlPos;
    c->m_bendFactor = m_bendFactor;

    if (scene())
        scene()->addItem(c);

    c->copyDataFrom(this);

    return c;
}

void BGDConnection::paint(QPainter *painter,
                          const QStyleOptionGraphicsItem *option,
                          QWidget *widget)
{
    // dont draw if no cache
    if (m_shapeCachePath.isEmpty()) {
        return;
    }

    // selection
    drawSelection(painter, option);

    // called before draw
    setupPainter(painter, option, widget);

    painter->setClipRect(boundingRect());

    auto len = line().length();
    bool isArrow = (len > ARROW_SIZE * 2);
    bool isDirect = (!isCircled() && (m_bendFactor == 0));

    if (isDirect) // straight line
    {
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(m_shapeCachePath);
        QRectF rf(option->rect);
        //  arrows
        if (isArrow && m_itemFlags & CF_Start_Arrow)
            drawArrow(painter, option, true, QLineF(line().p2(), line().p1()));

        if (isArrow && m_itemFlags & CF_End_Arrow)
            drawArrow(painter, option, false, line());
    } else // curve
    {
        painter->setBrush(Qt::NoBrush);
        painter->drawPath(m_shapeCachePath);
        // arrows
        if (isArrow && m_itemFlags & CF_Start_Arrow) {
            QLineF arrowLine = calculateArrowLine(m_shapeCachePath, true,
                                                  QLineF(m_controlPos, line().p1()));
            drawArrow(painter, option, true, arrowLine);
        }

        if (isArrow && m_itemFlags & CF_End_Arrow) {
            QLineF arrowLine = calculateArrowLine(m_shapeCachePath, false,
                                                  QLineF(m_controlPos, line().p2()));
            drawArrow(painter, option, false, arrowLine);
        }
    }
}

void BGDConnection::updateLabelPosition()
{
    auto r = m_labelItem->boundingRect();
    int w = (int)r.width();
    int h = (int)r.height();
    m_labelItem->setTransformOriginPoint(w / 2, h / 2);

    if (isCircled()) {
        m_labelItem->setPos(m_controlPoint.x() - w / 2,
                            m_controlPoint.y() - boundingRect().height() / 2 - h);

        m_labelItem->setRotation(0);
    } else {
        m_labelItem->setPos(m_controlPoint.x() - w / 2, m_controlPoint.y() - h / 2);
    }
}

// callbacks

void BGDConnection::onParentGeometryChanged()
{
    // optimize: no update while restoring
    if (s_duringRestore)
        return;

    if (!m_firstElement || !m_lastElement)
        return;

    prepareGeometryChange();

    // update line position
    QPointF p1c = m_firstElement->scenePos();
    QPointF p2c = m_lastElement->scenePos();

    // Port positions require higher boundary displacement to be kept in the
    // boundary, else will move inside and doesnt look good

    QPointF p11 = m_firstElement->getIntersectionPoint(QLineF(p1c, m_controlPos),
                                                       m_firstPortId, 5);
    QPointF p21 = m_lastElement->getIntersectionPoint(QLineF(p2c, m_controlPos),
                                                      m_lastPortId, 5);

    // Update ports position
    // TODO check with qsettings for auto behaviour
    if (m_firstPortId.size() && m_firstElement->getPort(m_firstPortId)) {
        auto p = m_firstElement->getPort(m_firstPortId);
        if (p->getAlign() == -1) { // If Auto
            p->setOffset(m_firstElement->mapFromScene(p11));
        }
    }
    if (m_lastPortId.size() && m_lastElement->getPort(m_lastPortId)) {
        auto p = m_lastElement->getPort(m_lastPortId);
        if (p->getAlign() == -1) { // If Auto
            p->setOffset(m_lastElement->mapFromScene(p21));
        }
    }

    // recompute for arrow ends
    p11 = m_firstElement->getIntersectionPoint(QLineF(p1c, m_controlPos),
                                               m_firstPortId, 1.5);
    p21 = m_lastElement->getIntersectionPoint(QLineF(p2c, m_controlPos),
                                              m_lastPortId, 1.5);

    bool intersected = (!p11.isNull()) && (!p21.isNull());

    // update shape path
    m_shapeCachePath = QPainterPath();

    double arrowSize = getVisibleWeight() + ARROW_SIZE;

    // center
    m_controlPos = (p11 + p21) / 2;
    QPointF cpos = m_controlPos;
    if (m_bendFactor < 0) { // Angle with respect starting element
        m_controlPos.setX(p11.x());
        m_controlPos.setY(p21.y());
        QLineF ld(m_controlPos, p11);
        if (ld.length() < m_firstElement
                              ->getMinDim()) { // to close leave it to be a straight line
            m_controlPos = cpos;
        }
    } else if (m_bendFactor > 0) {
        m_controlPos.setX(p21.x());
        m_controlPos.setY(p11.y());
        QLineF ld(m_controlPos, p21);
        if (ld.length() < m_lastElement->getMinDim()) { // to close leave it to be a straight line
            m_controlPos = cpos;
        }
    }

    QLineF l1(p11, p21);
    setLine(l1);

    // shift line by arrows
    auto len = l1.length();
    bool isArrow = (len > arrowSize * 2);

    if (isArrow && (m_itemFlags & CF_Mutual_Arrows)) {
        l1 = Utils::extendLine(l1, m_itemFlags & CF_Start_Arrow ? -(float)arrowSize : 0,
                               m_itemFlags & CF_End_Arrow ? (float)arrowSize : 0);
    }

    m_shapeCachePath.moveTo(p11);
    m_shapeCachePath.lineTo(m_controlPos);
    m_shapeCachePath.lineTo(p21);

    m_controlPoint = m_controlPos; // line().center();
    auto fullLen = QLineF(p1c, p2c).length();

    // if no intersection or fullLen < sidelength of element : drop the shape
    if (!intersected || fullLen < m_firstElement->getMinDim()) {
        m_shapeCachePath = QPainterPath();
    }
    // rasterise the line stroke
    QPainterPathStroker stroker;
    stroker.setWidth(6);
    m_selectionShapePath = stroker.createStroke(m_shapeCachePath);

    // update();

    // update text label
    if (getScene() && getScene()->itemLabelsEnabled()) {
        if (m_shapeCachePath.isEmpty()) {
            m_labelItem->hide();
        } else {
            m_labelItem->show();

            updateLabelPosition();
            updateLabelDecoration();
        }
    }
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR



