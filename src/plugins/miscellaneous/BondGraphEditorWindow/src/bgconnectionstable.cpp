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

#include <QElapsedTimer>
#include <QInputDialog>
#include <QMessageBox>
#include <QScrollBar>

#include "bgconnectionstable.h"

#include <bgconnection.h>
#include <bgelement.h>
#include <bgelementeditorscene.h>

namespace OpenCOR {
namespace BondGraphEditorWindow {

// NumSortItem: numeric sorting by ids

class NumSortItem : public QTreeWidgetItem
{
public:
    bool operator<(const QTreeWidgetItem &other) const
    {
        int col = treeWidget()->sortColumn();
        bool b1, b2;
        int i1 = text(col).toInt(&b1);
        int i2 = other.text(col).toInt(&b2);
        if (b1 && b2)
            return i1 < i2;

        return QTreeWidgetItem::operator<(other);
    }
};

// fixed section Ids

enum FixedSectionIds { EdgeId,
                       StartNodeId,
                       EndNodeId,
                       CustomId };

// BGConnectionsTable

BGConnectionsTable::BGConnectionsTable(QWidget *parent) :
    QWidget(parent), m_scene(nullptr)
{
    ui.setupUi(this);

    ui.Table->header()->setSortIndicator(2, Qt::AscendingOrder);
    connect(ui.Table, &QTreeWidget::itemChanged, this,
            &BGConnectionsTable::connectionNameChangeRequest);
}

BGConnectionsTable::~BGConnectionsTable()
{
}

void BGConnectionsTable::doReadSettings(QSettings &settings)
{
    QByteArray extraSections = settings.value("userColumns").toByteArray();
    if (!extraSections.isEmpty()) {
        m_extraSectionIds = extraSections.split(';');
        onSceneChanged();
    }

    auto *header = ui.Table->header();
    QByteArray headerState = settings.value("headerState").toByteArray();
    if (!headerState.isNull()) {
        header->restoreState(headerState);
    }
}

void BGConnectionsTable::doWriteSettings(QSettings &settings)
{
    auto *header = ui.Table->header();
    settings.setValue("headerState", header->saveState());
    settings.setValue("userColumns", m_extraSectionIds.join(';'));
}

void BGConnectionsTable::setScene(BGElementEditorScene *scene)
{
    ui.Table->clear();

    if (m_scene)
        onSceneDetached(m_scene);

    m_scene = scene;

    setEnabled(m_scene);

    if (m_scene)
        onSceneAttached(m_scene);
}

void BGConnectionsTable::connectSignals(BGEditorScene *scene)
{
    connect(scene, SIGNAL(sceneChanged()), this, SLOT(onSceneChanged()),
            Qt::QueuedConnection);
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()),
            Qt::QueuedConnection);
}

void BGConnectionsTable::onSceneAttached(BGEditorScene *scene)
{
    connectSignals(scene);

    onSceneChanged();
}

void BGConnectionsTable::onSceneDetached(BGEditorScene *scene)
{
    scene->disconnect(this);
}

void BGConnectionsTable::updateContents()
{
    ui.Table->setUpdatesEnabled(false);
    ui.Table->blockSignals(true);

    ui.Table->clear();
    m_edgeItemMap.clear();

    ui.Table->setColumnCount(m_extraSectionIds.size() + CustomId);

    int extraSectionIndex = CustomId;
    for (const auto &paramId : m_extraSectionIds) {
        ui.Table->headerItem()->setText(extraSectionIndex++, paramId);
    }

    QList<BGConnection *> edges = m_scene->getItems<BGConnection>();
    for (auto edge : edges) {
        auto item = new NumSortItem();
        ui.Table->addTopLevelItem(item);

        m_edgeItemMap[edge] = item;

        if (edge->firstElement()->getMaxPorts() > 1) {
            item->setText(StartNodeId, edge->firstElement()->getDisplayName() + " @ " + edge->firstPortId());
        } else {
            item->setText(StartNodeId, edge->firstElement()->getDisplayName());
        }
        if (edge->lastElement()->getMaxPorts() > 1) {
            item->setText(EndNodeId, edge->lastElement()->getDisplayName() + " @ " + edge->lastPortId());
        } else {
            item->setText(EndNodeId, edge->lastElement()->getDisplayName());
        }
        item->setText(EdgeId, edge->getId());
        item->setData(EdgeId, Qt::UserRole, edge->getId());
        item->setFlags(item->flags() | Qt::ItemIsEditable);

        int extraSectionIndex = CustomId;
        for (const auto &paramId : m_extraSectionIds) {
            QString val = edge->getAttribute(paramId).toString();
            item->setText(extraSectionIndex++, val);
        }
    }

    ui.Table->setUpdatesEnabled(true);
    ui.Table->blockSignals(false);
}

void BGConnectionsTable::onSceneChanged()
{
    updateContents();
    // update active selections if any
    onSelectionChanged();
}

void BGConnectionsTable::onSelectionChanged()
{
    ui.Table->setUpdatesEnabled(false);
    ui.Table->blockSignals(true);

    ui.Table->clearSelection();

    QTreeWidgetItem *scrollItem = nullptr;

    QList<BGConnection *> edges = m_scene->getSelectedConnections();

    // QElapsedTimer tm;
    // tm.start();

    QItemSelection selection;

    for (auto edge : edges) {
        if (m_edgeItemMap.contains(edge)) {
            auto item = m_edgeItemMap[edge];
            scrollItem = item;

            // item->setSelected(true);
            // version with QModelIndex is many ways faster...
            int row = ui.Table->indexOfTopLevelItem(item);

            QModelIndex leftIndex = ui.Table->model()->index(row, 0);
            QModelIndex rightIndex =
                ui.Table->model()->index(row, ui.Table->columnCount() - 1);

            QItemSelection rowSelection(leftIndex, rightIndex);
            selection.append(rowSelection);
        }
    }

    ui.Table->selectionModel()->select(selection, QItemSelectionModel::Select);

    if (scrollItem)
        ui.Table->scrollToItem(scrollItem);

    ui.Table->setUpdatesEnabled(true);
    ui.Table->blockSignals(false);
}

void BGConnectionsTable::connectionNameChangeRequest(QTreeWidgetItem *item,
                                                     int column)
{
    if (column == 0) {
        // Use id stored in data, as text is changed by the user
        QString id = item->data(column, Qt::UserRole).toString();
        auto edges = m_scene->getItemsById<BGConnection>(id);
        if (edges.count()) {
            auto sceneItem = edges.first();
            sceneItem->setId(item->text(column));
            item->setData(column, Qt::UserRole, item->text(column));
            sceneItem->updateLabelContent();
            m_scene->addUndoState();
            return;
        }
    }
}

void BGConnectionsTable::on_Table_itemSelectionChanged()
{
    if (!m_scene)
        return;

    ui.Table->blockSignals(true);

    m_scene->beginSelection();

    m_scene->deselectAll();

    auto selTableItems = ui.Table->selectedItems();
    QSet<QString> selIds;
    for (auto item : selTableItems) {
        selIds.insert(item->text(2));
    }

    QList<BGConnection *> edges = m_scene->getItems<BGConnection>();
    for (auto edge : edges) {
        if (selIds.contains(edge->getId())) {
            edge->setSelected(true);
            edge->ensureVisible();
        }
    }

    ui.Table->blockSignals(false);

    m_scene->endSelection();
}

void BGConnectionsTable::on_Table_itemDoubleClicked(QTreeWidgetItem *item,
                                                    int column)
{
    if (!m_scene || !item)
        return;

    if (column > 0) {
        QString iString = item->text(column);
        if (iString.contains("@"))
            iString = iString.left(iString.indexOf("@") - 1);

        auto nodes = m_scene->getItemsById<BGElement>(iString);

        if (nodes.count()) {
            m_scene->deselectAll();
            nodes.first()->setSelected(true);
            nodes.first()->ensureVisible();
            return;
        }
    }

    if (column == 0) {
        auto edges = m_scene->getItemsById<BGConnection>(item->text(column));
        if (edges.count()) {
            m_scene->deselectAll();
            edges.first()->setSelected(true);
            edges.first()->ensureVisible();
            ui.Table->editItem(item, column);
            return;
        }
    }
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR