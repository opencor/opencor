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

namespace Ui {
class BGEditorSearchDialog;
}

namespace OpenCOR {
namespace BondGraphUI {

class BGElementEditorScene;

class BGEditorSearchDialog : public QDialog {
  Q_OBJECT

public:
  explicit BGEditorSearchDialog(QWidget *parent = 0);
  ~BGEditorSearchDialog();

public Q_SLOTS:
  void exec(BGElementEditorScene &scene);

private Q_SLOTS:
  void updateButtons();
  void on_Find_clicked();
  void on_Results_itemSelectionChanged();

private:
  Ui::BGEditorSearchDialog *ui;

  BGElementEditorScene *m_scene = 0;
};

} // namespace BondGraphUI
} // namespace OpenCOR