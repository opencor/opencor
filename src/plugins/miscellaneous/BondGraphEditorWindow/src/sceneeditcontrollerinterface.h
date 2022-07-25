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
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QRectF>

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGEditorScene;

class SceneEditControllerInterface {
public:
  virtual void onActivated(BGEditorScene &scene) = 0;
  virtual void onDeactivated(BGEditorScene &scene) = 0;

  virtual void onSceneChanged(BGEditorScene &scene) = 0;
  virtual void onSelectionChanged(BGEditorScene &scene) = 0;
  virtual void onDragItem(BGEditorScene &scene,
                          QGraphicsSceneMouseEvent *mouseEvent,
                          QGraphicsItem *dragItem) = 0;

  virtual bool onMousePressed(BGEditorScene &scene,
                              QGraphicsSceneMouseEvent *mouseEvent) = 0;
  virtual bool onMouseMove(BGEditorScene &scene,
                           QGraphicsSceneMouseEvent *mouseEvent) = 0;
  virtual bool onMouseReleased(BGEditorScene &scene,
                               QGraphicsSceneMouseEvent *mouseEvent) = 0;

  virtual bool onKeyPressed(BGEditorScene &scene, QKeyEvent *keyEvent) {
    Q_UNUSED(scene);
    Q_UNUSED(keyEvent);
    return false;
  }
  virtual bool onKeyReleased(BGEditorScene &scene, QKeyEvent *keyEvent) {
    Q_UNUSED(scene);
    Q_UNUSED(keyEvent);
    return false;
  }

  virtual void draw(BGEditorScene &scene, QPainter *painter,
                    const QRectF &r) = 0;
};
} // namespace BondGraphEditorWindow
} // namespace OpenCOR