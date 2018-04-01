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
// Editor widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "editorwidgetglobal.h"
#include "widget.h"

//==============================================================================

#include <QColor>
#include <QString>

//==============================================================================

class QAction;
class QFrame;
class QLabel;
class QMenu;

//==============================================================================

class QsciLexer;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace QScintillaSupport {
    class QScintillaWidget;
}   // namespace QScintillaSupport

//==============================================================================

namespace EditorWidget {

//==============================================================================

class EditorWidgetFindReplaceWidget;

//==============================================================================

class EDITORWIDGET_EXPORT EditorWidget : public Core::Widget
{
    Q_OBJECT

public:
    explicit EditorWidget(const QString &pContents, bool pReadOnly,
                          QsciLexer *pLexer, QWidget *pParent);

    void loadSettings(QSettings *pSettings) override;
    void saveSettings(QSettings *pSettings) const override;

    void retranslateUi() override;

    void updateSettings(EditorWidget *pEditorWidget);

    QScintillaSupport::QScintillaWidget * editor() const;

    QMenu * contextMenu() const;
    void setContextMenu(const QList<QAction *> &pContextMenuActions);

    void cursorPosition(int &pLine, int &pColumn);
    void setCursorPosition(int pLine, int pColumn);

    int currentPosition() const;

    QString contents() const;
    void setContents(const QString &pContents, bool pKeepHistory = false);

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

    void resetUndoHistory();

    QLabel * cursorPositionWidget() const;
    QLabel * editingModeWidget() const;

    QString eolString() const;

    int zoomLevel() const;
    void setZoomLevel(int pZoomLevel);

    bool findReplaceIsVisible() const;
    void setFindReplaceVisible(bool pVisible);

    int styleAt(int pPosition) const;

private:
    QScintillaSupport::QScintillaWidget *mEditor;
    QFrame *mSeparator;
    EditorWidgetFindReplaceWidget *mFindReplace;

    int mCurrentLine;
    int mCurrentColumn;

    bool mFindReplaceVisible;

    QIntList mReadOnlyStyles;

    EditorWidgetFindReplaceWidget * findReplace();

    bool findText(const QString &pText, bool pForward);

signals:
    void zoomLevelChanged(int pZoomLevel);

    void cursorPositionChanged(int pRow, int pColumn);

    void textChanged();

    void copyAvailable(bool pCopyAvailable);

    void canFindReplace(bool pCanFindReplace);

    void canSelectAll(bool pCanSelectAll);

public slots:
    bool findPrevious();
    bool findNext();

    void replace();
    void replaceAndFind();
    void replaceAll();

private slots:
    void zoomLevelChanged();

    void keepTrackOfCursorPosition(int pLine, int pColumn);

    void editorKeyPressed(QKeyEvent *pEvent, bool &pHandled);
    void findReplaceKeyPressed(QKeyEvent *pEvent, bool &pHandled);

    void findTextChanged(const QString &pText);
};

//==============================================================================

}   // namespace EditorWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
