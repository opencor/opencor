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
    #pragma clang diagnostic ignored "-Wdelete-incomplete"
#endif

#include <QByteArrayList>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QMenu>
#include <QSet>
#include <QTimer>

#include "bgitemattribute.h"
#include "bgtextlabeledit.h"
#include "bgtransformrect.h"
#include "graphbase.h"

#include "thirdparty/json.hpp"

namespace OpenCOR {
namespace BondGraphEditorWindow {

class UndoManagerInterface;
class SceneItemFactoryInterface;
class InteractiveItemInterface;
class SceneMenuControllerInterface;
class SceneEditControllerInterface;

class SceneItem;
class BGEditorSceneActions;

class SceneItemEvaluator
{
public:
    virtual bool evaluate(const QGraphicsItem &item) const = 0;
};

class DeletableSceneItems : public SceneItemEvaluator
{
public:
    bool evaluate(const QGraphicsItem &item) const override;
};

class BGEditorScene : public QGraphicsScene
{
    Q_OBJECT

public:
    BGEditorScene(QObject *parent = nullptr);
    ~BGEditorScene() override;

    virtual void reset();
    virtual void initialize();

    // properties
    void setGridSize(int newSize);
    int getGridSize() const;
    bool gridEnabled() const;
    bool gridSnapEnabled() const;
    void setGridPen(const QPen &gridPen);
    const QPen &getGridPen() const;
    void setFontAntialiased(bool on);
    bool isFontAntialiased() const;
    bool itemLabelsEnabled() const;
    bool itemLabelsforceUpdate() const;

    enum LabelsPolicy { Auto,
                        AlwaysOn,
                        AlwaysOff };

    LabelsPolicy getLabelsPolicy() const;
    void setLabelsPolicy(LabelsPolicy value);

    // undo-redo
    int availableUndoCount() const;
    int availableRedoCount() const;
    // must be called after scene state changed
    void addUndoState();
    // must be called to discard recent changes without undo
    void revertUndoState();
    // sets initial scene state
    void setInitialState();

    // serialization
    virtual bool storeTo(QDataStream &out, bool storeOptions) const;
    virtual bool restoreFrom(QDataStream &out, bool readOptions);

    friend void to_json(nlohmann::json &json_, const BGEditorScene &bgescene);
    friend void from_json(const nlohmann::json &json_, BGEditorScene &bgescene);

    // item factories
    template<class T> bool registerItemFactory()
    {
        static T factory;
        return setItemFactory(&factory);
    }

    template<class T> static T *factory()
    {
        static T s_item;
        return &s_item;
    }

    bool setItemFactory(SceneItem *factoryItem, const QByteArray &typeId = "");
    SceneItem *getItemFactory(const QByteArray &typeId) const;

    virtual SceneItem *createItemOfType(const QByteArray &typeId) const;

    template<class T> T *createItemOfType(QPointF *itype = nullptr) const;

    void setItemFactoryFilter(SceneItemFactoryInterface *filter);
    // scene factory & copy
    virtual BGEditorScene *createScene() const;
    virtual BGEditorScene *clone();

    virtual void backupScene(QString file);
    virtual void removeBackUp(QString file);
    virtual void restoreFromFile(QString file);
    virtual void copyProperties(const BGEditorScene &from);

    // attributes
    QByteArray getSuperClassId(const QByteArray &classId) const;

    const BGItemAttribute getClassAttribute(const QByteArray &classId,
                                            const QByteArray &attrId,
                                            bool inherited) const;
    AttributesMap getClassAttributes(const QByteArray &classId,
                                     bool inherited) const;

    bool removeClassAttribute(const QByteArray &classId,
                              const QByteArray &attrId);

    void setClassAttribute(const QByteArray &classId, const BGItemAttribute &attr,
                           bool vis = false);
    void setClassAttribute(const QByteArray &classId, const QByteArray &attrId,
                           const QVariant &defaultValue);

    // convenience method to create a class attribute by single call
    BGItemAttribute &createClassAttribute(
        const QByteArray &classId, const QByteArray &attrId,
        const QString &attrName, const QVariant &defaultValue = QVariant(),
        int attrFlags = ATTR_FIXED,
        BGItemAttributeConstraints *constrains = nullptr, bool vis = false);

    QSet<QByteArray> getVisibleClassAttributes(const QByteArray &classId,
                                               bool inherited) const;
    void setVisibleClassAttributes(const QByteArray &classId,
                                   const QSet<QByteArray> &vis);

    void setClassAttributeVisible(const QByteArray &classId,
                                  const QByteArray &attrId, bool vis = true);
    bool isClassAttributeVisible(const QByteArray &classId,
                                 const QByteArray &attrId) const;

    BGItemAttributeConstraints *
    getClassAttributeConstrains(const QByteArray &classId,
                                const QByteArray &attrId) const;
    void setClassAttributeConstrains(const QByteArray &classId,
                                     const QByteArray &attrId,
                                     BGItemAttributeConstraints *cptr);

    // items
    template<class T = SceneItem, class L = T> QList<T *> getItems() const;

    template<class T = SceneItem>
    QList<T *> getItemsById(const QString &id) const;

    QGraphicsItem *getItemAt(const QPointF &pos) const;

    template<class T> T *isItemAt(const QPointF &pos) const
    {
        return dynamic_cast<T *>(getItemAt(pos));
    }

    // selections
    QList<QGraphicsItem *> createSelectedList(const SceneItemEvaluator &) const;

    template<class T = SceneItem, class L = T>
    QList<T *> getSelectedItems(bool triggeredIfEmpty = false) const;

    virtual void beginSelection();
    virtual void endSelection();

    void ensureSelectionVisible();

    void moveSelectedItemsBy(double x, double y, bool snapped = false);

    virtual void moveSelectedItemsBy(const QPointF &point, bool snapped = false);

    virtual void rotateElementPort(int dir);

    virtual QList<SceneItem *> cloneSelectedItems();

    virtual int getBoundingMargin() const;

    // to reimplement
    virtual QList<QGraphicsItem *> getCopyPasteItems() const;
    virtual QList<QGraphicsItem *> getTransformableItems() const;

    // operations
    void startDrag(QGraphicsItem *dragItem);
    void startTransform(bool on, bool moveOnly = false);

    // actions
    QObject *getActions();
    BGEditorSceneActions *actions();

    // edit extenders
    void setSceneEditController(SceneEditControllerInterface *controller);
    SceneEditControllerInterface *getSceneEditController() const;

    // context menu
    void setContextMenuController(SceneMenuControllerInterface *controller);
    SceneMenuControllerInterface *getContextMenuController() const;
    QGraphicsItem *getContextMenuTrigger() const;

    // other
    bool checkLabelRegion(const QRectF &region);
    void layoutItemLabels();
    void forceUpdate();

    virtual QPointF getSnapped(const QPointF &pos) const;

    int getInfoStatus() const;
    QGraphicsView *getCurrentView();

    // callbacks
    virtual void onItemDestroyed(SceneItem *citem);
    virtual void refresh(SceneItem *citem);

public Q_SLOTS:
    void enableGrid(bool on = true);
    void enableGridSnap(bool on = true);
    void enableItemLabels(bool on = true);

    void undo();
    void redo();

    void selectAll();
    void deselectAll();
    void selectItem(SceneItem *item, bool exclusive = true);
    void selectItems(const QList<SceneItem *> &items, bool exclusive = true);

    void del();
    void cut();
    void copy();

    void setPastePosition(const QPointF &anchor);
    void pasteAt(const QPointF &anchor);
    void paste();

    void crop();

    void setSceneCursor(const QCursor &cursor);

Q_SIGNALS:
    void undoAvailable(bool flag);
    void redoAvailable(bool flag);
    void itemDeleted(QGraphicsItem *item);
    void sceneChanged();
    void sceneDoubleClicked(QGraphicsSceneMouseEvent *mouseEvent,
                            QGraphicsItem *clickedItem);

    void infoStatusChanged(int status);

protected:
    void setInfoStatus(int status);

    void updateCursorState();
    virtual bool doUpdateCursorState(Qt::KeyboardModifiers keys,
                                     Qt::MouseButtons buttons,
                                     QGraphicsItem *hoverItem);

    virtual QObject *createActions();

    // internal call
    void selectUnderMouse(QGraphicsSceneMouseEvent *mouseEvent);

    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void keyPressEvent(QKeyEvent *keyEvent) override;
    void keyReleaseEvent(QKeyEvent *keyEvent) override;
    void focusInEvent(QFocusEvent *focusEvent) override;
    void
    contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent) override;

    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    // call from reimp
    void moveDrag(QGraphicsSceneMouseEvent *mouseEvent, QGraphicsItem *dragItem,
                  bool performDrag);
    virtual void processDrag(QGraphicsSceneMouseEvent *mouseEvent,
                             QGraphicsItem *dragItem);
    void finishDrag(QGraphicsSceneMouseEvent *mouseEvent, QGraphicsItem *dragItem,
                    bool dragCancelled);

    // callbacks
    virtual void
    onDragging(QGraphicsItem *dragItem,
               const QSet<InteractiveItemInterface *> &acceptedItems,
               const QSet<InteractiveItemInterface *> &rejectedItems);
    virtual void onMoving(QGraphicsSceneMouseEvent *mouseEvent,
                          QGraphicsItem *hoverItem);
    virtual void onDropped(QGraphicsSceneMouseEvent *mouseEvent,
                           QGraphicsItem *dragItem);
    virtual void onDropped(QGraphicsSceneDragDropEvent *mouseEvent);
    virtual void onLeftButtonPressed(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void onRightButtonPressed(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void onLeftClick(QGraphicsSceneMouseEvent *mouseEvent,
                             QGraphicsItem *clickedItem);
    virtual void onLeftDoubleClick(QGraphicsSceneMouseEvent *mouseEvent,
                                   QGraphicsItem *clickedItem);
    // called on drag after single click; returns true if handled
    virtual bool onClickDrag(QGraphicsSceneMouseEvent *mouseEvent,
                             const QPointF &clickPos);
    // called on drag after double click; returns true if handled
    virtual bool onDoubleClickDrag(QGraphicsSceneMouseEvent *mouseEvent,
                                   const QPointF &clickPos);

    virtual void onSceneChanged();

protected Q_SLOTS:
    virtual void onSelectionChanged();
    void onFocusItemChanged(QGraphicsItem *newFocusItem,
                            QGraphicsItem *oldFocusItem, Qt::FocusReason reason);
    void onItemEditingFinished(SceneItem *item, bool cancelled);

    void onActionDelete();
    void onActionSelectAll();
    void onActionEditLabel(SceneItem *item);

private:
    void removeItems();
    void checkUndoState();

protected:
    QPointF m_leftClickPos;
    QPointF m_mousePos;
    bool m_doubleClick = false;
    bool m_cancelled = false;
    bool m_dragInProgress = false;
    QGraphicsItem *m_startDragItem = nullptr;
    QPointF m_lastDragPos;
    QGraphicsItem *m_draggedItem = nullptr;
    QSet<InteractiveItemInterface *> m_acceptedHovers, m_rejectedHovers;
    bool m_skipMenuEvent = false;

    SceneItem *m_editItem = nullptr;

    BGTextLabelEdit m_labelEditor;
    BGTransformRect *m_transformRect;
    UndoManagerInterface *m_undoManager = nullptr;

private:
    int m_infoStatus;

    QMap<QByteArray, SceneItem *> m_itemFactories;
    SceneItemFactoryInterface *m_itemFactoryFilter = nullptr;

    bool m_inProgress = false;

    QGraphicsItem *m_menuTriggerItem = nullptr;
    SceneMenuControllerInterface *m_menuController = nullptr;

    QObject *m_actions = nullptr;

    SceneEditControllerInterface *m_editController = nullptr;

    QMap<QByteArray, QByteArray> m_classToSuperIds;
    ClassAttributesMap m_classAttributes;
    QMap<QByteArray, QSet<QByteArray>> m_classAttributesVis;
    AttributeConstrainsMap m_classAttributesConstrains;

    int m_gridSize;
    bool m_gridEnabled;
    bool m_gridSnap;
    QPen m_gridPen;

    bool m_forceUpdateItems = true;
    QPointF m_pastePos;

    // labels
    QPainterPath m_usedLabelsRegion;
    bool m_labelsEnabled, m_labelsUpdate;

    bool m_isFontAntialiased = true;
};

// factorization

template<class T> T *BGEditorScene::createItemOfType(QPointF *atype) const
{
    if (SceneItem *item = createItemOfType(T::TYPE())) {
        if (T *titem = dynamic_cast<T *>(item)) {
            if (atype) {
                (const_cast<BGEditorScene *>(this))->addItem(titem);
                titem->setPos(*atype);
            }

            return titem;
        }

        delete item;
        return nullptr;
    }

    return nullptr;
}

// selections

template<class T, class L>
QList<T *> BGEditorScene::getSelectedItems(bool triggeredIfEmpty) const
{
    QList<T *> result;

    QList<QGraphicsItem *> selItems = selectedItems();
    if (selItems.isEmpty() && triggeredIfEmpty && m_menuTriggerItem) {
        selItems.append(m_menuTriggerItem);
    }
    for (auto *item : selItems) {
        T *titem = dynamic_cast<L *>(item);
        if (titem) {
            result.append(titem);
        }
    }

    return result;
}

template<class T, class L> QList<T *> BGEditorScene::getItems() const
{
    QList<T *> result;

    auto allItems = items();
    for (auto *item : allItems) {
        T *titem = dynamic_cast<L *>(item);
        if (titem) {
            result.append(titem);
        }
    }

    return result;
}

template<class T>
QList<T *> BGEditorScene::getItemsById(const QString &id) const
{
    QList<T *> res;

    auto allItems = items();
    for (auto *item : allItems) {
        T *titem = dynamic_cast<T *>(item);

        if (titem && titem->getId() == id) {
            res << titem;
        }
    }

    return res;
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
