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
#include <QString>

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGEditorScene;
class BGPort;
class BGElement;
class BGDConnection;
class SceneItem;

void to_json(nlohmann::json &j, const BGEditorScene &p);
void from_json(const nlohmann::json &j, BGEditorScene &p);

void to_json(nlohmann::json &j, const BGDConnection &p);
void from_json(const nlohmann::json &j, BGDConnection &p);

void to_json(nlohmann::json &j, const BGElement &p);
void from_json(const nlohmann::json &j, BGElement &p);

void to_json(nlohmann::json &j, const BGPort &p);
void from_json(const nlohmann::json &j, BGPort &p);

void to_json(nlohmann::json &j, const SceneItem &p);
void from_json(const nlohmann::json &j, SceneItem &p);

QString to_cellML(QString name, QString dir, const BGEditorScene &p);

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
