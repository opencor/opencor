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
#include <QList>

namespace Ui {
class BGProjectDetails;
}

namespace OpenCOR {
namespace BondGraphUI {

class BGProjectDetails : public QDialog {
  Q_OBJECT

public:
  explicit BGProjectDetails(bool allowBrowse = false, QWidget *parent = 0);
  ~BGProjectDetails();
  nlohmann::json getJson();

private Q_SLOTS:
  void on_Apply();
  void on_Cancel();
  void on_Browse();

private:
  Ui::BGProjectDetails *ui;
};

} // namespace BondGraphUI
} // namespace OpenCOR
