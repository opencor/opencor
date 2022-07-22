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

#include "undomanagerinterface.h"

#include <QtCore/QByteArray>
#include <QtCore/QList>

namespace OpenCOR {
namespace BondGraphUI {

class BGEditorScene;

class BGDiffUndoManager : public UndoManagerInterface {
public:
  BGDiffUndoManager(BGEditorScene &scene);

  // override
  virtual void reset();
  virtual void addState();
  virtual void revertState();
  virtual void undo();
  virtual void redo();
  virtual int availableUndoCount() const;
  virtual int availableRedoCount() const;
  virtual void serialiseToFile(QString path);
  virtual void restoreFromFile(QString path);

private:
  struct Command {
    int index;
    int sizeToReplace;
    QByteArray data;
  };

  BGEditorScene *m_scene;
  QList<Command> m_redoStack, m_undoStack;
  QList<Command> m_redoStackTemp, m_undoStackTemp;
  QByteArray m_lastState;
  bool m_updated;
};

} // namespace BondGraphUI
} // namespace OpenCOR