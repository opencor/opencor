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
#include "bgeditorsceneconstants.h"
#include "sceneitem.h"

#include <QGraphicsSceneMouseEvent>
#include <QMenu>

namespace OpenCOR {
namespace BondGraphEditorWindow {

bool SceneItem::s_duringRestore = false;
int SceneItem::s_count = 0;

SceneItem::SceneItem()
{
    m_labelItem = nullptr;
    // default item flags
    m_itemFlags = IF_DeleteAllowed | IF_FramelessSelection;
    m_internalStateFlags = IS_Attribute_Changed | IS_Need_Update;
}

SceneItem::~SceneItem()
{
    BGEditorScene *scene = getScene();
    if (scene)
        scene->onItemDestroyed(this);
}

int SceneItem::itemFlags() const
{
    return m_itemFlags;
}

void SceneItem::setItemFlags(int f)
{
    m_itemFlags = f;
}

void SceneItem::setItemFlag(int f)
{
    m_itemFlags |= f;
}

void SceneItem::resetItemFlag(int f)
{
    m_itemFlags &= ~f;
}

int SceneItem::itemStateFlags() const
{
    return m_internalStateFlags;
}

void SceneItem::setItemStateFlag(int f)
{
    m_internalStateFlags |= f;
}

void SceneItem::resetItemStateFlag(int f)
{
    m_internalStateFlags &= ~f;
}

QByteArray SceneItem::TYPE()
{
    return "SceneItem";
}

QByteArray SceneItem::typeId() const
{
    return "SceneItem";
}

QString SceneItem::createNewId() const
{
    return QUuid::createUuid().toString();
}

QString SceneItem::getDisplayName() const
{
    return mDisplayName;
}

void SceneItem::setDisplayName(QString name)
{
    mDisplayName = name;
};

const QMap<QByteArray, QVariant> &SceneItem::getLocalAttributes() const
{
    return m_attributes;
}

// IO

bool SceneItem::storeTo(QDataStream &out, quint64 version64) const
{
    Q_UNUSED(version64);
    out << m_attributes;
    out << m_id;
    out << m_labelItem->text();
    return true;
}

bool SceneItem::restoreFrom(QDataStream &out, quint64 version64)
{
    Q_UNUSED(version64);
    if (!out.atEnd()) {
        out >> m_attributes;
        out >> m_id;
        QString labelText;
        out >> labelText;
        m_labelItem->setText(labelText);
        return true;
    }

    return false;
}

// attributes

bool SceneItem::hasLocalAttribute(const QByteArray &attr_id) const
{
    if (attr_id == "id")
        return true;
    else
        return m_attributes.contains(attr_id);
}

bool SceneItem::setAttribute(const QByteArray &attr_id, const QVariant &v)
{
    setItemStateFlag(IS_Attribute_Changed);

    if (attr_id == "id") {
        m_id = v.toString();
        return true;
    }

    // real attributes
    m_attributes[attr_id] = v;

    return true;
}

QByteArray SceneItem::classId() const
{
    return "item";
}
QByteArray SceneItem::superClassId() const
{
    return QByteArray();
}

QString SceneItem::getId() const
{
    return m_id;
}
void SceneItem::setId(const QString &id)
{
    setAttribute("id", id);
}
QGraphicsItem *SceneItem::getSceneItem() const
{
    return dynamic_cast<QGraphicsItem *>((SceneItem *)this);
}
void SceneItem::updateLabelPosition()
{
}
QString SceneItem::getLabelText()
{
    return m_labelItem->text();
};

QPixmap &SceneItem::getIcon()
{
    return m_icon;
};

void SceneItem::transform(const QRectF &old_rect, const QRectF &new_rect,
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

void SceneItem::setClassType(QString class_type)
{
    m_attributes["bgClass"] = class_type;
}

QString SceneItem::getClassType()
{
    if (m_attributes.contains("bgClass")) {
        return m_attributes["bgClass"].toString();
    }
    return "";
}

bool SceneItem::junctionType() const
{
    if (m_attributes.contains("bgClass")) {
        return m_attributes["bgClass"] == "junction";
    }
    return false;
}

bool SceneItem::removeAttribute(const QByteArray &attr_id)
{
    if (m_attributes.remove(attr_id)) {
        setItemStateFlag(IS_Attribute_Changed);
        return true;
    } else
        return false;
}

QVariant SceneItem::getAttribute(const QByteArray &attr_id) const
{
    if (attr_id == "id")
        return m_id;

    if (m_attributes.contains(attr_id))
        return m_attributes[attr_id];

    if (auto scene = getScene())
        return scene->getClassAttribute(classId(), attr_id, true).defaultValue;

    return QVariant();
}

QSet<QByteArray> SceneItem::getVisibleAttributeIds(int flags) const
{
    QSet<QByteArray> result;

    if (flags == VF_ANY || flags == VF_TOOLTIP)
        result = getLocalAttributes().keys().toSet();

    if (flags == VF_LABEL)
        result += "label";

    if (auto scene = getScene()) {
        if (flags == VF_ANY || flags == VF_TOOLTIP)
            result += scene->getClassAttributes(classId(), false).keys().toSet();
        else
            result += scene->getVisibleClassAttributes(classId(), false);
    }

    return result;
}

bool SceneItem::setDefaultId()
{
    if (m_id.isEmpty()) {
        m_id = createNewId();
        return true;
    }

    return false;
}

bool SceneItem::linkAfterRestore(const CItemLinkMap & /*idToItem*/)
{
    return true;
}
bool SceneItem::linkAfterPaste(const CItemLinkMap &id_to_item)
{
    return linkAfterRestore(id_to_item);
}

// default the same
void SceneItem::beginRestore()
{
    s_duringRestore = true;
}
void SceneItem::endRestore()
{
    s_duringRestore = false;
}

// scene stuff

BGEditorScene *SceneItem::getScene() const
{
    if (auto sceneItem = getSceneItem())
        return dynamic_cast<BGEditorScene *>(sceneItem->scene());
    else
        return nullptr;
}

void SceneItem::addUndoState()
{
    if (auto scene = getScene())
        scene->addUndoState();
}

// cloning

void SceneItem::copyDataFrom(SceneItem *from)
{
    m_itemFlags = from->m_itemFlags;

    // copy attrs
    m_attributes = from->m_attributes;

    updateCachedItems();
}

// painting

void SceneItem::updateLabelContent()
{
    auto scene = getScene();
    if (!scene)
        return;

    if (!(m_internalStateFlags & IS_Attribute_Changed) && !(scene->itemLabelsEnabled()) && !(scene->itemLabelsforceUpdate()))
        return;

    resetItemStateFlag(IS_Attribute_Changed);

    if (!m_labelItem)
        return;

    if (getClassType() != "annotation") { // Annotation label text is handled by bgelement

        QString labelToShow;
        auto idsToShow = getVisibleAttributeIds(SceneItem::VF_LABEL);

        QMap<QByteArray, QString> visibleLabels;
        for (const QByteArray &id : idsToShow) {
            QString text = Utils::variantToText(getAttribute(id));
            if (text.size())
                visibleLabels[id] = text;
        }

        // ids first
        if (idsToShow.contains("id")) {
            labelToShow = "[" + visibleLabels["id"] + "]";
            visibleLabels.remove("id");
        }

        // other labels
        if (visibleLabels.size() == 1 && idsToShow.contains("label")) {
            if (labelToShow.size())
                labelToShow += "\n";

            labelToShow += visibleLabels.values().first();
        } else {
            for (auto it = visibleLabels.constBegin(); it != visibleLabels.constEnd();
                 ++it) {
                if (labelToShow.size())
                    labelToShow += "\n";

                labelToShow += QString("%1: %2").arg(QString(it.key())).arg(it.value());
            }
        }

        // text
        setLabelText(labelToShow);
    }

    // label attrs
    m_labelItem->setBrush(getAttribute(attr_label_color).value<QColor>());

    QFont f(getAttribute(attr_label_font).value<QFont>());

    if (!scene->isFontAntialiased())
        f.setStyleStrategy(QFont::NoAntialias);

    m_labelItem->setFont(f);

    // update explicitly
    m_labelItem->update();
}

void SceneItem::updateLabelDecoration()
{
    if (!m_labelItem)
        return;

    if (m_internalStateFlags & IS_Selected)
        m_labelItem->setOpacity(0.6);
    else
        m_labelItem->setOpacity(1.0);

    // if (m_internalStateFlags & IS_Selected)
    //	m_labelItem->setBrush(QColor(QStringLiteral("orange")));
    // else
    //	m_labelItem->setBrush(getAttribute(QByteArrayLiteral("label.color")).value<QColor>());
}

void SceneItem::setLabelText(const QString &text)
{
    if (m_labelItem) {
        m_labelItem->setText(text);
    }
}

void SceneItem::onControlPointMoved(ControlPoint * /*controlPoint*/,
                                    const QPointF & /*pos*/)
{
}
void SceneItem::onControlPointDelete(ControlPoint * /*controlPoint*/)
{
}

// call from drag event
ItemDragTestResult
SceneItem::acceptDragFromItem(QGraphicsItem * /*draggedItem*/)
{
    return Accepted;
}

int SceneItem::getItemCount()
{
    return s_count;
}

void SceneItem::setItemCount(int count)
{
    s_count = count;
}

void SceneItem::setIcon(const QPixmap &icon)
{
    m_icon = icon;
}

void SceneItem::showLabel(bool on)
{
    if (m_labelItem) {
        m_labelItem->setVisible(on);

        if (on)
            updateLabelDecoration();
    }
}

QRectF SceneItem::getSceneLabelRect() const
{
    if (!m_labelItem)
        return QRectF();
    else
        return m_labelItem->mapRectToScene(m_labelItem->boundingRect());
}

QPointF SceneItem::getLabelCenter() const
{
    if (m_labelItem)
        return getSceneLabelRect().center();
    else if (auto sceneItem = getSceneItem())
        return sceneItem->boundingRect().center();
    else
        return QPointF();
}

// callbacks

void SceneItem::onItemRestored()
{
    updateCachedItems();
}

void SceneItem::onItemSelected(bool state)
{
    if (state)
        m_internalStateFlags |= IS_Selected;
    else
        m_internalStateFlags &= ~IS_Selected;

    updateLabelDecoration();
}

void SceneItem::onHoverEnter(QGraphicsItem *scene_item,
                             QGraphicsSceneHoverEvent *)
{
    /*
  // update tooltip
  auto idsToShow = getVisibleAttributeIds(SceneItem::VF_TOOLTIP).toList();
  if (idsToShow.isEmpty())
  {
          sceneItem->setToolTip("");
          return;
  }

  qSort(idsToShow);

  QString tooltipToShow("<table columns=2>");

  for (const QByteArray& id : idsToShow)
  {
  QString text = Utils::variantToText(getAttribute(id));
          tooltipToShow += QString("<tr><td><b>%1</b>: </td> <td>
  %2</td>").arg(QString(id)).arg(text);
  }

  tooltipToShow += "</table>";
  */

    QString tooltipToShow = Utils::variantToText(getAttribute("tooltip"));

    scene_item->setToolTip(tooltipToShow);
}

// internal

void SceneItem::updateCachedItems()
{
    setItemStateFlag(IS_Attribute_Changed);

    // update text label
    if (getScene() && getScene()->itemLabelsEnabled()) {
        updateLabelContent();
        updateLabelPosition();
        updateLabelDecoration();
    }
}
} // namespace BondGraphEditorWindow
} // namespace OpenCOR