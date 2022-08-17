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
#include <QPointF>

class QSortFilterProxyModel;

namespace Ui {
class BGUserdefinedImportDialog;
}

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGElement;
class BGElementEditorScene;
class QJsonTreeModel;

class BGUserdefinedImportDialog : public QDialog
{
    Q_OBJECT

public:
    BGUserdefinedImportDialog();
    ~BGUserdefinedImportDialog() override;

    int exec(BGElementEditorScene &scene);
    nlohmann::json getDefinition();
private Q_SLOTS:
    void uriSelect();
    void accept() override;
    void filterCellMLVariable(QString term);
    // void filterBGElement(QString term);
private:
    Ui::BGUserdefinedImportDialog *ui;
    int m_targetType = -1;
    bool m_updateInProgress = false;
    nlohmann::json m_result;
    QJsonTreeModel *m_cellVariableModel;
    QSortFilterProxyModel *m_dataProxy;
    QMap<QString, BGElement *> m_sharedElems;
    BGElementEditorScene *m_scene;
    QString m_timeVariable;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
