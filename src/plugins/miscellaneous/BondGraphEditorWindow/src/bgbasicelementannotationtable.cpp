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
#include "bgelementeditorscene.h"

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

#include "bgbasicelementannotationtable.h"
#include "ui_BGBasicElementAnnotationTable.h"

#include <bgeditorsceneconstants.h>
#include <bgelement.h>
#include <bgelementeditorscene.h>
#include <bgitemattribute.h>

#include "bgelementproxymapdialog.h"
#include "bgjsonmodels.h"
#include "bgparameterdelegate.h"

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGBasicElementAnnotationTable::BGBasicElementAnnotationTable(
    QWidget *parent, BGElementEditorScene *scene)
    : QWidget(parent), m_scene(scene), m_updateLock(false),
      m_currentElement(nullptr), ui(new Ui::BGBasicElementAnnotationTable) {
  ui->setupUi(this);
  QJsonTableModel::Header header;
  // Load header data, also specify the datatype associated with the column for
  // loading the correct delegate editor
  header.push_back(QJsonTableModel::Heading(
      {{"title", "Parameter"}, {"index", "name"}, {"datatype", "Anything"}}));
  header.push_back(QJsonTableModel::Heading(
      {{"title", "Value"}, {"index", "value"}, {"datatype", "FileName"}}));
  header.push_back(QJsonTableModel::Heading(
      {{"title", "Units"}, {"index", "dimension"}, {"datatype", "Unit"}}));

  m_parameterModel = new QJsonTableModel(header, this);

  ui->parameters->verticalHeader()->hide();
  ui->parameters->setStyleSheet("QHeaderView::section { background:lightgray; "
                                "color:black;min-height:2em;}");
  ui->parameters->setModel(m_parameterModel);
  ui->parameters->setEditTriggers(QAbstractItemView::DoubleClicked |
                                  QAbstractItemView::SelectedClicked |
                                  QAbstractItemView::EditKeyPressed);
  m_parameterDelegate = new ParameterDelegate(nullptr, m_scene);
  ui->parameters->setItemDelegate(m_parameterDelegate);

  // ui->parameters->resizeColumnsToContents();
  ui->parameters->setColumnWidth(0, 100);
  ui->parameters->setColumnWidth(1, 120);
  ui->parameters->setColumnWidth(2, 80);
  ui->parameters->horizontalHeader()->setSectionResizeMode(
      1, QHeaderView::Stretch);

  // Hide group box borders
  ui->proxyGB->setStyleSheet("#proxyGB {border:0;}");
  ui->elementsGB->setStyleSheet("#elementsGB {border:0;}");

  connect(ui->latexform, &QLineEdit::textChanged, this,
          &BGBasicElementAnnotationTable::updateLatex, Qt::QueuedConnection);

  connect(
      m_parameterDelegate, &ParameterDelegate::invalidUserInput, this,
      [this](const QString &st) {
        QMessageBox::critical(nullptr, tr("Inconsistent SI Units"), st);
      },
      Qt::QueuedConnection);

  connect(m_parameterDelegate, &ParameterDelegate::closeEditor, this,
          [this]() { flush(); });

  connect(ui->proxySetup, &QPushButton::clicked, this,
          &BGBasicElementAnnotationTable::setElementAsProxy);
  if (m_scene != nullptr)
    setScene(m_scene);
}

BGBasicElementAnnotationTable::~BGBasicElementAnnotationTable() {
  delete m_parameterDelegate;
  delete ui;
}

void BGBasicElementAnnotationTable::updateLatex(const QString &text) {
  if (m_currentElement != nullptr && !m_updateLock) {
    // Check of the name has changed, this slot is called when ever the
    // textChanged event occurs textChange event also occurs when ever a element
    // is selected - but the name has not changed this improves performance for
    // large BGs
    if (text != m_currentElement->getDisplayName()) {
      m_updateLock = true;
      m_currentElement->setDisplayName(text);
      ui->latexName->setPixmap(m_currentElement->getIcon());
      Q_EMIT elementUpdated(m_currentElement);
      m_updateLock = false;
    }
  }
}

void BGBasicElementAnnotationTable::setElementAsProxy() {
  if (m_currentElement != nullptr) {
    // Load bg file and select for element
    BGElementProxyMapDialog dialog;
    auto &definition = m_currentElement->getJson();

    auto eType = definition["ElementDefinition"]["name"];
    if (dialog.exec(QString::fromStdString(eType), *m_scene) ==
        QDialog::Accepted) {
      definition["proxy"] = dialog.getDefinition();
      m_currentElement->setJson(
          definition); // This updates the annotation_cache, captured by
                       // store/restore
      m_currentElement->setAsProxy();
      ui->proxySetup->setVisible(false);
      ui->proxyLabel->setVisible(true);
      ui->bgsourcefile->setVisible(true);
      ui->bgsourcefileLabel->setVisible(true);
      ui->bgsourcefile->setText(
          QString::fromStdString(definition["proxy"]["uri"]));
    }
  }
}

void BGBasicElementAnnotationTable::doReadSettings(QSettings &settings) {
  Q_UNUSED(settings);
}

void BGBasicElementAnnotationTable::doWriteSettings(QSettings &settings) {
  Q_UNUSED(settings);
}

void BGBasicElementAnnotationTable::setScene(BGElementEditorScene *scene) {
  if (m_scene)
    onSceneDetached(m_scene);

  m_scene = scene;

  setEnabled(m_scene);

  if (m_scene)
    onSceneAttached(m_scene);
}

void BGBasicElementAnnotationTable::updateFromScene(BGEditorScene *) {}

void BGBasicElementAnnotationTable::onSceneAttached(BGEditorScene *scene) {
  // factories for new items
  // m_currentElement = m_scene->getElementFactory();

  connect(scene, &BGEditorScene::sceneChanged, this,
          &BGBasicElementAnnotationTable::onSceneChanged, Qt::QueuedConnection);
  connect(scene, &BGEditorScene::selectionChanged, this,
          &BGBasicElementAnnotationTable::onSelectionChanged,
          Qt::QueuedConnection);
  connect(scene, &BGEditorScene::itemDeleted, this,
          &BGBasicElementAnnotationTable::itemDeleted, Qt::DirectConnection);
  // default attrs
  updateFromScene(scene);

  onSceneChanged();
}

void BGBasicElementAnnotationTable::onSceneDetached(BGEditorScene *scene) {
  scene->disconnect(this);
}

void BGBasicElementAnnotationTable::onSceneChanged() {
  // update active selections if any
  onSelectionChanged();
}

void BGBasicElementAnnotationTable::itemDeleted(QGraphicsItem *item) {
  if (m_currentElement != nullptr) {
    QGraphicsItem *m_itm = dynamic_cast<QGraphicsItem *>(m_currentElement);
    if (m_itm == item) {
      m_currentElement = nullptr;
    }
  }
}

void BGBasicElementAnnotationTable::flush() {
  if (m_currentElement != nullptr &&
      m_currentElement->getClassType() != "userdefined") {
    auto j1 = m_currentElement->getStateParameterJson();
    auto j2 = m_parameterModel->json();

    if (j1 != j2) {
      m_currentElement->setStateParameterJson(j2);
      m_scene->addUndoState();
    }
  }
  m_currentElement = nullptr;
}

void BGBasicElementAnnotationTable::onSelectionChanged() {
  if (m_updateLock || m_scene == nullptr)
    return;

  m_updateLock = true;
  blockSignals(true);
  QList<BGElement *> elements = m_scene->getSelectedElements();

  // Update current element

  if (m_currentElement != nullptr) {
    // Check if the current element is still in the scene
    if (elements.size() == 1) {
      BGElement *elem = elements.first();
      if (elem == m_currentElement) {
        // If same element then return
        m_updateLock = false;
        blockSignals(false);
        return;
      }
    }

    if (m_currentElement->getClassType() != "userdefined" &&
        m_currentElement->getClassType() != "annotation") {
      auto j1 = m_currentElement->getStateParameterJson();
      auto j2 = m_parameterModel->json();

      if (j1 != j2) {
        m_currentElement->setStateParameterJson(j2);
        m_scene->addUndoState();
      }
    }
  }

  m_currentElement = nullptr;
  ui->importedParameterBox->setVisible(false);
  ui->proxySetup->setVisible(false);
  ui->proxyLabel->setVisible(false);
  ui->bgsourcefile->setVisible(false);
  ui->bgsourcefileLabel->setVisible(false);

  // Set the term
  if (elements.size() == 1) {
    BGElement *elem = elements.first();
    if (elem->getId() == "ConnectHelp") {
      blockSignals(false);
      // allow updates
      m_updateLock = false;
      return;
    }
    // Check if the element can be a proxy
    if (elem->isProxy()) {
      ui->proxyGB->setVisible(true);
      ui->proxyLabel->setVisible(true);
      ui->proxySetup->setVisible(false);
    } else if (elem->canBeProxy()) {
      ui->proxyGB->setVisible(true);
      ui->proxyLabel->setVisible(false);
      ui->proxySetup->setVisible(true);
      ui->bgsourcefileLabel->setVisible(false);
      ui->bgsourcefile->setVisible(false);
    } else {
      ui->proxyGB->setVisible(false);
    }

    ui->elementsGB->setVisible(false);
    ui->elementIcon->setVisible(false);
    ui->iconLabel->setVisible(false);
    ui->parametersGB->setVisible(false);
    ui->constitutiveEqGB->setVisible(false);
    ui->elementSelection->setVisible(false);
    // Check class type. Handle differently for regions
    if (elem->getClassType() != "annotation") {
      m_currentElement = elem;
      m_parameterDelegate->setElementName(elem->getId());
      m_parameterDelegate->setDimensionAsMutable(
          elem->getJson()["ElementDefinition"]["domain"] == "Generic");
      ui->latexName->setPixmap(elem->getIcon());
      ui->latexform->setText(elem->getDisplayName());
      if (elem->getClassType() != "userdefined") {
        m_parameterModel->setJson(elem->getStateParameterJson());
        // qDebug()<<elem->getId()<<"\n"<<QString::fromStdString(elem->getJson().dump(2));
        // qDebug()<<elem->getId()<<"\n"<<QString::fromStdString(elem->getStateParameterJson().dump(2));
        QString cr = elem->getConstiutiveRelation();
        if (m_parameterModel->rowCount() > 0) {
          ui->parametersGB->setVisible(true);
        }
        if (cr.length() > 0) {
          ui->constitutiveEqGB->setVisible(true);
          ui->constitutiveEq->setText(cr);
        } else {
          ui->constitutiveEqGB->setVisible(false);
        }

        ui->elementsGB->setVisible(true);
        ui->latexName->setVisible(true);
        ui->elementSelection->setVisible(false);
      } else {
        auto &uds = elem->getJson();
        ui->elementSelection->setText(tr("Map not defined"));
        if (uds.contains("mapdefinition")) {
          if (uds["mapdefinition"]["type"] == "cellml") {
            ui->importedParameterBox->setVisible(true);
            ui->elementsGB->setVisible(true);
            ui->latexName->setVisible(true);
            ui->elementSelection->setVisible(false);
            ui->sourcefilename->setText(
                QString::fromStdString(uds["mapdefinition"]["uri"]));
            ui->importedParameterUnit->setText(
                QString::fromStdString(uds["mapdefinition"]["dimension"]));
            ui->importedParameter->setText(
                QString::fromStdString(uds["mapdefinition"]["variable"]));
          }
          if (uds["mapdefinition"].contains("shown")) {
            ui->proxyGB->setVisible(false);
          } else {
            uds["mapdefinition"]["shown"] = true;
          }
        } else {
          ui->elementSelection->setVisible(true);
          ui->proxyGB->setVisible(false);
        }
      }
    } else {
      m_currentElement = elem;
      ui->elementsGB->setVisible(false);
      ui->elementIcon->setVisible(false);
      ui->iconLabel->setVisible(false);
      ui->parametersGB->setVisible(false);
      ui->constitutiveEqGB->setVisible(false);
      ui->latexName->setVisible(false);
      ui->elementSelection->setVisible(true);
      ui->proxySetup->setVisible(false);
      ui->proxyLabel->setVisible(false);
      ui->elementSelection->setText(tr("Use Annotation View to update notes"));
    }
  } else {
    ui->elementsGB->setVisible(false);
    ui->latexName->setVisible(false);
    ui->elementIcon->setVisible(false);
    ui->iconLabel->setVisible(false);
    ui->parametersGB->setVisible(false);
    ui->constitutiveEqGB->setVisible(false);
    ui->elementSelection->setVisible(true);
    if (elements.size() == 0)
      ui->elementSelection->setText(tr("No Element Selected"));
    else
      ui->elementSelection->setText(tr("Multiple Elements Selected"));
  }
  // Check if a connection is selected
  QList<BGConnection *> edges = m_scene->getSelectedConnections();
  if (edges.count() > 0 && elements.size() == 0) {
    ui->elementSelection->setText(tr("See connections panel"));
  }

  blockSignals(false);
  // allow updates
  m_updateLock = false;
}

void BGBasicElementAnnotationTable::setElementAttribute(
    const QByteArray &attrId, const QVariant &v) {
  Q_UNUSED(attrId);
  Q_UNUSED(v);
  // if (m_currentElement)
  //	m_currentElement->setAttribute(attrId, v);

  if (m_updateLock || m_scene == nullptr)
    return;

  QList<BGElement *> elements = m_scene->getSelectedElements();

  if (elements.isEmpty())
    return;

  // set change to json

  m_scene->addUndoState();
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR