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
#include "contentpane.h"
namespace OpenCOR {
namespace BondGraphEditorWindow {
ContentPane::ContentPane(QString header, QWidget *parent) : QWidget(parent) {
  this->content = nullptr;

  this->initDefaults(std::move(header));
}

ContentPane::ContentPane(QString header, QFrame *content, QWidget *parent)
    :  QWidget(parent), content(content) {
  this->initDefaults(std::move(header));
}

bool ContentPane::getActive() const { return this->active; }

QFrame *ContentPane::getContentFrame() { return this->content; }

void ContentPane::setContentFrame(QFrame *content) {
  this->container->layout()->removeWidget(this->content);
  if (this->content != nullptr) {
    delete (this->content);
  }
  this->content = content;
  dynamic_cast<QVBoxLayout *>(this->container->layout())
      ->insertWidget(0, this->content);
}

int ContentPane::getMaximumHeight() { return this->container->maximumHeight(); }

void ContentPane::setMaximumHeight(int max_height) {
  this->containerAnimationMaxHeight = max_height;

  if (this->getActive()) {
    this->container->setMaximumHeight(this->containerAnimationMaxHeight);
  }
  this->openAnimation->setEndValue(this->containerAnimationMaxHeight);
  this->closeAnimation->setStartValue(this->containerAnimationMaxHeight);
}

void ContentPane::setTrigger(ClickableFrame::TRIGGER tr) {
  this->header->setTrigger(tr);
}

ClickableFrame::TRIGGER ContentPane::getTrigger() {
  return this->header->getTrigger();
}

void ContentPane::setHeader(QString header) {
  this->header->setHeader(std::move(header));
}

QString ContentPane::getHeader() { return this->header->getHeader(); }

void ContentPane::setHeaderIconActive(const QString &icon) {
  QPixmap pic(icon);
  if (!pic.isNull()) {
    this->headerIconActive = pic;
    if (this->getActive()) {
      this->header->setIcon(this->headerIconActive);
    }
  }
}

void ContentPane::setHeaderIconActive(const QPixmap &icon) {
  if (!icon.isNull()) {
    this->headerIconActive = icon;
    if (this->getActive()) {
      this->header->setIcon(this->headerIconActive);
    }
  }
}

QPixmap ContentPane::getHeaderIconActive() { return this->headerIconActive; }

void ContentPane::setHeaderIconInActive(const QString &icon) {
  QPixmap pic(icon);
  if (!pic.isNull()) {
    this->headerIconInActive = pic;
    if (!this->getActive()) {
      this->header->setIcon(this->headerIconInActive);
    }
  }
}

void ContentPane::setHeaderIconInActive(const QPixmap &icon) {
  if (!icon.isNull()) {
    this->headerIconInActive = icon;
    if (!this->getActive()) {
      this->header->setIcon(this->headerIconInActive);
    }
  }
}

QPixmap ContentPane::getHeaderIconInActive() {
  return this->headerIconInActive;
}

void ContentPane::setHeaderTooltip(const QString &tooltip) {
  this->header->setToolTip(tooltip);
}

QString ContentPane::getHeaderTooltip() { return this->header->toolTip(); }

void ContentPane::setHeaderStylesheet(QString stylesheet) {
  this->header->setNormalStylesheet(std::move(stylesheet));
}

QString ContentPane::getHeaderStylesheet() {
  return this->header->getNormalStylesheet();
}

void ContentPane::setHeaderHoverStylesheet(QString stylesheet) {
  this->header->setHoverStylesheet(std::move(stylesheet));
}

QString ContentPane::getHeaderHoverStylesheet() {
  return this->header->getHoverStylesheet();
}

void ContentPane::setHeaderFrameStyle(int style) {
  this->header->setFrameStyle(style);
}

int ContentPane::getHeaderFrameStyle() { return this->header->frameStyle(); }

void ContentPane::setContainerFrameStyle(int style) {
  this->container->setFrameStyle(style);
}

int ContentPane::getContainerFrameStyle() {
  return this->container->frameStyle();
}

void ContentPane::openContentPane() {
  if (this->getActive()) {
    return;
  }
  this->openAnimation->start();
  this->header->setIcon(this->headerIconActive);
  this->active = true;
}

void ContentPane::closeContentPane() {
  if (!this->getActive()) {
    return;
  }
  this->closeAnimation->start();
  this->header->setIcon(this->headerIconInActive);
  this->active = false;
}

void ContentPane::initDefaults(QString header) {
  this->active = false;

  this->headerFrameStyle = QFrame::Shape::StyledPanel | QFrame::Shadow::Raised;
  this->contentPaneFrameStyle =
      QFrame::Shape::StyledPanel | QFrame::Shadow::Plain;
  this->containerAnimationMaxHeight = 500;
  // TODO: Why do I need to set the vertial policy to Maximum? from the api
  // documentation Minimum would make more sens :/
  this->setSizePolicy(QSizePolicy::Policy::Preferred,
                      QSizePolicy::Policy::Maximum);

  this->setLayout(new QVBoxLayout());
  this->layout()->setSpacing(1);
  this->layout()->setContentsMargins(QMargins());

  this->initHeaderFrame(std::move(header));
  this->initContainerContentFrame();
  this->initAnimations();
}

void ContentPane::initHeaderFrame(QString header) {
  this->header = new ClickableFrame(std::move(header));
  this->header->setFrameStyle(this->headerFrameStyle);
  // init the icons
  this->setHeaderIconActive(this->header->CARRET_ICON_OPENED);
  this->setHeaderIconInActive(this->header->CARRET_ICON_CLOSED);
  this->layout()->addWidget(this->header);

  QObject::connect(this->header, &ClickableFrame::triggered, this,
                   &ContentPane::headerTriggered);
}

void ContentPane::initContainerContentFrame() {
  this->container = new QFrame();
  this->container->setLayout(new QVBoxLayout());
  this->container->setFrameStyle(this->contentPaneFrameStyle);
  this->container->setMaximumHeight(0);
  this->container->setSizePolicy(QSizePolicy::Policy::Preferred,
                                 QSizePolicy::Policy::Preferred);
  this->layout()->addWidget(this->container);

  if (this->content == nullptr) {
    this->content = new QFrame();
  }

  this->container->layout()->addWidget(this->content);
  this->container->layout()->setSpacing(0);
  this->container->layout()->setContentsMargins(QMargins());
}

void ContentPane::initAnimations() {
  this->openAnimation = std::make_unique<QPropertyAnimation>();
  this->closeAnimation = std::make_unique<QPropertyAnimation>();
  // TODO: Currently these animations only animate maximumHeight. This leads to
  // different behaviour depending on whether the Accordion Widget is placed
  // inside a QScollWidget or not. Maybe we also need to animate minimumHeight
  // as well to get the same effect.
  // TODO: Lots of boilerplate code here
  this->openAnimation->setTargetObject(this->container);
  this->openAnimation->setPropertyName("maximumHeight");
  this->closeAnimation->setTargetObject(this->container);
  this->closeAnimation->setPropertyName("maximumHeight");

  this->openAnimation->setDuration(300);
  this->closeAnimation->setDuration(300);
  this->openAnimation->setStartValue(0);
  this->closeAnimation->setStartValue(this->containerAnimationMaxHeight);
  this->openAnimation->setEndValue(this->containerAnimationMaxHeight);
  this->closeAnimation->setEndValue(0);
  this->openAnimation->setEasingCurve(QEasingCurve(QEasingCurve::Type::Linear));
  this->closeAnimation->setEasingCurve(
      QEasingCurve(QEasingCurve::Type::Linear));
}

void ContentPane::headerTriggered(QPoint pos) {
  Q_UNUSED(pos);
  Q_EMIT this->clicked();
}

void ContentPane::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR