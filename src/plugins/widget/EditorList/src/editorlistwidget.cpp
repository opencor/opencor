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

EditorListItem::EditorListItem(const Type &pType, const int &pLine,
                               const int &pColumn, const QString &pMessage) :
    mType(pType),
    mLine(pLine),
    mColumn(pColumn),
    mMessage(pMessage)
{
}

//==============================================================================

EditorListItem::Type EditorListItem::type() const
{
    // Return the item's type

    return mType;
}

//==============================================================================

int EditorListItem::line() const
{
    // Return the item's line

    return mLine;
}

//==============================================================================

int EditorListItem::column() const
{
    // Return the item's column

    return mColumn;
}

//==============================================================================

QString EditorListItem::message() const
{
    // Return the item's message

    return mMessage;
}

//==============================================================================

EditorListWidget::EditorListWidget(QWidget *pParent) :
    QListView(pParent)
{
    // Customise ourselves

#ifdef Q_OS_MAC
    setAttribute(Qt::WA_MacShowFocusRect, false);
    // Note: the above removes the focus border since it messes up our look
#endif
    setFrameShape(QFrame::NoFrame);
}

//==============================================================================

void EditorListWidget::reset()
{
//---GRY--- TO BE DONE...
}

//==============================================================================

void EditorListWidget::addItem(const EditorListItem &pItem)
{
Q_UNUSED(pItem);

//---GRY--- TO BE DONE...
}

//==============================================================================

}   // namespace EditorList
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
