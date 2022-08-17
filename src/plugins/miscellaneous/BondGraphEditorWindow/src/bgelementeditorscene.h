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
namespace BondGraphEditorWindow {

class BGElement;
class BGPort;
class BGElementSceneActions;

enum EditMode { EM_Default,
                EM_AddElements,
                EM_AddConnections,
                EM_Transform,
                EM_Factor };

class BGElementEditorScene : public BGEditorScene
{
    Q_OBJECT

public:
    BGElementEditorScene(QObject *parent = nullptr);

    // override
    BGEditorScene *createScene() const override;
    void initialize() override;

    // operations
    bool startNewConnection(const QPointF &pos);
    void cancel(const QPointF &pos = QPointF());

    EditMode getEditMode() const;

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

    BGElement *getElementFactory();
    BGConnection *getConnectionFactory();

    // selections
    void moveSelectedItemsBy(const QPointF &dPoint, bool snapped = false) override;
    int getBoundingMargin() const override;
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
    void onSelectionChanged() override;

protected:
    // selection
    void moveSelectedEdgesBy(const QPointF &d);
    void prefetchSelection() const;

    // scene events
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void keyPressEvent(QKeyEvent *keyEvent) override;
    void keyReleaseEvent(QKeyEvent *keyEvent) override;

    void onLeftButtonPressed(QGraphicsSceneMouseEvent *mouseEvent) override;
    // called on drag after single click; returns true if handled
    bool onClickDrag(QGraphicsSceneMouseEvent *mouseEvent,
                     const QPointF &clickPos) override;
    // called on drag after double click; returns true if handled
    bool onDoubleClickDrag(QGraphicsSceneMouseEvent *mouseEvent,
                           const QPointF &clickPos) override;
    void onDropped(QGraphicsSceneMouseEvent *mouseEvent,
                   QGraphicsItem *dragItem) override;
    void onDropped(QGraphicsSceneDragDropEvent *mouseEvent) override;
    void onLeftClick(QGraphicsSceneMouseEvent *mouseEvent,
                     QGraphicsItem *clickedItem) override;
    void onLeftDoubleClick(QGraphicsSceneMouseEvent * /*mouseEvent*/,
                           QGraphicsItem *clickedItem) override;

    // override
    QList<QGraphicsItem *> getCopyPasteItems() const override;
    QList<QGraphicsItem *> getTransformableItems() const override;
    bool doUpdateCursorState(Qt::KeyboardModifiers keys,
                             Qt::MouseButtons buttons,
                             QGraphicsItem *hoverItem) override;
    QObject *createActions() override;

    // draw
    void drawBackground(QPainter *painter, const QRectF &rect) override;

protected:
    // edit mode
    EditMode m_editMode;

    // creating
    BGElement *m_startElement = nullptr, *m_endElement = nullptr;
    BGConnection *m_connection = nullptr;
    bool m_realStart = false;
    BGPort *m_startElementPort = nullptr, *m_endElementPort = nullptr;

    enum InternState { IS_None,
                       IS_Creating,
                       IS_Finishing,
                       IS_Cancelling };
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

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
