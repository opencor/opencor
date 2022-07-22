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
#include <QHBoxLayout>
#include <QPainter>
#include <QPoint>
#include <QPropertyAnimation>
#include <QString>
#include <QStyleOption>
#include <QWidget>

#include <memory>

#include "clickableframe.h"

namespace OpenCOR {
namespace BondGraphUI {

class ContentPane : public QWidget {
  Q_OBJECT
public:
  ContentPane(QString header, QWidget *parent = nullptr);
  ContentPane(QString header, QFrame *content, QWidget *parent = nullptr);

  bool getActive() const;

  QFrame *getContentFrame();
  void setContentFrame(QFrame *content);
  int getMaximumHeight();
  void setMaximumHeight(int maxHeight);

  void setTrigger(ClickableFrame::TRIGGER tr);
  ClickableFrame::TRIGGER getTrigger();
  void setHeader(QString header);
  QString getHeader();
  void setHeaderIconActive(const QString &icon);
  void setHeaderIconActive(const QPixmap &icon);
  QPixmap getHeaderIconActive();
  void setHeaderIconInActive(const QString &icon);
  void setHeaderIconInActive(const QPixmap &icon);
  QPixmap getHeaderIconInActive();
  void setHeaderTooltip(const QString &tooltip);
  QString getHeaderTooltip();
  void setHeaderStylesheet(QString stylesheet);
  QString getHeaderStylesheet();
  void setHeaderHoverStylesheet(QString stylesheet);
  QString getHeaderHoverStylesheet();
  void setHeaderFrameStyle(int style);
  int getHeaderFrameStyle();
  void setContainerFrameStyle(int style);
  int getContainerFrameStyle();
  void setAnimationDuration(uint duration);
  uint getAnimationDuration();

Q_SIGNALS:

  void clicked();
  void isActive();
  void isInactive();

public Q_SLOTS:
  void headerTriggered(QPoint pos);

private:
  // yeah we are friends. this is important to keep openContentPane and
  // closeContentPane private
  friend class QAccordion;

  ClickableFrame *header;
  QFrame *container;
  QFrame *content;

  QPixmap headerIconActive;
  QPixmap headerIconInActive;

  int headerFrameStyle;
  int contentPaneFrameStyle;
  int containerAnimationMaxHeight;

  bool active;

  std::unique_ptr<QPropertyAnimation> openAnimation;
  std::unique_ptr<QPropertyAnimation> closeAnimation;

  void initDefaults(QString header);
  void initHeaderFrame(QString header);
  void initContainerContentFrame();
  void initAnimations();

private Q_SLOTS:
  void openContentPane();
  void closeContentPane();

protected:
  void paintEvent(QPaintEvent *event) override;
};

} // namespace BondGraphUI
} // namespace OpenCOR