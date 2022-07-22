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
#include <QPainter>

#include "categorybutton.h"

namespace OpenCOR {
namespace BondGraphUI {

CategoryButton::CategoryButton(QWidget *parent) : QPushButton(parent) {}

CategoryButton::~CategoryButton() {}

QSize CategoryButton::sizeHint() const {
  const auto parentHint = QPushButton::sizeHint();
  // add margins here if needed
  return QSize(parentHint.width() + m_pixmap.width(),
               std::max(parentHint.height(), m_pixmap.height()));
}

void CategoryButton::setPixmap(const QPixmap &pixmap) { m_pixmap = pixmap; }

void CategoryButton::paintEvent(QPaintEvent *e) {
  QPushButton::paintEvent(e);

  if (!m_pixmap.isNull()) {
    const int y = (height() - m_pixmap.height()) / 2; // add margin if needed
    QPainter painter(this);
    painter.drawPixmap(5, y, m_pixmap); // hardcoded horizontal margin
  }
}
} // namespace BondGraphUI
} // namespace OpenCOR