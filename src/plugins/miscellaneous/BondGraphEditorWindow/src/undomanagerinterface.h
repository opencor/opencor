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
#include <QString>

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif


namespace OpenCOR {
namespace BondGraphEditorWindow {

class UndoManagerInterface
{
public:
    virtual void reset() = 0;
    virtual void addState() = 0;
    virtual void revertState() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual int availableUndoCount() const = 0;
    virtual int availableRedoCount() const = 0;
    virtual void serialiseToFile(QString path) = 0;
    virtual void restoreFromFile(QString path) = 0;
};
} // namespace BondGraphEditorWindow
} // namespace OpenCOR