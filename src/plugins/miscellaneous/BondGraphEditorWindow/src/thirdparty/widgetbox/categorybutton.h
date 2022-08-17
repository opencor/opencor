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

#include <QPushButton>

namespace OpenCOR {
namespace BondGraphEditorWindow {

class CategoryButton : public QPushButton {
public:
  explicit CategoryButton(QWidget *parent = nullptr);
  virtual ~CategoryButton() override;

  void setPixmap(const QPixmap &pixmap);

  virtual QSize sizeHint() const override;

protected:
  virtual void paintEvent(QPaintEvent *e) override;

private:
  QPixmap m_pixmap;
};
} // namespace BondGraphEditorWindow
} // namespace OpenCOR
