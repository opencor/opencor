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

#include <QByteArray>
#include <QGraphicsLineItem>
#include <QtGlobal>

#include "sceneitem.h"

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGElement;

enum ConnectionFlags // extends SceneItemFlags
{
    CF_Start_Arrow = IF_LastFlag,
    CF_End_Arrow = IF_LastFlag << 2,
    CF_Mutual_Arrows = CF_Start_Arrow | CF_End_Arrow, // start | end
    CF_Half_Arrow = 32
};

class BGConnection : public SceneItem, public QGraphicsLineItem
{
public:
    using Shape = QGraphicsLineItem;

    BGConnection(QGraphicsItem *parent = Q_NULLPTR);
    ~BGConnection() override;

    // public
    void setFirstElement(BGElement *element, const QByteArray &portId = "",
                         bool updatePos = true);
    void setLastElement(BGElement *element, const QByteArray &portId = "",
                        bool updatePos = true);

    bool reattach(BGElement *oldNode, BGElement *newNode,
                  const QByteArray &portId = "");
    bool reattach(BGElement *element, const QByteArray &oldPortId,
                  const QByteArray &newPortId);

    BGElement *firstElement() const;
    BGElement *lastElement() const;

    const QByteArray &firstPortId() const;
    const QByteArray &lastPortId() const;
    bool isValid() const;
    bool isCircled() const;
    double getWeight() const;
    virtual void reverse();

    QString createNewId() const override;

    ItemDragTestResult
    acceptDragFromItem(QGraphicsItem *draggedItem) override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    // attributes
    bool hasLocalAttribute(const QByteArray &attrId) const override;
    bool setAttribute(const QByteArray &attrId, const QVariant &value) override;
    bool removeAttribute(const QByteArray &attrId) override;

    // serialization
    bool storeTo(QDataStream &out, quint64 version64) const override;
    bool restoreFrom(QDataStream &out, quint64 version64) override;
    bool linkAfterRestore(const CItemLinkMap &idToItem) override;
    bool linkAfterPaste(const CItemLinkMap &idToItem) override;

    // callbacks
    virtual void onElementMoved(BGElement *element);
    virtual void onElementDetached(BGElement *element);
    virtual void onElementDeleted(BGElement *element);
    virtual void onElementPortDeleted(BGElement *element,
                                      const QByteArray &portId);
    virtual void onElementPortRenamed(BGElement *element,
                                      const QByteArray &portId,
                                      const QByteArray &oldId);
    virtual void onParentGeometryChanged() = 0;
    void onItemRestored() override;

protected:
    void setupPainter(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget = Q_NULLPTR);
    void drawSelection(QPainter *painter,
                       const QStyleOptionGraphicsItem *option) const;
    void drawArrow(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   bool first, const QLineF &direction) const;
    void drawArrow(QPainter *painter, qreal shift, const QLineF &direction) const;
    QLineF calculateArrowLine(const QPainterPath &path, bool first,
                              const QLineF &direction) const;

    // override
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
                        const QVariant &value) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    // cached attributes
    void updateCachedItems() override;
    virtual void updateArrowFlags(const QString &direction);

    double getVisibleWeight() const;

protected:
    BGElement *m_firstElement = nullptr;
    BGElement *m_lastElement = nullptr;
    QByteArray m_firstPortId, m_lastPortId;
    QPolygonF arrowHead;
    QPainterPath m_selectionShapePath;
    QPainterPath m_shapeCachePath;
    quint64 m_tempFirstElementId = 0;
    quint64 m_tempLastElementId = 0;
    const int ARROW_SIZE = 15;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR