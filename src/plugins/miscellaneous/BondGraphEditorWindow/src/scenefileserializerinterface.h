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

#include <QString>

namespace OpenCOR {
namespace BondGraphEditorWindow {

class BGEditorScene;

/**
        Common interface to file format serializers.
*/
class FileSerializerInterface
{
public:
    virtual QString description() const = 0;
    virtual QString filters() const = 0;
    virtual QString defaultFileExtension() const = 0;

    virtual bool loadSupported() const = 0;
    virtual bool load(const QString &fileName, BGEditorScene &scene,
                      QString *lastError = nullptr) const = 0;

    virtual bool saveSupported() const = 0;
    virtual bool save(const QString &fileName, BGEditorScene &scene,
                      QString *lastError = nullptr) const = 0;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
