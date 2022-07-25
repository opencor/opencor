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

#include <QList>
#include <QMap>
#include <QSettings>
#include <QWidget>

#include "ui_BGConnectionsTable.h"

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGEditorScene;
class BGElementEditorScene;
struct BGItemAttribute;
class BGConnection;

class BGConnectionsTable : public QWidget {
  Q_OBJECT

public:
  BGConnectionsTable(QWidget *parent = 0);
  ~BGConnectionsTable();

  void setScene(BGElementEditorScene *scene);
  void updateContents();
  void doReadSettings(QSettings &settings);
  void doWriteSettings(QSettings &settings);

protected:
  void connectSignals(BGEditorScene *scene);
  void onSceneAttached(BGEditorScene *scene);
  void onSceneDetached(BGEditorScene *scene);

protected Q_SLOTS:
  void onSceneChanged();
  void onSelectionChanged();
  void on_Table_itemSelectionChanged();
  void on_Table_itemDoubleClicked(QTreeWidgetItem *item, int column);
  void connectionNameChangeRequest(QTreeWidgetItem *item, int column);

private:
  Ui::BGConnectionsTable ui;

  BGElementEditorScene *m_scene;

  QMap<BGConnection *, QTreeWidgetItem *> m_edgeItemMap;

  QByteArrayList m_extraSectionIds;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
