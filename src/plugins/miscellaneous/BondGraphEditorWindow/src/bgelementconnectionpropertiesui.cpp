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

#include <QInputDialog>
#include <QMessageBox>

#include "ui_BGElementConnectionPropertiesUI.h"

//#include "CPropertyEditorUIBase.h"
//#include "bgattributeseditorui.h"
#include "bgelementconnectionpropertiesui.h"

#include <bgconnection.h>
#include <bgdconnection.h>
#include <bgeditorsceneconstants.h>
#include <bgelement.h>
#include <bgelementeditorscene.h>
#include <bgitemattribute.h>

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGElementConnectionPropertiesUI::BGElementConnectionPropertiesUI(
    QWidget *parent) :
    QWidget(parent),
    m_scene(nullptr), m_updateLock(false),
    ui(new Ui::BGElementConnectionPropertiesUI)
{
    ui->setupUi(this);

    ui->ElementColour->setColorScheme(QSint::OpenOfficeColors());
    ui->ElementColour->enableNoColor(true);

    ui->ElementShape->addAction(QIcon(":/Icons/Node-Disc"), tr("Disc"), "disc");
    ui->ElementShape->addAction(QIcon(":/Icons/Node-Square"), tr("Square"),
                                "square");
    ui->ElementShape->addAction(QIcon(":/Icons/Node-Triangle"), tr("Triangle Up"),
                                "triangle");
    ui->ElementShape->addAction(QIcon(":/Icons/Node-Diamond"), tr("Diamond"),
                                "diamond");
    ui->ElementShape->addAction(QIcon(":/Icons/Node-Triangle-Down"),
                                tr("Triangle Down"), "triangle2");
    ui->ElementShape->addAction(QIcon(":/Icons/Node-Hexagon"), tr("Hexagon"),
                                "hexagon");

    // ui->ElementAttrBox->setChecked(false);
    /*
  ui->ConnectionDirection->addAction(QIcon(":/Icons/Edge-Directed"),
                                     tr("Directed (one end)"), "directed");
  ui->ConnectionDirection->addAction(QIcon(":/Icons/Edge-Mutual"),
                                     tr("Mutual (both ends)"), "mutual");
  ui->ConnectionDirection->addAction(QIcon(":/Icons/Edge-Undirected"),
                                     tr("None (no ends)"), "undirected");
  */

    ui->ConnectionDirection->addAction(QIcon(":/Icons/Edge-Directed"),
                                       tr("Full Head"), "fullhead");
    ui->ConnectionDirection->addAction(QIcon(":/Icons/Edge-Half"),
                                       tr("Half Head"), "halfhead");

    ui->ConnectionType->addAction(QIcon(":/Icons/Edge-Line"), tr("Direct"),
                                  "connection");
    ui->ConnectionType->addAction(QIcon(":/Icons/Edge-Xbend"), tr("X bend"),
                                  "xbend");
    ui->ConnectionType->addAction(QIcon(":/Icons/Edge-Ybend"), tr("Y bend"),
                                  "ybend");

    ui->ConnectionColour->setColorScheme(QSint::OpenOfficeColors());

    ui->ConnectionStyle->setUsedRange(Qt::SolidLine, Qt::DashDotDotLine);
    ui->StrokeStyle->setUsedRange(Qt::SolidLine, Qt::DashDotDotLine);

    // ui->ConnectionAttrBox->setChecked(false);

    // font size
    QList<int> fontSizes = { 10, 11, 12, 14, 16, 18, 20, 24, 28, 32, 36,
                             40, 44, 48, 54, 60, 66, 72, 80, 88, 96 };
    ui->LabelFontSize->setValueList(fontSizes);

    // element size
    QList<int> elementSizes = { 40, 50, 75, 100, 150, 200 };
    ui->ElementSizeX->setValueList(elementSizes);
    ui->ElementSizeY->setValueList(elementSizes);

    // element outline
    QList<double> strokeSizes = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    ui->StrokeSize->setValueList(strokeSizes);

    // connection weight
    QList<double> connectionSizes = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    ui->ConnectionWeight->setValueList(connectionSizes);

    // Hide annotation specific items
    ui->ElementShape->hide();
    ui->zValueWidget->hide();

    // update status & tooltips etc.
    ui->retranslateUi(this);
}

BGElementConnectionPropertiesUI::~BGElementConnectionPropertiesUI()
{
    delete ui;
}

void BGElementConnectionPropertiesUI::doReadSettings(QSettings &settings)
{
    Q_UNUSED(settings);
    // int pos = "settings.value("BondGraphEditorWindow/elements/splitterPosition",
    // -1).toInt(); if (pos >= 0)
    //  ui->ElementAttrEditor->getEditor()->setSplitterPosition(pos);

    // int pos = "settings.value("BondGraphEditorWindow/connections/splitterPosition",
    // -1).toInt(); if (pos >= 0)
    // ui->ConnectionAttrEditor->getEditor()->setSplitterPosition(pos);
}

void BGElementConnectionPropertiesUI::doWriteSettings(QSettings &settings)
{
    Q_UNUSED(settings);
    // settings.setValue("elements/splitterPosition",
    //                  ui->ElementAttrEditor->getEditor()->splitterPosition());
    // settings.setValue("connections/splitterPosition",
    //                  ui->ConnectionAttrEditor->getEditor()->splitterPosition());
}

void BGElementConnectionPropertiesUI::setScene(BGElementEditorScene *scene)
{
    if (m_scene != nullptr) {
        onSceneDetached(m_scene);
    }
    m_scene = scene;

    setEnabled(m_scene);

    if (m_scene != nullptr) {
        onSceneAttached(m_scene);
    }
}

void BGElementConnectionPropertiesUI::connectSignals(BGEditorScene *scene)
{
    connect(scene, SIGNAL(sceneChanged()), this, SLOT(onSceneChanged()));
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
}

void BGElementConnectionPropertiesUI::updateFromScene(BGEditorScene *scene)
{
    // default attrs
    auto elementAttrs = scene->getClassAttributes("element", false);
    ui->ElementColour->setColor(
        elementAttrs["color"].defaultValue.value<QColor>());
    ui->ElementShape->selectAction(elementAttrs["shape"].defaultValue);

    QSize size = elementAttrs["size"].defaultValue.toSize();
    ui->ElementSizeSwitch->setChecked(size.width() == size.height());
    ui->ElementSizeY->setVisible(size.width() != size.height());
    ui->ElementSizeX->setValue(size.width());
    ui->ElementSizeY->setValue(size.height());

    ui->StrokeColour->setColor(
        elementAttrs["stroke.color"].defaultValue.value<QColor>());
    ui->StrokeStyle->setPenStyle(Utils::textToPenStyle(
        elementAttrs["stroke.style"].defaultValue.toString()));
    ui->StrokeSize->setValue(elementAttrs["stroke.size"].defaultValue.toDouble());

    auto connectionAttrs = scene->getClassAttributes("connection", false);
    ui->ConnectionColour->setColor(
        connectionAttrs["color"].defaultValue.value<QColor>());
    ui->ConnectionWeight->setValue(
        connectionAttrs["weight"].defaultValue.toDouble());
    ui->ConnectionStyle->setPenStyle(
        Utils::textToPenStyle(connectionAttrs["style"].defaultValue.toString()));
    // ui->ConnectionDirection->selectAction(connectionAttrs["direction"].defaultValue);

    QFont font(connectionAttrs["label.font"].defaultValue.value<QFont>());
    ui->LabelFont->setCurrentFont(font);
    ui->LabelFontSize->setValue(font.pointSize());
    ui->LabelColour->setColor(
        connectionAttrs["label.color"].defaultValue.value<QColor>());
}

void BGElementConnectionPropertiesUI::onSceneAttached(BGEditorScene *scene)
{
    // factories for new items
    m_elementFactory = m_scene->getElementFactory();
    m_connectionFactory = m_scene->getConnectionFactory();

    // default attrs
    updateFromScene(scene);

    // connect & go
    connectSignals(scene);

    onSceneChanged();
}

void BGElementConnectionPropertiesUI::onSceneDetached(BGEditorScene *scene)
{
    scene->disconnect(this);
}

void BGElementConnectionPropertiesUI::onSceneChanged()
{
    // update active selections if any
    onSelectionChanged();
}

void BGElementConnectionPropertiesUI::onSelectionChanged()
{
    if (m_updateLock || m_scene == nullptr)
        return;

    m_updateLock = true;

    QList<BGConnection *> connections = m_scene->getSelectedConnections();
    QList<BGElement *> elements = m_scene->getSelectedElements();

    // elements
    ui->ElementsBox->setTitle(tr("Elements: %1").arg(elements.count()));

    if (elements.count()) {
        auto element = elements.first();

        ui->ElementColour->setColor(element->getAttribute("color").value<QColor>());
        if (element->getClassType() == "annotation") {
            ui->ElementShape->selectAction(element->getAttribute("shape"));
            ui->ElementShape->show();
            ui->zValueWidget->show();
            ui->zValueSpinBox->setValue(element->zValue());
        } else {
            ui->ElementShape->hide();
            ui->zValueWidget->hide();
        }

        QSize size = element->getAttribute("size").toSize();
        ui->ElementSizeSwitch->setChecked(size.width() == size.height());
        ui->ElementSizeY->setVisible(size.width() != size.height());
        ui->ElementSizeX->setValue(size.width());
        ui->ElementSizeY->setValue(size.height());

        ui->StrokeColour->setColor(
            element->getAttribute("stroke.color").value<QColor>());
        ui->StrokeStyle->setPenStyle(Utils::textToPenStyle(
            element->getAttribute("stroke.style").toString()));
        ui->StrokeSize->setValue(element->getAttribute("stroke.size").toDouble());
    }

    QList<SceneItem *> elementItems;
    for (auto item : elements)
        elementItems << item;
    // int attrCount = ui->ElementAttrEditor->setupFromItems(*m_scene,
    // elementItems); ui->ElementAttrBox->setTitle(tr("Custom Attributes:
    // %1").arg(attrCount));

    // connections
    ui->ConnectionsBox->setTitle(tr("Connections: %1").arg(connections.count()));

    if (connections.count() > 0) {
        auto connection = connections.first();

        ui->ConnectionColour->setColor(
            connection->getAttribute("color").value<QColor>());
        ui->ConnectionWeight->setValue(
            connection->getAttribute("weight").toDouble());
        ui->ConnectionStyle->setPenStyle(
            Utils::textToPenStyle(connection->getAttribute("style").toString()));
        ui->ConnectionDirection->selectAction(
            connection->getAttribute("direction"));
        int bendfactor = connection->getAttribute("bendfactor").toInt();

        if (bendfactor < 0) {
            ui->ConnectionType->selectAction("xbend");
        } else if (bendfactor > 0) {
            ui->ConnectionType->selectAction("ybend");
        } else {
            ui->ConnectionType->selectAction("connection");
        }
    }

    QList<SceneItem *> edgeItems;
    for (auto item : connections)
        edgeItems << item;

    // labels
    QList<SceneItem *> itemList;
    for (auto edgeItem : connections) {
        itemList << edgeItem;
    }

    for (auto nodeItem : elements) {
        itemList << nodeItem;
    }

    for (auto item : itemList) {
        QFont font(item->getAttribute("label.font").value<QFont>());
        ui->LabelFont->setCurrentFont(font);
        ui->LabelFontSize->setValue(font.pointSize());
        ui->LabelFontBold->setChecked(font.bold());
        ui->LabelFontItalic->setChecked(font.italic());
        ui->LabelFontUnderline->setChecked(font.underline());
        ui->LabelColour->setColor(
            item->getAttribute("label.color").value<QColor>());
        break;
    }

    // allow updates
    m_updateLock = false;
}

void BGElementConnectionPropertiesUI::setElementAttribute(
    const QByteArray &attr_id, const QVariant &v)
{
    // if (m_elementFactory)
    //	m_elementFactory->setAttribute(attrId, v);

    if (m_updateLock || m_scene == nullptr)
        return;

    QList<BGElement *> elements = m_scene->getSelectedElements();

    if (elements.isEmpty())
        return;

    for (auto element : elements) {
        if (attr_id == "shape")
            if (element->getClassType() != "annotation")
                continue;
        element->setAttribute(attr_id, v);
    }

    m_scene->addUndoState();
}

void BGElementConnectionPropertiesUI::setConnectionAttribute(
    const QByteArray &attr_id, const QVariant &v)
{
    // if (m_connectionFactory)
    //	m_connectionFactory->setAttribute(attrId, v);

    if (m_updateLock || m_scene == nullptr)
        return;

    QList<BGConnection *> connections = m_scene->getSelectedConnections();
    if (connections.isEmpty())
        return;

    for (auto connection : connections)
        connection->setAttribute(attr_id, v);

    m_scene->addUndoState();
}

void BGElementConnectionPropertiesUI::on_ElementColour_activated(
    const QColor &color)
{
    setElementAttribute("color", color);
}

void BGElementConnectionPropertiesUI::on_ElementShape_activated(
    QVariant data)
{
    setElementAttribute("shape", data);
}

void BGElementConnectionPropertiesUI::on_ElementSizeX_valueChanged(
    int /*value*/)
{
    ui->ElementSizeX->blockSignals(true);
    ui->ElementSizeY->blockSignals(true);

    if (ui->ElementSizeSwitch->isChecked())
        ui->ElementSizeY->setValue(ui->ElementSizeX->value());

    QSize size(ui->ElementSizeX->value(), ui->ElementSizeY->value());

    setElementAttribute("size", size);

    ui->ElementSizeX->blockSignals(false);
    ui->ElementSizeY->blockSignals(false);
}

void BGElementConnectionPropertiesUI::on_ElementSizeY_valueChanged(int value)
{
    on_ElementSizeX_valueChanged(value);
}

void BGElementConnectionPropertiesUI::on_ElementSizeSwitch_toggled(bool on)
{
    ui->ElementSizeY->setVisible(!on);

    if (on) {
        ui->ElementSizeY->setValue(ui->ElementSizeX->value());
        // ui->ElementSizeX->setFocus();
    }
    // else
    //	ui->ElementSizeY->setFocus();
}

void BGElementConnectionPropertiesUI::on_StrokeColour_activated(
    const QColor &color)
{
    setElementAttribute("stroke.color", color);
}

void BGElementConnectionPropertiesUI::on_StrokeStyle_activated(QVariant data)
{
    QString style = Utils::penStyleToText(data.toInt());

    setElementAttribute("stroke.style", style);
}

void BGElementConnectionPropertiesUI::on_StrokeSize_valueChanged(double value)
{
    setElementAttribute("stroke.size", value);
}

void BGElementConnectionPropertiesUI::on_zValueSpinBox_valueChanged(int value)
{
    QList<BGElement *> elements = m_scene->getSelectedElements();

    for (auto elem : elements) {
        if (elem->getClassType() == "annotation") {
            elem->setZValue(value);
        }
    }
}

void BGElementConnectionPropertiesUI::on_ConnectionColour_activated(
    const QColor &color)
{
    setConnectionAttribute("color", color);
}

void BGElementConnectionPropertiesUI::on_ConnectionWeight_valueChanged(
    double value)
{
    setConnectionAttribute("weight", value);
}

void BGElementConnectionPropertiesUI::on_ConnectionStyle_activated(
    QVariant data)
{
    QString style = Utils::penStyleToText(data.toInt());
    setConnectionAttribute("style", style);
}

void BGElementConnectionPropertiesUI::on_ConnectionDirection_activated(
    QVariant data)
{
    setConnectionAttribute("direction", data);
}

void BGElementConnectionPropertiesUI::on_ConnectionType_activated(
    QVariant data)
{
    if (m_scene) {
        m_connectionFactory = m_scene->getConnectionFactory();
        int bf = 0;
        if (data.toString() == "connection") {
            bf = 0;
        } else if (data.toString() == "xbend") {
            bf = -1;
        } else if (data.toString() == "ybend") {
            bf = 1;
        }
        dynamic_cast<BGDConnection *>(m_connectionFactory)->setBendFactor(bf);
        QList<BGConnection *> connections = m_scene->getSelectedConnections();
        if (connections.isEmpty())
            return;

        for (auto connection : connections) {
            dynamic_cast<BGDConnection *>(connection)->setBendFactor(bf);
            connection->onParentGeometryChanged();
        }
        m_scene->addUndoState();
    }
}

void BGElementConnectionPropertiesUI::on_LabelFont_activated(
    const QFont &font)
{
    ui->LabelFontSize->blockSignals(true);
    ui->LabelFontSize->setValue(font.pointSize());
    ui->LabelFontSize->blockSignals(false);

    if (m_updateLock || m_scene == nullptr)
        return;

    QList<SceneItem *> items = m_scene->getSelectedItems();
    if (items.isEmpty())
        return;

    for (auto item : items) {
        item->setAttribute(attr_label_font, font);
    }

    m_scene->addUndoState();
}

void BGElementConnectionPropertiesUI::on_LabelColour_activated(
    const QColor &color)
{
    if (m_updateLock || m_scene == nullptr)
        return;

    QList<SceneItem *> items = m_scene->getSelectedItems();
    if (items.isEmpty())
        return;

    for (auto item : items) {
        item->setAttribute(attr_label_color, color);
    }

    m_scene->addUndoState();
}

void BGElementConnectionPropertiesUI::on_LabelFontSize_valueChanged(int value)
{
    if (m_updateLock || m_scene == nullptr)
        return;

    QList<SceneItem *> items = m_scene->getSelectedItems();
    if (items.isEmpty())
        return;

    bool set = false;

    for (auto item : items) {
        QFont font = item->getAttribute(attr_label_font).value<QFont>();
        if (font.pointSize() != value) {
            font.setPointSize(value);
            item->setAttribute(attr_label_font, font);
            set = true;
        }
    }

    if (set)
        m_scene->addUndoState();
}

void BGElementConnectionPropertiesUI::on_LabelFontBold_toggled(bool on)
{
    if (m_updateLock || m_scene == nullptr)
        return;

    QList<SceneItem *> items = m_scene->getSelectedItems();
    if (items.isEmpty())
        return;

    bool set = false;

    for (auto item : items) {
        QFont font = item->getAttribute(attr_label_font).value<QFont>();
        if (font.bold() != on) {
            font.setBold(on);
            item->setAttribute(attr_label_font, font);
            set = true;
        }
    }

    if (set)
        m_scene->addUndoState();
}

void BGElementConnectionPropertiesUI::on_LabelFontItalic_toggled(bool on)
{
    if (m_updateLock || m_scene == nullptr)
        return;

    QList<SceneItem *> items = m_scene->getSelectedItems();
    if (items.isEmpty())
        return;

    bool set = false;

    for (auto item : items) {
        QFont font = item->getAttribute(attr_label_font).value<QFont>();
        if (font.italic() != on) {
            font.setItalic(on);
            item->setAttribute(attr_label_font, font);
            item->updateLabelContent();
            set = true;
        }
    }

    if (set)
        m_scene->addUndoState();
}

void BGElementConnectionPropertiesUI::on_LabelFontUnderline_toggled(bool on)
{
    if (m_updateLock || m_scene == nullptr)
        return;

    QList<SceneItem *> items = m_scene->getSelectedItems();
    if (items.isEmpty())
        return;

    bool set = false;

    for (auto item : items) {
        QFont font = item->getAttribute(attr_label_font).value<QFont>();
        if (font.underline() != on) {
            font.setUnderline(on);
            item->setAttribute(attr_label_font, font);
            set = true;
        }
    }

    if (set)
        m_scene->addUndoState();
}
} // namespace BondGraphEditorWindow
} // namespace OpenCOR