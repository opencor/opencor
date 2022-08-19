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

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wold-style-cast"
#endif

#include "bgeditorscene.h"
#include "bgeditorview.h"

#include <QGLWidget>
#include <QGuiApplication>
#include <QMimeData>
#include <QMouseEvent>
#include <QScrollBar>

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGEditorView::BGEditorView(QWidget *parent) :
    QGraphicsView(parent), m_menuModeTmp(Qt::PreventContextMenu)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    // setViewport(new QGLWidget);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    setDragMode(RubberBandDrag);

    setRenderHint(QPainter::Antialiasing);
    setOptimizationFlags(DontSavePainterState);
    setOptimizationFlags(DontAdjustForAntialiasing);

    setFocus();

    connect(&m_scrollTimer, SIGNAL(timeout()), this, SLOT(onScrollTimeout()));
    m_scrollTimer.setInterval(100);
}

BGEditorView::BGEditorView(BGEditorScene *scene, QWidget *parent) :
    BGEditorView(parent)
{
    setScene(scene);
}

BGEditorView::~BGEditorView()
{
}

// zoom

double BGEditorView::getZoom() const
{
    return m_currentZoom;
}
double BGEditorView::getZoomBeforeFit() const
{
    return m_zoomBeforeFit;
}

QGraphicsItem *BGEditorView::getDragItem()
{
    if (scene() != nullptr) {
        return scene()->mouseGrabberItem();
    }

    return nullptr;
}
void BGEditorView::paintEvent(QPaintEvent *event) 
{
    auto *newEvent = new QPaintEvent(event->region().boundingRect());
    QGraphicsView::paintEvent(newEvent);
    delete newEvent;
}
void BGEditorView::zoomTo(double target)
{
    QTransform mat;
    mat.scale(target, target);
    setTransform(mat);

    m_currentZoom = target;

    Q_EMIT scaleChanged(m_currentZoom);
}

void BGEditorView::zoomBy(double factor)
{
    double target = m_currentZoom * factor;

    zoomTo(target);
}

void BGEditorView::fitToView()
{
    m_zoomBeforeFit = m_currentZoom;
    m_dxyBeforeFit = getCenter();

    fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);

    m_currentZoom = matrix().m11();

    Q_EMIT scaleChanged(m_currentZoom);
}

void BGEditorView::fitSelectedToView()
{
    auto items = scene()->selectedItems();
    if (items.isEmpty())
        return;

    m_zoomBeforeFit = m_currentZoom;
    m_dxyBeforeFit = getCenter();

    QRectF r;
    for (const auto item : items) {
        r |= item->sceneBoundingRect();
    }

    fitInView(r, Qt::KeepAspectRatio);

    m_currentZoom = matrix().m11();

    Q_EMIT scaleChanged(m_currentZoom);
}

void BGEditorView::zoomBack()
{
    zoomTo(m_zoomBeforeFit);
    centerOn(m_dxyBeforeFit);
}

QPointF BGEditorView::getCenter() const
{
    return mapToScene(viewport()->rect().center());
}

void BGEditorView::centerContent()
{
    fitToView();
    zoomTo(1);
}

void BGEditorView::restoreContextMenu()
{
    setContextMenuPolicy(m_menuModeTmp);
}

void BGEditorView::onScrollTimeout()
{
    const int scrollStep = 100;

    if (QGuiApplication::mouseButtons() & Qt::LeftButton) {
        auto globTopLeft =
            viewport()->mapToGlobal(QPoint(viewport()->x(), viewport()->y()));
        QRect globRect = QRect(globTopLeft, viewport()->size());

        if (QCursor::pos().x() > globRect.right()) {
            // int dx = QCursor::pos().x() - globRect.right();
            int dx = scrollStep;
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() + dx);
        } else if (QCursor::pos().x() < globRect.left()) {
            // int dx = globRect.left() - QCursor::pos().x();
            int dx = scrollStep;
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - dx);
        }

        if (QCursor::pos().y() > globRect.bottom()) {
            int dy = scrollStep;
            // int dy = QCursor::pos().y() - globRect.bottom();
            verticalScrollBar()->setValue(verticalScrollBar()->value() + dy);
        } else if (QCursor::pos().y() < globRect.top()) {
            int dy = scrollStep;
            // int dy = globRect.top() - QCursor::pos().y();
            verticalScrollBar()->setValue(verticalScrollBar()->value() - dy);
        }
    }
}

// override

#if defined Q_OS_WIN && !defined Q_OS_CYGWIN // Windows-confirm panning & context menu

void BGEditorView::contextMenuEvent(QContextMenuEvent *e)
{
    // ignore when dragging
    if (getDragItem()) {
        e->accept();
        return;
    }

    QGraphicsView::contextMenuEvent(e);
}

void BGEditorView::mousePressEvent(QMouseEvent *e)
{
    // ignore when dragging
    if (e->button() == Qt::RightButton && getDragItem()) {
        e->accept();
        return;
    }

    QGraphicsView::mousePressEvent(e);

    // start scroll when dragging
    if (e->button() == Qt::LeftButton) {
        m_scrollTimer.start();
    }
}

void BGEditorView::mouseMoveEvent(QMouseEvent *e)
{
    // enable RMB pan
    if (e->buttons() == Qt::RightButton) {
        if (dragMode() != ScrollHandDrag) {
            m_menuModeTmp = contextMenuPolicy();
            setContextMenuPolicy(Qt::PreventContextMenu);

            m_dragModeTmp = dragMode();
            setDragMode(ScrollHandDrag);

            m_interactiveTmp = isInteractive();
            setInteractive(false);

            QMouseEvent fake(e->type(), e->pos(), Qt::LeftButton, Qt::LeftButton,
                             e->modifiers());
            QGraphicsView::mousePressEvent(&fake);
        }
    }

    QGraphicsView::mouseMoveEvent(e);
}

void BGEditorView::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        m_scrollTimer.stop();
    }

    // disable RMB pan
    if (e->button() == Qt::RightButton && !e->buttons() && (dragMode() == ScrollHandDrag)) {
        QMouseEvent fake(e->type(), e->pos(), Qt::LeftButton, Qt::LeftButton,
                         e->modifiers());
        QGraphicsView::mouseReleaseEvent(&fake);

        setDragMode(m_dragModeTmp);

        setInteractive(m_interactiveTmp);

        QTimer::singleShot(100, this, SLOT(restoreContextMenu()));
    }
    // else
    {
        QGraphicsView::mouseReleaseEvent(e);
    }
}

#else // Linux/Unix/etc.

void BGEditorView::mousePressEvent(QMouseEvent *e)
{
    m_moved = false;

    // enable RMB pan
    if (e->button() == Qt::RightButton) {
        if (dragMode() != ScrollHandDrag) {
            m_menuModeTmp = contextMenuPolicy();
            setContextMenuPolicy(Qt::PreventContextMenu);

            setDragMode(ScrollHandDrag);

            m_interactiveTmp = isInteractive();
            setInteractive(false);

            QMouseEvent fake(e->type(), e->pos(), Qt::LeftButton, Qt::LeftButton,
                             e->modifiers());
            QGraphicsView::mousePressEvent(&fake);

            return;
        }
    }

    QGraphicsView::mousePressEvent(e);
}

void BGEditorView::mouseMoveEvent(QMouseEvent *e)
{
    m_moved = true;

    QGraphicsView::mouseMoveEvent(e);

    // check LMB selection
    // if (e->buttons() == Qt::LeftButton)
    //{
    //	onLeftClickMouseMove(e);
    //}
}

void BGEditorView::mouseReleaseEvent(QMouseEvent *e)
{
    // disabel RMB pan
    if (e->button() == Qt::RightButton && dragMode() == ScrollHandDrag) {
        QMouseEvent fake(e->type(), e->pos(), Qt::LeftButton, Qt::LeftButton,
                         e->modifiers());
        QGraphicsView::mouseReleaseEvent(&fake);

        setDragMode(RubberBandDrag);

        setInteractive(m_interactiveTmp);

        setContextMenuPolicy(m_menuModeTmp);

        if (!m_moved) {
            QMouseEvent fake1(QEvent::MouseButtonPress, e->pos(), Qt::RightButton,
                             Qt::RightButton, Qt::NoModifier);
            QGraphicsView::mousePressEvent(&fake1);

            QGraphicsView::mouseReleaseEvent(e);

            QContextMenuEvent fake2(QContextMenuEvent::Mouse, e->pos());
            contextMenuEvent(&fake2);
        }

        return;
    }

    QGraphicsView::mouseReleaseEvent(e);
}

void BGEditorView::contextMenuEvent(QContextMenuEvent *e)
{
    QGraphicsView::contextMenuEvent(e);
}

#endif

void BGEditorView::keyReleaseEvent(QKeyEvent *key_event)
{
    if (key_event->key() == Qt::Key_Home) {
        centerContent();
    }
}

void BGEditorView::wheelEvent(QWheelEvent *e)
{
    // original taken from
    // http://blog.automaton2000.com/2014/04/mouse-centered-zooming-in-qgraphicsview.html

    if ((e->modifiers() & Qt::ControlModifier) == Qt::ControlModifier && e->angleDelta().x() == 0) {
        QPoint pos = e->pos();
        QPointF posf = this->mapToScene(pos);

        double by = 1.0;
        double angle = e->angleDelta().y();

        if (angle > 0) {
            by = 1 + (angle / 360 * 0.5);
        } else if (angle < 0) {
            by = 1 - (-angle / 360 * 0.5);
        }

        // this->scale(by, by);
        zoomBy(by);

        double w = this->viewport()->width();
        double h = this->viewport()->height();

        double wf = this->mapToScene(QPoint((int)(w - 1), 0)).x() - this->mapToScene(QPoint(0, 0)).x();
        double hf = this->mapToScene(QPoint(0,(int)(h - 1))).y() - this->mapToScene(QPoint(0, 0)).y();

        double lf = posf.x() - pos.x() * wf / w;
        double tf = posf.y() - pos.y() * hf / h;

        /* try to set viewport properly */
        this->ensureVisible(lf, tf, wf, hf, 0, 0);

        QPointF newPos = this->mapToScene(pos);

        /* readjust according to the still remaining offset/drift
     * I don't know how to do this any other way */
        this->ensureVisible(QRectF(QPointF(lf, tf) - newPos + posf, QSizeF(wf, hf)),
                            0, 0);

        e->accept();
    }

    if ((e->modifiers() & Qt::ControlModifier) != Qt::ControlModifier) {
        /* no scrolling while control is held */
        QGraphicsView::wheelEvent(e);
    }
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR



