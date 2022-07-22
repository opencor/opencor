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
#include <QMap>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QVariant>

#include "bgeditorsearchdialog.h"
#include "ui_BGEditorSearchDialog.h"

#include <bgconnection.h>
#include <bgelement.h>
#include <bgelementeditorscene.h>
#include <sceneitem.h>

namespace OpenCOR {
namespace BondGraphUI {

static bool look(const QString &where, const QString &what,
                 Qt::CaseSensitivity sens, bool word) {
  if (word)
    return (where.compare(what, sens) == 0);
  else
    return where.contains(what, sens);
}

BGEditorSearchDialog::BGEditorSearchDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::BGEditorSearchDialog) {
  ui->setupUi(this);
  setMinimumSize(750, 550);
  connect(ui->Text, &QLineEdit::textChanged, this,
          &BGEditorSearchDialog::updateButtons);
  connect(ui->NamesScope, &QCheckBox::toggled, this,
          &BGEditorSearchDialog::updateButtons);
  connect(ui->AttrNamesScope, &QCheckBox::toggled, this,
          &BGEditorSearchDialog::updateButtons);
  connect(ui->AttrValuesScope, &QCheckBox::toggled, this,
          &BGEditorSearchDialog::updateButtons);
}

BGEditorSearchDialog::~BGEditorSearchDialog() { delete ui; }

void BGEditorSearchDialog::exec(BGElementEditorScene &scene) {
  m_scene = &scene;

  ui->Text->setFocus();
  ui->Text->selectAll();

  updateButtons();

  show();
}

void BGEditorSearchDialog::updateButtons() {
  bool isOk = false;
  isOk |= ui->NamesScope->isChecked();
  isOk |= ui->AttrNamesScope->isChecked();
  isOk |= ui->AttrValuesScope->isChecked();

  isOk &= !ui->Text->text().isEmpty();

  ui->Find->setEnabled(isOk);
}

void BGEditorSearchDialog::on_Find_clicked() {
  ui->Results->setUpdatesEnabled(false);
  ui->Results->clear();

  auto items =
      ui->EdgesOnly->isChecked() ? m_scene->getItems<SceneItem, BGConnection>()
      : ui->NodesOnly->isChecked() ? m_scene->getItems<SceneItem, BGElement>()
                                   : m_scene->getItems<SceneItem>();

  bool lookNames = ui->NamesScope->isChecked();
  bool lookAttrNames = ui->AttrNamesScope->isChecked();
  bool lookAttrs = ui->AttrValuesScope->isChecked();
  QString text = ui->Text->text();
  Qt::CaseSensitivity sens =
      ui->CaseSense->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
  bool word = ui->WholeWords->isChecked();

  for (const SceneItem *item : items) {
    QString textToShow;

    if (lookNames) {
      // QString id = item->getId();
      QString id = item->getDisplayName();
      if (look(id, text, sens, word)) {
        textToShow = "ID:" + id;
      }
    }

    if (lookAttrNames || lookAttrs) {
      const auto &attrMap = item->getLocalAttributes();
      for (auto it = attrMap.constBegin(); it != attrMap.constEnd(); it++) {
        QString key(it.key());
        QString val(it.value().toString());

        if ((lookAttrNames && look(key, text, sens, word)) ||
            (lookAttrs && look(val, text, sens, word))) {
          if (textToShow.size())
            textToShow += " | ";
          textToShow += key + ": " + val;
        }
      }
    }

    if (textToShow.isEmpty())
      continue;

    QStringList res;
    res << item->typeId() << item->getId() << textToShow;
    auto *ritem = new QTreeWidgetItem(res);

    bool isNode = (dynamic_cast<const BGElement *>(item) != nullptr);
    ritem->setData(0, Qt::UserRole, isNode);

    ui->Results->addTopLevelItem(ritem);
  }

  ui->Results->setUpdatesEnabled(true);
}

void BGEditorSearchDialog::on_Results_itemSelectionChanged() {
  auto ritems = ui->Results->selectedItems();

  QList<SceneItem *> selected;

  for (const auto *ritem : ritems) {
    QString id = ritem->text(1);
    bool isNode = ritem->data(0, Qt::UserRole).toBool();
    if (isNode) {
      auto itemList = m_scene->getItemsById<BGElement>(id);
      if (!itemList.isEmpty())
        selected << itemList.first();
    } else {
      auto itemList = m_scene->getItemsById<BGConnection>(id);
      if (!itemList.isEmpty())
        selected << itemList.first();
    }
  }

  m_scene->selectItems(selected);
  m_scene->ensureSelectionVisible();
}

} // namespace BondGraphUI
} // namespace OpenCOR