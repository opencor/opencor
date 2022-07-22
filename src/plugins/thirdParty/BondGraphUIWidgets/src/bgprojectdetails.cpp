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
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QSize>

#include "ui_BGProjectDetails.h"

#include "bgprojectdetails.h"

namespace OpenCOR {
namespace BondGraphUI {

BGProjectDetails::BGProjectDetails(bool allowBrowse, QWidget *parent)
    : QDialog(parent), ui(new Ui::BGProjectDetails) {
  ui->setupUi(this);
  setMinimumSize(600, 400);
  if (!allowBrowse) {
    ui->browse->setVisible(false);
    ui->srcDirectory->setEnabled(false);
  } else {
    ui->srcDirectory->setEnabled(true);
  }

  QSettings settings;
  settings.beginGroup("BondGraphUI/Bondgraph/ProjectDetails");
  auto pn = settings.value("Project_Name", ui->projectName->text()).toString();
  auto pa = settings.value("Project_Authors", ui->authors->text()).toString();
  auto pd =
      settings.value("Project_Description", ui->description->toPlainText())
          .toString();
  auto ps =
      settings.value("Project_Directory", ui->srcDirectory->text()).toString();
  settings.endGroup();

  ui->projectName->setText(pn);
  ui->authors->setText(pa);
  ui->description->setPlainText(pd);
  ui->srcDirectory->setText(ps);

  connect(ui->applyChange, &QPushButton::clicked, this,
          &BGProjectDetails::on_Apply);
  connect(ui->cancelChange, &QPushButton::clicked, this,
          &BGProjectDetails::on_Cancel);
  connect(ui->browse, &QPushButton::clicked, this,
          &BGProjectDetails::on_Browse);
}

BGProjectDetails::~BGProjectDetails() { delete ui; }

nlohmann::json BGProjectDetails::getJson() {
  QSettings settings;

  settings.beginGroup("BondGraphUI/Bondgraph/ProjectDetails");

  auto pn = settings.value("Project_Name", ui->projectName->text()).toString();
  auto pa = settings.value("Project_Authors", ui->authors->text()).toString();
  auto pd =
      settings.value("Project_Description", ui->description->toPlainText())
          .toString();
  auto ps =
      settings.value("Project_Directory", ui->srcDirectory->text()).toString();
  settings.endGroup();

  nlohmann::json js;
  js["projectname"] = pn.toStdString();
  js["authors"] = pa.toStdString();
  js["description"] = pd.toStdString();
  js["directory"] = ps.toStdString();
  return js;
}

void BGProjectDetails::on_Browse() {
  const QString DEFAULT_DIR_KEY("default_load_dir");
  QSettings bgSettings;
  QString dirName = QFileDialog::getExistingDirectory(
      nullptr, tr("Save Directory"),
      bgSettings.value(DEFAULT_DIR_KEY).toString(),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!dirName.isEmpty())
    ui->srcDirectory->setText(dirName);
}

void BGProjectDetails::on_Apply() {
  QSettings settings;

  settings.beginGroup("BondGraphUI/Bondgraph/ProjectDetails");

  settings.setValue("Project_Name", ui->projectName->text());
  settings.setValue("Project_Authors", ui->authors->text());
  settings.setValue("Project_Description", ui->description->toPlainText());
  settings.setValue("Project_Directory", ui->srcDirectory->text());
  settings.endGroup();

  QDialog::accept();
}

void BGProjectDetails::on_Cancel() { QDialog::reject(); }

} // namespace BondGraphUI
} // namespace OpenCOR