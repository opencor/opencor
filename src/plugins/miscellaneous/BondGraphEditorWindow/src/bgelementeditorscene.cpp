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
#include "bgelementeditorscene.h"
#include "bgconnection.h"
#include "bgdconnection.h"
#include "bgdiffundomanager.h"
#include "bgeditorsceneconstants.h"
#include "bgelement.h"
#include "bgelementsceneactions.h"
#include "bgport.h"
#include "controlpoint.h"


#include <graphbase.h>

#include <QApplication>
#include <QColorDialog>
#include <QDebug>
#include <QElapsedTimer>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMimeData>

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGElementEditorScene::BGElementEditorScene(QObject *parent)
    : BGEditorScene(parent), m_editMode(EM_Default), m_startElement(nullptr),
      m_endElement(nullptr), m_connection(nullptr), m_realStart(false),
      m_state(IS_None) {
  // default factories
  registerItemFactory<BGDConnection>();
  registerItemFactory<BGElement>();
  // registerItemFactory<BGCurveConnection>();

  m_elementsFactory = factory<BGElement>();
  m_connectionsFactory = factory<BGDConnection>();

  // go
  initialize();
}

void BGElementEditorScene::initialize() {
  BGEditorScene::initialize();

  // common constrains
  static BGAttributeConstraintsList *edgeStyles =
      new BGAttributeConstraintsList();
  if (edgeStyles->ids.isEmpty()) {
    edgeStyles->names << tr("None") << tr("Solid") << tr("Dots") << tr("Dashes")
                      << tr("Dash-Dot") << tr("Dash-Dot-Dot");
    edgeStyles->ids << "none"
                    << "solid"
                    << "dotted"
                    << "dashed"
                    << "dashdot"
                    << "dashdotdot";
  }

  // default element attr
  BGItemAttribute nodeAttr("color", "Color", QColor(Qt::white), ATTR_FIXED);
  setClassAttribute("element", nodeAttr);

  BGItemAttribute shapeAttr("shape", "Shape", "square", ATTR_FIXED);
  setClassAttribute("element", shapeAttr);

  createClassAttribute("element", "size", "Size", QSizeF(50.0, 50.0),
                       ATTR_MAPPED | ATTR_FIXED);
  // createClassAttribute("element", "width", "Width", 11.0f, ATTR_MAPPED);
  // createClassAttribute("element", "height", "Height", 11.0f, ATTR_MAPPED);

  // createClassAttribute("element", "pos", "Position", QPointF(),
  // ATTR_NODEFAULT | ATTR_MAPPED);
  createClassAttribute("element", "x", "X-Coordinate", 0.0f,
                       ATTR_NODEFAULT | ATTR_MAPPED | ATTR_FIXED);
  createClassAttribute("element", "y", "Y-Coordinate", 0.0f,
                       ATTR_NODEFAULT | ATTR_MAPPED | ATTR_FIXED);

  createClassAttribute("element", "stroke.style", "Stroke Style", "solid",
                       ATTR_FIXED, edgeStyles);
  createClassAttribute("element", "stroke.size", "Stroke Size", 1.0,
                       ATTR_FIXED);
  createClassAttribute("element", "stroke.color", "Stroke Color",
                       QColor(Qt::black), ATTR_FIXED);

  createClassAttribute("element", "degree", "Degree", 0,
                       ATTR_NODEFAULT | ATTR_VIRTUAL | ATTR_FIXED);

  // default connection attr
  BGItemAttribute edgeAttr("color", "Color", QColor(Qt::blue), ATTR_FIXED);
  setClassAttribute("connection", edgeAttr);

  BGItemAttribute directionAttr("direction", "Direction", "directed",
                                ATTR_FIXED);
  setClassAttribute("connection", directionAttr);

  BGItemAttribute weightAttr("weight", "Weight", 1.5, ATTR_FIXED);
  setClassAttribute("connection", weightAttr);

  BGItemAttribute styleAttr("style", "Style", "solid", ATTR_FIXED);
  setClassAttribute("connection", styleAttr);

  createClassAttribute("connection", "points", "Polyline Points", "",
                       ATTR_NODEFAULT | ATTR_MAPPED | ATTR_FIXED);

  static BGAttributeConstraintsList *edgeDirections =
      new BGAttributeConstraintsList();
  if (edgeDirections->ids.isEmpty()) {
    edgeDirections->names << tr("Directed (one end)")
                          << tr("Mutual (both ends)") << tr("None (no ends)");
    // edgeDirections->ids << "directed" << "mutual" << "undirected";
    // edgeDirections->icons << QIcon(":/Icons/Edge-Directed") <<
    // QIcon(":/Icons/Edge-Mutual") << QIcon(":/Icons/Edge-Undirected");
    edgeDirections->ids << "fullhead"
                        << "halfhead";
    edgeDirections->icons << QIcon(":/Icons/Edge-Directed")
                          << QIcon(":/Icons/Edge-Half");
  }
  setClassAttributeConstrains("connection", "direction", edgeDirections);

  setClassAttributeConstrains("connection", "style", edgeStyles);

  static BGAttributeConstraintsList *nodeShapes =
      new BGAttributeConstraintsList();
  if (nodeShapes->ids.isEmpty()) {
    nodeShapes->names << tr("Disc") << tr("Square") << tr("Triangle (up)")
                      << tr("Triangle (down)") << tr("Diamond")
                      << tr("Hexagon");
    nodeShapes->ids << "disc"
                    << "square"
                    << "triangle"
                    << "triangle2"
                    << "diamond"
                    << "hexagon";
    nodeShapes->icons << QIcon(":/Icons/Node-Disc")
                      << QIcon(":/Icons/Node-Square")
                      << QIcon(":/Icons/Node-Triangle")
                      << QIcon(":/Icons/Node-Triangle-Down")
                      << QIcon(":/Icons/Node-Diamond")
                      << QIcon(":/Icons/Node-Hexagon");
  }
  setClassAttributeConstrains("element", "shape", nodeShapes);
}

