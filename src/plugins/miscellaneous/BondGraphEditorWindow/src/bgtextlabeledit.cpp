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
#include "bgtextlabeledit.h"
#include "sceneitem.h"

#include <QKeyEvent>
#include <QTextCursor>
#include <QTextDocument>

namespace OpenCOR {
namespace BondGraphEditorWindow {
BGTextLabelEdit::BGTextLabelEdit()
{
    setTextInteractionFlags(Qt::TextEditorInteraction);

    connect(document(), &QTextDocument::contentsChanged, this,
            &BGTextLabelEdit::updateGeometry);
}

BGTextLabelEdit::~BGTextLabelEdit()
{
}

void BGTextLabelEdit::updateGeometry()
{
    if (m_item) {
        QPointF center = m_item->getLabelCenter();
        double w = boundingRect().width();
        double h = boundingRect().height();
        setPos(center.x() - w / 2, center.y() - h / 2);
    }
}

bool BGTextLabelEdit::onKeyPressed(BGEditorScene &scene, QKeyEvent *key_event)
{
    Q_UNUSED(scene);
    return QGraphicsTextItem::sceneEvent(key_event);
}

bool BGTextLabelEdit::onKeyReleased(BGEditorScene &scene, QKeyEvent *key_event)
{
    Q_UNUSED(scene);
    if (key_event->matches(QKeySequence::Cancel)) // Esc
    {
        finishEdit(true);
        return true;
    }

    return QGraphicsTextItem::sceneEvent(key_event);
}

bool BGTextLabelEdit::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::FocusOut) {
        finishEdit(true);
        return true;
    }

    return QGraphicsTextItem::sceneEvent(event);
}

void BGTextLabelEdit::startEdit(SceneItem *item)
{
    m_item = item;

    if (m_item == nullptr)
        return;

    auto scene = m_item->getScene();
    if (scene == nullptr)
        return;

    scene->selectItem(m_item);

    m_storedText = m_item->getAttribute("label").toString();
    m_item->showLabel(false);

    setPlainText(m_storedText);
    setFont(m_item->getAttribute("label.font").value<QFont>());
    setDefaultTextColor(m_item->getAttribute("label.color").value<QColor>());

    updateGeometry();

    QTextCursor c = textCursor();
    c.select(QTextCursor::Document);
    setTextCursor(c);

    setFocus();

    scene->addItem(this);
    show();

    Q_EMIT editingStarted(m_item);
}

void BGTextLabelEdit::finishEdit(bool accept)
{
    if (m_item == nullptr)
        return;

    Q_EMIT editingFinished(m_item, !accept);

    auto scene = m_item->getScene();
    if (scene == nullptr)
        return;

    QString text = toPlainText();
    if (accept && m_storedText != text) {
        m_item->setAttribute("label", text);

        scene->addUndoState();
    }

    m_item->showLabel(true);

    m_item = nullptr;
    scene->removeItem(this);
}
} // namespace BondGraphEditorWindow
} // namespace OpenCOR