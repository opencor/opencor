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

#pragma once

//==============================================================================

#include "editorwidgetglobal.h"

//==============================================================================

#include <QStandardItem>
#include <QString>

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

class EDITORWIDGET_EXPORT EditorListItem : public QStandardItem
{
public:
    enum Type {
        Error       = QStandardItem::UserType,
        Warning     = QStandardItem::UserType+1,
        Hint        = QStandardItem::UserType+2,
        Information = QStandardItem::UserType+3,
        Fatal       = QStandardItem::UserType+4
    };

    explicit EditorListItem(const Type &pType, const int &pLine,
                            const int &pColumn, const QString &pMessage);

    virtual int type() const;
    int line() const;
    int column() const;
    QString message() const;

private:
    Type mType;
    int mLine;
    int mColumn;
    QString mMessage;
};

//==============================================================================

typedef QList<EditorListItem> EditorListItems;

//==============================================================================

}   // namespace EditorWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
