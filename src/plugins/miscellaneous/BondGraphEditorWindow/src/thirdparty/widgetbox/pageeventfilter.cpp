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
#include <QEvent>
#include <QTreeWidgetItem>

#include "pageeventfilter.h"

namespace OpenCOR {
namespace BondGraphEditorWindow {

/*!
 * \class propagates mouse press event filter to parent QTreeWidget
 */

PageEventFilter::PageEventFilter(QObject *parent, QTreeWidgetItem *item)
    : QObject(parent), mItem(item) {
  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
          mItem->treeWidget(), SIGNAL(itemClicked(QTreeWidgetItem *, int)));
}

bool PageEventFilter::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::MouseButtonPress) {
    // Resend signal to QTreeWidget
    Q_EMIT itemClicked(mItem, 0);
    return true; // Filter event to the object
  } else {
    // standard event processing
    return QObject::eventFilter(obj, event);
  }
}

/*!
 * \class PageResizeFilter provides event filter to change item size hint in
 * Qt Designer on page widget resize as seems QTreeWidgetItem knows nothing
 * about its widget in design time and does not automatically adjust item
 * size like it does in run-time
 */

PageResizeFilter::PageResizeFilter(QObject *parent, QTreeWidgetItem *item)
    : QObject(parent), mItem(item) {}

/*!
 * \brief PageResizeFilter::eventFilter changes item size hint in Qt Designer
 * on page widget resize as seems QTreeWidgetItem knows nothing about its
 * widget in design time and does not automatically adjust item size
 * like it does in run-time
 * \param obj page widget to filter QEvent::Resize
 * \param event
 * \return false to continue event processing by page widget
 */
bool PageResizeFilter::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::Resize) {
    QWidget *page = ((QWidget *)obj);
    mItem->setSizeHint(0, page->geometry().size());
    return false; // Sent event to the object (do not filter it)
  } else {
    // standard event processing
    return QObject::eventFilter(obj, event);
  }
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR