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
#include "bgelementproxymapdialog.h"
#include "ui_BGUserdefinedImportDialog.h"

#include <fstream>

#include <QFileDialog>
#include <QRegularExpression>
#include <QSettings>


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

BGElementProxyMapDialog::BGElementProxyMapDialog() :
    ui(new Ui::BGUserdefinedImportDialog), m_typeFilter("")
{
    ui->setupUi(this);
    ui->bondgraphElements->setColumnCount(4);
    ui->bondgraphElements->setShowGrid(true);
    ui->bondgraphElements->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->bondgraphElements->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bondgraphElements->setHorizontalHeaderLabels(
        { "ID", "Element", "Type", "Notes" });
    ui->bondgraphElements->horizontalHeader()->setStretchLastSection(true);
    ui->bondgraphElements->hideColumn(0);

    connect(ui->browse, &QPushButton::clicked, this,
            &BGElementProxyMapDialog::uriSelect);
    connect(ui->okButton, &QPushButton::clicked, this,
            &BGElementProxyMapDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->jsonElementSearch, &QLineEdit::textChanged, this,
            &BGElementProxyMapDialog::filterBGElement, Qt::QueuedConnection);
}

BGElementProxyMapDialog::~BGElementProxyMapDialog()
{
}

void BGElementProxyMapDialog::filterBGElement(QString term)
{
    if (m_updateInProgress)
        return;
    m_updateInProgress = true;
    if (term != "") {
        QRegularExpression re(term, QRegularExpression::MultilineOption | QRegularExpression::CaseInsensitiveOption);

        for (int i = 0; i < ui->bondgraphElements->rowCount(); i++) {
            QString en = ui->bondgraphElements->item(i, 0)->text();
            QString tp = ui->bondgraphElements->item(i, 1)->text();
            QString no = ui->bondgraphElements->item(i, 2)->text();
            QRegularExpressionMatch m1 = re.match(en);
            QRegularExpressionMatch m2 = re.match(tp);
            QRegularExpressionMatch m3 = re.match(no);
            if (!(m1.hasMatch() || m2.hasMatch() || m3.hasMatch())) {
                ui->bondgraphElements->hideRow(i);
            } else {
                ui->bondgraphElements->showRow(i);
            }
        }
    } else {
        for (int i = 0; i < ui->bondgraphElements->rowCount(); i++) {
            ui->bondgraphElements->showRow(i);
        }
    }
    m_updateInProgress = false;
}

void BGElementProxyMapDialog::accept()
{
    // Check data and then send accept
    bool success = false;
    QString errorMsg = tr("not implemented");
    auto selectedItems = ui->bondgraphElements->selectedItems();

    errorMsg = tr("No element selected!");
    if (selectedItems.size() == 3) {
        auto rc = selectedItems[0]->row();
        auto itm = ui->bondgraphElements->item(rc, 0);
        // m_result["definition"] =
        // nlohmann::json::parse(itm->data(Qt::UserRole).toString().toStdString());
        // m_result["mid"] = m_result["definition"]["mid"];
        auto def =
            nlohmann::json::parse(itm->data(Qt::UserRole).toString().toStdString());
        m_result["mid"] = def["mid"];
        auto anot = def["annotation"];
        m_result["statesAndParameters"] = anot["statesAndParameters"];
        m_result["Annotation"] = anot["Annotation"];
        if (anot.contains("Notes")) {
            m_result["Notes"] = anot["Notes"];
        } else {
            m_result["Notes"] = "";
        }
        m_result["constitutive_relations"] = anot["constitutive_relations"];
        auto linkItem = ui->linkWithExistingJsonInstances->itemData(
            ui->linkWithExistingJsonInstances->currentIndex());
        bool used = false;
        if (linkItem.toString() == "-" || linkItem.toString() == "") {
            m_result["link"] = false;
            success = true;
        } else {
            // Check if the variable has not be used
            for (auto &elk : m_sharedElems.keys()) {
                auto bg = m_sharedElems[elk];
                auto &uds = bg->getJson();
                if (uds.contains("proxy")) {
                    if (uds["proxy"]["mid"] == m_result["mid"]) {
                        used = true;
                        errorMsg =
                            "Selected element is already a proxy!\nCurrent proxy name " + bg->getDisplayName();
                        break;
                    }
                }
            }

            if (!used) {
                m_result["link"] = true;
                m_result["element"] = linkItem.toString().toStdString();
                success = true;
            }
        }
    } else if (selectedItems.size() > 3) {
        errorMsg = tr("Multiple elements selected!");
    }

    if (success) {
        QDialog::accept();
    } else {
        QMessageBox::critical(this, tr("Invalid specification"), errorMsg,
                              QMessageBox::Ok);
    }
}

void BGElementProxyMapDialog::uriSelect()
{
    m_result.clear();
    const QString defaultDirKey("default_load_dir");
    QSettings bgSettings;

    QString title = tr("Open BondGraph");
    QString filter = tr("BondGraph (*.json)");
    QString modelsDirectory = bgSettings.value(defaultDirKey).toString();
    QString fileName =
        QFileDialog::getOpenFileName(nullptr, title, modelsDirectory, filter);
    if (fileName.isEmpty())
        return;

    QDir currentDir;
    bgSettings.setValue(defaultDirKey, currentDir.absoluteFilePath(fileName));

    ui->bgfilename->setText(fileName);
    // Check if the file has already been loaded
    if (m_sharedElems.size() > 0)
        m_sharedElems.clear();
    QMap<QString, BGElement *> udlist;
    auto items = m_scene->getItems();

    for (auto elem : items) {
        if (BGElement *bge = dynamic_cast<BGElement *>(elem)) {
            if (bge->isProxy()) {
                auto &uds = bge->getJson();
                if (uds.contains("proxy"))
                    udlist[bge->getId()] = bge;
            }
        }
    }

    std::string fname = fileName.toStdString();
    ui->linkWithExistingJsonInstances->clear();
    ui->linkWithExistingJsonInstances->addItem("-", "");
    for (auto key : udlist.keys()) {
        auto &anot = udlist[key]->getJson();
        if (anot.contains("proxy")) {
            auto &map = anot["proxy"];

            if (fname.compare(map["uri"]) == 0) {
                m_sharedElems[key] = udlist[key];
                if (!map["link"]) {
                    ui->linkWithExistingJsonInstances->addItem(
                        udlist[key]->getDisplayName(), key);
                }
            }
        }
    }

    ui->linkWithExistingJsonInstances->hide();
    ui->existingJsonInstancesLabel->hide();

    {
        std::ifstream ifs(fileName.toStdString());

        try {
            nlohmann::json jf = nlohmann::json::parse(ifs);
            nlohmann::json jSceneItems = jf["sceneItems"];
            int i = 0;
            std::string ftype = m_typeFilter.toStdString();
            for (const auto &js : jSceneItems.items()) {
                QString iid = QString::fromStdString(js.key());
                const nlohmann::json &sJson = js.value();
                std::string type = sJson["typeId"];
                if (type == "BGElement") {
                    const nlohmann::json &son = sJson["definition"];
                    auto mtype = son["annotation"]["ElementDefinition"]["name"];

                    if (mtype == ftype) {
                        QString mid = QString::fromStdString(son["mid"]);
                        QString dn = QString::fromStdString(son["displayname"]);
                        QString tp = QString::fromStdString(mtype);
                        QString notes = "";
                        if (son["annotation"].contains("Notes")) {
                            notes = QString::fromStdString(son["annotation"]["Notes"]);
                        }

                        ui->bondgraphElements->insertRow(i);
                        auto elem = new QTableWidgetItem(mid);
                        elem->setData(Qt::UserRole, QString::fromStdString(son.dump()));
                        ui->bondgraphElements->setItem(i, 0, elem);
                        ui->bondgraphElements->setItem(i, 1, new QTableWidgetItem(dn));
                        ui->bondgraphElements->setItem(i, 2, new QTableWidgetItem(tp));
                        ui->bondgraphElements->setItem(i, 3, new QTableWidgetItem(notes));
                        i++;
                    }
                }
            }
            if (i < 4) {
                ui->jsonElementSearch->setVisible(false);
                ui->jsonElementSearch->setVisible(false);
            }
            ui->selectionWidget->setCurrentIndex(2);
            if (m_sharedElems.size() > 0) {
                ui->linkWithExistingJsonInstances->show();
                ui->existingJsonInstancesLabel->show();
            }
        } catch (nlohmann::json::exception &ex) {
            Q_UNUSED(ex);
            ui->bondgraphElements->clear();
            m_result.clear();
            return;
        }
    }
    m_result["uri"] = fname;
}

nlohmann::json BGElementProxyMapDialog::getDefinition()
{
    return m_result;
}

int BGElementProxyMapDialog::exec(QString type, BGElementEditorScene &scene)
{
    m_scene = &scene;
    m_typeFilter = type;
    return QDialog::exec();
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
