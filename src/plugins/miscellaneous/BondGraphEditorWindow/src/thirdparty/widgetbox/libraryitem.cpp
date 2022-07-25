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
#include "libraryitem.h"
#include <QDebug>
#include <QDrag>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>

#include "thirdparty/jkqtlatex/jkqtmathtext.h"

namespace OpenCOR {
namespace BondGraphEditorWindow {

QPixmap getImage(QString text) {
  JKQTMathText mathText;

  // configure its properties to influence the rendering (e.g. fonts to use,
  // font size, ...)
  mathText.useXITS();
  mathText.setFontSize(11);
  mathText.setFontColor(Qt::black);
  // parse some LaTeX code (the Schroedinger's equation)
  mathText.parse(text);

  // use the draw() methods to draw the equation using a QPainter (here onto a
  // QPixmap)
  QPainter painter;
  QSizeF lsize = mathText.getSize(painter);
  auto width = qMax(lsize.width(), lsize.height()) + 5;
  auto height = qMax(lsize.width(), lsize.height()) + 5;
  QPixmap pix(width, height);
  pix.fill(Qt::transparent);
  painter.begin(&pix);
  mathText.draw(painter, Qt::AlignCenter,
                QRectF(-5, 0, pix.width(), pix.height()), false);
  painter.end();
  return pix;
}

LibraryItem::LibraryItem(QString text, QString iconFile, QWidget *parent)
    : QFrame(parent) {
  auto icon = new QLabel(this);
  icon->setScaledContents(true);
  m_iMap = new QPixmap(iconFile);
  if (m_iMap->isNull()) {
    delete m_iMap;
    m_iMap = new QPixmap(getImage(iconFile));
  }
  icon->setPixmap(*m_iMap);
  icon->setFixedHeight(15);
  icon->setFixedWidth(20);

  auto layout = new QHBoxLayout;
  layout->setSpacing(6);
  layout->addWidget(icon);
  auto title = new QLabel(text);
  layout->addWidget(title);
  setLayout(layout);
  setStyleSheet("QFrame { background-color : white;}");
  m_mimeData = new QByteArray(text.toUtf8().toBase64());
}

LibraryItem::~LibraryItem() {
  delete m_iMap;
  delete m_mimeData;
}

void LibraryItem::enterEvent(QEvent *event) {
  Q_UNUSED(event);
  setFrameStyle(QFrame::Panel | QFrame::Sunken);
  setCursor(Qt::OpenHandCursor);
}

void LibraryItem::leaveEvent(QEvent *event) {
  Q_UNUSED(event);
  setFrameStyle(QFrame::NoFrame);
  setCursor(Qt::ArrowCursor);
}

void LibraryItem::mousePressEvent(QMouseEvent *event) {
  Q_UNUSED(event);
  setCursor(Qt::ClosedHandCursor);
}

void LibraryItem::mouseReleaseEvent(QMouseEvent *event) {
  Q_UNUSED(event);
  setCursor(Qt::OpenHandCursor);
}

void LibraryItem::setMimeData(QByteArray mdata) {
  delete m_mimeData;
  m_mimeData = new QByteArray(mdata);
}

void LibraryItem::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() != Qt::LeftButton)
    return;
  // Store the classname of the node in the D-n-D
  auto mimeData = new QMimeData();
  mimeData->setImageData(*m_iMap);
  mimeData->setData("diagramType", *m_mimeData);
  // Initialize Drag-and-drop action
  auto drag = new QDrag(this);
  drag->setMimeData(mimeData);
  auto hotspot = QPoint(m_iMap->width() / 2, m_iMap->height() / 2);
  drag->setHotSpot(hotspot);
  drag->setPixmap(*m_iMap);

  auto dropAction = drag->exec(Qt::MoveAction);
  if (dropAction == Qt::MoveAction)
    close();
}
} // namespace BondGraphEditorWindow
} // namespace OpenCOR