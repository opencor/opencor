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
#include "controlpoint.h"
#include "sceneitem.h"

namespace OpenCOR {
namespace BondGraphEditorWindow {

ControlPoint::ControlPoint(SceneItem *parent)
    : Shape(dynamic_cast<QGraphicsItem *>(parent)), m_parentItem(parent) {
  Q_ASSERT(parent != nullptr);

  setRect(-4, -4, 8, 8);
  setBrush(Qt::black);
  setPen(QPen(Qt::gray, 1));
}

// override

QVariant ControlPoint::itemChange(QGraphicsItem::GraphicsItemChange change,
                                  const QVariant &value) {
  // if (change == ItemPositionChange)
  //{
  //	if (auto editScene = dynamic_cast<BGEditorScene*>(scene()))
  //	{
  //		return editScene->getSnapped(value.toPointF());
  //	}

  //	return value;
  //}

  if (change == ItemPositionHasChanged) {
    m_parentItem->onControlPointMoved(this, value.toPointF());

    return value;
  }

  return Shape::itemChange(change, value);
}

// menu

bool ControlPoint::populateMenu(
    QMenu &menu, const QList<QGraphicsItem *> & /*selectedItems*/) {
  menu.addAction(tr("Delete point"), this, SLOT(onActionDelete()));

  return true;
}

void ControlPoint::onActionDelete() {
  m_parentItem->onControlPointDelete(this);
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR