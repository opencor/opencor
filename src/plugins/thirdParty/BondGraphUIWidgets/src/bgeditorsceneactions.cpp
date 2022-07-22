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
#include <QIcon>
#include <QKeySequence>

#include "bgconnection.h"
#include "bgeditorscene.h"
#include "bgeditorsceneactions.h"
#include "bgelement.h"

namespace OpenCOR {
namespace BondGraphUI {

BGEditorSceneActions::BGEditorSceneActions(BGEditorScene *scene) {
  cutAction = new QAction(QIcon(":/Icons/Cut"), tr("Cut"), scene);
  cutAction->setStatusTip(tr("Cut selected item(s) to clipboard"));
  cutAction->setToolTip(tr("Cut selection"));
  cutAction->setShortcut(QKeySequence::Cut);
  connect(cutAction, &QAction::triggered, scene, &BGEditorScene::cut);

  copyAction = new QAction(QIcon(":/Icons/Copy"), tr("Copy"), scene);
  copyAction->setStatusTip(tr("Copy selected item(s) to clipboard"));
  copyAction->setToolTip(tr("Copy selection"));
  copyAction->setShortcut(QKeySequence::Copy);
  connect(copyAction, &QAction::triggered, scene, &BGEditorScene::copy);

  pasteAction = new QAction(QIcon(":/Icons/Paste"), tr("Paste"), scene);
  pasteAction->setStatusTip(tr("Paste item(s) from clipboard"));
  pasteAction->setToolTip(tr("Paste from clipboard"));
  pasteAction->setShortcut(QKeySequence::Paste);
  connect(pasteAction, &QAction::triggered, scene, &BGEditorScene::paste);

  deleteAction =
      new QAction(QIcon(":/Icons/Delete"), QObject::tr("Delete"), scene);
  deleteAction->setStatusTip(QObject::tr("Delete selected item(s)"));
  deleteAction->setToolTip(QObject::tr("Delete selection"));
  deleteAction->setShortcut(QKeySequence::Delete);
  QObject::connect(deleteAction, &QAction::triggered, scene,
                   &BGEditorScene::del);
}
} // namespace BondGraphUI
} // namespace OpenCOR