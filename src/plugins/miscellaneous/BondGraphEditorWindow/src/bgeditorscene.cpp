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
#include <QApplication>
#include <QClipboard>

#include <QDir>
#include <QElapsedTimer>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMimeData>
#include <QOpenGLFunctions>
#include <QPaintEngine>
#include <QPainter>
#include <QPixmapCache>

#include <qopengl.h>

#include "bgconnection.h"
#include "bgelement.h"
#include "sceneitem.h"
#include "utils.h"

#include "bgcontextmenuproviderinterface.h"
#include "bgdiffundomanager.h"
#include "bgeditorsceneactions.h"
#include "bgeditorsceneconstants.h"
#include "controlpoint.h"
#include "sceneitemfactoryinterface.h"
#include "scenemenucontrollerinterface.h"

#include "bgeditorscene.h"

namespace OpenCOR {
namespace BondGraphEditorWindow {

const quint64 version64 = 12; // build
const char *versionId = "VersionId";

BGEditorScene::BGEditorScene(QObject *parent) :
    QGraphicsScene(parent), m_doubleClick(false), m_dragInProgress(false),
    m_startDragItem(nullptr), m_draggedItem(nullptr), m_undoManager(new BGDiffUndoManager(*this)),
    m_infoStatus(-1), m_menuTriggerItem(nullptr),
    m_forceUpdateItems(true), m_labelsEnabled(true), m_labelsUpdate(false),
    m_isFontAntialiased(true)
{
    m_gridSize = 25;
    m_gridEnabled = true;
    m_gridSnap = true;
    m_gridPen = QPen(Qt::gray, 0, Qt::DotLine);
    m_transformRect = new BGTransformRect();
    setBackgroundBrush(Qt::white);

    // setSceneRect(-500, -500, 1000, 1000);
    setSceneRect(0, 0, m_gridSize * 200, m_gridSize * 200);

    // optimizations
    setItemIndexMethod(QGraphicsScene::NoIndex);

    setMinimumRenderSize(5);

    QPixmapCache::setCacheLimit(100000);

    // connections
    connect(this, &BGEditorScene::selectionChanged, this,
            &BGEditorScene::onSelectionChanged, Qt::DirectConnection);
    connect(this, &BGEditorScene::focusItemChanged, this,
            &BGEditorScene::onFocusItemChanged);
    connect(&m_labelEditor, &BGTextLabelEdit::editingFinished, this,
            &BGEditorScene::onItemEditingFinished);
}

BGEditorScene::~BGEditorScene()
{
    disconnect();
    clear();
    delete m_transformRect;
}

void BGEditorScene::backupScene(QString file)
{
    if (!items().isEmpty())
        m_undoManager->serialiseToFile(file);
}

void BGEditorScene::restoreFromFile(QString file)
{
    reset();
    m_undoManager->restoreFromFile(file);
}

void BGEditorScene::reset()
{
    initialize();

    if (m_undoManager)
        m_undoManager->reset();

    // setSceneRect(QRectF(-500, -500, 1000, 1000));
    setSceneRect(0, 0, m_gridSize * 200, m_gridSize * 200);
}

void BGEditorScene::initialize()
{
    removeItems();

    m_classAttributes.clear();
    m_classAttributesVis.clear();
    m_classAttributesConstrains.clear();

    // default item attrs
    createClassAttribute(class_item, "label", "Label", "",
                         ATTR_NODEFAULT | ATTR_FIXED, nullptr, true);
    createClassAttribute(class_item, "label.color", "Label Color",
                         QColor(Qt::black));

    QFont labelFont;
    BGItemAttribute labelFontAttr("label.font", "Label Font", labelFont,
                                  ATTR_FIXED);
    setClassAttribute(class_item, labelFontAttr);

    createClassAttribute(class_item, "id", "ID", "", ATTR_NODEFAULT | ATTR_FIXED,
                         nullptr, true);

    // labels policy
    static BGAttributeConstraintsEnum *labelsPolicy =
        new BGAttributeConstraintsEnum();
    if (labelsPolicy->ids.isEmpty()) {
        labelsPolicy->names << tr("Auto") << tr("Always On") << tr("Always Off");
        labelsPolicy->ids << Auto << AlwaysOn << AlwaysOff;
    }

    createClassAttribute(class_scene, attr_labels_policy, "Labels Policy", Auto,
                         ATTR_FIXED, labelsPolicy);
}

int BGEditorScene::getGridSize() const
{
    return m_gridSize;
}

bool BGEditorScene::gridEnabled() const
{
    return m_gridEnabled;
}
bool BGEditorScene::gridSnapEnabled() const
{
    return m_gridSnap;
}

const QPen &BGEditorScene::getGridPen() const
{
    return m_gridPen;
}

bool BGEditorScene::isFontAntialiased() const
{
    return m_isFontAntialiased;
}

bool BGEditorScene::itemLabelsEnabled() const
{
    return m_labelsEnabled;
}
bool BGEditorScene::itemLabelsforceUpdate() const
{
    return m_labelsUpdate;
}

void BGEditorScene::setItemFactoryFilter(SceneItemFactoryInterface *filter)
{
    m_itemFactoryFilter = filter;
}

BGEditorScene *BGEditorScene::createScene() const
{
    return new BGEditorScene();
}

int BGEditorScene::getBoundingMargin() const
{
    return 0;
}

void BGEditorScene::moveSelectedItemsBy(double x, double y, bool snapped)
{
    moveSelectedItemsBy(QPointF(x, y), snapped);
}

QByteArray BGEditorScene::getSuperClassId(const QByteArray &class_id) const
{
    if (m_classToSuperIds.contains(class_id)) {
        return m_classToSuperIds[class_id];
    }
    return QByteArray();
}

void BGEditorScene::removeBackUp(QString file)
{
    QFile backup(file);
    if (backup.exists()) {
        backup.remove();
    }
}

void BGEditorScene::removeItems()
{
    SceneItem::beginRestore();

    deselectAll();

    while (!items().isEmpty()) {
        auto itm = items().first();
        Q_EMIT itemDeleted(itm);
        delete itm;
    }

    clear();

    SceneItem::endRestore();
}

// properties

void BGEditorScene::setGridSize(int new_size)
{
    if (new_size <= 0)
        return;

    m_gridSize = new_size;

    update();
}

void BGEditorScene::enableGrid(bool on)
{
    m_gridEnabled = on;

    update();
}

void BGEditorScene::enableGridSnap(bool on)
{
    m_gridSnap = on;
}

void BGEditorScene::setGridPen(const QPen &grid_pen)
{
    m_gridPen = grid_pen;

    update();
}

void BGEditorScene::enableItemLabels(bool on)
{
    m_labelsEnabled = on;

    layoutItemLabels();
}

void BGEditorScene::setFontAntialiased(bool on)
{
    m_isFontAntialiased = on;

    layoutItemLabels();

    update();
}

void BGEditorScene::copyProperties(const BGEditorScene &from)
{
    m_classAttributes = from.m_classAttributes;
    m_classToSuperIds = from.m_classToSuperIds;
    m_classAttributesVis = from.m_classAttributesVis;
}

BGEditorScene *BGEditorScene::clone()
{
    QByteArray buffer;
    QDataStream out(&buffer, QIODevice::WriteOnly);
    if (!storeTo(out, true))
        return nullptr;

    BGEditorScene *tempScene = createScene();
    QDataStream in(buffer);
    if (tempScene->restoreFrom(in, true))
        return tempScene;

    delete tempScene;
    return nullptr;
}

// undo-redo

void BGEditorScene::undo()
{
    if (m_inProgress)
        return;

    m_inProgress = true;

    if (m_undoManager) {
        m_undoManager->undo();

        checkUndoState();

        onSceneChanged();
    }

    m_inProgress = false;
}

void BGEditorScene::redo()
{
    if (m_inProgress)
        return;

    m_inProgress = true;

    if (m_undoManager) {
        m_undoManager->redo();

        checkUndoState();

        onSceneChanged();
    }

    m_inProgress = false;
}

void BGEditorScene::addUndoState()
{
    if (m_inProgress)
        return;

    m_inProgress = true;

    onSceneChanged();

    // canvas size
    QRectF minRect(sceneRect());
    minRect |= itemsBoundingRect().adjusted(-10, -10, 10, 10);
    setSceneRect(minRect);

    // undo-redo
    if (m_undoManager) {
        m_undoManager->addState();

        checkUndoState();
    }

    m_inProgress = false;
}

void BGEditorScene::revertUndoState()
{
    if (m_inProgress)
        return;

    m_inProgress = true;

    if (m_undoManager) {
        m_undoManager->revertState();

        checkUndoState();
    }

    onSceneChanged();

    m_inProgress = false;
}

void BGEditorScene::setInitialState()
{
    m_inProgress = false;

    if (m_undoManager) {
        m_undoManager->reset();
    }

    addUndoState();
}

int BGEditorScene::availableUndoCount() const
{
    return m_undoManager ? m_undoManager->availableUndoCount() : 0;
}

int BGEditorScene::availableRedoCount() const
{
    return m_undoManager ? m_undoManager->availableRedoCount() : 0;
}

void BGEditorScene::checkUndoState()
{
    Q_EMIT undoAvailable(m_undoManager->availableUndoCount() > 0);
    Q_EMIT redoAvailable(m_undoManager->availableRedoCount() > 0);
}

// io

bool BGEditorScene::storeTo(QDataStream &out, bool store_options) const
{
    out << versionId << version64;

    // items
    QMap<SceneItem *, uint> sortedMap;

    QList<QGraphicsItem *> allItems = items();

    for (QGraphicsItem *item : allItems) {
        SceneItem *citem = dynamic_cast<SceneItem *>(item);
        if (citem) {
            sortedMap[citem] = quint64(citem);
        }
    }

    for (SceneItem *citem : sortedMap.keys()) {
        out << citem->typeId() << quint64(citem);

        citem->storeTo(out, version64);
    }

    // attributes
    out << QByteArray("_attr_");
    out << (quint64)0x12345678;

    out << m_classAttributes.size();
    for (auto classAttrsIt = m_classAttributes.constBegin();
         classAttrsIt != m_classAttributes.constEnd(); ++classAttrsIt) {
        out << classAttrsIt.key();
        out << classAttrsIt.value().size();
        for (const auto &attr : classAttrsIt.value()) {
            attr.storeTo(out, version64);
        }
    }

    out << m_classToSuperIds;

    // visible attributes
    out << m_classAttributesVis;

    // options
    if (store_options) {
        out << backgroundBrush();
        out << m_gridPen;
        out << m_gridSize;
        out << m_gridEnabled << m_gridSnap;
    }

    // 9+: scene rect
    out << sceneRect();
    QList<SceneItem *> elements = getSelectedItems();
    out << elements.size();
    for (auto &e : elements) {
        out << quint64(e);
    }

    return true;
}

bool BGEditorScene::restoreFrom(QDataStream &out, bool read_options)
{
    initialize();

    // version
    quint64 storedVersion = 0;

    // read
    SceneItem::CItemLinkMap idToItem;

    while (!out.atEnd()) {
        QByteArray id;
        out >> id;
        quint64 ptrId;
        out >> ptrId;

        if (storedVersion == 0 && strcmp(id.data(), versionId) == 0) {
            storedVersion = ptrId;
            out >> id >> ptrId;
        }

        // started attr section
        if (storedVersion >= 3 && id == "_attr_" && ptrId == 0x12345678)
            break;

        SceneItem *item = createItemOfType(id);
        if (item) {
            if (item->restoreFrom(out, storedVersion)) {
                idToItem[ptrId] = item;
                continue;
            }
        }

        // failed: cleanup
        qDeleteAll(idToItem.values());
        return false;
    }

    // link items
    SceneItem::beginRestore();

    for (SceneItem *item : idToItem.values()) {
        if (item->linkAfterRestore(idToItem)) {
            addItem(dynamic_cast<QGraphicsItem *>(item));
        } else {
            // failed: cleanup
            qDeleteAll(idToItem.values());

            clear();

            SceneItem::endRestore();

            return false;
        }
    }

    // attributes
    if (storedVersion >= 3) {
        int classAttrSize = 0;
        out >> classAttrSize;

        for (int i = 0; i < classAttrSize; ++i) {
            QByteArray classId;
            if (storedVersion >= 6)
                out >> classId;

            int attrSize = 0;
            out >> attrSize;

            for (int j = 0; j < attrSize; ++j) {
                BGItemAttribute attr;
                if (attr.restoreFrom(out, storedVersion)) {
                    if (storedVersion < 6)
                        classId = attr.classId; // deprecated for now

                    setClassAttribute(classId, attr);
                } else {
                    SceneItem::endRestore();

                    return false;
                }
            }
        }
    }

    // visible attributes
    if (storedVersion >= 5) {
        out >> m_classToSuperIds;
        out >> m_classAttributesVis;
    }

    // options
    if (read_options && storedVersion >= 8) {
        QBrush b;
        out >> b;
        setBackgroundBrush(b);
        out >> m_gridPen;
        out >> m_gridSize;
        out >> m_gridEnabled >> m_gridSnap;
    }

    // scene rect
    if (storedVersion >= 9) {
        QRectF sr;
        out >> sr;
        setSceneRect(sr);
    }

    // restore selections
    int numSelectedItems;
    out >> numSelectedItems;

    for (int it = 0; it < numSelectedItems; it++) {
        quint64 ptrid;
        out >> ptrid;
        dynamic_cast<QGraphicsItem *>(idToItem[ptrid])->setSelected(true);
    }

    // finish
    SceneItem::endRestore();

    for (SceneItem *item : idToItem.values()) {
        item->onItemRestored();
    }

    return true;
}

// factorization

bool BGEditorScene::setItemFactory(SceneItem *factory_item,
                                   const QByteArray &type_id)
{
    if (factory_item) {
        // register class inheritance
        QByteArray classId = factory_item->classId();
        QByteArray superClassId = factory_item->superClassId();
        m_classToSuperIds[classId] = superClassId;

        QByteArray id = type_id.isEmpty() ? factory_item->typeId() : type_id;
        m_itemFactories[id] = factory_item;
        return true;
    }

    return false;
}

SceneItem *BGEditorScene::getItemFactory(const QByteArray &type_id) const
{
    return m_itemFactories.contains(type_id) ? m_itemFactories[type_id] : nullptr;
}

SceneItem *BGEditorScene::createItemOfType(const QByteArray &id) const
{
    // check for filter
    if (m_itemFactoryFilter) {
        if (SceneItem *item = m_itemFactoryFilter->createItemOfType(id, *this))
            return item;
    }

    // else default creation
    if (m_itemFactories.contains(id)) {
        return m_itemFactories[id]->create();
    }

    return nullptr;
}

// attributes

BGItemAttribute &BGEditorScene::createClassAttribute(
    const QByteArray &class_id, const QByteArray &attr_id,
    const QString &attr_name, const QVariant &default_value, int attr_flags,
    BGItemAttributeConstraints *constrains, bool vis)
{
    if (m_classAttributes[class_id].contains(attr_id)) {
        // just update the value
        m_classAttributes[class_id][attr_id].defaultValue = default_value;
    } else {
        m_classAttributes[class_id][attr_id] =
            BGItemAttribute(attr_id, attr_name, default_value, attr_flags);

        setClassAttributeVisible(class_id, attr_id, vis);

        if (constrains)
            setClassAttributeConstrains(class_id, attr_id, constrains);
    }

    return m_classAttributes[class_id][attr_id];
}

void BGEditorScene::setClassAttribute(const QByteArray &class_id,
                                      const BGItemAttribute &attr, bool vis)
{
    // only update value if exists
    if (m_classAttributes[class_id].contains(attr.id))
        m_classAttributes[class_id][attr.id].defaultValue = attr.defaultValue;
    else
        // else insert
        m_classAttributes[class_id][attr.id] = attr;

    setClassAttributeVisible(class_id, attr.id, vis);

    forceUpdate();
}

void BGEditorScene::setClassAttribute(const QByteArray &class_id,
                                      const QByteArray &attr_id,
                                      const QVariant &default_value)
{
    if (m_classAttributes[class_id].contains(attr_id)) {
        // just update the value
        m_classAttributes[class_id][attr_id].defaultValue = default_value;
        forceUpdate();
        return;
    }

    // clone from super if not found
    auto superId = getSuperClassId(class_id);
    while (!superId.isEmpty() && !m_classAttributes[superId].contains(attr_id)) {
        superId = getSuperClassId(superId);
    }

    if (!superId.isEmpty()) {
        auto attr = m_classAttributes[superId][attr_id];
        attr.defaultValue = default_value;
        m_classAttributes[class_id][attr_id] = attr;

        forceUpdate();
        return;
    }

    // else create new attribute with name = id
    BGItemAttribute attr(attr_id, attr_id, default_value);
    m_classAttributes[class_id][attr_id] = attr;
    forceUpdate();
}

bool BGEditorScene::removeClassAttribute(const QByteArray &class_id,
                                         const QByteArray &attr_id)
{
    auto it = m_classAttributes.find(class_id);
    if (it == m_classAttributes.end())
        return false;

    forceUpdate();

    return (*it).remove(attr_id);
}

void BGEditorScene::setClassAttributeVisible(const QByteArray &class_id,
                                             const QByteArray &attr_id,
                                             bool vis)
{
    if (vis == m_classAttributesVis[class_id].contains(attr_id))
        return;

    if (vis)
        m_classAttributesVis[class_id].insert(attr_id);
    else
        m_classAttributesVis[class_id].remove(attr_id);

    // set label update flag
    m_labelsUpdate = true;

    // schedule update
    invalidate();
}

bool BGEditorScene::isClassAttributeVisible(const QByteArray &class_id,
                                            const QByteArray &attr_id) const
{
    return m_classAttributesVis[class_id].contains(attr_id);
}

QSet<QByteArray>
BGEditorScene::getVisibleClassAttributes(const QByteArray &class_id,
                                         bool inherited) const
{
    QSet<QByteArray> result = m_classAttributesVis[class_id];

    if (inherited) {
        QByteArray superId = getSuperClassId(class_id);
        while (!superId.isEmpty()) {
            result = result.unite(m_classAttributesVis[superId]);
            superId = getSuperClassId(superId);
        }
    }

    return result;
}

void BGEditorScene::setVisibleClassAttributes(const QByteArray &class_id,
                                              const QSet<QByteArray> &vis)
{
    m_classAttributesVis[class_id] = vis;

    // set label update flag
    m_labelsUpdate = true;

    // schedule update
    invalidate();
}

const BGItemAttribute BGEditorScene::getClassAttribute(
    const QByteArray &class_id, const QByteArray &attr_id, bool inherited) const
{
    BGItemAttribute attr = m_classAttributes[class_id][attr_id];
    if (attr.id.size() || !inherited)
        return attr;

    // else inherited
    QByteArray superId = getSuperClassId(class_id);
    if (superId.isEmpty())
        // fail
        return BGItemAttribute();

    return getClassAttribute(superId, attr_id, true);
}

AttributesMap BGEditorScene::getClassAttributes(const QByteArray &class_id,
                                                bool inherited) const
{
    AttributesMap result = m_classAttributes[class_id];

    if (inherited) {
        QByteArray superId = getSuperClassId(class_id);
        while (!superId.isEmpty()) {
            // result = result.unite(m_classAttributes[superId]);
            // unite does not check for existing elements :(
            // there must be insertUnique
            OpenCOR::BondGraphEditorWindow::Utils::insertUnique(result,
                                                                m_classAttributes[superId]);

            superId = getSuperClassId(superId);
        }
    }

    return result;
}

BGItemAttributeConstraints *
BGEditorScene::getClassAttributeConstrains(const QByteArray &class_id,
                                           const QByteArray &attr_id) const
{
    ClassAttrIndex index(class_id, attr_id);

    if (m_classAttributesConstrains.contains(index))
        return m_classAttributesConstrains[index];
    else
        return nullptr;
}

void BGEditorScene::setClassAttributeConstrains(
    const QByteArray &class_id, const QByteArray &attr_id,
    BGItemAttributeConstraints *cptr)
{
    ClassAttrIndex index(class_id, attr_id);

    // do we need to clean up?
    // if (m_classAttributesConstrains.contains(index))
    //	delete m_classAttributesConstrains[index];

    if (cptr)
        m_classAttributesConstrains[index] = cptr;
    else
        m_classAttributesConstrains.remove(index);
}

// copy-paste

void BGEditorScene::cut()
{
    copy();
    del();
}

SceneEditControllerInterface *BGEditorScene::getSceneEditController() const
{
    return m_editController;
}

// context menu
void BGEditorScene::setContextMenuController(SceneMenuControllerInterface *controller)
{
    m_menuController = controller;
}

SceneMenuControllerInterface *BGEditorScene::getContextMenuController() const
{
    return m_menuController;
}

QGraphicsItem *BGEditorScene::getContextMenuTrigger() const
{
    return m_menuTriggerItem;
}

int BGEditorScene::getInfoStatus() const
{
    return m_infoStatus;
}

void BGEditorScene::setPastePosition(const QPointF &anchor)
{
    m_pastePos = anchor;
}

void BGEditorScene::del()
{
    QList<QGraphicsItem *> itemList = createSelectedList(DeletableSceneItems());
    if (itemList.isEmpty())
        return;

    QList<QGraphicsItem *> cItems;
    QList<QGraphicsItem *> eItems;

    for (QGraphicsItem *item : itemList) {
        if (items().contains(item)) { // Delete elements, attached ports and
                                      // connections taken care by the element, port
                                      // delete's are handled by elements
            if (BGConnection *bgconn = dynamic_cast<BGConnection *>(item)) {
                Q_UNUSED(bgconn);
                Q_EMIT itemDeleted(item);
                cItems.push_back(item);
            } else if (BGElement *bgelem = dynamic_cast<BGElement *>(item)) {
                Q_UNUSED(bgelem);
                Q_EMIT itemDeleted(item);
                eItems.push_back(item);
            }
        }
    }

    // Block signals
    beginSelection();
    // First delete connections, else when deleting elements connected connections
    // are also deleted and we will not know what these connnections are unless we
    // traverse the elements and get the connections

    for (QGraphicsItem *it : cItems) {
        delete it;
    }
    for (QGraphicsItem *it : eItems) {
        delete it;
    }
    endSelection();

    addUndoState();
}

void BGEditorScene::copy()
{
    // store selected items only
    QMap<SceneItem *, quint64> sortedMap;

    QList<QGraphicsItem *> allItems = getCopyPasteItems();

    for (QGraphicsItem *item : allItems) {
        if (SceneItem *citem = dynamic_cast<SceneItem *>(item)) {
            sortedMap[citem] = quint64(citem);
        }
    }

    if (sortedMap.isEmpty()) {
        QApplication::clipboard()->clear();
        return;
    }

    // write version and items
    QByteArray buffer;
    QDataStream out(&buffer, QIODevice::WriteOnly);

    out << version64;

    for (SceneItem *citem : sortedMap.keys()) {
        out << citem->typeId() << quint64(citem);

        citem->storeTo(out, version64);
    }

    // create mime object
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("BGUI_Selection", buffer);
    QApplication::clipboard()->setMimeData(mimeData);

    // paste it to a temp scene & render into mime image
    BGEditorScene *tempScene = createScene();
    tempScene->copyProperties(*this);
    tempScene->enableGrid(false);
    tempScene->paste();
    tempScene->deselectAll();
    tempScene->crop();

    QImage image(tempScene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(Qt::white);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    tempScene->render(&painter);
    painter.end();

    mimeData->setImageData(QVariant(image));
    QApplication::clipboard()->setMimeData(mimeData);

    delete tempScene;
}

void BGEditorScene::paste()
{
    if (!m_pastePos.isNull()) {
        pasteAt(m_pastePos);
        return;
    }

    auto *view = getCurrentView();
    if (view) {
        if (view->underMouse()) {
            auto p = view->mapFromGlobal(QCursor::pos());
            pasteAt(view->mapToScene(p));
        } else {
            QRectF vp = view->mapToScene(view->viewport()->geometry()).boundingRect();
            auto center = vp.center();
            pasteAt(center);
        }
    } else {
        pasteAt(QPointF());
    }
}

void BGEditorScene::pasteAt(const QPointF &anchor)
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (!mimeData)
        return;
    if (!mimeData->hasFormat("BGUI_Selection"))
        return;

    deselectAll();

    // read items from the buffer
    QByteArray buffer = mimeData->data("BGUI_Selection");
    QDataStream out(buffer);

    // version
    quint64 storedVersion = 0;
    out >> storedVersion;

    SceneItem::CItemLinkMap idToItem;
    QList<SceneItem *> deathList, lifeList;

    while (!out.atEnd()) {
        QByteArray typeId;
        out >> typeId;
        quint64 ptrId;
        out >> ptrId;

        SceneItem *item = createItemOfType(typeId);
        if (item) {
            if (item->restoreFrom(out, storedVersion)) {
                idToItem[ptrId] = item;
            } else
                deathList << item;
        }
    }

    // link items
    QSignalBlocker blocker(this);

    for (SceneItem *item : idToItem.values()) {
        if (item->linkAfterPaste(idToItem)) {
            auto sceneItem = dynamic_cast<QGraphicsItem *>(item);
            addItem(sceneItem);
            sceneItem->setSelected(true);

            lifeList << item;
        } else
            deathList << item;
    }

    // cleanup
    qDeleteAll(deathList);

    if (lifeList.isEmpty())
        return;

    // shift & rename pasted items which were not removed
    QMap<QString, int> ids;
    auto allItems = getItems<SceneItem>();
    for (auto item : allItems)
        ids[item->getId() + item->typeId()]++;

    // shift if not in-place
    auto selItems = selectedItems();

    if (!anchor.isNull()) {
        QRectF r = Utils::getBoundingRect(selItems);
        QPointF d = anchor - r.center();
        moveSelectedItemsBy(d);
    }

    // rename pasted items
    for (auto sceneItem : selItems) {
        SceneItem *item = dynamic_cast<SceneItem *>(sceneItem);
        if (item) {
            QString id = item->getId();
            QString typeId = item->typeId();
            if (ids[id + typeId] > 1) {
                int counter = 1;
                QString newId = id;

                while (ids.contains(newId + typeId))
                    newId = QString("Copy%1 of %2").arg(counter++).arg(id);

                item->setId(newId);
            }
        }
    }

    for (SceneItem *item : idToItem.values()) {
        item->onItemRestored();
    }

    blocker.unblock();
    Q_EMIT selectionChanged();

    // finish
    addUndoState();
}

QList<QGraphicsItem *> BGEditorScene::getCopyPasteItems() const
{
    return selectedItems();
}

QList<SceneItem *> BGEditorScene::cloneSelectedItems()
{
    QList<SceneItem *> clonedList;

    // store selected items only
    QMap<SceneItem *, quint64> sortedMap;

    QList<QGraphicsItem *> allItems = getCopyPasteItems();

    for (QGraphicsItem *item : allItems) {
        if (SceneItem *citem = dynamic_cast<SceneItem *>(item)) {
            sortedMap[citem] = quint64(citem);
        }
    }

    if (sortedMap.isEmpty())
        return clonedList;

    // write version and items
    QByteArray buffer;
    {
        QDataStream out(&buffer, QIODevice::WriteOnly);

        for (SceneItem *citem : sortedMap.keys()) {
            out << citem->typeId() << quint64(citem);

            citem->storeTo(out, version64);
        }
    }

    // read cloned items from the buffer
    deselectAll();

    SceneItem::CItemLinkMap idToItem;
    QList<SceneItem *> deathList;

    {
        QDataStream in(buffer);

        while (!in.atEnd()) {
            QByteArray typeId;
            in >> typeId;
            quint64 ptrId;
            in >> ptrId;

            SceneItem *item = createItemOfType(typeId);
            if (item) {
                if (item->restoreFrom(in, version64)) {
                    idToItem[ptrId] = item;
                } else
                    deathList << item;
            }
        }
    }

    // link items
    QSignalBlocker blocker(this);

    for (SceneItem *item : idToItem.values()) {
        if (item->linkAfterPaste(idToItem)) {
            auto sceneItem = dynamic_cast<QGraphicsItem *>(item);
            addItem(sceneItem);
            sceneItem->setSelected(true);

            clonedList << item;
        } else
            deathList << item;
    }

    // cleanup
    qDeleteAll(deathList);

    if (clonedList.isEmpty())
        return clonedList;

    // shift & rename pasted items which were not removed
    QMap<QString, int> ids;
    auto allCItems = getItems<SceneItem>();
    for (auto item : allCItems)
        ids[item->getId() + item->typeId()]++;

    // shift if not in-place
    auto selItems = selectedItems();

    // rename pasted items
    for (auto sceneItem : selItems) {
        SceneItem *item = dynamic_cast<SceneItem *>(sceneItem);
        if (item) {
            QString id = item->getId();
            QString typeId = item->typeId();
            if (ids[id + typeId] > 1) {
                int counter = 1;
                QString newId = id;

                while (ids.contains(newId + typeId))
                    newId = QString("Copy%1 of %2").arg(counter++).arg(id);

                item->setId(newId);
            }
        }
    }

    for (SceneItem *item : idToItem.values()) {
        item->onItemRestored();
    }

    blocker.unblock();
    Q_EMIT selectionChanged();

    return clonedList;
}

// crop

void BGEditorScene::crop()
{
    QRectF itemsRect = itemsBoundingRect().adjusted(-20, -20, 20, 20);
    if (itemsRect == sceneRect())
        return;

    // update scene rect
    setSceneRect(itemsRect);

    addUndoState();
}

// transform

QList<QGraphicsItem *> BGEditorScene::getTransformableItems() const
{
    return selectedItems();
}

// callbacks
void BGEditorScene::refresh(SceneItem *citem)
{
    citem->updateLabelContent();
}

void BGEditorScene::onItemDestroyed(SceneItem *citem)
{
    Q_ASSERT(citem);
}

void BGEditorScene::onSceneChanged()
{
    Q_EMIT sceneChanged();

    layoutItemLabels();

    if (m_editController) {
        m_editController->onSceneChanged(*this);
    }
}

void BGEditorScene::onSelectionChanged()
{
    int selectionCount = selectedItems().size();
    actions()->cutAction->setEnabled(selectionCount > 0);
    actions()->copyAction->setEnabled(selectionCount > 0);
    actions()->deleteAction->setEnabled(selectionCount > 0);

    if (m_editController) {
        m_editController->onSelectionChanged(*this);
    }
}

void BGEditorScene::onFocusItemChanged(QGraphicsItem *new_focus_item,
                                       QGraphicsItem *old_focus_item,
                                       Qt::FocusReason reason)
{
    Q_UNUSED(new_focus_item);
    Q_UNUSED(old_focus_item);
    Q_UNUSED(reason);
}

// drawing

void BGEditorScene::drawBackground(QPainter *painter, const QRectF &)
{
    // invalidate items if needed
    if (m_forceUpdateItems) {
        m_forceUpdateItems = false;
        auto citems = getItems<SceneItem>();
        for (auto citem : citems) {
            citem->updateCachedItems();
            citem->getSceneItem()->update();
        }
    }

    // update layout if needed
    if (m_labelsUpdate) {
        layoutItemLabels();
    }

    // fill background

    /*
  if (painter->paintEngine()->type() == QPaintEngine::OpenGL ||
      painter->paintEngine()->type() == QPaintEngine::OpenGL2) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
  */
    painter->setPen(QPen(Qt::darkGray, 2, Qt::SolidLine));
    painter->setBrush(backgroundBrush());
    painter->drawRect(sceneRect());

    // draw grid if needed
    if (m_gridSize <= 0 || !m_gridEnabled)
        return;

    painter->setPen(m_gridPen);

    QRectF rect = sceneRect();

    qreal left = int(rect.left()) - (int(rect.left()) % m_gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % m_gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += m_gridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += m_gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    painter->drawLines(lines.data(), lines.size());
}

void BGEditorScene::drawForeground(QPainter *painter, const QRectF &r)
{
    QGraphicsScene::drawForeground(painter, r);

    // drop label update flag
    m_labelsUpdate = false;

    // draw transformer etc
    if (m_editController)
        m_editController->draw(*this, painter, r);
}

bool BGEditorScene::checkLabelRegion(const QRectF &r)
{
    if (!r.isValid())
        return false;

    if (m_usedLabelsRegion.intersects(r))
        return false;

    m_usedLabelsRegion.addRect(r);
    return true;
}

BGEditorScene::LabelsPolicy BGEditorScene::getLabelsPolicy() const
{
    int labelPolicy = getClassAttribute(class_scene, attr_labels_policy, false)
                          .defaultValue.toInt();
    return (BGEditorScene::LabelsPolicy)labelPolicy;
}

void BGEditorScene::setLabelsPolicy(BGEditorScene::LabelsPolicy v)
{
    setClassAttribute(class_scene, attr_labels_policy, (QVariant)v);
}

void BGEditorScene::layoutItemLabels()
{
    // reset region
    m_usedLabelsRegion = QPainterPath();

    QList<SceneItem *> allItems = getItems<SceneItem>();

    // get labeling policy
    auto labelPolicy = getLabelsPolicy();

    // hide all if disabled
    if (!m_labelsEnabled || labelPolicy == AlwaysOff) {
        for (auto citem : allItems) {
            citem->showLabel(false);
        }

        return;
    }

    QElapsedTimer tm;
    tm.start();

    // else layout texts
    for (auto citem : allItems) {
        citem->updateLabelContent();
        citem->updateLabelPosition();

        if (labelPolicy == AlwaysOn)
            citem->showLabel(true);
        else {
            QRectF labelRect = citem->getSceneLabelRect();
            QRectF reducedRect(labelRect.topLeft() / 10, labelRect.size() / 10);

            citem->showLabel(checkLabelRegion(reducedRect));
        }
    }
}

void BGEditorScene::forceUpdate()
{
    m_labelsUpdate = true;
    m_forceUpdateItems = true;

    update();
}

// mousing

void BGEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouse_event)
{
    if (mouse_event->button() == Qt::LeftButton) {
        m_cancelled = false;
    }

    if (m_editController) {
        if (m_editItem) {
            m_labelEditor.finishEdit();
        }

        if (m_editController->onMousePressed(*this, mouse_event)) {
            mouse_event->setAccepted(true);
            return;
        } else
            mouse_event->setAccepted(false);
    }

    if (m_editItem) {
        // call super
        QGraphicsScene::mousePressEvent(mouse_event);
        return;
    }

    // check RMB
    if (mouse_event->button() == Qt::RightButton) {
        onRightButtonPressed(mouse_event);
    }

    // check LMB
    if (mouse_event->button() == Qt::LeftButton) {
        onLeftButtonPressed(mouse_event);
    }

    // call super is allowed
    if (!mouse_event->isAccepted()) {
        QGraphicsScene::mousePressEvent(mouse_event);
    }
}

void BGEditorScene::selectUnderMouse(QGraphicsSceneMouseEvent *mouse_event)
{
    auto item = getItemAt(mouse_event->scenePos());
    if (item) {
        if (!item->isSelected()) {
            deselectAll();
            item->setSelected(true);
        }
    }
}

void BGEditorScene::onLeftButtonPressed(QGraphicsSceneMouseEvent *mouse_event)
{
    m_draggedItem = nullptr;
    m_dragInProgress = false;

    m_leftClickPos = mouse_event->scenePos();
}

void BGEditorScene::onRightButtonPressed(QGraphicsSceneMouseEvent *mouse_event)
{
    auto item = getItemAt(mouse_event->scenePos());
    if (!item)
        return;

    // bypass control points
    if (dynamic_cast<ControlPoint *>(item))
        return;

    // do not deselect selected items by RMB, but select exclusive if not selected
    if (!item->isSelected()) {
        deselectAll();
        item->setSelected(true);
    }

    mouse_event->accept();
    return;
}

void BGEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouse_event)
{
    // by some reason (Qt bug?) this method can be entered twice -> stack overflow
    // so this is an ugly workaround...
    static bool entered = false;
    if (entered)
        return;
    entered = true;

    // ignore if cancelled
    if (m_cancelled) {
        entered = false;
        return;
    }

    if (m_editController) {
        if (m_editController->onMouseMove(*this, mouse_event)) {
            mouse_event->accept();
            entered = false;
            return;
        } else
            mouse_event->ignore();
    }

    if (m_editItem) {
        // call super
        QGraphicsScene::mouseMoveEvent(mouse_event);
        entered = false;
        return;
    }

    // store the last position
    m_mousePos = mouse_event->scenePos();
    m_dragInProgress = mouse_event->buttons() == Qt::LeftButton;

    if (m_doubleClick) {
        m_doubleClick = false;

        // moved after double click?
        if (m_dragInProgress && !onDoubleClickDrag(mouse_event, m_leftClickPos)) {
            entered = false;
            return;
        }
    }

    // no double click and no drag
    if (m_startDragItem == nullptr) {
        // moved after single click?
        if (m_dragInProgress && onClickDrag(mouse_event, m_leftClickPos)) {
            moveDrag(mouse_event, m_startDragItem, true);
            entered = false;
            return;
        }

        // call super
        QGraphicsScene::mouseMoveEvent(mouse_event);

        m_draggedItem = mouseGrabberItem();

        moveDrag(mouse_event, m_draggedItem, false);

        updateCursorState();

        entered = false;
        return;
    }

    // custom dragging
    moveDrag(mouse_event, m_startDragItem, m_dragInProgress);

    entered = false;
}

void BGEditorScene::startDrag(QGraphicsItem *drag_item)
{
    m_startDragItem = drag_item;
    m_dragInProgress = true;
    m_lastDragPos = m_leftClickPos;
    m_cancelled = false;
}

void BGEditorScene::processDrag(QGraphicsSceneMouseEvent *mouse_event,
                                QGraphicsItem *drag_item)
{
    if (m_editController) {
        m_editController->onDragItem(*this, mouse_event, drag_item);
    }

    QPointF d =
        (mouse_event->scenePos() - mouse_event->lastScenePos()); // delta pos

    if (m_startDragItem) {
        auto keys = qApp->queryKeyboardModifiers();

        if (keys & Qt::ShiftModifier) {
            auto hpos = mouse_event->scenePos();
            auto delta = hpos - m_leftClickPos;
            if (qAbs(delta.x()) > qAbs(delta.y()))
                hpos.setY(m_leftClickPos.y());
            else
                hpos.setX(m_leftClickPos.x());

            d = hpos - m_lastDragPos;
            m_lastDragPos = hpos;
        } else {
            d = (mouse_event->scenePos() - m_lastDragPos); // delta pos
            m_lastDragPos = mouse_event->scenePos();
            // Moving selected items will cause the start_item to go to the
            // bottom left. So juts move the dragged item and return
            drag_item->moveBy(d.x(), d.y());
            return;
        }
    }

    // if control point: move only it
    if (auto ctrl = dynamic_cast<ControlPoint *>(m_startDragItem)) {
        // deselectAll();
        ctrl->moveBy(d.x(), d.y());
        return;
    }

    // fallback
    moveSelectedItemsBy(d);
}

void BGEditorScene::moveDrag(QGraphicsSceneMouseEvent *mouse_event,
                             QGraphicsItem *drag_item, bool perform_drag)
{
    if (drag_item) {
        if (drag_item->flags() & drag_item->ItemIsMovable) {
            if (perform_drag) {
                processDrag(mouse_event, drag_item);
            }

            QSet<InteractiveItemInterface *> oldHovers =
                m_acceptedHovers + m_rejectedHovers;

            QList<QGraphicsItem *> hoveredItems = drag_item->collidingItems();

            for (auto hoverItem : hoveredItems) {
                // filter out children
                if (hoverItem->parentItem() == drag_item)
                    continue;

                // dont drop on disabled
                if (!hoverItem->isEnabled())
                    continue;

                InteractiveItemInterface *item =
                    dynamic_cast<InteractiveItemInterface *>(hoverItem);
                SceneItem *citem = dynamic_cast<SceneItem *>(hoverItem);

                if (item) {
                    oldHovers.remove(item);
                    if (m_acceptedHovers.contains(item) || m_rejectedHovers.contains(item))
                        continue;

                    ItemDragTestResult result = item->acceptDragFromItem(drag_item);

                    if (result == Accepted) {
                        m_acceptedHovers.insert(item);

                        if (citem) {
                            citem->setItemStateFlag(IS_Drag_Accepted);
                            citem->resetItemStateFlag(IS_Drag_Rejected);
                        }
                    } else if (result == Rejected) {
                        m_rejectedHovers.insert(item);

                        if (citem) {
                            citem->resetItemStateFlag(IS_Drag_Accepted);
                            citem->setItemStateFlag(IS_Drag_Rejected);
                        }
                    }

                    hoverItem->update();
                }
            }

            // deactivate left hovers
            for (InteractiveItemInterface *item : oldHovers) {
                item->leaveDragFromItem(drag_item);

                m_acceptedHovers.remove(item);
                m_rejectedHovers.remove(item);

                SceneItem *citem = dynamic_cast<SceneItem *>(item);
                if (citem) {
                    citem->resetItemStateFlag(IS_Drag_Accepted);
                    citem->resetItemStateFlag(IS_Drag_Rejected);
                }

                if (auto hoverItem = dynamic_cast<QGraphicsItem *>(item))
                    hoverItem->update();
            }

            // inform the dragger
            InteractiveItemInterface *draggedItem =
                dynamic_cast<InteractiveItemInterface *>(drag_item);
            if (draggedItem) {
                draggedItem->onDraggedOver(m_acceptedHovers, m_rejectedHovers);
            }

            // inform the scene
            onDragging(drag_item, m_acceptedHovers, m_rejectedHovers);
        }
    } else // no drag, just hover
    {
        QGraphicsItem *hoverItem = getItemAt(mouse_event->scenePos());

        // inform the scene
        onMoving(mouse_event, hoverItem);
    }
}

void BGEditorScene::mouseDoubleClickEvent(
    QGraphicsSceneMouseEvent *mouse_event)
{
    QGraphicsScene::mouseDoubleClickEvent(mouse_event);

    if (mouse_event->button() == Qt::LeftButton) {
        m_doubleClick = true;
    }
}

void BGEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouse_event)
{
    // ignore if cancelled
    if (m_cancelled) {
        if (mouse_event->button() == Qt::LeftButton)
            m_cancelled = false;

        QGraphicsScene::mouseReleaseEvent(mouse_event);
        return;
    }

    if (m_editController) {
        if (m_editController->onMouseReleased(*this, mouse_event)) {
            mouse_event->setAccepted(true);
            return;
        } else
            mouse_event->setAccepted(false);
    }

    QGraphicsItem *prevGrabber = m_draggedItem;

    QGraphicsScene::mouseReleaseEvent(mouse_event);

    m_draggedItem = mouseGrabberItem();

    if (mouse_event->button() == Qt::LeftButton) {
        if (m_dragInProgress) {
            finishDrag(mouse_event, prevGrabber, false);
        } else if (m_leftClickPos == mouse_event->scenePos()) {
            QGraphicsItem *clickedItem = getItemAt(mouse_event->scenePos());
            if (BGPort *item = dynamic_cast<BGPort *>(clickedItem)) {
                item->onClick(mouse_event);
            }
        }
        /*
    else if (m_leftClickPos == mouseEvent->scenePos()) {
      QGraphicsItem *hoverItem = getItemAt(mouseEvent->scenePos());

      if (m_doubleClick)
        onLeftDoubleClick(mouseEvent, hoverItem);
      else
        onLeftClick(mouseEvent, hoverItem);
    }*/
    }

    m_doubleClick = false;
    m_dragInProgress = false;
    m_cancelled = false;
}

void BGEditorScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("diagramType")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void BGEditorScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->hasFormat("diagramType")) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void BGEditorScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
}

void BGEditorScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasFormat("diagramType")) {
        onDropped(event);
    }
}

void BGEditorScene::finishDrag(QGraphicsSceneMouseEvent *mouse_event,
                               QGraphicsItem *drag_item, bool drag_cancelled)
{
    // cleanup drag state
    m_startDragItem = nullptr;
    m_dragInProgress = false;

    m_cancelled = drag_cancelled;
    if (m_cancelled) {
        m_leftClickPos = QPointF();
        m_lastDragPos = QPointF();
        m_mousePos = QPointF();
        m_draggedItem = nullptr;
    }

    if (drag_item) {
        // deactivate left hovers
        for (InteractiveItemInterface *item : m_acceptedHovers) {
            item->leaveDragFromItem(drag_item);

            if (auto *citem = dynamic_cast<SceneItem *>(item)) {
                citem->resetItemStateFlag(IS_Drag_Accepted);
                citem->resetItemStateFlag(IS_Drag_Rejected);
            }
        }

        for (InteractiveItemInterface *item : m_rejectedHovers) {
            item->leaveDragFromItem(drag_item);

            if (auto *citem = dynamic_cast<SceneItem *>(item)) {
                citem->resetItemStateFlag(IS_Drag_Accepted);
                citem->resetItemStateFlag(IS_Drag_Rejected);
            }
        }

        // inform the port or the dragger
        BGPort *pitem = dynamic_cast<BGPort *>(drag_item);
        if (pitem == nullptr) {
            InteractiveItemInterface *draggedItem =
                dynamic_cast<InteractiveItemInterface *>(drag_item);
            if (draggedItem && !drag_cancelled) {
                draggedItem->onDroppedOn(m_acceptedHovers, m_rejectedHovers);
            }

            // drag finish
            m_acceptedHovers.clear();
            m_rejectedHovers.clear();

            if (!drag_cancelled) {
                // snap after drop if dragItem still alive (can die after onDroppedOn??)
                if (items().contains(drag_item)) {
                    onDropped(mouse_event, drag_item);
                }
                // update undo manager
                addUndoState();
            }
        } else {
            pitem->onClickDrag(mouse_event, pitem->scenePos());

            // drag finish
            m_acceptedHovers.clear();
            m_rejectedHovers.clear();
            // update undo manager
            addUndoState();
        }
    }
}

void BGEditorScene::onMoving(QGraphicsSceneMouseEvent *mouse_event,
                             QGraphicsItem *hover_item)
{
    Q_UNUSED(mouse_event);
    updateCursorState();

    if (hover_item)
        setInfoStatus(SSI_Hover);
    else
        setInfoStatus(SSI_Select);
}

void BGEditorScene::onDragging(
    QGraphicsItem * /*dragItem*/,
    const QSet<InteractiveItemInterface *> & /*acceptedItems*/,
    const QSet<InteractiveItemInterface *> & /*rejectedItems*/)
{
    updateCursorState();

    setInfoStatus(SSI_Drag);
}

void BGEditorScene::onDropped(QGraphicsSceneDragDropEvent *mouse_event)
{
    // implemented by child
    mouse_event->setDropAction(Qt::IgnoreAction);
    return;
}

void BGEditorScene::onDropped(QGraphicsSceneMouseEvent *mouse_event,
                              QGraphicsItem *drag_item)
{
    auto keys = mouse_event->modifiers();
    bool isSnap = (keys & Qt::AltModifier) ? !m_gridSnap : m_gridSnap;
    if (isSnap) {
        auto pos = getSnapped(drag_item->pos());
        auto d = pos - drag_item->pos();
        drag_item->setPos(pos);

        for (auto item : selectedItems()) {
            if (item != drag_item)
                item->moveBy(d.x(), d.y());
        }
    }
}

void BGEditorScene::onLeftClick(QGraphicsSceneMouseEvent *mouse_event,
                                QGraphicsItem *clicked_item)
{
    if (SceneItem *item = dynamic_cast<SceneItem *>(clicked_item)) {
        item->onClick(mouse_event);
    }
}

void BGEditorScene::onLeftDoubleClick(QGraphicsSceneMouseEvent *mouse_event,
                                      QGraphicsItem *clicked_item)
{
    // clicked on empty space?
    // if (!clickedItem)
    //	return;

    // else check clicked item...
    if (SceneItem *item = dynamic_cast<SceneItem *>(clicked_item)) {
        onActionEditLabel(item);
        // item->onDoubleClick(mouseEvent);
    }

    // emit signals
    Q_EMIT sceneDoubleClicked(mouse_event, clicked_item);
}

bool BGEditorScene::onClickDrag(QGraphicsSceneMouseEvent *mouse_event,
                                const QPointF &click_pos)
{
    if (QGraphicsItem *item = getItemAt(click_pos)) {
        if (!item->isEnabled())
            return false;

        if (!(item->flags() & item->ItemIsMovable))
            return false;

        if (SceneItem *citem = dynamic_cast<SceneItem *>(item)) {
            // clone?
            if (mouse_event->modifiers() == Qt::ControlModifier) {
                // select under mouse if clone
                selectUnderMouse(mouse_event);

                // clone selection
                QList<SceneItem *> clonedList = cloneSelectedItems();
                if (clonedList.isEmpty())
                    return false;

                selectItems(clonedList);

                // start drag via 1st item
                startDrag(clonedList.first()->getSceneItem());

                return true;
            }

            // else handle by item
            if (!citem->onClickDrag(mouse_event, click_pos))
                return false;
        }
        // Handle ports (they are not a scene item)
        else if (BGPort *pitem = dynamic_cast<BGPort *>(item)) {
            Q_UNUSED(pitem);
            startDrag(item);
            return true;
        }
        // else start drag of item
        startDrag(item);
        return true;
    }

    // nothing to do
    return false;
}

bool BGEditorScene::onDoubleClickDrag(QGraphicsSceneMouseEvent *mouse_event,
                                      const QPointF &click_pos)
{
    // handle by object under mouse
    QGraphicsItem *item = getItemAt(click_pos);
    if (item) {
        if (!item->isEnabled())
            return false;

        if (!(item->flags() & item->ItemIsMovable))
            return false;

        SceneItem *citem = dynamic_cast<SceneItem *>(item);
        if (citem)
            return citem->onDoubleClickDrag(mouse_event, click_pos);
    }

    // nothing to do
    return false;
}

// scene

void BGEditorScene::setInfoStatus(int status)
{
    if (m_infoStatus != status) {
        m_infoStatus = status;

        Q_EMIT infoStatusChanged(status);
    }
}

void BGEditorScene::updateCursorState()
{
    auto keys = qApp->queryKeyboardModifiers();
    auto buttons = qApp->mouseButtons();
    QGraphicsItem *hoverItem = getItemAt(m_mousePos);
    doUpdateCursorState(keys, buttons, hoverItem);
}

bool BGEditorScene::doUpdateCursorState(Qt::KeyboardModifiers keys,
                                        Qt::MouseButtons buttons,
                                        QGraphicsItem *hover_item)
{
    // drag?
    if (m_dragInProgress) {
        if (m_acceptedHovers.size()) {
            setSceneCursor(Qt::CrossCursor);
            return true;
        }

        if (m_rejectedHovers.size()) {
            setSceneCursor(Qt::ForbiddenCursor);
            return true;
        }

        // clone?
        // if (keys == Qt::ControlModifier)
        //{
        //	setSceneCursor(Qt::DragCopyCursor);
        //	return;
        //}

        setSceneCursor(Qt::SizeAllCursor);
        return true;
    }

    // can drag a hover item?
    if (hover_item) {
        if (hover_item->isEnabled() && (hover_item->flags() & hover_item->ItemIsMovable)) {
            // clone?
            if (keys == Qt::ControlModifier) {
                setSceneCursor(Qt::DragCopyCursor);
                return true;
            }

            if (buttons == Qt::NoButton) {
                setSceneCursor(Qt::SizeAllCursor);
                return true;
            }
        }
    }

    // default
    setSceneCursor(Qt::ArrowCursor);
    return false;
}

QPointF BGEditorScene::getSnapped(const QPointF &pos) const
{
    auto keys = qApp->queryKeyboardModifiers();
    bool isSnap =
        (keys & Qt::AltModifier) ? !gridSnapEnabled() : gridSnapEnabled();
    if (isSnap) {
        QPointF newPos(pos);

        if (newPos.x() < 0)
            newPos.setX(newPos.x() - m_gridSize / 2);
        else
            newPos.setX(newPos.x() + m_gridSize / 2);

        if (newPos.y() < 0)
            newPos.setY(newPos.y() - m_gridSize / 2);
        else
            newPos.setY(newPos.y() + m_gridSize / 2);

        newPos.setX((int)newPos.x() - (int)newPos.x() % m_gridSize);
        newPos.setY((int)newPos.y() - (int)newPos.y() % m_gridSize);

        return newPos;
    } else
        return pos;
}

QGraphicsItem *BGEditorScene::getItemAt(const QPointF &pos) const
{
    QGraphicsItem *hoverItem = itemAt(pos, QTransform());

    // if label: return parent instead
    if (dynamic_cast<QGraphicsSimpleTextItem *>(hoverItem)) {
        return hoverItem->parentItem();
    } else
        return hoverItem;
}

QGraphicsView *BGEditorScene::getCurrentView()
{
    for (auto view : views()) {
        if (view->underMouse() || view->hasFocus())
            return view;
    }

    if (views().count() == 1)
        return views().first();

    return nullptr;
}

