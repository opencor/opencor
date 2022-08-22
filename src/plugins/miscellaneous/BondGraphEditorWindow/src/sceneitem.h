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

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnon-virtual-dtor"
    #pragma clang diagnostic ignored "-Wdelete-incomplete"
#endif

#include <QGraphicsItem>
#include <QGraphicsSimpleTextItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QUuid>

#include "bgeditorscene.h"
#include "interactiveiteminterface.h"
#include "utils.h"

namespace OpenCOR {
namespace BondGraphEditorWindow {

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

class SceneItem : public InteractiveItemInterface
{
public:
    SceneItem();
    virtual ~SceneItem();

    int itemFlags() const;
    void setItemFlags(int flag);
    void setItemFlag(int flag);
    void resetItemFlag(int flag);
    int itemStateFlags() const;
    void setItemStateFlag(int flag);
    void resetItemStateFlag(int flag);
    static QByteArray TYPE();
    virtual QByteArray typeId() const;
    virtual QString createNewId() const;
    virtual bool setDefaultId();
    virtual QString getDisplayName() const;
    virtual void setDisplayName(QString name);
    template<class C> QString createUniqueId(const QString &Tmpl) const;

    // attributes
    virtual bool hasLocalAttribute(const QByteArray &attrId) const;
    const QMap<QByteArray, QVariant> &getLocalAttributes() const;
    virtual bool setAttribute(const QByteArray &attrId, const QVariant &v);
    virtual bool removeAttribute(const QByteArray &attrId);
    virtual QVariant getAttribute(const QByteArray &attrId) const;
    virtual void setClassType(QString classType);
    virtual QString getClassType();
    virtual bool junctionType() const;
    virtual QByteArray classId() const;
    virtual QByteArray superClassId() const;
    QString getId() const;
    void setId(const QString &id);

    enum VisibleFlags { VF_ANY = 0,
                        VF_LABEL = 1,
                        VF_TOOLTIP = 2 };
    virtual QSet<QByteArray> getVisibleAttributeIds(int flags) const;

    // scene access
    QGraphicsItem *getSceneItem() const;

    BGEditorScene *getScene() const;

    void addUndoState();

    // labels
    virtual void updateLabelContent();
    virtual void updateLabelDecoration();
    virtual void updateLabelPosition();
    void setLabelText(const QString &text);
    QString getLabelText();
    void showLabel(bool on);
    void setIcon(const QPixmap &icon);
    QPixmap &getIcon();
    QRectF getSceneLabelRect() const;
    virtual QPointF getLabelCenter() const;

    // transformations
    virtual void transform(const QRectF &oldRect, const QRectF &newRect,
                           double xc, double yc, bool changeSize,
                           bool changePos);

    // serialization
    virtual bool storeTo(QDataStream &out, quint64 version64) const;
    virtual bool restoreFrom(QDataStream &out, quint64 version64);
    friend void to_json(nlohmann::json &j, const SceneItem &p);
    friend void from_json(const nlohmann::json &j, SceneItem &p);

    typedef QMap<quint64, SceneItem *> CItemLinkMap;
    virtual bool linkAfterRestore(const CItemLinkMap &idToItem);
    virtual bool linkAfterPaste(const CItemLinkMap &idToItem);

    // default the same
    static void beginRestore();
    static void endRestore();

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
    virtual void onControlPointMoved(ControlPoint *controlPoint,
                                     const QPointF &pos);
    virtual void onControlPointDelete(ControlPoint *controlPoint);

    // call from drag event
    virtual ItemDragTestResult
    acceptDragFromItem(QGraphicsItem *draggedItem);
    // called after restoring data (reimplement to update cached attribute values)
    virtual void updateCachedItems();
    static int getItemCount();
    static void setItemCount(int count);

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

template<class C>
QString SceneItem::createUniqueId(const QString &tmpl) const
{
    return tmpl.arg(++s_count);
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
