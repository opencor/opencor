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

class BGEditorView : public QGraphicsView
{
    Q_OBJECT

public:
    BGEditorView(BGEditorScene *scene, QWidget *parent = nullptr);
    explicit BGEditorView(QWidget *parent = nullptr);
    ~BGEditorView() override;

    // zoom
    double getZoom() const;
    // target is % value to zoom to (1.0 = 100% = reset zoom, 0.25 = 25% = 1/4
    // etc.)
    void zoomTo(double target);
    // factor if relative % value to zoom by (2 = 2x from current etc.)
    void zoomBy(double factor);

    double getZoomBeforeFit() const;
    void fitToView();
    void fitSelectedToView();
    void zoomBack();

    // center
    QPointF getCenter() const;
    void centerContent();

    // scene
    QGraphicsItem *getDragItem();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyReleaseEvent(QKeyEvent *keyEvent) override;

    void paintEvent(QPaintEvent *event) override;

Q_SIGNALS:
    void scaleChanged(double newScale);

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