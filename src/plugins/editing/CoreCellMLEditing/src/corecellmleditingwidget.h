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
// Core CellML editing widget
//==============================================================================

#ifndef CORECELLMLEDITINGWIDGET_H
#define CORECELLMLEDITINGWIDGET_H

//==============================================================================

#include "commonwidget.h"
#include "corecellmleditingglobal.h"
#include "corecliutils.h"

//==============================================================================

#include <QSplitter>
#include <QString>

//==============================================================================

namespace Ui {
    class CoreCellmlEditingWidget;
}

//==============================================================================

class QsciLexer;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Editor {
    class EditorWidget;
}   // namespace Editor

//==============================================================================

namespace EditorList {
    class EditorListItem;
    class EditorListWidget;
}   // namespace EditorList

//==============================================================================

namespace Viewer {
    class ViewerWidget;
}   // namespace QScintillaSupport

//==============================================================================

namespace CoreCellMLEditing {

//==============================================================================

class CORECELLMLEDITING_EXPORT CoreCellmlEditingWidget : public QSplitter,
                                                         public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CoreCellmlEditingWidget(const QString &pContents,
                                     const bool &pReadOnly, QsciLexer *pLexer,
                                     QWidget *pParent);
    ~CoreCellmlEditingWidget();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    void updateSettings(CoreCellmlEditingWidget *pCoreCellmlEditingWidget);

    Viewer::ViewerWidget * viewer() const;
    Editor::EditorWidget * editor() const;
    EditorList::EditorListWidget * editorList() const;

    QIntList editingWidgetSizes() const;

private:
    Ui::CoreCellmlEditingWidget *mGui;

    Viewer::ViewerWidget *mViewer;
    Editor::EditorWidget *mEditor;
    EditorList::EditorListWidget *mEditorList;

    QIntList mEditingWidgetSizes;

private Q_SLOTS:
    void splitterMoved();
    void itemRequested(EditorList::EditorListItem *pItem);
};

//==============================================================================

}   // namespace CoreCellMLEditing
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
