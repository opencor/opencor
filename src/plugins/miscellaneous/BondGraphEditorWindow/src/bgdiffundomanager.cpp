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
#include "bgdiffundomanager.h"
#include "bgeditorscene.h"
#include <QDataStream>

#include <QFile>
#include <QMessageBox>
#include <QSettings>

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGDiffUndoManager::BGDiffUndoManager(BGEditorScene &scene) :
    m_scene(&scene), m_updated(false)
{
}

void BGDiffUndoManager::reset()
{
    m_redoStack.clear();
    m_undoStack.clear();
    m_redoStackTemp.clear();
    m_undoStackTemp.clear();
    m_lastState.clear();
    m_updated = false;
}

void BGDiffUndoManager::addState()
{
    m_updated = true;
    // drop temp stacks
    m_redoStack.clear();
    m_undoStackTemp.clear();

    // serialize & compress
    QByteArray snap;
    QDataStream ds(&snap, QIODevice::WriteOnly);
    m_scene->storeTo(ds, true);

    // check if 1st store
    if (m_lastState.isEmpty() && m_undoStack.isEmpty() && m_redoStack.isEmpty()) {
        m_lastState = snap;
        return;
    }

    // push states into stacks
    int leftDiffIndex = 0;
    int len = qMin(snap.size(), m_lastState.size());
    while (leftDiffIndex < len && snap[leftDiffIndex] == m_lastState[leftDiffIndex])
        ++leftDiffIndex;

    int rightDiffIndex1 = m_lastState.size() - 1;
    int rightDiffIndex2 = snap.size() - 1;
    while (rightDiffIndex1 >= 0 && rightDiffIndex2 >= 0 && rightDiffIndex1 > leftDiffIndex && rightDiffIndex2 > leftDiffIndex && snap[rightDiffIndex2] == m_lastState[rightDiffIndex1]){
        --rightDiffIndex1;
        --rightDiffIndex2;
    }

    int len1 = rightDiffIndex1 - leftDiffIndex + 1;
    int len2 = rightDiffIndex2 - leftDiffIndex + 1;

    Command cUndo = { leftDiffIndex, len2,
                      qCompress(m_lastState.mid(leftDiffIndex, len1)) };
    Command cRedo = { leftDiffIndex, len1,
                      qCompress(snap.mid(leftDiffIndex, len2)) };

    m_undoStack << cUndo;
    m_redoStackTemp << cRedo;

    // write last state
    m_lastState = snap;
}

void BGDiffUndoManager::serialiseToFile(QString path)
{
    if (!m_updated)
        return;
    QFile file(path);
    QSettings bgSettings;
    bgSettings.beginGroup("BondGraphEditorWindow/Bondgraph/ProjectDetails");

    auto pn = bgSettings.value("Project_Name", "").toString();
    auto pa = bgSettings.value("Project_Authors", "").toString();
    auto pd = bgSettings.value("Project_Description", "").toString();
    auto projectdir = bgSettings.value("Project_Directory", "").toString();

    bgSettings.endGroup();
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);

        // Store project details if any
        QByteArray prov;
        QDataStream pp(&prov, QIODevice::WriteOnly);
        pp << pn;
        pp << pa;
        pp << pd;
        pp << projectdir;

        out << prov.size();
        out.writeRawData(prov.data(), prov.size());
        QByteArray snap;
        QDataStream ds(&snap, QIODevice::WriteOnly);
        m_scene->storeTo(ds, true);
        out << snap.size();
        out.writeRawData(snap.data(), snap.size());

        file.close();

        m_updated = false;
    }
}

void BGDiffUndoManager::restoreFromFile(QString path)
{
    QFile file(path);
    QString pn, pa, pd, projectdir;
    if (file.open(QIODevice::ReadOnly)) {
        reset();
        int qSize = 0;
        QDataStream in(&file);

        // Restore project details if any
        QByteArray prov;
        in >> qSize;
        prov.resize(qSize);
        in.readRawData(prov.data(), qSize);
        QDataStream pp(&prov, QIODevice::ReadOnly);
        pp >> pn;
        pp >> pa;
        pp >> pd;
        pp >> projectdir;

        in >> qSize;

        QByteArray snap;
        snap.resize(qSize);
        in.readRawData(snap.data(), qSize);
        QDataStream ds(&snap, QIODevice::ReadOnly);
        try {
            m_scene->restoreFrom(ds, true);
        } catch (...) {
            QMessageBox::critical(
                nullptr, "Load failed",
                "Failed to load backup, it may have been corrupted.");
            m_scene->clear();
        }

        file.close();

        QSettings bgSettings;
        bgSettings.beginGroup("BondGraphEditorWindow/Bondgraph/ProjectDetails");

        bgSettings.setValue("Project_Name", pn);
        bgSettings.setValue("Project_Authors", pa);
        bgSettings.setValue("Project_Description", pd);
        bgSettings.setValue("Project_Directory", projectdir);

        bgSettings.endGroup();
    }
    m_updated = false;
}

void BGDiffUndoManager::revertState()
{
    m_updated = true;
    QDataStream ds(&m_lastState, QIODevice::ReadOnly);
    m_scene->restoreFrom(ds, true);
}

void BGDiffUndoManager::undo()
{
    if (availableUndoCount()) {
        Command cUndo = m_undoStack.takeLast();
        m_lastState.replace(cUndo.index, cUndo.sizeToReplace,
                            qUncompress(cUndo.data));
        QDataStream ds(&m_lastState, QIODevice::ReadOnly);
        m_scene->restoreFrom(ds, true);

        m_redoStack << m_redoStackTemp.takeLast();
        m_undoStackTemp << cUndo;
        m_updated = true;
    }
}

void BGDiffUndoManager::redo()
{
    if (availableRedoCount()) {
        Command cRedo = m_redoStack.takeLast();
        m_lastState.replace(cRedo.index, cRedo.sizeToReplace,
                            qUncompress(cRedo.data));
        QDataStream ds(&m_lastState, QIODevice::ReadOnly);
        m_scene->restoreFrom(ds, true);

        m_undoStack << m_undoStackTemp.takeLast();
        m_redoStackTemp << cRedo;
        m_updated = true;
    }
}

int BGDiffUndoManager::availableUndoCount() const
{
    return m_undoStack.size();
}

int BGDiffUndoManager::availableRedoCount() const
{
    return m_redoStack.size();
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR



