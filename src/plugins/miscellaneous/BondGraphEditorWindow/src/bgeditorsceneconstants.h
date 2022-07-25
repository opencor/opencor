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

#include <QByteArray>

namespace OpenCOR {
namespace BondGraphEditorWindow {

// common scene states

enum SceneInfoState {
  SIS_Select = 0,
  SIS_Hover,
  SIS_Drag,
  SIS_Hover_Port,
  SIS_Edit_Label,
  SIS_Move_Port
};

// common scene attributes
const QByteArray class_scene = QByteArrayLiteral("");
const QByteArray class_item = QByteArrayLiteral("item");
const QByteArray class_element = QByteArrayLiteral("element");
const QByteArray class_connection = QByteArrayLiteral("connection");

const QByteArray attr_id = QByteArrayLiteral("id");
const QByteArray attr_size = QByteArrayLiteral("size");
const QByteArray attr_weight = QByteArrayLiteral("weight");
const QByteArray attr_color = QByteArrayLiteral("color");
const QByteArray attr_style = QByteArrayLiteral("style");
const QByteArray attr_label = QByteArrayLiteral("label");
const QByteArray attr_label_font = QByteArrayLiteral("label.font");
const QByteArray attr_label_color = QByteArrayLiteral("label.color");
const QByteArray attr_labels_policy = QByteArrayLiteral("labels.policy");
const QByteArray attr_labels_visIds = QByteArrayLiteral("labels.visibleIds");
const QByteArray attr_edge_direction = QByteArrayLiteral("direction");

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
