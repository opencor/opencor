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
#include <QFrame>
#include <QPixmap>

namespace OpenCOR {
namespace BondGraphUI {

class LibraryItem : public QFrame {
  Q_OBJECT
public:
  LibraryItem(QString text, QString icon, QWidget *parent = nullptr);
  ~LibraryItem();
  void setMimeData(QByteArray data);
public Q_SLOTS:
  void enterEvent(QEvent *event);
  void leaveEvent(QEvent *event);

  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);

protected:
  QPixmap *m_iMap;
  QByteArray *m_mimeData;
};
} // namespace BondGraphUI
} // namespace OpenCOR