/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Editor list item
//==============================================================================

#include "editorlistitem.h"

//==============================================================================

namespace OpenCOR {
namespace EditorList {

//==============================================================================

EditorListItem::EditorListItem(const Type &pType, const int &pLine,
                               const int &pColumn, const QString &pMessage) :
    QStandardItem(),
    mType(pType),
    mLine(pLine),
    mColumn(pColumn),
    mMessage(pMessage)
{
    // Customise ourselves

    static const QIcon ErrorIcon       = QIcon(":/oxygen/emblems/emblem-important.png");
    static const QIcon WarningIcon     = QIcon(":/oxygen/status/task-attention.png");
    static const QIcon HintIcon        = QIcon(":/oxygen/actions/help-hint.png");
    static const QIcon InformationIcon = QIcon(":/oxygen/status/dialog-information.png");
    static const QIcon FatalIcon       = QIcon(":/oxygen/status/edit-bomb.png");

    if ((pLine == -1) && (pColumn == -1))
        setText(pMessage);
    else if (pColumn == -1)
        setText(QString("[%1] %2").arg(pLine).arg(pMessage));
    else
        setText(QString("[%1:%2] %3").arg(pLine).arg(pColumn).arg(pMessage));

    setToolTip(text());

    switch (pType) {
    case Error:
        setIcon(ErrorIcon);

        break;
    case Warning:
        setIcon(WarningIcon);

        break;
    case Hint:
        setIcon(HintIcon);

        break;
    case Information:
        setIcon(InformationIcon);

        break;
    case Fatal:
        setIcon(FatalIcon);

        break;
    }

}

//==============================================================================

int EditorListItem::type() const
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

}   // namespace EditorList
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