// nodes creation

void BGElementEditorScene::setEditMode(EditMode mode) {
  if (m_editMode != mode) {
    m_editMode = mode;

    switch (m_editMode) {
    case EM_Transform:
      getCurrentView()->setDragMode(QGraphicsView::RubberBandDrag);
      startTransform(true, false);
      break;

    case EM_Factor:
      getCurrentView()->setDragMode(QGraphicsView::RubberBandDrag);
      startTransform(true, true);
      break;

    case EM_AddElements:
      getCurrentView()->setDragMode(QGraphicsView::NoDrag);
      startTransform(false);
      break;

    default:
      getCurrentView()->setDragMode(QGraphicsView::RubberBandDrag);
      startTransform(false);
      break;
    }

    Q_EMIT editModeChanged(m_editMode);
  }
}

bool BGElementEditorScene::startNewConnection(const QPointF &pos) {

  if (m_editMode == EM_Transform)
    return false;

  if (m_editMode == EM_Factor)
    return false;
  m_startElement = nullptr;
  if (QGraphicsItem *item = getItemAt(pos)) {
    if (!item->isEnabled())
      return false;

    // check for port first
    BGPort *port = dynamic_cast<BGPort *>(item);
    if (port) {
      // Check if the port allows out going connections
      if (port->getDirection() == -1)
        return false;
      // Check if it is a free port
      BGElement *element = port->getElement();
      Q_ASSERT(element != nullptr);
      auto conn = element->getConnections();
      bool freePort = true;
      for (auto &con : conn) {

        if (con->firstElement() == element &&
            con->firstPortId() == port->getId()) {
          // qDebug() << element->getId()<<"\t"<<port->getId();
          freePort = false;
          break;
        }
        if (con->lastElement() == element &&
            con->lastPortId() == port->getId()) {
          // qDebug() << element->getId()<<"\t"<<port->getId();
          freePort = false;
          break;
        }
      }
      if (freePort && element->allowStartConnection()) {
        m_realStart = false;
        m_startElement = element;
        m_startElementPort = port;
      }
    }
  }

  if (m_startElement == nullptr)
    return false;
  // m_endElement = createNewElement(getSnapped(pos));
  m_endElement = createNewElement();
  m_endElement->setId("ConnectHelp");
  auto strokeColor = QColor("red");
  m_endElement->setAttribute(QByteArrayLiteral("stroke.color"), strokeColor);
  m_endElement->setAttribute(QByteArrayLiteral("stroke.size"), 3);
  m_endElement->setAttribute(QByteArrayLiteral("shape"),
                             m_startElement->getAttribute("shape"));
  m_endElement->setIcon(m_startElement->getIcon());
  addItem(m_endElement);
  m_endElement->setZValue(m_startElement->zValue() - 1);
  m_endElement->setPos(getSnapped(pos));

  BGEditorScene::startDrag(m_endElement);
  m_connection = createNewConnection(m_startElement, m_endElement);

  if (m_startElementPort)
    m_connection->setFirstElement(m_startElement, m_startElementPort->getId());

  m_state = IS_Creating;

  // auto select created items
  m_startElement->setSelected(false);
  m_connection->setSelected(true);
  m_endElement->setSelected(true);

  return true;
}

void BGElementEditorScene::cancel(const QPointF & /*pos*/) {
  // nothing to cancel
  if (m_state == IS_None)
    return;

  // if not cancelling already
  if (m_state != IS_Cancelling) {
    // cancel current drag operation
    BGEditorScene::finishDrag(nullptr, m_startDragItem, true);
  }

  m_state = IS_None;

  // if no creating state: return
  if (m_connection == nullptr)
    return;

  // kill connector
  m_connection->setFirstElement(nullptr);
  m_connection->setLastElement(nullptr);
  delete m_connection;
  m_connection = nullptr;

  // kill end
  delete m_endElement;
  m_endElement = nullptr;

  // kill start if real
  if (m_realStart)
    delete m_startElement;

  m_startElement = nullptr;
  m_realStart = false;
}

BGElement *BGElementEditorScene::createNewElement() const {
  if (m_elementsFactory) {
    auto element = dynamic_cast<BGElement *>(m_elementsFactory->create());
    Q_ASSERT(element);
    element->copyDataFrom(m_elementsFactory);
    return element;
  }

  // here default
  return new BGElement;
}

BGElement *BGElementEditorScene::createNewElement(const QPointF &pos) {
  auto element = createNewElement();
  addItem(element);
  element->setPos(pos);
  return element;
}

BGConnection *BGElementEditorScene::createNewConnection() const {
  if (m_connectionsFactory) {
    auto connection =
        dynamic_cast<BGConnection *>(m_connectionsFactory->create());
    Q_ASSERT(connection);
    connection->copyDataFrom(m_connectionsFactory);
    return connection;
  }

  // here default
  return new BGDConnection();
}

BGConnection *BGElementEditorScene::createNewConnection(BGElement *startElement,
                                                        BGElement *endElement) {
  auto connection = createNewConnection();

  connection->setFirstElement(startElement);
  connection->setLastElement(endElement);

  addItem(connection);
  return connection;
}

void BGElementEditorScene::setElementFactory(BGElement *elementFactory) {
  m_elementsFactory = elementFactory;
}

void BGElementEditorScene::setConnectionFactory(
    BGConnection *connectionFactory) {
  m_connectionsFactory = connectionFactory;
}

// events

void BGElementEditorScene::mouseReleaseEvent(
    QGraphicsSceneMouseEvent *mouseEvent) {
  if (m_cancelled) {
    // call super
    BGEditorScene::mouseReleaseEvent(mouseEvent);
    return;
  }

  if (m_editItem) {
    // call super
    BGEditorScene::mouseReleaseEvent(mouseEvent);
    return;
  }

  if (m_startDragItem == nullptr) {
    // call super
    BGEditorScene::mouseReleaseEvent(mouseEvent);
    return;
  } else
      // cancel on RMB
      if (mouseEvent->button() == Qt::RightButton) {
    m_state = IS_Cancelling;
    m_skipMenuEvent = true;
  }

  // release local grabber if any
  if (m_state == IS_Creating) {
    m_state = IS_Finishing;

    // cancel on RMB
    if (mouseEvent->button() == Qt::RightButton) {
      m_state = IS_Cancelling;
      m_skipMenuEvent = true;
    }

    // cancel on same position
    if (m_startElement->pos() == m_endElement->pos()) {
      m_state = IS_Cancelling;
    }
  }

  // finish or cancel drag
  finishDrag(mouseEvent, m_startDragItem, m_state == IS_Cancelling);

  // finalize
  if (m_state == IS_Cancelling) {
    cancel(mouseEvent->scenePos());
  }

  m_state = IS_None;

  // necessary to handle scene events properly
  QGraphicsScene::mouseReleaseEvent(mouseEvent);

  updateCursorState();
}

void BGElementEditorScene::keyReleaseEvent(QKeyEvent *keyEvent) {
  // forward scene events if in text editor mode
  if (m_editItem) {
    bool done = m_labelEditor.onKeyReleased(*this, keyEvent);
    if (done)
      updateCursorState();
    return;
  }

  BGEditorScene::keyReleaseEvent(keyEvent);
}

void BGElementEditorScene::keyPressEvent(QKeyEvent *keyEvent) {
  // forward scene events if in text editor mode
  if (m_editItem) {
    m_labelEditor.onKeyPressed(*this, keyEvent);
    return;
  }

  bool isCtrl = (keyEvent->modifiers() == Qt::ControlModifier);
  //	bool isAlt = (keyEvent->modifiers() == Qt::AltModifier);
  //	bool isShift = (keyEvent->modifiers() == Qt::ShiftModifier);

  if (keyEvent->key() == Qt::Key_Z && isCtrl) {
    if (m_undoManager->availableUndoCount() > 0) {
      undo();
    }
    keyEvent->accept();
    return;
  }

  if (keyEvent->key() == Qt::Key_Y && isCtrl) {

    if (m_undoManager->availableRedoCount() > 0) {
      redo();
    }
    keyEvent->accept();
    return;
  }

  // Ctrl+Up/Down; alter element size by 10%
  if (keyEvent->key() == Qt::Key_Up && isCtrl) {
    auto &nodes = getSelectedElements();
    if (nodes.isEmpty()) {
      keyEvent->accept();
      return;
    }

    for (auto &element : nodes) {
      element->setAttribute(attr_size, element->getSize() * 1.1);
    }

    addUndoState();

    keyEvent->accept();
    return;
  }

  if (keyEvent->key() == Qt::Key_Down && isCtrl) {
    auto &nodes = getSelectedElements();
    if (nodes.isEmpty()) {
      keyEvent->accept();
      return;
    }

    for (auto &element : nodes) {
      element->setAttribute(attr_size, element->getSize() / 1.1);
    }

    addUndoState();

    keyEvent->accept();
    return;
  }

  // Ctrl+Left/Right; alter connection weight by 10%
  if (keyEvent->key() == Qt::Key_Right && isCtrl) {
    auto &edges = getSelectedConnections();
    if (edges.isEmpty()) {
      keyEvent->accept();
      return;
    }

    for (auto &connection : edges) {
      connection->setAttribute(attr_weight, connection->getWeight() * 1.1);
    }

    addUndoState();

    keyEvent->accept();
    return;
  }

  if (keyEvent->key() == Qt::Key_Left && isCtrl) {
    auto &edges = getSelectedConnections();
    if (edges.isEmpty()) {
      keyEvent->accept();
      return;
    }

    for (auto &connection : edges) {
      connection->setAttribute(attr_weight, connection->getWeight() / 1.1);
    }

    addUndoState();

    keyEvent->accept();
    return;
  }

  // cancel label edit
  if (keyEvent->key() == Qt::Key_Escape) {
    cancel();
    return;
  }

  BGEditorScene::keyPressEvent(keyEvent);
}

// handlers

void BGElementEditorScene::onLeftButtonPressed(
    QGraphicsSceneMouseEvent *mouseEvent) {
  BGEditorScene::onLeftButtonPressed(mouseEvent);

  // add nodes?
  if (m_editMode == EM_AddElements || isItemAt<BGPort>(mouseEvent->scenePos())) {
    deselectAll();

    // skip calling super to avoid auto selection
    mouseEvent->accept();
  }
}

bool BGElementEditorScene::onClickDrag(QGraphicsSceneMouseEvent *mouseEvent,
                                       const QPointF &clickPos) {
  // add nodes?

  if ((m_editMode == EM_AddElements) || isItemAt<BGPort>(clickPos)) {
    if (startNewConnection(clickPos)) {
      // setEditMode(EM_Default);
      return true;
    }
  }

  // else super
  return BGEditorScene::onClickDrag(mouseEvent, clickPos);
}

bool BGElementEditorScene::onDoubleClickDrag(
    QGraphicsSceneMouseEvent *mouseEvent, const QPointF &clickPos) {
  // debug
  if (clickPos == QPointF())
    qDebug() << "bug";

  // try to start new connection at click point
  if (startNewConnection(clickPos)) {
    // mouseEvent->accept();
    return true;
  }

  // else call super
  return BGEditorScene::onDoubleClickDrag(mouseEvent, clickPos);
}

void BGElementEditorScene::onDropped(QGraphicsSceneDragDropEvent *mouseEvent) {
  mouseEvent->setDropAction(Qt::IgnoreAction);
  const QMimeData *mimeData = mouseEvent->mimeData();
  auto data = mimeData->data("diagramType");
  // Create new Element and call onDropped
  // qDebug() <<Q_FUNC_INFO<<" "<< QString(data);
  for (auto itm : selectedItems()) {
    itm->setSelected(false);
  }

  // create a element here
  auto element = dynamic_cast<BGElement *>(m_elementsFactory->create());
  Q_ASSERT(element);
  element->pauseCacheUpdates();
  element->copyDataFrom(m_elementsFactory);
  auto pos = getSnapped(mouseEvent->scenePos());
  QPixmap image = qvariant_cast<QPixmap>(mimeData->imageData());
  element->setIcon(image);
  element->setSelected(true);
  element->restartCacheUpdates();
  // Check item class type and create ports as required
  try {
    nlohmann::json json = element->setBGElementDefinitions(QString(data));
    // For userdefined type, create a port and hide it
    if (element->getClassType() == "userdefined") {
      auto port = element->addPort("temp");
      port->setAlignment(Qt::AlignBottom + Qt::AlignHCenter);
      port->hide();
    }
    addItem(element);
    // For annotations switch to transform mode
    setEditMode(EM_Default);
    if (element->getClassType() == "annotation") {
      setEditMode(EM_Transform);
    }
    // qDebug()<<Q_FUNC_INFO<<" "<<"Added json for "<<element->getId();
    element->setPos(pos);
    addUndoState();
  } catch (...) {
    QMessageBox::critical(
        nullptr, tr("Internal error"),
        tr("Failed to create element. Error occured while parsing definitions. "
           "Element definitions file may be corrupt!!"));
  }
  return;
}

void BGElementEditorScene::onDropped(QGraphicsSceneMouseEvent *mouseEvent,
                                     QGraphicsItem *dragItem) {
  BGElement *dragNode = dynamic_cast<BGElement *>(dragItem);
  BGConnection *dragEdge = dynamic_cast<BGConnection *>(dragItem);
  // Do allow nodes to be created when making connections, dragItem is a
  // temporary sprite created
  if (dragNode == nullptr) {
    cancel();
    return;
  }
  // for visual help
  if (dragNode != nullptr && dragNode->getId() == "ConnectHelp") {
    cancel();
    return;
  }
  // perform snap
  auto keys = mouseEvent->modifiers();
  bool isSnap =
      (keys & Qt::AltModifier) ? !gridSnapEnabled() : gridSnapEnabled();
  if (isSnap) {
    // control point:
    if (auto cp = dynamic_cast<ControlPoint *>(dragItem)) {
      auto newPos = getSnapped(cp->scenePos());
      cp->setPos(newPos);
      return;
    }

    // nodes & edges:
    QSet<QGraphicsItem *> items;
    QSet<BGConnection *> edges;

    if (dragEdge) {
      edges << dragEdge;

      dragNode = dragEdge->firstElement();
    }

    if (dragNode) {
      items << dragNode;

      auto newPos = getSnapped(dragNode->scenePos());
      auto d = newPos - dragNode->scenePos();

      for (auto item : selectedItems()) {
        if (auto connection = dynamic_cast<BGConnection *>(item)) {
          edges << connection;

          if (dragEdge) {
            items << connection->firstElement();
            items << connection->lastElement();
          }
        } else
          items << item;
      }

      for (auto item : items)
        item->moveBy(d.x(), d.y());

      for (auto connection : edges)
        connection->onItemMoved(d);

      return;
    }

    // whatever:
  }

  BGEditorScene::onDropped(mouseEvent, dragItem);
}

void BGElementEditorScene::onLeftClick(QGraphicsSceneMouseEvent *mouseEvent,
                                       QGraphicsItem *clickedItem) {
  if (m_editMode == EM_AddElements) {
    // clicked on empty space?
    if (!clickedItem) {
      onLeftDoubleClick(mouseEvent, clickedItem);
      // setEditMode(EM_Default);
      return;
    }
  }

  BGEditorScene::onLeftClick(mouseEvent, clickedItem);
}

void BGElementEditorScene::onLeftDoubleClick(
    QGraphicsSceneMouseEvent *mouseEvent, QGraphicsItem *clickedItem) {
  // clicked on empty space?
  if (!clickedItem) {
    // create a element here
    auto element = createNewElement(getSnapped(mouseEvent->scenePos()));
    element->setSelected(true);

    addUndoState();
    return;
  }

  BGEditorScene::onLeftDoubleClick(mouseEvent, clickedItem);
}

// movement

void BGElementEditorScene::moveSelectedEdgesBy(const QPointF &d) {
  auto edges = getSelectedConnections();
  if (edges.size()) {
    QSet<BGElement *> unselNodes; // not selected nodes

    // move selected edges
    for (auto connection : edges) {
      if (!connection->firstElement()->isSelected())
        unselNodes << connection->firstElement();

      if (!connection->lastElement()->isSelected())
        unselNodes << connection->lastElement();

      connection->onItemMoved(d);
    }

    // force move non selected nodes of the selected edges
    for (auto element : unselNodes) {
      element->moveBy(d.x(), d.y());
    }
  }
}

// override

void BGElementEditorScene::moveSelectedItemsBy(const QPointF &d, bool snapped) {
  QSet<QGraphicsItem *> items;
  QSet<BGConnection *> edges;
  QGraphicsItem *focusItem = nullptr;

  // if dragging nodes and there are selected nodes: do not drag not-selected
  // nodes
  auto dragNode = dynamic_cast<BGElement *>(m_startDragItem);

  for (auto item : selectedItems()) {
    if (!(item->flags() & item->ItemIsMovable))
      continue;

    if (!focusItem)
      focusItem = item;

    if (auto connection = dynamic_cast<BGConnection *>(item)) {
      edges << connection;

      if (!dragNode) {
        items << connection->firstElement();
        items << connection->lastElement();
      }
    } else {
      items << item;
    }
  }

  for (auto item : items) { // If the element is a port, ensure that the
                            // connection is updated
    if (BGPort *pitem = dynamic_cast<BGPort *>(item)) {
      pitem->moveBy(d.x(), d.y());
      auto elem = pitem->getElement();
      auto conns = elem->getConnections();
      auto pid = pitem->getId();
      for (auto conn : conns) {
        if (conn->firstPortId() == pid || conn->lastPortId() == pid) {
          conn->onParentGeometryChanged();
          break;
        }
      }
    } else {
      item->moveBy(d.x(), d.y());
    }
  }

  if (snapped)
    for (auto item : items)
      item->setPos(getSnapped(item->pos()));

  for (auto connection : edges)
    connection->onItemMoved(d);

  if (focusItem)
    focusItem->ensureVisible();
}

QList<QGraphicsItem *> BGElementEditorScene::getCopyPasteItems() const {
  // only selected edges & their nodes
  QList<QGraphicsItem *> result;

  QSet<QGraphicsItem *> nodes;

  for (auto item : selectedItems()) {
    if (auto connection = dynamic_cast<BGConnection *>(item)) {
      result << connection;
      nodes << connection->firstElement();
      nodes << connection->lastElement();
    } else if (auto element = dynamic_cast<BGElement *>(item)) {
      // orphaned nodes only
      if (element->bgElementFlags() & NF_OrphanAllowed)
        nodes << element;
    } else
      result << item;
  }

  result << nodes.toList();

  return result;
}

QList<QGraphicsItem *> BGElementEditorScene::getTransformableItems() const {
  QList<QGraphicsItem *> result;

  auto items = getSelectedItems();
  for (auto item : items)
    result << item->getSceneItem();

  return result;
}

bool BGElementEditorScene::doUpdateCursorState(Qt::KeyboardModifiers keys,
                                               Qt::MouseButtons buttons,
                                               QGraphicsItem *hoverItem) {
  // port?
  if (BGPort *portItem = dynamic_cast<BGPort *>(hoverItem)) {
    if (portItem->isEnabled()) {
      if (m_editMode != EM_Transform) {
        setSceneCursor(Qt::CrossCursor);
        setInfoStatus(SIS_Hover_Port);
      } else {
        setSceneCursor(Qt::SizeAllCursor);
        setInfoStatus(SIS_Move_Port);
      }
      return true;
    }
  }

  // hover item?
  if (m_editMode == EM_AddElements) {
    if (hoverItem) {
      if (hoverItem->isEnabled()) {
        setSceneCursor(Qt::CrossCursor);
        setInfoStatus(SIS_Hover);
        return true;
      }
    }
  }

  // handled by super?
  if (BGEditorScene::doUpdateCursorState(keys, buttons, hoverItem))
    return true;

  // still not handled
  return false;
}

// painting

void BGElementEditorScene::drawBackground(QPainter *painter, const QRectF &r) {
  BGEditorScene::drawBackground(painter, r);
}

// selections

const QList<BGElement *> &BGElementEditorScene::getSelectedElements() const {
  if (m_selectedElements.isEmpty())
    prefetchSelection();

  return m_selectedElements;
}

const QList<BGConnection *> &
BGElementEditorScene::getSelectedConnections() const {
  if (m_selectedConnections.isEmpty())
    prefetchSelection();

  return m_selectedConnections;
}

const QList<SceneItem *> &BGElementEditorScene::getSelectedItems() const {
  if (m_selectedItems.isEmpty())
    prefetchSelection();

  return m_selectedItems;
}

int BGElementEditorScene::getNumberOfElements() { return items().size(); }

void BGElementEditorScene::updateConnectionDescritptions() {
  // Todo update connections table
}

void BGElementEditorScene::onSelectionChanged() {
  // drop cached selections
  m_selectedElements.clear();
  m_selectedConnections.clear();
  m_selectedItems.clear();

  BGEditorScene::onSelectionChanged();
}

void BGElementEditorScene::prefetchSelection() const {
  m_selectedElements.clear();
  m_selectedConnections.clear();
  m_selectedItems.clear();

  auto selItems = selectedItems();

  for (auto *item : selItems) {
    if (BGElement *element = dynamic_cast<BGElement *>(item)) {
      m_selectedElements << element;
      m_selectedItems << element;
      continue;
    }

    if (BGConnection *connection = dynamic_cast<BGConnection *>(item)) {
      m_selectedConnections << connection;
      m_selectedItems << connection;
      continue;
    }
  }
}

QMap<QString, BGElement *> BGElementEditorScene::userDefinedElements() {
  auto items = getItems();
  QMap<QString, BGElement *> udlist;
  for (auto elem : items) {
    if (BGElement *bge = dynamic_cast<BGElement *>(elem)) {
      if (bge->getClassType() == "userdefined") {
        auto &uds = bge->getJson();
        if (uds.contains("mapdefinition"))
          udlist[bge->getId()] = bge;
      }
    }
  }
  return udlist;
}

// menu & actions

QObject *BGElementEditorScene::createActions() {
  return new BGElementSceneActions(this);
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR