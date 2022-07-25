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
#include <QMap>

#include "thirdparty/json.hpp"
#include <bgelementeditorscene.h>

class QSortFilterProxyModel;

namespace Ui {
class BGParameterImport;
}

namespace OpenCOR {
namespace BondGraphEditorWindow {

class QJsonTreeModel;
class BGElement;

class BGParameterImport : public QDialog {
  Q_OBJECT

public:
  explicit BGParameterImport(const nlohmann::json &res,
                             BGElementEditorScene *scene, QWidget *parent = 0);
  ~BGParameterImport();
  void constrainDimension(QString si);
  void setElementName(QString name);
  void setAsState(bool state);
  static nlohmann::json getParameterRecord(const nlohmann::json &res,
                                           QString eName, bool parmIsState,
                                           BGElementEditorScene *parent = 0);
  static nlohmann::json getParameterRecordWithDimensionalConsistency(
      const nlohmann::json &result, std::string siunit, QString eName,
      bool parmIsState, BGElementEditorScene *parent = 0);

private Q_SLOTS:
  void loadFile();
  void variableSelectionChanged(const QModelIndex &index);
  void searchForCellMLVariable(const QString &str);
  void on_existingCellMLInstances_currentIndexChanged(int index);

private:
  void loadCellML(QString cellmlfile);
  Ui::BGParameterImport *ui;
  QJsonTreeModel *m_cellVariableModel;
  QSortFilterProxyModel *m_dataProxy;
  BGElementEditorScene *m_scene;
  nlohmann::json m_record;
  QMap<QString, BGElement *> m_sharedElems;
  QString m_dimension;
  QString m_timeVariable;
  QString m_elementName;
  bool m_isState; // Only values can be assigned
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR