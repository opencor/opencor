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
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

#include "bgconnection.h"
#include "bgeditorscene.h"
#include "bgelement.h"
#include "bgtransformrect.h"
#include "sceneitem.h"

#include <QDebug>

namespace OpenCOR {
namespace BondGraphEditorWindow {

const double MIN_RECT_SIZE = 15.0;

BGTransformRect::BGTransformRect() {
  m_points[0].cursor = Qt::SizeFDiagCursor;
  m_points[1].cursor = Qt::SizeVerCursor;
  m_points[2].cursor = Qt::SizeBDiagCursor;
  m_points[3].cursor = Qt::SizeHorCursor;
  m_points[4].cursor = Qt::SizeHorCursor;
  m_points[5].cursor = Qt::SizeBDiagCursor;
  m_points[6].cursor = Qt::SizeVerCursor;
  m_points[7].cursor = Qt::SizeFDiagCursor;
}

BGTransformRect::~BGTransformRect() {}

void BGTransformRect::setMoveOnly(bool on) { m_moveOnlyMode = on; }

void BGTransformRect::onActivated(BGEditorScene &scene) {
  m_dragPoint = -1;

  onSelectionChanged(scene);
}

void BGTransformRect::onSelectionChanged(BGEditorScene &scene) {
  auto selItems = scene.getTransformableItems();
  if (selItems.size()) {
    QRectF r = Utils::getBoundingRect(selItems);
    m_lastRect = r;
  } else {
    m_lastRect = QRectF();
  }

  scene.update();
}

void BGTransformRect::onSceneChanged(BGEditorScene &scene) {
  onSelectionChanged(scene);
}

void BGTransformRect::onDragItem(BGEditorScene &scene,
                                 QGraphicsSceneMouseEvent * /*mouseEvent*/,
                                 QGraphicsItem * /*dragItem*/) {
  onSelectionChanged(scene);
}

void BGTransformRect::draw(class BGEditorScene &scene, QPainter *painter,
                           const QRectF &) {
  QRectF r = m_lastRect;
  if (r.isEmpty() || r.isNull() || !r.isValid())
    return;

  // update points
  m_points[0].pos = r.topLeft();
  m_points[1].pos = QPointF(r.center().x(), r.top());
  m_points[2].pos = r.topRight();
  m_points[3].pos = QPointF(r.left(), r.center().y());
  m_points[4].pos = QPointF(r.right(), r.center().y());
  m_points[5].pos = r.bottomLeft();
  m_points[6].pos = QPointF(r.center().x(), r.bottom());
  m_points[7].pos = r.bottomRight();

  const QPen rectPen(QColor(0x333333), 0, Qt::SolidLine);
  painter->setPen(rectPen);
  painter->drawRect(r);

  auto view = scene.getCurrentView();
  if (view) {
    for (int i = 0; i < 8; ++i) {
      // zoom-independend control points
      QPoint viewPos = view->mapFromScene(m_points[i].pos);
      QPointF topLeft =
          view->mapToScene(QPoint(viewPos.x() - 4, viewPos.y() - 4));
      QPointF bottomRight =
          view->mapToScene(QPoint(viewPos.x() + 4, viewPos.y() + 4));

      m_points[i].sceneRect = QRectF(topLeft, bottomRight);

      painter->fillRect(m_points[i].sceneRect, Qt::SolidPattern);
    }

    scene.invalidate();
  }
}

bool BGTransformRect::onMousePressed(BGEditorScene &scene,
                                     QGraphicsSceneMouseEvent *mouseEvent) {
  bool isDragging = (mouseEvent->button() == Qt::LeftButton);
  if (!isDragging)
    return false;

  auto pos = mouseEvent->scenePos();

  for (int i = 0; i < 8; ++i) {
    if (m_points[i].sceneRect.contains(pos)) {
      m_dragRect = m_lastRect;
      m_dragPos = m_lastPos = pos;
      m_dragPoint = i;

      doSetupItems(scene);
      return true;
    }
  }

  doReset();

  return false;
}

bool BGTransformRect::onMouseReleased(BGEditorScene &scene,
                                      QGraphicsSceneMouseEvent *mouseEvent) {
  bool isDragging = (mouseEvent->button() == Qt::LeftButton);
  if (!isDragging)
    return false;

  // nothing was dragged
  if (m_dragPoint == -1)
    return false;

  // else finish the drag
  if (m_lastPos != m_dragPos) {
    // snap after transform
    // if (scene.gridSnapEnabled())
    {
      for (auto element : m_nodesTransform) {
        element->setPos(scene.getSnapped(element->pos()));
      }

      for (auto element : m_nodesMove) {
        element->setPos(scene.getSnapped(element->pos()));
      }
    }

    scene.addUndoState();
  }

  scene.setSceneCursor(Qt::ArrowCursor);

  doReset();

  return true;
}

bool BGTransformRect::onMouseMove(BGEditorScene &scene,
                                  QGraphicsSceneMouseEvent *mouseEvent) {

  auto pos = mouseEvent->scenePos();

  bool isDragging = (mouseEvent->buttons() & Qt::LeftButton);

  if (isDragging && m_dragPoint >= 0) {
    // drag

    QPointF deltaPos = pos - m_lastPos;

    if (!deltaPos.isNull()) {
      QRectF newRect = m_lastRect;

      bool isShift = (mouseEvent->modifiers() & Qt::ShiftModifier);

      // default transform
      switch (m_dragPoint) {
      case 0:
        newRect.setTopLeft(pos);
        break;
      case 1:
        newRect.setTop(pos.y());
        break;
      case 2:
        newRect.setTopRight(pos);
        break;
      case 3:
        newRect.setLeft(pos.x());
        break;
      case 4:
        newRect.setRight(pos.x());
        break;
      case 5:
        newRect.setBottomLeft(pos);
        break;
      case 6:
        newRect.setBottom(pos.y());
        break;
      case 7:
        newRect.setBottomRight(pos);
        break;
      }

      // if shift pressed: mirror around center
      if (isShift && newRect.isValid() && m_dragRect.isValid()) {
        qreal dx_r = newRect.right() - m_dragRect.right();
        qreal dx_l = newRect.left() - m_dragRect.left();
        qreal dy_t = newRect.top() - m_dragRect.top();
        qreal dy_b = newRect.bottom() - m_dragRect.bottom();

        switch (m_dragPoint) {
        case 0:
          newRect.setBottomRight(m_dragRect.bottomRight() -
                                 QPointF(dx_l, dy_t));
          break;
        case 1:
          newRect.setBottom(m_dragRect.bottom() - dy_t);
          break;
        case 2:
          newRect.setBottomLeft(m_dragRect.bottomLeft() - QPointF(dx_r, dy_t));
          break;
        case 3:
          newRect.setRight(m_dragRect.right() - dx_l);
          break;
        case 4:
          newRect.setLeft(m_dragRect.left() - dx_r);
          break;
        case 5:
          newRect.setTopRight(m_dragRect.topRight() - QPointF(dx_l, dy_b));
          break;
        case 6:
          newRect.setTop(m_dragRect.top() - dy_b);
          break;
        case 7:
          newRect.setTopLeft(m_dragRect.topLeft() - QPointF(dx_r, dy_b));
          break;
        }
      }

      // do transform if rect is valid
      if (newRect.isValid() && newRect.width() >= MIN_RECT_SIZE &&
          newRect.height() >= MIN_RECT_SIZE) {
        doTransformBy(scene, m_lastRect, newRect);
        m_lastRect = newRect;
      } else
        return false;
    }

    m_lastPos = pos;
    return true;
  }

  // no drag - check hover
  for (int i = 0; i < 8; ++i) {
    if (m_points[i].sceneRect.contains(pos)) {
      scene.setSceneCursor(m_points[i].cursor);
      return true;
    }
  }

  // no hit
  return false;
}

// privates

void BGTransformRect::doReset() {
  m_dragRect = QRectF();
  m_dragPos = m_lastPos = QPointF();
  m_dragPoint = -1;

  m_nodesTransform.clear();
  m_nodesMove.clear();
  m_others.clear();
}

void BGTransformRect::doSetupItems(BGEditorScene &scene) {
  // prepare transform lists
  auto selItems = scene.getTransformableItems();

  // go for nodes
  for (auto item : selItems) {
    auto cnode = dynamic_cast<BGElement *>(item);
    if (cnode) {
      m_nodesTransform << cnode;
      continue;
    }
  }

  // go for edges & rest
  for (auto item : selItems) {
    auto cnode = dynamic_cast<BGElement *>(item);
    if (cnode)
      continue;

    auto cedge = dynamic_cast<BGConnection *>(item);
    if (cedge) {
      m_others << cedge;
      auto n1 = cedge->firstElement();
      auto n2 = cedge->lastElement();
      if (!m_nodesTransform.contains(n1) && !m_nodesMove.contains(n1))
        m_nodesMove << n1;
      if (!m_nodesTransform.contains(n2) && !m_nodesMove.contains(n2))
        m_nodesMove << n2;
      continue;
    }

    auto citem = dynamic_cast<SceneItem *>(item);
    if (citem) {
      m_others << citem;
      continue;
    }
  }
}

void BGTransformRect::doTransformBy(BGEditorScene &scene, QRectF oldRect,
                                    QRectF newRect) {
  if (oldRect == newRect)
    return;

  // normalize borders
  const int margin = scene.getBoundingMargin();
  oldRect.adjust(margin, margin, -margin, -margin);
  newRect.adjust(margin, margin, -margin, -margin);

  if (!oldRect.isValid() || !newRect.isValid())
    return;

  double xc = newRect.width() / oldRect.width();
  double yc = newRect.height() / oldRect.height();

  // run transformation
  bool changeSize = !m_moveOnlyMode;

  for (auto element : m_nodesTransform) {
    element->transform(oldRect, newRect, xc, yc, changeSize, true);
  }

  for (auto element : m_nodesMove) {
    element->transform(oldRect, newRect, xc, yc, false, true);
  }

  for (auto item : m_others) {
    item->transform(oldRect, newRect, xc, yc, changeSize, true);
  }
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR