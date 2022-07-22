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
#include "bgimageexportdialog.h"
#include "bgeditorscene.h"
#include "ui_BGImageExportDialog.h"

namespace OpenCOR {

namespace BondGraphUI {

BGImageExportDialog::BGImageExportDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::BGImageExportDialog) {
  ui->setupUi(this);
  setMinimumSize(300, 200);
  QImage temp(QSize(100, 100), QImage::Format_ARGB32);
  m_dpi = temp.physicalDpiX();
  if (m_dpi > 0)
    ui->Resolution->setCurrentText(QString::number(m_dpi));
  else
    m_dpi = 300; // default

  connect(ui->Resolution, &QComboBox::currentTextChanged, this,
          &BGImageExportDialog::updateTargetSize);
  connect(ui->CutToContent, &QCheckBox::stateChanged, this,
          &BGImageExportDialog::updateTargetSize);
}

BGImageExportDialog::~BGImageExportDialog() {}

void BGImageExportDialog::doReadSettings(QSettings &settings) {
  settings.beginGroup("BondGraphUI/ImageExport");
  ui->Resolution->setCurrentText(
      settings.value("DPI", ui->Resolution->currentText()).toString());
  ui->CutToContent->setChecked(
      settings.value("CutContent", ui->CutToContent->isChecked()).toBool());
  settings.endGroup();
}

void BGImageExportDialog::doWriteSettings(QSettings &settings) {
  settings.beginGroup("BondGraphUI/ImageExport");
  settings.setValue("DPI", ui->Resolution->currentText());
  settings.setValue("CutContent", ui->CutToContent->isChecked());
  settings.endGroup();
}

void BGImageExportDialog::setScene(BGEditorScene &scene) {
  m_scene = &scene;

  updateTargetSize();
}

void BGImageExportDialog::updateTargetSize() {
  if (!m_scene)
    return;

  QSize size = m_scene->sceneRect().size().toSize();
  if (cutToContent())
    size =
        m_scene->itemsBoundingRect().adjusted(-20, -20, 20, 20).size().toSize();

  int res = resolution();
  if (res <= 0)
    res = m_dpi;

  double coeff = (double)res / (double)m_dpi;
  QSize newSize = size * coeff;
  ui->ImageSize->setText(
      QString("%1 x %2").arg(newSize.width()).arg(newSize.height()));
}

bool BGImageExportDialog::cutToContent() const {
  return ui->CutToContent->isChecked();
}

int BGImageExportDialog::resolution() const {
  int res = ui->Resolution->currentText().toUInt();
  return res;
}
} // namespace BondGraphUI
} // namespace OpenCOR