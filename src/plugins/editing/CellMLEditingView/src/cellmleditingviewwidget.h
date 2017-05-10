/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// CellML Editing view widget
//==============================================================================

#pragma once

//==============================================================================

#include "cellmleditingviewglobal.h"
#include "corecliutils.h"
#include "splitterwidget.h"

//==============================================================================

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

class CELLMLEDITINGVIEW_EXPORT CellmlEditingViewWidget : public Core::SplitterWidget
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
    EditorWidget::EditorWidget * editorWidget() const;
    EditorWidget::EditorListWidget * editorListWidget() const;

    QIntList editingWidgetSizes() const;

private:
    MathMLViewerWidget::MathmlViewerWidget *mMathmlViewerWidget;
    EditorWidget::EditorWidget *mEditorWidget;
    EditorWidget::EditorListWidget *mEditorListWidget;

    QIntList mEditingWidgetSizes;

private slots:
    void splitterMoved();
    void itemRequested(OpenCOR::EditorWidget::EditorListItem *pItem);
};

//==============================================================================

}   // namespace CellMLEditingView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
