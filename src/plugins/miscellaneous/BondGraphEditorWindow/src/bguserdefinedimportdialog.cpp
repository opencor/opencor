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
#include "bguserdefinedimportdialog.h"
#include "ui_BGUserdefinedImportDialog.h"

#include <fstream>

#include <QFileDialog>
#include <QRegularExpression>
#include <QSettings>
#include <QSortFilterProxyModel>

#include "bgjsonmodels.h"

#if defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wnewline-eof"
    #pragma clang diagnostic ignored "-Wold-style-cast"
    #pragma clang diagnostic ignored "-Wreserved-id-macro"
    #pragma clang diagnostic ignored "-Wdeprecated-dynamic-exception-spec"
    #pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif
#include "bondgraph.hpp"

#if defined(__clang__)
    #pragma clang diagnostic pop
#endif


#include "utils.h"
#include <bgelement.h>
#include <bgelementeditorscene.h>

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGUserdefinedImportDialog::BGUserdefinedImportDialog() :
    ui(new Ui::BGUserdefinedImportDialog)
{
    ui->setupUi(this);
    QString returnChar = QChar(0x23CE);
    m_cellVariableModel =
        new QJsonTreeModel(this, "Component", returnChar + " Variable/Unit");

    m_dataProxy = new Utils::CellMLVariableFilter(this);
    m_dataProxy->setSourceModel(m_cellVariableModel);
    m_dataProxy->setFilterKeyColumn(1);
    m_dataProxy->setRecursiveFilteringEnabled(true);
    ui->cellMLVariableTree->setModel(m_dataProxy);
    connect(ui->browse, &QPushButton::clicked, this,
            &BGUserdefinedImportDialog::uriSelect);
    connect(ui->okButton, &QPushButton::clicked, this,
            &BGUserdefinedImportDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->cellMLVariableSearch, &QLineEdit::textChanged, this,
            &BGUserdefinedImportDialog::filterCellMLVariable);
}

BGUserdefinedImportDialog::~BGUserdefinedImportDialog()
{
}

void BGUserdefinedImportDialog::filterCellMLVariable(QString term)
{
    m_dataProxy->setFilterFixedString(term);
    ui->cellMLVariableTree->expandAll();
}

void BGUserdefinedImportDialog::accept()
{
    // Check data and then send accept
    bool success = false;
    QString errorMsg = tr("not implemented");
    if (m_targetType == 1) {
        // Since parent information is required, go through the list
        QModelIndexList indexes =
            ui->cellMLVariableTree->selectionModel()->selectedIndexes();
        if (indexes.size() == 2) {
            QModelIndex selectedIndex = indexes.at(0);
            if (indexes.at(1).data().isValid()) {
                QString variable, units, component;
                auto model = ui->cellMLVariableTree->model();
                for (int i = 0; i < model->rowCount(); i++) {
                    QModelIndex index = model->index(i, 0); // column no 0
                    for (int j = 0; j < model->rowCount(index); j++) {
                        //QModelIndex child = index.child(j, 0);
                        QModelIndex child = model->index(j, 0, index);
                        if (child == selectedIndex) {
                            variable = child.data().toString();
                            units = model->index(j, 1, index).data().toString();
                            component = index.data().toString();
                            break;
                        }
                    }
                }

                auto linkItem = ui->existingCellMLInstances->itemData(
                    ui->existingCellMLInstances->currentIndex());
                bool used = false;
                if (linkItem.toString() == "-" || linkItem.toString() == "") {
                    m_result["link"] = false;
                } else {
                    // Check if the variable has not be used
                    for (auto &elk : m_sharedElems.keys()) {
                        auto bg = m_sharedElems[elk];
                        auto &uds = bg->getJson();
                        if (uds.contains("mapdefinition")) {
                            if (uds["mapdefinition"]["variable"] == variable.toStdString() && uds["mapdefinition"]["dimension"] == units.toStdString() && uds["mapdefinition"]["component"] == component.toStdString()) {
                                used = true;
                                errorMsg = "Variable used in " + bg->getDisplayName();
                                break;
                            }
                        }
                    }
                    if (!used) {
                        m_result["link"] = true;
                        m_result["element"] = linkItem.toString().toStdString();
                    }
                }
                if (!used) {
                    m_result["variable"] = variable.toStdString();
                    m_result["dimension"] = units.toStdString();
                    m_result["component"] = component.toStdString();
                    m_result["type"] = "cellml";
                    m_result["sourceType"] = ui->sourceType->checkState() == Qt::Checked;
                    success = true;
                }
            } else {
                errorMsg = "Component selected! Select a variable";
            }
        } else if (indexes.size() == 0) {
            errorMsg = "No variable selected!";
        } else {
            errorMsg = "Multiple variables selected!";
        }
    }

    if (success) {
        QDialog::accept();
    } else {
        QMessageBox::critical(this, tr("Invalid specification"), errorMsg,
                              QMessageBox::Ok);
    }
}

void BGUserdefinedImportDialog::uriSelect()
{
    m_result.clear();
    const QString defaultDirKey("default_load_dir");
    QSettings bgSettings;

    QString title = tr("Open File");
    QString filter = tr("CellML (*.cellml *.cellML *.xml)");
    QString modelsDirectory = bgSettings.value(defaultDirKey).toString();
    QString fileName =
        QFileDialog::getOpenFileName(nullptr, title, modelsDirectory, filter);
    if (fileName.isEmpty())
        return;

    QDir currentDir;
    bgSettings.setValue(defaultDirKey, currentDir.absoluteFilePath(fileName));

    ui->cellmlfilename->setText(fileName);
    // Check if the file has already been loaded
    if (m_sharedElems.size() > 0)
        m_sharedElems.clear();

    QMap<QString, BGElement *> udlist = m_scene->userDefinedElements();
    std::string fname = fileName.toStdString();
    ui->existingCellMLInstances->clear();
    ui->existingCellMLInstances->addItem("-", "");

    for (auto key : udlist.keys()) {
        auto &anot = udlist[key]->getJson();
        if (anot.contains("mapdefinition")) {
            auto &map = anot["mapdefinition"];

            if (fname.compare(map["uri"]) == 0) {
                m_sharedElems[key] = udlist[key];
                if (!map["link"]) {
                    ui->existingCellMLInstances->addItem(udlist[key]->getDisplayName(),
                                                         key);
                }
            }
        }
    }

    ui->existingCellMLInstances->hide();
    ui->existingCellMLInstancesLabel->hide();

    m_timeVariable = "";
    if (fileName.endsWith("cellml") || fileName.endsWith("cellML") || fileName.endsWith("xml")) {
        const auto &vars = Utils::getCellMLOutVariables(fileName, &m_timeVariable);
        nlohmann::json vtree;
        for (auto k : vars.keys()) {
            auto &vlist = vars[k];
            nlohmann::json v;
            v[vlist[0].toStdString()] = vlist[2].toStdString();
            std::string component = vlist[1].toStdString();
            if (vtree.contains(component)) {
                vtree[component].update(v);
            } else {
                vtree[component] = v;
            }
        }

        m_cellVariableModel->loadJson(vtree);
        ui->cellMLVariableTree->expandAll();
        ui->selectionWidget->setCurrentIndex(1);
        if (m_sharedElems.size() > 0) {
            ui->existingCellMLInstances->show();
            ui->existingCellMLInstancesLabel->show();
        }
        m_targetType = 1;
    }

    m_result["uri"] = fname;
    m_result["type"] = m_targetType;
    if (m_timeVariable != "") {
        m_result["timeVariable"] = m_timeVariable.toStdString();
    }
}

nlohmann::json BGUserdefinedImportDialog::getDefinition()
{
    m_result["sourceType"] = ui->sourceType->checkState() == Qt::Checked;
    return m_result;
}

int BGUserdefinedImportDialog::exec(BGElementEditorScene &scene)
{
    m_scene = &scene;

    return QDialog::exec();
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
