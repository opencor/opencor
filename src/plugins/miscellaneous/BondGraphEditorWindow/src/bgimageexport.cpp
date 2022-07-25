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
#include <QByteArray>
#include <QImage>
#include <QImageWriter>
#include <QMap>
#include <QPainter>
#include <QScopedPointer>
#include <QSet>

#include "bgeditorscene.h"
#include "bgimageexport.h"

namespace OpenCOR {
namespace BondGraphEditorWindow {

QString BGImageExport::filters() const {
  static QList<QByteArray> formats = QImageWriter::supportedImageFormats();
  if (formats.isEmpty())
    return QString();

  static QMap<QByteArray, QString> formatNames = {
      {"bmp", "Windows Bitmap (*.bmp)"},
      {"ico", "Windows Icon (*.ico *.cur)"},
      {"gif", "Graphic Interchange Format (*.gif)"},
      {"jpg", "Joint Photographic Experts Group (*.jpg *.jpeg)"},
      {"png", "Portable Network Graphics (*.png)"},
      {"pbm", "Portable Bitmap (*.pbm)"},
      {"pgm", "Portable Graymap (*.pgm)"},
      {"ppm", "Portable Pixmap (*.ppm)"},
      {"svg", "Scalable Vector Graphics (*.svg)"},
      {"tif", "Tagged Image File Format (*.tif *.tiff)"},
      {"xbm", "X11 Bitmap (*.xbm)"},
      {"xpm", "X11 Pixmap (*.xpm)"},
      {"wbmp", "Wireless Bitmap (*.wbmp)"},
      {"webp", "WebP (*.webp)"},
      {"icns", "Apple Icon Image (*.icns)"}};

  static QMap<QByteArray, QByteArray> recodeMap = {
      {"jpeg", "jpg"}, {"tiff", "tif"}, {"cur", "ico"}};

  static QString filter;
  if (filter.isEmpty()) {
    QSet<QByteArray> usedFormats;

    for (auto format : formats) {
      auto suffix = format.toLower();
      if (recodeMap.contains(suffix))
        usedFormats << recodeMap[suffix];
      else
        usedFormats << suffix;
    }

    // add known formats
    for (auto it = formatNames.constBegin(); it != formatNames.constEnd();
         ++it) {
      if (usedFormats.contains(it.key())) {
        usedFormats.remove(it.key());
        filter += it.value() + ";;";
      }
    }

    // add evtl. unlisted ones
    for (auto format : usedFormats) {
      filter += format + " (*." + format + ");;";
    }

    filter.chop(2);
  }

  return filter;
}

bool BGImageExport::save(const QString &fileName, BGEditorScene &scene,
                         QString * /*lastError*/) const {
  QScopedPointer<BGEditorScene> tempScene(scene.clone());

  if (m_cutContent)
    tempScene->crop();

  QImage image(tempScene->sceneRect().size().toSize(), QImage::Format_ARGB32);
  QRect targetRect; // empty by default

  // resolution
  int old_dpi = image.physicalDpiX();
  if (old_dpi <= 0)
    old_dpi = 96;

  if (m_resolution > 0 && old_dpi != m_resolution) {
    double coeff = (double)m_resolution / (double)old_dpi;
    int dpm = m_resolution / 0.0254;
    image.setDotsPerMeterX(dpm);
    image.setDotsPerMeterY(dpm);

    QSize newSize = image.size() * coeff;
    image = image.scaled(newSize);

    targetRect = QRect(0, 0, newSize.width(), newSize.height());
  }

  image.fill(Qt::white);
  QPainter painter(&image);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::TextAntialiasing);
  tempScene->render(&painter, targetRect);
  painter.end();

  return image.save(fileName);
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR