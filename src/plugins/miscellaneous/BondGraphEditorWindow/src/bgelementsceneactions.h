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

#include "bgeditorsceneactions.h"
namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGElementEditorScene;
class BGElement;
class BGConnection;

class BGElementSceneActions : public BGEditorSceneActions {
  Q_OBJECT

public:
  BGElementSceneActions(BGElementEditorScene *scene);

public Q_SLOTS:
  bool editElementId(BGElement *);
  bool editConnectionId(BGConnection *);

  void onActionElementColour();
  void onActionLink();
  void onActionUnlink();

  // void onActionConnectionColour();
  void onActionConnectionReverse();
  // void onActionConnectionDirected();
  // void onActionConnectionMutual();
  // void onActionConnectionUndirected();

private:
  BGElementEditorScene &bgElementScene;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR