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
// Editor widget
//==============================================================================

#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

//==============================================================================

#include "editorglobal.h"

//==============================================================================

#include <QString>
#include <QWidget>

//==============================================================================

class QsciLexer;

//==============================================================================

namespace Ui {
    class EditorWidget;
}

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace QScintillaSupport {
    class QScintillaWidget;
}   // namespace QScintillaSupport

//==============================================================================

namespace Editor {

//==============================================================================

class EDITOR_EXPORT EditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditorWidget(const QString &pContents, const bool &pReadOnly,
                          QsciLexer *pLexer, QWidget *pParent);

private:
    Ui::EditorWidget *mGui;

    QScintillaSupport::QScintillaWidget *mEditor;
};

//==============================================================================

}   // namespace Editor
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
