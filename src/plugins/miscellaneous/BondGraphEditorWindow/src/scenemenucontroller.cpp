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
#include "scenemenucontroller.h"

#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

#include <bgconnection.h>
#include <bgeditorview.h>
#include <bgelement.h>
#include <bgelementeditorscene.h>
#include <bgelementsceneactions.h>
#include <bgporteditordialog.h>

namespace OpenCOR {
namespace BondGraphEditorWindow {

SceneMenuController::SceneMenuController(BGElementEditorUIController *parent) :
    QObject(parent), m_controller(parent)
{
}

SceneMenuController::~SceneMenuController()
{
}

bool SceneMenuController::exec(
    BGEditorScene *scene, QGraphicsItem *trigger_item,
    QGraphicsSceneContextMenuEvent *context_menu_event)
{
    m_scene = dynamic_cast<BGElementEditorScene *>(scene);
    m_scene->setPastePosition(context_menu_event->scenePos());
    BGPort *portItem = dynamic_cast<BGPort *>(trigger_item);
    if (portItem == nullptr) {
        QMenu menu;
        fillMenu(menu, scene, trigger_item, context_menu_event);

        Q_EMIT onContextMenu(menu);

        // execute
        menu.exec(context_menu_event->screenPos());
        m_scene->setPastePosition(QPointF());
    } else {
        BGPortEditorDialog dialog(m_scene);
        if (dialog.exec(*portItem) == QDialog::Accepted)
            m_scene->addUndoState();
        else
            m_scene->revertUndoState();
    }

    return false;
}

void SceneMenuController::fillMenu(
    QMenu &menu, BGEditorScene *scene, QGraphicsItem *trigger_item,
    QGraphicsSceneContextMenuEvent * /*contextMenuEvent*/)
{
    auto portItem = dynamic_cast<BGPort *>(trigger_item);
    if (portItem == nullptr) {
        auto sceneActions = scene->getActions();
        auto bgElementScene = m_scene;

        int elementCount = bgElementScene->getSelectedElements().size();
        //bool elementSelected = (elementCount > 0);

        int connectionCount = bgElementScene->getSelectedConnections().size();
        bool connectionSelected = (connectionCount > 0);

        // add default actions
        if ((elementCount + connectionCount) == 1) {
            //QAction *changeIdAction =
            menu.addAction(tr("Change Id..."), parent(), SLOT(changeItemId()));
            // changeIdAction->setEnabled((elementCount + connectionCount) == 1);

            menu.addSeparator();
        }
        menu.addAction(scene->actions()->cutAction);
        menu.addAction(scene->actions()->copyAction);
        menu.addAction(scene->actions()->pasteAction);
        menu.addAction(scene->actions()->deleteAction);

        // add default element actions
        if (elementCount > 0) {
            menu.addSeparator();

            // QAction *linkAction =
            //     menu.addAction(tr("Link"), sceneActions, SLOT(onActionLink()));
            // linkAction->setEnabled(elementCount > 1);

            // QAction *unlinkAction =
            //     menu.addAction(tr("Unlink"), sceneActions, SLOT(onActionUnlink()));
            // unlinkAction->setEnabled(elementSelected);

            // menu.addSeparator();
            /*
      QAction *addPortAction =
          menu.addAction(tr("Add Port..."), parent(), SLOT(addElementPort()));
      addPortAction->setEnabled(elementCount == 1);
      */

            QAction *editPortAction =
                menu.addAction(tr("Edit Port..."), parent(), SLOT(editNodePort()));
            if (portItem != nullptr && portItem->getDeleteFlag())
                editPortAction->setEnabled(true);
            else
                editPortAction->setEnabled(false);
        }
        // add default edge actions
        if (connectionCount > 0) {
            menu.addSeparator();

            QAction *edgeDirectionAction = menu.addAction(
                tr("Reverse"), sceneActions, SLOT(onActionConnectionReverse()));
            edgeDirectionAction->setEnabled(connectionSelected);
        }
        // add zoom actions
        menu.addSeparator();
        menu.addAction(m_controller->zoomAction);
        menu.addAction(m_controller->unzoomAction);
        menu.addAction(m_controller->resetZoomAction);
        menu.addAction(m_controller->fitZoomAction);
        menu.addAction(m_controller->fitZoomSelectedAction);
        menu.addAction(m_controller->fitZoomBackAction);

        menu.addAction(m_controller->sceneCropAction);
        menu.addSeparator();
        menu.addAction(m_controller->gridAction);
        menu.addAction(m_controller->gridSnapAction);
        menu.addSeparator();
        menu.addAction(m_controller->schemesAction);

        menu.addSeparator();
        if (elementCount > 0) {
            menu.addAction(m_controller->m_actionShowNodeIds);
        }
        if (connectionCount > 0) {
            menu.addAction(m_controller->m_actionShowEdgeIds);
        }
    } else {
        // Provide a menu suitable for ports
    }

    /*
   QMenu *arrowsMenu = menu.addMenu(tr("Direction"));
   arrowsMenu->setEnabled(connectionSelected);
   arrowsMenu->addAction(tr("Directed"), sceneActions,
                         SLOT(onActionConnectionDirected()));
   arrowsMenu->addAction(tr("Mutual"), sceneActions,
   SLOT(onActionConnectionMutual())); arrowsMenu->addAction(tr("None"),
   sceneActions, SLOT(onActionConnectionUndirected()));
   arrowsMenu->addSeparator();
   arrowsMenu->addAction(tr("Reverse"), sceneActions,
                         SLOT(onActionConnectionReverse()));
                         */
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