// private

void BGEditorScene::setSceneCursor(const QCursor &c)
{
    for (QGraphicsView *v : views()) {
        v->setCursor(c);
    }
}

// keys

void BGEditorScene::keyReleaseEvent(QKeyEvent *key_event)
{
    if (m_editController) {
        if (m_editController->onKeyReleased(*this, key_event)) {
            updateCursorState();
            return;
        }
    }

    updateCursorState();
}

void BGEditorScene::keyPressEvent(QKeyEvent *key_event)
{
    if (m_editController) {
        if (m_editController->onKeyPressed(*this, key_event)) {
            updateCursorState();
            return;
        }
    }

    updateCursorState();

    bool isCtrl = (key_event->modifiers() & Qt::ControlModifier);
    bool isAlt = (key_event->modifiers() & Qt::AltModifier);
    bool isShift = (key_event->modifiers() & Qt::ShiftModifier);

    // if (isAlt /*|| keyEvent->isAccepted()*/)
    //	return;

    if (key_event->key() == Qt::Key_Delete) {
        onActionDelete();

        key_event->accept();
        return;
    }

    if (key_event->key() == Qt::Key_A && isCtrl) {
        onActionSelectAll();

        key_event->accept();
        return;
    }

    if (key_event->key() == Qt::Key_Right && isAlt && isCtrl) {
        // rotate ports
        rotateElementPort(Qt::Key_Right);
        addUndoState();

        key_event->accept();
        return;
    }

    if (key_event->key() == Qt::Key_Up && isAlt && isCtrl) {
        // rotate ports
        rotateElementPort(Qt::Key_Up);
        addUndoState();

        key_event->accept();
        return;
    }

    // no modifier moves by 1 pixel, shift moves by grid size
    int moveStep = isShift ? m_gridSize : 1;
    bool moveSnapped = isShift;

    if (key_event->key() == Qt::Key_Right) {
        moveSelectedItemsBy(moveStep, 0, moveSnapped);
        addUndoState();

        key_event->accept();
        return;
    }

    if (key_event->key() == Qt::Key_Left) {
        moveSelectedItemsBy(-moveStep, 0, moveSnapped);
        addUndoState();

        key_event->accept();
        return;
    }

    if (key_event->key() == Qt::Key_Up) {
        moveSelectedItemsBy(0, -moveStep, moveSnapped);
        addUndoState();

        key_event->accept();
        return;
    }

    if (key_event->key() == Qt::Key_Down) {
        moveSelectedItemsBy(0, moveStep, moveSnapped);
        addUndoState();

        key_event->accept();
        return;
    }
}

// general events

void BGEditorScene::focusInEvent(QFocusEvent *focus_event)
{
    QGraphicsScene::focusInEvent(focus_event);

    static bool sFirstRun = true;
    if (sFirstRun) {
        sFirstRun = false;

        // init scene
        addUndoState();
    }
}

// menu stuff

void BGEditorScene::contextMenuEvent(
    QGraphicsSceneContextMenuEvent *context_menu_event)
{
    if (m_skipMenuEvent) {
        m_skipMenuEvent = false;
        return;
    }

    m_menuTriggerItem =
        getItemAt(context_menu_event->scenePos()); // Get the item at the position

    // check if item provides own menu
    if (auto menuItem =
            dynamic_cast<BGContextMenuProviderInterface *>(m_menuTriggerItem)) {
        QMenu sceneMenu;
        if (menuItem->populateMenu(sceneMenu, selectedItems())) {
            sceneMenu.exec(context_menu_event->screenPos());
            return;
        }
    }

    // else custom menu
    if (m_menuController)
        m_menuController->exec(this, m_menuTriggerItem, context_menu_event);
}

void BGEditorScene::onActionDelete()
{
    QList<QGraphicsItem *> itemList = createSelectedList(DeletableSceneItems());
    if (itemList.isEmpty())
        return;

    if (QMessageBox::question(nullptr, tr("Delete Items"),
                              tr("You are about to delete %1 item(s). Sure?")
                                  .arg(itemList.size()))
        == QMessageBox::No)
        return;

    del();
}

void BGEditorScene::onActionSelectAll()
{
    selectAll();
}

// label edit

void BGEditorScene::onActionEditLabel(SceneItem *item)
{
    setInfoStatus(SSI_Edit_Label);
    setSceneCursor(Qt::IBeamCursor);

    m_labelEditor.startEdit(item);

    m_editItem = item;
}

void BGEditorScene::onItemEditingFinished(SceneItem * /*item*/,
                                          bool /*cancelled*/)
{
    m_editItem = nullptr;
}

// selections

void BGEditorScene::selectAll()
{
    QPainterPath path;
    path.addRect(sceneRect());
    setSelectionArea(path, QTransform());
}

void BGEditorScene::deselectAll()
{
    QPainterPath path;
    setSelectionArea(path, QTransform());
}

void BGEditorScene::selectItem(SceneItem *item, bool exclusive)
{
    if (!item)
        return;

    beginSelection();

    if (exclusive)
        deselectAll();

    item->getSceneItem()->setSelected(true);

    endSelection();
}

void BGEditorScene::selectItems(const QList<SceneItem *> &items,
                                bool exclusive)
{
    beginSelection();

    if (exclusive)
        deselectAll();

    for (auto item : items)
        if (item)
            item->getSceneItem()->setSelected(true);

    endSelection();
}

void BGEditorScene::beginSelection()
{
    blockSignals(true);
}

void BGEditorScene::endSelection()
{
    blockSignals(false);

    Q_EMIT selectionChanged();
}

void BGEditorScene::ensureSelectionVisible()
{
    auto items = selectedItems();
    /*
  //This was focusing things elsewhere
  QRectF r;
  for (const auto item : items){
    r |= item->sceneBoundingRect();
  }
  */

    if (items.count())
        items.first()->ensureVisible();
}

void BGEditorScene::rotateElementPort(int dir)
{
    auto items = selectedItems();

    for (auto sceneItem : items) {
        BGElement *elem = dynamic_cast<BGElement *>(sceneItem);
        if (elem) {
            elem->reorientPort(dir);
        } else {
            BGPort *port = dynamic_cast<BGPort *>(sceneItem);
            if (port) {
                port->getElement()->reorientPort(dir);
            }
        }
    }

    if (items.count())
        items.first()->ensureVisible();
}

void BGEditorScene::moveSelectedItemsBy(const QPointF &d, bool /*snapped*/)
{
    auto items = selectedItems();

    for (auto sceneItem : items) {
        sceneItem->moveBy(d.x(), d.y());
    }

    if (items.count())
        items.first()->ensureVisible();
}

// evaluators

QList<QGraphicsItem *>
BGEditorScene::createSelectedList(const SceneItemEvaluator &eval) const
{
    QList<QGraphicsItem *> result;
    QList<QGraphicsItem *> itemList = getSelectedItems<QGraphicsItem>(true);

    for (auto i = 0; i < itemList.size(); ++i) {
        if (eval.evaluate(*itemList.at(i)))
            result << itemList.at(i);
    }

    return result;
}

bool DeletableSceneItems::evaluate(const QGraphicsItem &item) const
{
    const SceneItem *citem = dynamic_cast<const SceneItem *>(&item);
    if (citem) {
        return citem->itemFlags() & IF_DeleteAllowed;
    }

    // can delete QGraphicsItem
    return true;
}

// actions

BGEditorSceneActions *BGEditorScene::actions()
{
    return dynamic_cast<BGEditorSceneActions *>(getActions());
}

QObject *BGEditorScene::getActions()
{
    if (m_actions == nullptr)
        m_actions = createActions();

    return m_actions;
}

QObject *BGEditorScene::createActions()
{
    return new BGEditorSceneActions(this);
}

// edit extenders

void BGEditorScene::startTransform(bool on, bool move_only)
{
    if (on) {
        setSceneEditController(m_transformRect);

        m_transformRect->setMoveOnly(move_only);
    } else
        setSceneEditController(nullptr);
}

void BGEditorScene::setSceneEditController(
    SceneEditControllerInterface *controller)
{
    if (m_editController != controller) {
        if (m_editController)
            m_editController->onDeactivated(*this);

        m_editController = controller;

        if (m_editController)
            m_editController->onActivated(*this);
    }
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR