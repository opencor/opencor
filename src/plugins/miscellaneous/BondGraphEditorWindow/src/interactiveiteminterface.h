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

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnon-virtual-dtor"
    #pragma clang diagnostic ignored "-Wweak-vtables"
#endif

#include <QGraphicsItem>
#include <QPointF>
#include <QSet>

namespace OpenCOR {
namespace BondGraphEditorWindow {

enum ItemDragTestResult { Rejected,
                          Accepted,
                          Ignored };

class InteractiveItemInterface
{
public:
    // callbacks
    virtual void onItemMoved(const QPointF & /*delta*/)
    {
    }
    virtual void
    onDraggedOver(const QSet<InteractiveItemInterface *> & /*acceptedItems*/,
                  const QSet<InteractiveItemInterface *> & /*rejectedItems*/)
    {
    }
    virtual void
    onDroppedOn(const QSet<InteractiveItemInterface *> & /*acceptedItems*/,
                const QSet<InteractiveItemInterface *> & /*rejectedItems*/)
    {
    }
    virtual void onHoverEnter(QGraphicsItem * /*sceneItem*/,
                              QGraphicsSceneHoverEvent * /*event*/)
    {
    }
    virtual void onHoverLeave(QGraphicsItem * /*sceneItem*/,
                              QGraphicsSceneHoverEvent * /*event*/)
    {
    }
    virtual void onClick(QGraphicsSceneMouseEvent * /*mouseEvent*/)
    {
    }
    virtual void onDoubleClick(QGraphicsSceneMouseEvent * /*mouseEvent*/)
    {
    }
    virtual bool onClickDrag(QGraphicsSceneMouseEvent * /*mouseEvent*/,
                             const QPointF & /*clickPos*/)
    {
        return true;
    }
    virtual bool onDoubleClickDrag(QGraphicsSceneMouseEvent * /*mouseEvent*/,
                                   const QPointF & /*clickPos*/)
    {
        return false;
    }
    virtual ItemDragTestResult
    acceptDragFromItem(QGraphicsItem * /*draggedItem*/)
    {
        return Ignored;
    }
    virtual void leaveDragFromItem(QGraphicsItem * /*draggedItem*/)
    {
    }
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
