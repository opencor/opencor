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

//==============================================================================
// Editor widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "editorwidgetglobal.h"
#include "qscintillawidget.h"

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

class EditorWidget;
class EditorWidgetFindReplaceWidget;
class EditorWidgetScrollBar;

//==============================================================================

class EDITORWIDGET_EXPORT EditorWidgetEditorWidget : public QScintillaWidget::QScintillaWidget
{
    Q_OBJECT

public:
    explicit EditorWidgetEditorWidget(QsciLexer *pLexer,
                                      EditorWidgetFindReplaceWidget *pFindReplace,
                                      EditorWidget *pParent);

    void setReadOnly(bool pReadOnly) override;

    QIntSet highlightedLines() const;

    void clearHighlighting();

    void updateLineColumn();

    void del() override;

    using QsciScintilla::replace;

protected:
    void keyPressEvent(QKeyEvent *pEvent) override;
    void mouseDoubleClickEvent(QMouseEvent *pEvent) override;
    void mousePressEvent(QMouseEvent *pEvent) override;

private:
    enum class Action {
        HighlightAll,
        ReplaceAll
    };

    EditorWidget *mOwner;

    EditorWidgetFindReplaceWidget *mFindReplace;

    EditorWidgetScrollBar *mVerticalScrollBar;

    QIntList mReadOnlyStyles;

    int mHighlightIndicatorNumber;

    QIntSet mHighlightedLines;

    QStringList mTexts;

    int mLine = 0;
    int mColumn = 0;

    void processAll(Action pAction);

    bool findText(const QString &pText, bool pForward = true,
                  bool pFirstTime = true);

public slots:
    void highlightAll();
    void replaceAll();

    bool findNext() override;
    bool findPrevious();

    void findTextChanged(const QString &pText);

    void replace();
    void replaceAndFind();

    void cut() override;
    void paste() override;

    void undo() override;
    void redo() override;

private slots:
    void highlightAllAndFind();
};

//==============================================================================

} // namespace EditorWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
