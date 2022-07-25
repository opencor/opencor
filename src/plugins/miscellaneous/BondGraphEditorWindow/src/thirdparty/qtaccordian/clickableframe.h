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
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QString>
#include <qnamespace.h>

namespace OpenCOR {
namespace BondGraphEditorWindow {

class ClickableFrame : public QFrame {
  Q_OBJECT
public:
  enum class ICON_POSITION { LEFT, RIGHT };
  enum class TRIGGER { NONE, SINGLECLICK, DOUBLECLICK, MOUSEOVER };

  const char *const CARRET_ICON_CLOSED =
      ":/qAccordionIcons/caret-right.png"; /**< Qt qrc "path" for the closed
                                              icon */
  const char *const CARRET_ICON_OPENED =
      ":/qAccordionIcons/caret-bottom.png"; /**< Qt qrc "path" for the opened
                                               icon */

  ClickableFrame(QString header, QWidget *parent = nullptr,
                 Qt::WindowFlags f = Qt::Widget);

  void setTrigger(TRIGGER tr);
  TRIGGER getTrigger();
  void setHeader(QString header);
  QString getHeader();
  void setNormalStylesheet(QString stylesheet);
  QString getNormalStylesheet();
  void setHoverStylesheet(QString stylesheet);
  QString getHoverStylesheet();

Q_SIGNALS:
  void triggered(QPoint pos);

public Q_SLOTS:
  void setIcon(const QPixmap &icon);

private:
  QLabel *iconLabel;
  QLabel *nameLabel;

  QString hoverStylesheet;
  QString normalStylesheet;

  QString header;
  QString tooltip;

  TRIGGER headerTrigger;

  void initFrame();

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseDoubleClickEvent(QMouseEvent *event) override;
  void enterEvent(QEvent *event) override;
  void leaveEvent(QEvent *event) override;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR