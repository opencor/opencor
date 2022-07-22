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

#include <QMenu>
#include <QObject>

#include "bgelementeditoruicontroller.h"
#include <scenemenucontrollerinterface.h>

namespace OpenCOR {
namespace BondGraphUI {

class BGElementEditorScene;

class SceneMenuController : public QObject,
                            public SceneMenuControllerInterface {
  Q_OBJECT

public:
  SceneMenuController(BGElementEditorUIController *parent = nullptr);
  virtual ~SceneMenuController();

  // called before \a menu is about to show.
  virtual void fillMenu(QMenu &menu, BGEditorScene *scene,
                        QGraphicsItem *triggerItem,
                        QGraphicsSceneContextMenuEvent *contextMenuEvent);

  // override: SceneMenuControllerInterface
  virtual bool exec(BGEditorScene *scene, QGraphicsItem *triggerItem,
                    QGraphicsSceneContextMenuEvent *contextMenuEvent);

Q_SIGNALS:
  // emitted before \a menu is about to show.
  void onContextMenu(QMenu &menu);

private:
  BGElementEditorScene *m_scene = nullptr;
  BGElementEditorUIController *m_controller;
};
} // namespace BondGraphUI
} // namespace OpenCOR