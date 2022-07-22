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

#include "scenefileserializerinterface.h"

namespace OpenCOR {
namespace BondGraphUI {

class BGImageExport : public FileSerializerInterface {
public:
  BGImageExport(bool cutContent = true, int resolution = 0)
      : m_cutContent(cutContent), m_resolution(resolution) {}

  // override
  virtual QString description() const { return "Image Format"; }

  virtual QString filters() const;

  virtual QString defaultFileExtension() const { return "bmp"; }

  virtual bool loadSupported() const { return false; }

  virtual bool load(const QString & /*fileName*/, BGEditorScene & /*scene*/,
                    QString * /*lastError = nullptr*/) const {
    return false;
  }

  virtual bool saveSupported() const { return true; }

  virtual bool save(const QString &fileName, BGEditorScene &scene,
                    QString *lastError = nullptr) const;

private:
  bool m_cutContent = true;
  int m_resolution = 96;
};

} // namespace BondGraphUI
} // namespace OpenCOR