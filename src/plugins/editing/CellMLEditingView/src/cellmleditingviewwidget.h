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
// CellML Editing view widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmleditingviewglobal.h"
#include "commonwidget.h"
#include "corecliutils.h"

//==============================================================================

#include <QSplitter>
#include <QString>

//==============================================================================

class QsciLexer;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace EditorWidget {
    class EditorListItem;
    class EditorListWidget;
    class EditorWidget;
}   // namespace EditorWidget

//==============================================================================

namespace MathMLViewerWidget {
    class MathmlViewerWidget;
}   // namespace MathMLViewerWidget

//==============================================================================

namespace CellMLEditingView {

//==============================================================================

class CELLMLEDITINGVIEW_EXPORT CellmlEditingViewWidget : public QSplitter,
                                                         public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit CellmlEditingViewWidget(const QString &pContents,
                                     const bool &pReadOnly, QsciLexer *pLexer,
                                     QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    virtual void retranslateUi();

    void updateSettings(CellmlEditingViewWidget *pCellmlEditingViewWidget);

    MathMLViewerWidget::MathmlViewerWidget * mathmlViewer() const;
    EditorWidget::EditorWidget * editor() const;
    EditorWidget::EditorListWidget * editorList() const;

    QIntList editingWidgetSizes() const;

private:
    MathMLViewerWidget::MathmlViewerWidget *mMathmlViewer;
    EditorWidget::EditorWidget *mEditor;
    EditorWidget::EditorListWidget *mEditorList;

    QIntList mEditingWidgetSizes;

private Q_SLOTS:
    void splitterMoved();
    void itemRequested(OpenCOR::EditorWidget::EditorListItem *pItem);
};

//==============================================================================

}   // namespace CellMLEditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
