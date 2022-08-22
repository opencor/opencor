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
#endif

#include "undomanagerinterface.h"

#include <QtCore/QByteArray>
#include <QtCore/QList>

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGEditorScene;

class SceneUndoManager : public UndoManagerInterface
{
public:
    SceneUndoManager(BGEditorScene &scene);

    // override
    virtual void reset();
    virtual void addState();
    virtual void revertState();
    virtual void undo();
    virtual void redo();
    virtual int availableUndoCount() const;
    virtual int availableRedoCount() const;

private:
    BGEditorScene *m_scene;
    QList<QByteArray> m_stateStack;
    int m_stackIndex;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR


