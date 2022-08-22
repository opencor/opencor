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
#include "bgeditorscene.h"
#include "sceneundomanager.h"

#include <QDataStream>

namespace OpenCOR {
namespace BondGraphEditorWindow {

SceneUndoManager::SceneUndoManager(BGEditorScene &scene) :
    m_scene(&scene), m_stackIndex(-1)
{
}

void SceneUndoManager::reset()
{
    m_stackIndex = -1;
    m_stateStack.clear();
}

void SceneUndoManager::addState()
{
    // serialize & compress
    QByteArray snap;
    QDataStream ds(&snap, QIODevice::WriteOnly);
    m_scene->storeTo(ds, false);
    QByteArray compressedSnap = qCompress(snap);

    // push state into stack
    if (m_stateStack.size() == ++m_stackIndex) {
        m_stateStack.append(compressedSnap);
    } else {
        while (m_stateStack.size() > m_stackIndex)
            m_stateStack.takeLast();

        m_stateStack.append(compressedSnap);
    }
}

void SceneUndoManager::revertState()
{
    if (availableUndoCount()) {
        QByteArray &compressedSnap = m_stateStack[m_stackIndex];
        QByteArray snap = qUncompress(compressedSnap);
        QDataStream ds(&snap, QIODevice::ReadOnly);
        m_scene->restoreFrom(ds, false);
    }
}

void SceneUndoManager::undo()
{
    if (availableUndoCount()) {
        QByteArray &compressedSnap = m_stateStack[--m_stackIndex];
        QByteArray snap = qUncompress(compressedSnap);
        QDataStream ds(&snap, QIODevice::ReadOnly);
        m_scene->restoreFrom(ds, false);
    }
}

void SceneUndoManager::redo()
{
    if (availableRedoCount()) {
        QByteArray &compressedSnap = m_stateStack[++m_stackIndex];
        QByteArray snap = qUncompress(compressedSnap);
        QDataStream ds(&snap, QIODevice::ReadOnly);
        m_scene->restoreFrom(ds, false);
    }
}

int SceneUndoManager::availableUndoCount() const
{
    return (m_stackIndex > 0);
}

int SceneUndoManager::availableRedoCount() const
{
    return (m_stackIndex >= 0) && (m_stackIndex < m_stateStack.size() - 1);
}
} // namespace BondGraphEditorWindow
} // namespace OpenCOR
