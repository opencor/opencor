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

#include "bgporteditordialog.h"
#include "ui_BGPortEditorDialog.h"

#include <bgeditorscene.h>
#include <bgelement.h>
#include <bgport.h>

namespace OpenCOR {
namespace BondGraphEditorWindow {

BGPortEditorDialog::BGPortEditorDialog(BGEditorScene *scene) :
    ui(new Ui::BGPortEditorDialog), m_scene(scene)
{
    ui->setupUi(this);
    //Set Icons for port direction flip buttons
    ui->flipHorizontal->setIcon(QIcon(":/Icons/FlipHorizontal"));
    ui->flipVertical->setIcon(QIcon(":/Icons/FlipVertical"));
    setMinimumSize(300, 200);
    ui->Anchor->addItem(tr("Auto"), -1);
    ui->Anchor->addItem(tr("Left"), Qt::AlignLeft + Qt::AlignVCenter);
    ui->Anchor->addItem(tr("Top-Left"), Qt::AlignTop + Qt::AlignLeft);
    ui->Anchor->addItem(tr("Top"), Qt::AlignTop + Qt::AlignHCenter);
    ui->Anchor->addItem(tr("Top-Right"), Qt::AlignTop + Qt::AlignRight);
    ui->Anchor->addItem(tr("Right"), Qt::AlignRight + Qt::AlignVCenter);
    ui->Anchor->addItem(tr("Bottom-Right"), Qt::AlignBottom + Qt::AlignRight);
    ui->Anchor->addItem(tr("Bottom"), Qt::AlignBottom + Qt::AlignHCenter);
    ui->Anchor->addItem(tr("Bottom-Left"), Qt::AlignBottom + Qt::AlignLeft);
    ui->Anchor->addItem(tr("Center"), Qt::AlignCenter);
}

BGPortEditorDialog::~BGPortEditorDialog()
{
}

void BGPortEditorDialog::on_flipVertical_clicked()
{
    // rotate ports
    m_scene->rotateElementPort(Qt::Key_Up);
}

void BGPortEditorDialog::on_flipHorizontal_clicked()
{
    // rotate ports
    m_scene->rotateElementPort(Qt::Key_Right);
}

int BGPortEditorDialog::exec(BGPort &port)
{
    m_port = nullptr;
    m_Element = nullptr;

    ui->PortId->setText(port.getId());

    int align = port.getAlign();
    int alignIndex = ui->Anchor->findData(align);
    if (alignIndex < 0)
        alignIndex = 0;
    // int alignIndex = 0;
    ui->Anchor->setCurrentIndex(alignIndex);

    ui->OffsetX->setValue(port.getX());
    ui->OffsetY->setValue(port.getY());

    ui->Color->setColor(port.getColor());

    // set here in order to make interactive during editing
    m_port = &port;
    m_Element = port.getElement();
    // Set x,y bounds
    QRectF bRect = m_Element->Shape::boundingRect();
    ui->OffsetX->setMinimum(bRect.left());
    ui->OffsetX->setMaximum(bRect.right());
    ui->OffsetX->setValue(m_port->getX());
    ui->OffsetY->setMinimum(bRect.top());
    ui->OffsetY->setMaximum(bRect.bottom());
    ui->OffsetY->setValue(m_port->getY());

    int result = QDialog::exec();
    if (result == QDialog::Accepted) {
        m_Element->renamePort(m_port->getId(), ui->PortId->text().toLocal8Bit());
    }

    return result;
}

void BGPortEditorDialog::doUpdate()
{
    if (m_updateInProgress)
        return;
    if (m_port != nullptr && m_Element != nullptr) {
        m_updateInProgress = true;
        int xv = ui->OffsetX->value();
        int yv = ui->OffsetY->value();
        // m_port->setAlignment(-1);
        ui->Anchor->setCurrentIndex(0);
        m_Element->movePort(m_port->getId(), QPointF(xv, yv));
        m_updateInProgress = false;
    }
}

void BGPortEditorDialog::on_Color_activated(const QColor &col)
{
    Q_UNUSED(col);
    if (m_port != nullptr && m_Element != nullptr) {
        m_port->setColor(ui->Color->color());
    }
}

void BGPortEditorDialog::on_Anchor_currentIndexChanged(int ix)
{
    Q_UNUSED(ix);
    if (m_updateInProgress)
        return;
    if (m_port != nullptr && m_Element != nullptr) {
        m_updateInProgress = true;
        int align = ui->Anchor->currentData().toInt();
        int xv = ui->OffsetX->value();
        int yv = ui->OffsetY->value();
        if (align > -1) {
            m_port->setAlignment(align);
            ui->OffsetX->setValue(m_port->getX());
            ui->OffsetY->setValue(m_port->getY());
            m_Element->updatePortsLayout();
        } else {
            m_port->setAlignment(-1);
            m_Element->movePort(m_port->getId(), QPointF(xv, yv));
        }
        m_updateInProgress = false;
    }
}

void BGPortEditorDialog::on_OffsetX_valueChanged(int value)
{
    Q_UNUSED(value);
    doUpdate();
}

void BGPortEditorDialog::on_OffsetY_valueChanged(int value)
{
    Q_UNUSED(value);
    doUpdate();
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR