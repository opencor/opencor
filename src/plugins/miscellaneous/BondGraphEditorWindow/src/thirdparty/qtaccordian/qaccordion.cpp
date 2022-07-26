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
#include "qaccordion.h"
#include "contentpane.h"
#include <stdexcept>

// Q_INIT_RESOURCE should be called from global namespace
void initializeAccordianResources() { Q_INIT_RESOURCE(qaccordionicons); }

namespace OpenCOR {
namespace BondGraphEditorWindow {

QAccordion::QAccordion(QWidget *parent) : QWidget(parent) {
  // make sure our resource file gets initialized

  initializeAccordianResources();

  this->multiActive = false;
  this->collapsible = true;

  // set our basic layout
  this->setLayout(new QVBoxLayout());

  // add a stretch to the end so all content panes are at the top
  dynamic_cast<QVBoxLayout *>(this->layout())->addStretch();
  this->layout()->setSpacing(1);
  this->layout()->setContentsMargins(QMargins());
  // TODO: Do we need to keep a pointer to the spacer?
  this->spacer = dynamic_cast<QSpacerItem *>(this->layout()->itemAt(0));

  // seome things we want to do if the number of panes change
  QObject::connect(this, &QAccordion::numberOfContentPanesChanged, this,
                   &QAccordion::numberOfPanesChanged);
}

int QAccordion::numberOfContentPanes() const {
  return (int)this->contentPanes.size();
}

int QAccordion::addContentPane(QString header) {
  return this->internalAddContentPane(std::move(header));
}

int QAccordion::addContentPane(QString header, QFrame *content_frame) {
  return this->internalAddContentPane(std::move(header), content_frame);
}

int QAccordion::addContentPane(ContentPane *cpane) {
  return this->internalAddContentPane("", nullptr, cpane);
}

bool QAccordion::insertContentPane(uint index, QString header) {
  return this->internalInsertContentPane(index, std::move(header));
}

bool QAccordion::insertContentPane(uint index, QString header,
                                   QFrame *content_frame) {
  return this->internalInsertContentPane(index, std::move(header),
                                         content_frame);
}

bool QAccordion::insertContentPane(uint index, ContentPane *cpane) {
  return this->internalInsertContentPane(index, "", nullptr, cpane);
}

bool QAccordion::swapContentPane(uint index, ContentPane *cpane) {
  const QString msg = "Can not swap content pane at index " +
                      QString::number(index) + ". Index out of range.";
  if (this->checkIndexError(index, false, tr(msg.toStdString().c_str()))) {
    return false;
  }

  if (this->findContentPaneIndex("", nullptr, cpane) != -1) {
    this->errorString = tr("Can not swap content pane as new pane is already "
                           "managed by accordion widget");
    return false;
  }

  // remove the old content pane from the accordion layout
  dynamic_cast<QVBoxLayout *>(this->layout())
      ->removeWidget(this->contentPanes.at(index));
  delete this->contentPanes.at(index);

  // add the new content pane to the appropriate vector
  this->contentPanes.at(index) = cpane;

  // add the new content pane to the layout
  dynamic_cast<QVBoxLayout *>(this->layout())
      ->insertWidget(index, this->contentPanes.at(index));

  return true;
}

bool QAccordion::removeContentPane(bool delete_object, uint index) {
  return this->internalRemoveContentPane(delete_object, index);
}

bool QAccordion::removeContentPane(bool delete_object, QString header) {
  return this->internalRemoveContentPane(delete_object, -1, std::move(header));
}

bool QAccordion::removeContentPane(bool delete_object, QFrame *contentframe) {
  return this->internalRemoveContentPane(delete_object, -1, "", contentframe);
}

bool QAccordion::removeContentPane(bool delete_object,
                                   ContentPane *content_pane) {
  return this->internalRemoveContentPane(delete_object, -1, "", nullptr,
                                         content_pane);
}

bool QAccordion::moveContentPane(uint current_index, uint new_index) {
  const QString msg = "Can not move from " + QString::number(current_index) +
                      ". Index out of range.";
  const QString msg2 =
      "Can not move to " + QString::number(new_index) + ". Index out of range.";

  if (this->checkIndexError(current_index, false,
                            tr(msg.toStdString().c_str())) ||
      this->checkIndexError(new_index, false, tr(msg2.toStdString().c_str()))) {
    return false;
  }

  auto *layout = dynamic_cast<QVBoxLayout *>(this->layout());
  // get the pane we want to move
  ContentPane *movePane = this->contentPanes.at(current_index);

  // remove the widget from the layout and insert it at the new position
  layout->removeWidget(movePane);
  layout->insertWidget(new_index, movePane);

  // keep our vector synchronized
  this->contentPanes.erase(this->contentPanes.begin() + current_index);
  this->contentPanes.insert(this->contentPanes.begin() + new_index, movePane);

  return true;
}

ContentPane *QAccordion::getContentPane(uint index) {
  try {
    return this->contentPanes.at(index);
  } catch (const std::out_of_range &ex) {
    this->errorString =
        tr("Can not return Content Pane: ") + QString(ex.what());
    return nullptr;
  }
}

int QAccordion::getContentPaneIndex(QString header) const {
  return this->findContentPaneIndex(std::move(header));
}

int QAccordion::getContentPaneIndex(QFrame *content_frame) const {
  return this->findContentPaneIndex("", content_frame);
}

int QAccordion::getContentPaneIndex(ContentPane *content_pane) const {
  return this->findContentPaneIndex("", nullptr, content_pane);
}

void QAccordion::getActiveContentPaneIndex(
    std::vector<int> &index_vector) const {
  // first of all make sure it is empty
  index_vector.clear();
  std::for_each(this->contentPanes.begin(), this->contentPanes.end(),
                [&index_vector, this](ContentPane *pane) {
                  if (pane->getActive()) {
                    index_vector.push_back(
                        this->findContentPaneIndex("", nullptr, pane));
                  }
                });
}

int QAccordion::getNumberOfContentPanes() const {
  return (int)this->contentPanes.size();
}

void QAccordion::setMultiActive(bool status) { this->multiActive = status; }

bool QAccordion::getMultiActive() const { return this->multiActive; }

void QAccordion::setCollapsible(bool status) { this->collapsible = status; }

bool QAccordion::getCollapsible() const { return this->collapsible; }

QString QAccordion::getError() { return this->errorString; }

int QAccordion::internalAddContentPane(QString header, QFrame *cframe,
                                       ContentPane *cpane) {
  if (this->findContentPaneIndex(header, cframe, cpane) != -1) {
    this->errorString = tr("Can not add content pane as it already exists");
    return -1;
  }

  if (cpane == nullptr) {
    if (cframe != nullptr) {
      cpane = new ContentPane(std::move(header), cframe);
    } else {
      cpane = new ContentPane(std::move(header));
    }
  }
  dynamic_cast<QVBoxLayout *>(this->layout())
      ->insertWidget(this->layout()->count() - 1, cpane);
  this->contentPanes.push_back(cpane);

  // manage the clicked signal in a lambda expression
  QObject::connect(cpane, &ContentPane::clicked,
                   [this, cpane]() { this->handleClickedSignal(cpane); });

  Q_EMIT numberOfContentPanesChanged((int)this->contentPanes.size());

  return static_cast<int>(this->contentPanes.size() - 1);
}

bool QAccordion::internalInsertContentPane(uint index, QString header,
                                           QFrame *content_frame,
                                           ContentPane *cpane) {
  const QString msg = "Can not insert Content Pane at index " +
                      QString::number(index) + ". Index out of range";
  if (this->checkIndexError(index, true, tr(msg.toStdString().c_str()))) {
    return false;
  }

  if (this->findContentPaneIndex(header, content_frame, cpane) != -1) {
    return false;
  }

  if (cpane == nullptr) {
    if (content_frame != nullptr) {
      cpane = new ContentPane(std::move(header), content_frame);
    } else {
      cpane = new ContentPane(std::move(header));
    }
  }

  dynamic_cast<QVBoxLayout *>(this->layout())->insertWidget(index, cpane);

  this->contentPanes.insert(this->contentPanes.begin() + index, cpane);

  // manage the clicked signal in a lambda expression
  QObject::connect(cpane, &ContentPane::clicked,
                   [this, cpane]() { this->handleClickedSignal(cpane); });

  Q_EMIT numberOfContentPanesChanged((int)this->contentPanes.size());

  return true;
}

bool QAccordion::internalRemoveContentPane(bool delete_oject, int index,
                                           QString name, QFrame *content_frame,
                                           ContentPane *cpane) {

  const QString msg = "Can not remove content pane at index " +
                      QString::number(index) + ". Index out of range";
  if (index != -1 &&
      this->checkIndexError(index, false, tr(msg.toStdString().c_str()))) {
    return false;
  }

  if (index == -1) {
    index = this->findContentPaneIndex(std::move(name), content_frame, cpane);
    if (index == -1) {
      this->errorString = tr("Can not remove content pane as it is not part of "
                             "the accordion widget");
      return false;
    }
  }

  dynamic_cast<QVBoxLayout *>(this->layout())
      ->removeWidget(this->contentPanes.at(index));

  // only delete the object if user wants to.
  if (delete_oject) {
    delete this->contentPanes.at(index);
    this->contentPanes.at(index) = nullptr;
  }

  this->contentPanes.erase(this->contentPanes.begin() + index);

  Q_EMIT numberOfContentPanesChanged((int)this->contentPanes.size());

  return true;
}

int QAccordion::findContentPaneIndex(QString name, QFrame *cframe,
                                     ContentPane *cpane) const {
  // simple method that finds the index of a content by Header, content frame
  // or content pane.
  int index = -1;
  if (name != "") {
    auto result = std::find_if(
        this->contentPanes.begin(), this->contentPanes.end(),
        [&name](ContentPane *pane) { return pane->getHeader() == name; });
    if (result != std::end(this->contentPanes)) {
      // get the index by subtracting begin iterator from result
      // iterator
      // TODO: Is this cast really necessary?
      index = static_cast<int>(result - this->contentPanes.begin());
    }
  }
  if (cframe != nullptr) {
    auto result =
        std::find_if(this->contentPanes.begin(), this->contentPanes.end(),
                     [cframe](ContentPane *cpane) {
                       return cpane->getContentFrame() == cframe;
                     });
    if (result != std::end(this->contentPanes)) {
      index = static_cast<int>(result - this->contentPanes.begin());
    }
  }
  if (cpane != nullptr) {
    auto result =
        std::find(this->contentPanes.begin(), this->contentPanes.end(), cpane);
    if (result != std::end(this->contentPanes)) {
      index = static_cast<int>(result - this->contentPanes.begin());
    }
  }
  return index;
}

bool QAccordion::checkIndexError(uint index, bool size_index_allowed,
                                 const QString &err_message) {
  // sizeIndexAllowed is only used by inserting. If there is one pane you will
  // be able to insert a new one before and after.
  // FIXME: Actually there seem to be some bugs hidden here. User may now for
  // example delete index 0 even if there isn't any content pane. I think we
  // excluded checking 0 because of inserting.
  // Update, I removed the 0 exclusion in the second if statement. Really a
  // fix??
  if (size_index_allowed) {
    if (index != 0 && index > this->contentPanes.size()) {
      this->errorString = err_message;
      return true;
    }
  } else {
    if (index >= this->contentPanes.size()) {
      this->errorString = err_message;
      return true;
    }
  }
  return false;
}

void QAccordion::handleClickedSignal(ContentPane *cpane) {
  // if the clicked content pane is open we simply close it and return
  if (cpane->getActive()) {
    // if collapsible and multiActive are false we are not allowed to close
    // this pane
    if (!this->collapsible && !this->multiActive) {
      return;
    }
    // when multiActive is true we have to check if there is any other open
    // cpane. if so we can close this one
    std::vector<int> activePanes;
    if (!this->collapsible) {
      this->getActiveContentPaneIndex(activePanes);
      if (activePanes.size() == 1) {
        return; // only one active --> good bye :)
      }
    }
    cpane->closeContentPane();
    return;
  }
  // if it is not open we will open it and search our vector for other
  // panes that are already open.
  // TODO: Is it really necessary to search for more than one open cpane?
  if (!cpane->getActive()) {
    // check if multiActive is allowed
    if (!this->getMultiActive()) {
      std::for_each(this->contentPanes.begin(), this->contentPanes.end(),
                    [](ContentPane *pane) {
                      if (pane->getActive()) {
                        pane->closeContentPane();
                      }
                    });
    }
    cpane->openContentPane();
  }
}

void QAccordion::numberOfPanesChanged(int number) {
  // automatically open contentpane if we have only one and collapsible is
  // false
  if (number == 1 && !this->collapsible) {
    this->contentPanes.at(0)->openContentPane();
  }
}

void QAccordion::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QStyleOption o;
  o.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR