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

#include <QMap>

#include "bgconnection.h"
#include "bgeditorscene.h"
#include "graphbase.h"
#include "thirdparty/json.hpp"

namespace OpenCOR {
namespace BondGraphUI {

class BGElement;
class BGPort;
class BGElementSceneActions;

enum EditMode { EM_Default, EM_AddElements, EM_AddConnections, EM_Transform, EM_Factor };

class BGElementEditorScene : public BGEditorScene {
  Q_OBJECT

public:
  BGElementEditorScene(QObject *parent = nullptr);

  // override
  virtual BGEditorScene *createScene() const {
    return new BGElementEditorScene();
  }

  virtual void initialize();

  // operations
  bool startNewConnection(const QPointF &pos);
  void cancel(const QPointF &pos = QPointF());

  EditMode getEditMode() const { return m_editMode; }

  // factorizations
  virtual BGElement *createNewElement() const;
  BGElement *createNewElement(
      const QPointF
          &pos); // calls createNewElement(), attaches to scene and sets pos

  virtual BGConnection *createNewConnection() const;
  BGConnection *createNewConnection(
      BGElement *startElement,
      BGElement *endElement); // calls createNewConnection(), attaches to scene
                              // and sets nodes

  void setElementFactory(BGElement *element);
  void setConnectionFactory(BGConnection *element);

  BGElement *getElementFactory() { return m_elementsFactory; }
  BGConnection *getConnectionFactory() { return m_connectionsFactory; }

  // selections
  virtual void moveSelectedItemsBy(const QPointF &d, bool snapped = false);

  virtual int getBoundingMargin() const { return 5; }
  int getNumberOfElements();
  const QList<BGElement *> &getSelectedElements() const;
  const QList<BGConnection *> &getSelectedConnections() const;
  const QList<SceneItem *> &getSelectedItems() const;

  void addElementAnnotation(QString &eid, nlohmann::json &anot);
  QMap<QString, BGElement *> userDefinedElements();
  void updateConnectionDescritptions();
Q_SIGNALS:
  void editModeChanged(int mode);

public Q_SLOTS:
  void setEditMode(EditMode mode);

protected Q_SLOTS:
  virtual void onSelectionChanged();

protected:
  // selection
  void moveSelectedEdgesBy(const QPointF &d);
  void prefetchSelection() const;

  // scene events
  // virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
  virtual void keyPressEvent(QKeyEvent *keyEvent);
  virtual void keyReleaseEvent(QKeyEvent *keyEvent);

  virtual void onLeftButtonPressed(QGraphicsSceneMouseEvent *mouseEvent);
  // called on drag after single click; returns true if handled
  virtual bool onClickDrag(QGraphicsSceneMouseEvent *mouseEvent,
                           const QPointF &clickPos);
  // called on drag after double click; returns true if handled
  virtual bool onDoubleClickDrag(QGraphicsSceneMouseEvent *mouseEvent,
                                 const QPointF &clickPos);
  virtual void onDropped(QGraphicsSceneMouseEvent *mouseEvent,
                         QGraphicsItem *dragItem);
  virtual void onDropped(QGraphicsSceneDragDropEvent *mouseEvent);
  virtual void onLeftClick(QGraphicsSceneMouseEvent *mouseEvent,
                           QGraphicsItem *clickedItem);
  virtual void onLeftDoubleClick(QGraphicsSceneMouseEvent * /*mouseEvent*/,
                                 QGraphicsItem *clickedItem);

  // override
  virtual QList<QGraphicsItem *> getCopyPasteItems() const;
  virtual QList<QGraphicsItem *> getTransformableItems() const;
  virtual bool doUpdateCursorState(Qt::KeyboardModifiers keys,
                                   Qt::MouseButtons buttons,
                                   QGraphicsItem *hoverItem);
  virtual QObject *createActions();

  // draw
  virtual void drawBackground(QPainter *painter, const QRectF &);

protected:
  // edit mode
  EditMode m_editMode;

  // creating
  BGElement *m_startElement = nullptr, *m_endElement = nullptr;
  BGConnection *m_connection = nullptr;
  bool m_realStart = false;
  BGPort *m_startElementPort = nullptr, *m_endElementPort = nullptr;

  enum InternState { IS_None, IS_Creating, IS_Finishing, IS_Cancelling };
  InternState m_state = IS_None;

  BGElement *m_elementsFactory = nullptr;
  BGConnection *m_connectionsFactory = nullptr;

  // cached selections
  mutable QList<BGElement *> m_selectedElements;
  mutable QList<BGConnection *> m_selectedConnections;
  mutable QList<SceneItem *> m_selectedItems;

  // drawing
  int m_nextIndex = 0;
};

} // namespace BondGraphUI
} // namespace OpenCOR