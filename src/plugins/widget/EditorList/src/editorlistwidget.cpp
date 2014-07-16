/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Editor list widget
//==============================================================================

#include "editorlistwidget.h"

//==============================================================================

namespace OpenCOR {
namespace EditorList {

//==============================================================================

EditorListWidget::EditorListWidget(QWidget *pParent) :
    QListView(pParent),
    mModel(new QStandardItemModel(this))
{
    // Customise ourselves

#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above removes the focus border since it messes up our look
#endif
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFrameShape(QFrame::NoFrame);
    setModel(mModel);
}

//==============================================================================

void EditorListWidget::clear()
{
    // Reset our list of items

    mModel->clear();
}

//==============================================================================

void EditorListWidget::addItem(const EditorListItem::Type &pType,
                               const int &pLine, const int &pColumn,
                               const QString &pMessage)
{
    // Add the given item to our list

    mModel->invisibleRootItem()->appendRow(new EditorListItem(pType, pLine, pColumn, pMessage));
}

//==============================================================================

void EditorListWidget::selectFirstItem()
{
    // Select the first item in our list

    QStandardItem *listViewItem = mModel->invisibleRootItem()->child(0);

    if (listViewItem)
        setCurrentIndex(listViewItem->index());
}

//==============================================================================

}   // namespace EditorList
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
