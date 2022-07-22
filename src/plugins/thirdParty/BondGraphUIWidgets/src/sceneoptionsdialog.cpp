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
#include <QPixmapCache>

#include "sceneoptionsdialog.h"
#include "ui_SceneOptionsDialog.h"

namespace OpenCOR {
namespace BondGraphUI {

SceneOptionsDialog::SceneOptionsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::SceneOptionsDialog) {
  ui->setupUi(this);

  ui->BackgroundColor->setColorScheme(QSint::OpenOfficeColors());
  ui->GridColor->setColorScheme(QSint::OpenOfficeColors());
}

SceneOptionsDialog::~SceneOptionsDialog() { delete ui; }

int SceneOptionsDialog::exec(BGEditorScene &scene, BGEditorView &view,
                             OptionsData &data_) {
  ui->BackgroundColor->setColor(scene.backgroundBrush().color());

  QPen gridPen = scene.getGridPen();
  ui->GridColor->setColor(gridPen.color());

  ui->GridSize->setValue(scene.getGridSize());
  ui->GridVisible->setChecked(scene.gridEnabled());
  ui->GridSnap->setChecked(scene.gridSnapEnabled());

  ui->Antialiasing->setChecked(
      view.renderHints().testFlag(QPainter::Antialiasing));

  ui->CacheSlider->setValue(QPixmapCache::cacheLimit() / 1024);

  ui->CacheSlider->setMaximum((int)128);
  ui->CacheSlider->setUnitText(tr("MB"));

  ui->EnableBackups->setChecked(data_.backupPeriod > 0);
  ui->BackupPeriod->setValue(data_.backupPeriod);

#ifdef USE_GVGRAPH
  ui->ExtraSection->setVisible(true);
  ui->GraphvizPath->setObjectsToPick(QSint::PathPicker::PF_EXISTING_DIR);
  ui->GraphvizPath->setCurrentPath(data_.graphvizPath);
  ui->GraphvizDefaultEngine->setCurrentText(data_.graphvizDefaultEngine);
#else
  ui->ExtraSection->setVisible(false);
#endif

  if (QDialog::exec() == QDialog::Rejected)
    return QDialog::Rejected;

  scene.setBackgroundBrush(ui->BackgroundColor->color());

  gridPen.setColor(ui->GridColor->color());
  scene.setGridPen(gridPen);

  scene.setGridSize(ui->GridSize->value());
  scene.enableGrid(ui->GridVisible->isChecked());
  scene.enableGridSnap(ui->GridSnap->isChecked());

  bool isAA = ui->Antialiasing->isChecked();
  view.setRenderHint(QPainter::Antialiasing, isAA);
  scene.setFontAntialiased(isAA);

  QPixmapCache::setCacheLimit(ui->CacheSlider->value() * 1024);

  data_.backupPeriod =
      ui->EnableBackups->isChecked() ? ui->BackupPeriod->value() : 0;

#ifdef USE_GVGRAPH
  data_.graphvizPath = ui->GraphvizPath->currentPath();
  data_.graphvizDefaultEngine = ui->GraphvizDefaultEngine->currentText();
#endif

  return QDialog::Accepted;
}

void SceneOptionsDialog::on_GraphvizTest_clicked() {
  Q_EMIT testGraphviz(ui->GraphvizPath->currentPath());
}

} // namespace BondGraphUI
} // namespace OpenCOR