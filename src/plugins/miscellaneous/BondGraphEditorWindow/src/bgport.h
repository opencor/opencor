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
#include <QGraphicsEllipseItem>
#include <QUuid>

#include "interactiveiteminterface.h"
#include "thirdparty/json.hpp"

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGElement;
class BGConnection;
class BGElementEditorScene;

class BGPort : public QGraphicsEllipseItem, public InteractiveItemInterface
{
public:
    explicit BGPort(BGElement *element, const QByteArray &portId = QByteArray(),
                    QPointF pos = QPointF(0, 0));
    virtual ~BGPort();

    BGElement *getElement() const;
    QUuid getUid() const;
    const QByteArray &getId() const;
    bool getDeleteFlag() const;
    double getX() const;
    double getY() const;
    QColor getColor() const;
    int getAlign() const;
    int getDirection() const;
    void setDirection(int dir);
    void setId(const QByteArray &portId);
    void setDeleteFlag(const bool flag);
    void setAlignment(int align);
    void setOffset(QPointF pos);
    void setColor(const QColor &color);
    void setConnection(BGConnection *conn);
    BGConnection *getConnection() const;
    void copyDataFrom(const BGPort &port);
    // serialization
    virtual bool storeTo(QDataStream &out, quint64 version64) const;
    friend void to_json(nlohmann::json &j, const BGPort &p);
    friend void from_json(const nlohmann::json &j, BGPort &p);

    // callbacks
    void onParentDeleted();
    virtual void onParentGeometryChanged();

    virtual void onClick(QGraphicsSceneMouseEvent * /*mouseEvent*/);

    virtual ItemDragTestResult acceptDragFromItem(QGraphicsItem *draggedItem);
    virtual void leaveDragFromItem(QGraphicsItem *draggedItem);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = Q_NULLPTR);

    BGElement *m_Element = nullptr;
    BGConnection *m_Connection = nullptr;
    QPolygonF m_shapeCache;
    QByteArray m_id;
    QPointF m_position;
    int m_align;
    int m_direction;
    bool m_canDelete;
    QRectF m_anchorSize;
    QUuid uid;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR