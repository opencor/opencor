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

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QPaintEvent>
#include <QTimer>

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGEditorScene;

class BGEditorView : public QGraphicsView {
  Q_OBJECT

public:
  BGEditorView(BGEditorScene *scene, QWidget *parent = nullptr);
  BGEditorView(QWidget *parent = nullptr);
  virtual ~BGEditorView();

  // zoom
  double getZoom() const { return m_currentZoom; }
  // target is % value to zoom to (1.0 = 100% = reset zoom, 0.25 = 25% = 1/4
  // etc.)
  void zoomTo(double target);
  // factor if relative % value to zoom by (2 = 2x from current etc.)
  void zoomBy(double factor);

  double getZoomBeforeFit() const { return m_zoomBeforeFit; }
  void fitToView();
  void fitSelectedToView();
  void zoomBack();

  // center
  QPointF getCenter() const;

  void centerContent();

  // scene
  QGraphicsItem *getDragItem() {
    if (scene())
      return scene()->mouseGrabberItem();
    else
      return nullptr;
  }

  // override
  virtual void mousePressEvent(QMouseEvent *e);
  virtual void mouseMoveEvent(QMouseEvent *e);
  virtual void mouseReleaseEvent(QMouseEvent *e);
  virtual void contextMenuEvent(QContextMenuEvent *e);
  virtual void wheelEvent(QWheelEvent *e);
  virtual void keyReleaseEvent(QKeyEvent *keyEvent);

  /*
          void dragEnterEvent(QDragEnterEvent* event);
          void dragMoveEvent(QDragMoveEvent* event);
          void dragLeaveEvent(QDragLeaveEvent* event);
          void dropEvent(QDropEvent* event);
  */

  void paintEvent(QPaintEvent *event) {
    QPaintEvent *newEvent = new QPaintEvent(event->region().boundingRect());
    QGraphicsView::paintEvent(newEvent);
    delete newEvent;
  }

Q_SIGNALS:
  void scaleChanged(double);

private Q_SLOTS:
  void restoreContextMenu();
  void onScrollTimeout();

private:
  DragMode m_dragModeTmp;
  Qt::ContextMenuPolicy m_menuModeTmp;
  bool m_interactiveTmp = false;
  bool m_moved = false;
  QPoint m_pos;

  double m_currentZoom = 1.0;
  double m_zoomBeforeFit = 0.0; // 0 means no zoom called yet
  QPointF m_dxyBeforeFit;

  QTimer m_scrollTimer;
  float m_scrollThreshold = 30;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR