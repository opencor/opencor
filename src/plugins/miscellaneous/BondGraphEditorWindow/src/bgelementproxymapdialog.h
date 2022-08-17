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

#include "thirdparty/json.hpp"
#include <QDialog>
#include <QMap>
#include <QMessageBox>

class QSortFilterProxyModel;

namespace Ui {
class BGUserdefinedImportDialog;
}

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGElement;
class BGElementEditorScene;
class QJsonTreeModel;

class BGElementProxyMapDialog : public QDialog
{
    Q_OBJECT

public:
    BGElementProxyMapDialog();
    ~BGElementProxyMapDialog() override;

    int exec(QString type, BGElementEditorScene &scene);
    nlohmann::json getDefinition();
private Q_SLOTS:
    void uriSelect();
    void accept() override;
    void filterBGElement(QString term);

private:
    Ui::BGUserdefinedImportDialog *ui;
    nlohmann::json m_result;
    QString m_typeFilter;
    QMap<QString, BGElement *> m_sharedElems;
    BGElementEditorScene *m_scene;
    bool m_updateInProgress = false;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
