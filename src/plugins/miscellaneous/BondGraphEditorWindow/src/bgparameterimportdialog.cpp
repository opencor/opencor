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
#include "bgparameterimportdialog.h"
#include "ui_BGParameterImport.h"
#include "utils.h"

#include <QComboBox>

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

#include "bgjsonmodels.h"

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnewline-eof"
    #pragma clang diagnostic ignored "-Wold-style-cast"
    #pragma clang diagnostic ignored "-Wreserved-id-macro"
    #pragma clang diagnostic ignored "-Wdeprecated-dynamic-exception-spec"
#endif
#include "bondgraph.hpp"

#if defined(__clang__)
    #pragma clang diagnostic pop
#endif


#include "utils.h"
#include <bgelement.h>

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGParameterImport::BGParameterImport(const nlohmann::json &result,
                                     BGElementEditorScene *scene,
                                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BGParameterImport), m_scene(scene), m_dimension("NONE"),
    m_timeVariable(""), m_elementName(""), m_isState(false)
{
    ui->setupUi(this);
    setMinimumSize(500, 250);
    QString returnChar = QChar(0x23CE);
    m_cellVariableModel =
        new QJsonTreeModel(this, "Component", returnChar + " Variable/Unit");
    m_dataProxy = new Utils::CellMLVariableFilter(this);
    m_dataProxy->setSourceModel(m_cellVariableModel);
    m_dataProxy->setFilterKeyColumn(1);
    m_dataProxy->setRecursiveFilteringEnabled(true);
    ui->cellMLVariablesView->setModel(m_dataProxy);
    ui->findCellMLVariable->setVisible(false);
    ui->findLabel->setVisible(false);

    if (result.contains("file")) {
        QString fname = QString::fromStdString(result["file"]);
        ui->cellmlfileuri->setText(fname);
        if (result.contains("dimension")) {
            constrainDimension(QString::fromStdString(result["dimension"]));
        }
        loadCellML(fname);
        if (result.contains("value")) {
            if (result["value"].is_number()) {
                ui->variableNameMap->setText(
                    QString::number(result["value"].get<double>()));
            } else {
                ui->variableNameMap->setText(QString::fromStdString(result["value"]));
            }
        }
        if (result["link"]) {
            int index = ui->existingCellMLInstances->findText(
                QString::fromStdString(result["element"]));
            ui->existingCellMLInstances->setCurrentIndex(index);
        } else {
            ui->existingCellMLInstances->hide();
            ui->existingCellMLInstancesLabel->hide();
        }
        m_record = result;
    }

    connect(ui->loadButton, &QPushButton::clicked, this,
            &BGParameterImport::loadFile);
    connect(ui->applyButton, &QPushButton::clicked, this,
            &BGParameterImport::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this,
            &BGParameterImport::reject);
    connect(ui->findCellMLVariable, &QLineEdit::textChanged, this,
            &BGParameterImport::searchForCellMLVariable);
    connect(ui->cellMLVariablesView, &QTreeView::clicked, this,
            &BGParameterImport::variableSelectionChanged);
}

BGParameterImport::~BGParameterImport()
{
    delete ui;
}

void BGParameterImport::on_existingCellMLInstances_currentIndexChanged(
    int index)
{
    auto linkItem = ui->existingCellMLInstances->itemData(index);
    if (linkItem.toString() == "-" || linkItem.toString() == "") {
        m_record["link"] = false;
        if (m_record.contains("element"))
            m_record.erase("element");
    } else {
        m_record["link"] = true;
        m_record["element"] = linkItem.toString().toStdString();
    }
}

void BGParameterImport::loadCellML(QString cellmlfile)
{
    m_timeVariable = "";
    auto vars = Utils::getCellMLOutVariables(cellmlfile, &m_timeVariable);
    m_sharedElems.clear();

    auto items = m_scene->getItems<BGElement>();
    QMap<QString, BGElement *> udlist;
    std::string fname = cellmlfile.toStdString();
    ui->existingCellMLInstances->clear();
    ui->existingCellMLInstances->addItem("-", "");

    for (auto *elem : items) {
        if (BGElement *bge = dynamic_cast<BGElement *>(elem)) {
            if (bge->getClassType() != "annotation") {
                auto &uds = bge->getJson();
                if (uds.contains("mapdefinition")) { // For userdefined
                    if (fname.compare(uds["mapdefinition"]["uri"]) == 0) {
                        // Check and remove current
                        if (m_elementName != bge->getId()) {
                            m_sharedElems[bge->getId()] = bge;
                            if (!uds["mapdefinition"]["link"]) {
                                ui->existingCellMLInstances->addItem(bge->getDisplayName(),
                                                                     bge->getId());
                            }
                        }
                    }
                } else {
                    auto &stp = uds["statesAndParameters"];
                    for (auto &c : stp) {
                        auto &val = c["value"];
                        if (val.is_object()) {
                            if (val.contains("file") && fname.compare(val["file"]) == 0) {
                                // Check and remove current
                                if (m_elementName != bge->getId()) {
                                    m_sharedElems[bge->getId()] = bge;
                                    if (!val["link"]) {
                                        ui->existingCellMLInstances->addItem(bge->getDisplayName(),
                                                                             bge->getId());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    ui->existingCellMLInstances->hide();
    ui->existingCellMLInstancesLabel->hide();

    if (m_dimension != "NONE") {
        QMap<QString, QList<QString>> matchVars;
        std::string target = m_dimension.toStdString();
        for (auto k : vars.keys()) {
            auto &vlist = vars[k];
            if (vlist.size() < 5) {
                std::string unit = vlist[2].toStdString();
                auto res = BG::checkUnits(unit, target);
                if (res.size() == 0) {
                    matchVars[k] = vlist;
                }
            } else { // User defined units
                std::string unit = vlist[4].toStdString();
                auto res = BG::checkUnits(unit, target);
                if (res.size() == 0) {
                    matchVars[k] = vlist;
                }
            }
        }
        vars = matchVars;
    }
    if (vars.size() > 0) {
        nlohmann::json vtree;
        for (auto k : vars.keys()) {
            auto &vlist = vars[k];
            nlohmann::json v;
            if (vlist[3] != "")
                v[vlist[0].toStdString() + " (" + vlist[3].toStdString() + ")"] =
                    vlist[2].toStdString();
            else
                v[vlist[0].toStdString()] = vlist[2].toStdString();
            std::string component = vlist[1].toStdString();
            if (vtree.contains(component)) {
                vtree[component].update(v);
            } else {
                vtree[component] = v;
            }
        }
        m_cellVariableModel->loadJson(vtree);
        ui->cellMLVariablesView->expandAll();
        if (vtree.size() > 2) {
            ui->findCellMLVariable->setVisible(true);
            ui->findLabel->setVisible(true);
        }
        if (m_sharedElems.size() > 0) {
            ui->existingCellMLInstances->show();
            ui->existingCellMLInstancesLabel->show();
        }
    } else {
        QMessageBox::information(
            nullptr, tr("Not found"),
            tr("Selected file does not have any public_interface=out variables "
               "whose unit is consistent with the expected unit of ")
                + m_dimension,
            QMessageBox::Ok);
        ui->applyButton->setDisabled(true);
    }
}

void BGParameterImport::loadFile()
{
    const QString defaultDirKey("default_load_dir");
    QSettings bgSettings;

    QString name = QFileDialog::getOpenFileName(
        this, tr("Open File"), bgSettings.value(defaultDirKey).toString(),
        tr("CellML Files (*.cellml)"));
    // Update the json as well as what is displayed
    blockSignals(true);
    if (!name.isEmpty() && !name.isNull()) {
        ui->cellmlfileuri->setText(name);
        loadCellML(name);
        QDir currentDir;
        bgSettings.setValue(defaultDirKey, currentDir.absoluteFilePath(name));
    }
    blockSignals(false);
}

void BGParameterImport::searchForCellMLVariable(const QString &str)
{
    m_dataProxy->setFilterFixedString(str);
    ui->cellMLVariablesView->expandAll();
}

void BGParameterImport::variableSelectionChanged(const QModelIndex &)
{
    QModelIndexList indexes =
        ui->cellMLVariablesView->selectionModel()->selectedIndexes();
    if (indexes.size() == 2) {
        QModelIndex selectedIndex = indexes.at(0);
        if (indexes.at(1).data().isValid()) {
            QString variable, units, component;
            auto model = ui->cellMLVariablesView->model();
            for (int i = 0; i < model->rowCount(); i++) {
                QModelIndex index = model->index(i, 0); // column no 0
                for (int j = 0; j < model->rowCount(index); j++) {
                    //QModelIndex child = index.child(j, 0);
                    QModelIndex child = model->index(j, 0, index);
                    if (child == selectedIndex) {
                        variable = child.data().toString();
                        // if (m_isState) {
                        //   if (variable.indexOf("(") > -1) {
                        //     auto ix = variable.indexOf("(");
                        //     auto ie = variable.indexOf(")", ix + 1);
                        //     variable = variable.mid(ix + 1, ie - ix - 1);
                        //   } else {
                        //     QMessageBox::critical(
                        //         nullptr, tr("Invalid selection"),
                        //         tr("Only numeric values can be assigned to state
                        //         variables "
                        //            "(initial value).\nSelection does not have a numeric "
                        //            "value!"));
                        //     return;
                        //   }
                        // } else {
                        //   if (variable.indexOf("(") > -1) {
                        //     auto ix = variable.indexOf("(");
                        //     variable = variable.left(ix - 1);
                        //   }
                        // }
                        if (variable.indexOf("(") > -1) {
                            auto ix = variable.indexOf("(");
                            variable = variable.left(ix - 1);
                        }
                        units = model->index(j, 1, index).data().toString();
                        component = index.data().toString();
                        break;
                    }
                }
            }

            ui->variableNameMap->setText(variable);
            // if (!m_isState) {
            //   m_record["file"] = ui->cellmlfileuri->text().toStdString();
            //   m_record["component"] = component.toStdString();
            //   m_record["dimension"] = units.toStdString();
            //   m_record["variable"] = variable.toStdString();
            //   auto linkItem = ui->existingCellMLInstances->itemData(
            //       ui->existingCellMLInstances->currentIndex());

            //   if (linkItem.toString() == "-" || linkItem.toString() == "") {
            //     m_record["link"] = false;
            //   } else {
            //     m_record["link"] = true;
            //     m_record["element"] = linkItem.toString().toStdString();
            //   }

            //   if (m_timeVariable != "") {
            //     m_record["timeVariable"] = m_timeVariable.toStdString();
            //   }
            // } else {
            //   m_record.clear();
            //   m_record["value"] = variable.toStdString();
            // }

            m_record["file"] = ui->cellmlfileuri->text().toStdString();
            m_record["component"] = component.toStdString();
            m_record["dimension"] = units.toStdString();
            m_record["variable"] = variable.toStdString();
            auto linkItem = ui->existingCellMLInstances->itemData(
                ui->existingCellMLInstances->currentIndex());

            if (linkItem.toString() == "-" || linkItem.toString() == "") {
                m_record["link"] = false;
            } else {
                m_record["link"] = true;
                m_record["element"] = linkItem.toString().toStdString();
            }

            if (m_timeVariable != "") {
                m_record["timeVariable"] = m_timeVariable.toStdString();
            }
            if (m_isState) {
                m_record["stateValue"] = true;
            }
        }
    }
}

void BGParameterImport::constrainDimension(QString si)
{
    m_dimension = si;
}

void BGParameterImport::setElementName(QString name)
{
    m_elementName = name;
}

void BGParameterImport::setAsState(bool state)
{
    m_isState = state;
}

nlohmann::json
BGParameterImport::getParameterRecord(const nlohmann::json &result,
                                      QString e_name, bool parm_is_state,
                                      BGElementEditorScene *parent)
{
    BGParameterImport dialog(result, parent, nullptr);
    dialog.setElementName(e_name);
    dialog.setAsState(parm_is_state);
    nlohmann::json res;
    if (dialog.exec() == QDialog::Accepted) {
        res = dialog.m_record;
        if (res.size() > 0) {
            QString cp = dialog.ui->variableNameMap->text();
            if (cp.size() > 0)
                res["value"] = cp.toStdString();
            else
                res["value"] = res["variable"];
            return res;
        }
    }
    return res;
}

nlohmann::json BGParameterImport::getParameterRecordWithDimensionalConsistency(
    const nlohmann::json &result, std::string siunit, QString e_name,
    bool parm_is_state, BGElementEditorScene *parent)
{
    BGParameterImport dialog(result, parent, nullptr);
    dialog.setElementName(e_name);
    dialog.setAsState(parm_is_state);
    // Check for unit consistancy
    dialog.constrainDimension(QString::fromStdString(siunit));
    nlohmann::json res;
    if (dialog.exec() == QDialog::Accepted) {
        res = dialog.m_record;
        if (res.size() > 0) {
            QString cp = dialog.ui->variableNameMap->text();
            if (cp.size() > 0)
                res["value"] = cp.toStdString();
            else
                res["value"] = res["variable"];
            return res;
        }
    }

    return res;
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
