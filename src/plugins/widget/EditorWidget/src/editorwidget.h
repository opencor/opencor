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

#include "editorwidgetglobal.h"
#include "widget.h"

//==============================================================================

class QFrame;
class QLabel;
class QMenu;

//==============================================================================

class QsciLexer;

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

class EditorWidgetEditorWidget;
class EditorWidgetFindReplaceWidget;

//==============================================================================

class EDITORWIDGET_EXPORT EditorWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit EditorWidget(const QString &pContents, bool pReadOnly,
                          QsciLexer *pLexer, QWidget *pParent);

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

    void retranslateUi() override;

    void updateSettings(EditorWidget *pEditorWidget);

    virtual bool handleEditorKeyPressEvent(QKeyEvent *pEvent);

    void setLexer(QsciLexer *pLexer);

    QMenu * contextMenu() const;
    void setContextMenu(const QList<QAction *> &pContextMenuActions);

    int length() const;

    QString text(int pLine) const;

    void selection(int &pLineFrom, int &pColumnFrom, int &pLineTo,
                   int &pColumnTo);
    void setSelection(int pLineFrom, int pColumnFrom, int pLineTo,
                      int pColumnTo);

    int position(int pLine, int pColumn);

    void cursorPosition(int &pLine, int &pColumn);
    void setCursorPosition(int pLine, int pColumn);

    int currentPosition() const;

    QString contents() const;
    void setContents(const QString &pContents, bool pEmptyUndoBuffer = true);

    int contentsSize() const;

    bool isReadOnly() const;
    void setReadOnly(bool pReadOnly);

    bool hasSelectedText() const;
    QString selectedText() const;

    QString textInRange(int pStartRange, int pEndRange) const;
    int findTextInRange(int pStartRange, int pEndRange, const QString &pText,
                        bool pRegularExpression, bool pCaseSensitive,
                        bool pWholeWordsOnly) const;

    bool isUndoAvailable() const;
    bool isRedoAvailable() const;

    bool isFindPreviousNextAvailable() const;

    bool isSelectAllAvailable() const;

    void cut();
    void copy();
    void paste();
    void del();

    void undo();
    void redo();

    void selectAll();

    void highlightAll();

    bool wordWrap() const;
    void setWordWrap(bool pWordWrap);

    void resetUndoHistory();

    QLabel * cursorPositionWidget() const;
    QLabel * editingModeWidget() const;

    QString eolString() const;

    int zoomLevel() const;
    void setZoomLevel(int pZoomLevel);

    bool isFindReplaceVisible() const;
    void setFindReplaceVisible(bool pVisible, bool pSelectWord = true);

    int styleAt(int pPosition) const;

    bool findNext();
    bool findPrevious();

    void insertText(const QString &pText, int pLine, int pColumn);
    void removeText(int pPosition, int pLength);

    void replaceSelectedText(const QString &pText);
    void replaceAndFind();

    void beginUndoAction();
    void endUndoAction();

    bool handleEditorChanges() const;

private:
    EditorWidgetEditorWidget *mEditor;
    QFrame *mSeparator;
    EditorWidgetFindReplaceWidget *mFindReplace;

signals:
    void cursorPositionChanged(int pRow, int pColumn);

    void textChanged();

    void copyAvailable(bool pCopyAvailable);

    void canFindReplace(bool pCanFindReplace);

    void canSelectAll(bool pCanSelectAll);
};

//==============================================================================

} // namespace EditorWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
