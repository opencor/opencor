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

#include <QGraphicsRectItem>

#include "bgcontextmenuproviderinterface.h"

namespace OpenCOR {
namespace BondGraphUI {

class SceneItem;

class ControlPoint : public QObject,
                     public QGraphicsRectItem,
                     public BGContextMenuProviderInterface {
  Q_OBJECT
public:
  typedef QGraphicsRectItem Shape;

  explicit ControlPoint(SceneItem *parent);
  virtual ~ControlPoint() {}

  // menu
  virtual bool populateMenu(QMenu &menu,
                            const QList<QGraphicsItem *> &selectedItems);

protected Q_SLOTS:
  void onActionDelete();

protected:
  // override
  virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
                              const QVariant &value);

  SceneItem *m_parentItem;
};

} // namespace BondGraphUI
} // namespace OpenCOR