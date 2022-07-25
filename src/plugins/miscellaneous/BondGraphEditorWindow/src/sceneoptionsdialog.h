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

#include <QDialog>

#include <bgeditorscene.h>
#include <bgeditorview.h>

// compiled from UI file
namespace Ui {
class SceneOptionsDialog;
}

namespace OpenCOR {
namespace BondGraphEditorWindow {

struct OptionsData {
  int backupPeriod = 10;

  QString graphvizPath;
  // QStringList graphvizEngines;
  QString graphvizDefaultEngine;
};

class SceneOptionsDialog : public QDialog {
  Q_OBJECT

public:
  explicit SceneOptionsDialog(QWidget *parent = 0);
  ~SceneOptionsDialog();

public Q_SLOTS:
  virtual int exec(BGEditorScene &scene, BGEditorView &view, OptionsData &data);

Q_SIGNALS:
  void testGraphviz(const QString &graphvizPath);

private Q_SLOTS:
  void on_GraphvizTest_clicked();

private:
  Ui::SceneOptionsDialog *ui;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR