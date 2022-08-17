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
#pragma once

#include <QDialog>

namespace Ui {
class BGPortEditorDialog;
}

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGElement;
class BGPort;
class BGEditorScene;

class BGPortEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BGPortEditorDialog(BGEditorScene *scene);
    ~BGPortEditorDialog() override;

    int exec(BGPort &port);

private Q_SLOTS:
    void on_Anchor_currentIndexChanged(int index);
    void on_OffsetX_valueChanged(int value);
    void on_OffsetY_valueChanged(int value);
    void on_Color_activated(const QColor &color);
    void on_flipVertical_clicked();
    void on_flipHorizontal_clicked();

private:
    void doUpdate();

    Ui::BGPortEditorDialog *ui;
    bool m_updateInProgress = false;
    BGPort *m_port = nullptr;
    BGElement *m_Element = nullptr;
    BGEditorScene *m_scene;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
