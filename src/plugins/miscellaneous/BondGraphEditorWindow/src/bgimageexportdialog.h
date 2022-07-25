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
#include <QSettings>

namespace Ui {
class BGImageExportDialog;
}

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGEditorScene;

class BGImageExportDialog : public QDialog {
  Q_OBJECT

public:
  BGImageExportDialog(QWidget *parent = 0);
  ~BGImageExportDialog();

  void doReadSettings(QSettings &settings);
  void doWriteSettings(QSettings &settings);

  void setScene(BGEditorScene &scene);

  bool cutToContent() const;
  int resolution() const;

private Q_SLOTS:
  void updateTargetSize();

private:
  Ui::BGImageExportDialog *ui;

  BGEditorScene *m_scene = nullptr;
  int m_dpi = 300;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR