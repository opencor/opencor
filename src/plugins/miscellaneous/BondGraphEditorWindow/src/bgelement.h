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

class BGElement : public SceneItem, public QGraphicsRectItem
{
public:
    using Shape = QGraphicsRectItem;

    explicit BGElement(QGraphicsItem *parent = nullptr);
    ~BGElement() override;
    static QByteArray TYPE();
    virtual QByteArray typeId() const;
    virtual QString createNewId() const;
    int bgElementFlags() const;
    void setBGElementFlags(int flag);
    void setBGElementFlag(int flag);
    void resetBGElementFlag(int flag);
    nlohmann::json setBGElementDefinitions(QString json);
    nlohmann::json &getStateParameterJson();
    void setStateParameterJson(nlohmann::json &json);

    nlohmann::json &getJson();
    void setJson(nlohmann::json &json);
    QPixmap getRenderedLatex(int fontSize = 12);

    QString getDisplayName() const;
    void setDisplayName(QString name);
    QString getConstiutiveRelation();
    bool isProxy();
    bool canBeProxy();
    void setAsProxy();

    // override
    SceneItem *create() const override;
    virtual SceneItem *clone();
    virtual void copyDataFrom(SceneItem *from);

    // transformations
    void transform(const QRectF &oldRect, const QRectF &newRect,
                   double xc, double yc, bool changeSize,
                   bool changePos) override;

    // attributes
    virtual QSizeF getSize() const;
    virtual int getMinDim() const;
    virtual void setSize(float w, float h);

    virtual bool hasLocalAttribute(const QByteArray &attrId) const;
    virtual bool setAttribute(const QByteArray &attrId, const QVariant &value);
    virtual bool removeAttribute(const QByteArray &attrId);
    virtual QVariant getAttribute(const QByteArray &attrId) const;
    virtual QByteArray classId() const;
    virtual QByteArray superClassId() const;

    // ports
    BGPort *addPort(const QByteArray &portId = "",
                    QPointF pos = QPointF(-1000, -1000));
    bool removePort(const QByteArray &portId);
    void reorientPort(int dir);
    bool movePort(const QByteArray &portId, QPointF pos);
    bool renamePort(const QByteArray &portId, const QByteArray &newId);
    BGPort *getPort(const QByteArray &portId) const;
    int getMaxPorts() const;
    QByteArrayList getPortIds() const;

    // serialization
    virtual bool storeTo(QDataStream &out, quint64 version64) const;
    virtual bool restoreFrom(QDataStream &out, quint64 version64);
    friend void to_json(nlohmann::json &json_, const BGElement &pelem);
    friend void from_json(const nlohmann::json &json_, BGElement &pelem);

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
    QSet<BGConnection *> getConnections() const;

    // returns all incoming connections of the element.
    QSet<BGConnection *> getInConnections() const;

    // returns all outgoing connections of the element.
    QSet<BGConnection *> getOutConnections() const;

    // returns true if new connection from this element is allowed.
    virtual bool allowStartConnection() const;

    // returns true if a connection from this element to itself is allowed.
    virtual bool allowCircledConnection() const;

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
    friend void from_json(const nlohmann::json &json_, BGEditorScene &pscene);

protected:
    // override
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
                        const QVariant &value) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = Q_NULLPTR) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void updateLabelPosition() override;
    void updateCachedItems() override;

private:
    void recalculateShape();
    void updateConnections();
    void removeOrphanPorts();
    void resize(float size);
    void resize(float w, float h);
    void resize(const QSizeF &size);

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