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
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "bgconnection.h"
#include "bgelement.h"
#include "bgelementeditorscene.h"
#include "bgelementsceneactions.h"

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGElementSceneActions::BGElementSceneActions(BGElementEditorScene *scene) :
    BGEditorSceneActions(scene), bgElementScene(*scene)
{
}

bool BGElementSceneActions::editElementId(BGElement *editElement)
{
    if (editElement == nullptr)
        return false;

    QString id = editElement->getId();
    QString editId = id;

    bool redo = false;
    QString newId;
    do {
        newId = QInputDialog::getText(0, tr("Change element Id"),
                                      tr("Specify new element Id:"),
                                      QLineEdit::Normal, editId);

        if (newId.isEmpty() || newId == id)
            return false;

        auto items = bgElementScene.getItemsById(newId);
        for (auto item : items) {
            BGElement *elements = dynamic_cast<BGElement *>(item);
            if (elements == nullptr || elements == editElement)
                continue;

            if (elements->getId() == newId) {
                int count = 0;
                QString nextFreeId = newId + QString::number(count++);
                while (bgElementScene.getItemsById(nextFreeId).count()) {
                    nextFreeId = newId + QString::number(count++);
                }

                QString autoId = QString(tr("Suggested Id: %1").arg(nextFreeId));

                int r = QMessageBox::warning(
                    0, tr("Warning: Id is in use"),
                    tr("Id %1 is already used by another element.").arg(newId), autoId,
                    tr("Swap element Ids"), tr("Continue editing"), 0, 2);

                if (r == 2) {
                    editId = newId;
                    redo = true;
                    break;
                }

                if (r == 1) {
                    editElement->setId(newId);
                    elements->setId(id);
                    bgElementScene.addUndoState();
                    return true;
                }

                // r = 0
                editId = nextFreeId;
                redo = true;
                break;
            }
        }
    } while (redo);

    editElement->setId(newId);
    bgElementScene.addUndoState();
    return true;
}

void BGElementSceneActions::onActionElementColour()
{
    auto elements = bgElementScene.getSelectedElements();
    if (elements.isEmpty())
        return;

    QColor color = QColorDialog::getColor(
        elements.first()->getAttribute("color").value<QColor>());
    if (!color.isValid())
        return;

    for (auto elem : elements) {
        elem->setAttribute("color", color);
    }

    bgElementScene.addUndoState();
}

void BGElementSceneActions::onActionLink()
{
    auto elements = bgElementScene.getSelectedElements();
    if (elements.count() < 2)
        return;

    auto baseNode = elements.takeFirst();
    for (auto elem : elements) {
        baseNode->merge(elem);
    }

    bgElementScene.addUndoState();
}

void BGElementSceneActions::onActionUnlink()
{
    auto elements = bgElementScene.getSelectedElements();
    if (elements.isEmpty())
        return;

    for (auto elem : elements) {
        elem->unlink();
    }

    bgElementScene.addUndoState();
}

bool BGElementSceneActions::editConnectionId(BGConnection *editConnection)
{
    if (editConnection == nullptr)
        return false;

    QString id = editConnection->getId();
    QString editId = id;
    bool redo = false;
    QString newId;
    do {
        newId = QInputDialog::getText(0, tr("Change connection Id"),
                                      tr("Specify new connection Id:"),
                                      QLineEdit::Normal, editId);

        if (newId.isEmpty() || newId == id)
            return false;

        auto items = bgElementScene.getItemsById(newId);
        for (auto item : items) {
            BGConnection *connection = dynamic_cast<BGConnection *>(item);
            if (connection == nullptr || connection == editConnection)
                continue;

            if (connection->getId() == newId) {
                int count = 0;
                QString nextFreeId = newId + QString::number(count++);
                while (bgElementScene.getItemsById(nextFreeId).count()) {
                    nextFreeId = newId + QString::number(count++);
                }

                QString autoId = QString(tr("Suggested Id: %1").arg(nextFreeId));

                int r = QMessageBox::warning(
                    0, tr("Warning: Id is in use"),
                    tr("Id %1 is already used by another connection.").arg(newId),
                    autoId, tr("Swap connection Ids"), tr("Continue editing"), 0, 2);

                if (r == 2) {
                    editId = newId;
                    redo = true;
                    break;
                }

                if (r == 1) {
                    editConnection->setId(newId);
                    connection->setId(id);
                    bgElementScene.addUndoState();
                    return true;
                }

                // r = 0
                editId = nextFreeId;
                redo = true;
                break;
            }
        }
    } while (redo);

    editConnection->setId(newId);
    bgElementScene.addUndoState();
    return true;
}

void BGElementSceneActions::onActionConnectionReverse()
{
    auto connections = bgElementScene.getSelectedConnections();
    if (connections.isEmpty())
        return;

    for (auto connection : connections) {
        connection->reverse();
    }

    bgElementScene.addUndoState();
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
