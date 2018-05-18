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
// QScintilla widget
//==============================================================================

#pragma once

//==============================================================================

#include "corecliutils.h"
#include "qscintillasupportglobal.h"

//==============================================================================

#include <QFont>
#include <QScrollBar>
#include <QString>

//==============================================================================

#include "Qsci/qsciscintilla.h"

//==============================================================================

class QContextMenuEvent;
class QDragEnterEvent;
class QFocusEvent;
class QKeyEvent;
class QLabel;
class QMenu;
class QWheelEvent;

//==============================================================================

namespace OpenCOR {
namespace QScintillaSupport {

//==============================================================================

class QScintillaWidget;

//==============================================================================

class QSCINTILLASUPPORT_EXPORT QScintillaScrollBar : public QScrollBar
{
    Q_OBJECT

public:
    explicit QScintillaScrollBar(QScintillaWidget *pParent);

protected:
    void paintEvent(QPaintEvent *pEvent) override;

private:
    QScintillaWidget *mOwner;
};

//==============================================================================

class QSCINTILLASUPPORT_EXPORT QScintillaWidget : public QsciScintilla
{
    Q_OBJECT

public:
    explicit QScintillaWidget(QsciLexer *pLexer, QWidget *pParent);

    QMenu * contextMenu() const;
    void setContextMenu(const QList<QAction *> &pContextMenuActions);

    void cursorPosition(int &pLine, int &pColumn);

    void setCursorPosition(int pLine, int pColumn) override;
    void setLexer(QsciLexer *pLexer) override;

    int currentPosition() const;
    void setCurrentPosition(int pCurrentPosition);

    QString contents() const;
    void setContents(const QString &pContents, bool pKeepHistory = false);

    int contentsSize() const;

    QString textInRange(int pStartRange, int pEndRange) const;
    int findTextInRange(int pStartRange, int pEndRange, const QString &pText,
                        bool pRegularExpression, bool pCaseSensitive,
                        bool pWholeWordsOnly) const;

    QString wordAt(int pLine, int pColumn) const;

    void selectWordAt(int pLine, int pColumn);

    bool isSelectAllAvailable() const;

    void del();

    void resetUndoHistory();

    QLabel * cursorPositionWidget() const;
    QLabel * editingModeWidget() const;

    QString eolString() const;

    QColor backgroundColor(int pStyle);
    void setBackgroundColor(int pStyle, const QColor &pBackgroundColor);

    QColor foregroundColor(int pStyle);
    void setForegroundColor(int pStyle, const QColor &pForegroundColor);

    int zoomLevel() const;

    QIntList highlightedLines() const;

    void clearHighlighting();
    void addHighlighting(int pFromLine, int pFromColumn,
                         int pToLine, int pToColumn);

    static QString specials(const QString &pString);

protected:
    void changeEvent(QEvent *pEvent) override;
    void contextMenuEvent(QContextMenuEvent *pEvent) override;
    void dragEnterEvent(QDragEnterEvent *pEvent) override;
    bool event(QEvent *pEvent) override;
    void focusInEvent(QFocusEvent *pEvent) override;
    void focusOutEvent(QFocusEvent *pEvent) override;
    void keyPressEvent(QKeyEvent *pEvent) override;
    void wheelEvent(QWheelEvent *pEvent) override;

private:
    QFont mFont;

    QMenu *mContextMenu;

    QScintillaScrollBar *mVerticalScrollBar;

    bool mCanSelectAll;

    bool mInsertMode;

    QLabel *mCursorPositionWidget;
    QLabel *mEditingModeWidget;

    QIntList mHighlightedLines;

    int mHighlightIndicatorNumber;

    void updateColors();

signals:
    void canSelectAll(bool pCanSelectAll);

    void keyPressed(QKeyEvent *pEvent, bool &pHandled);

public slots:
    void zoomIn() override;
    void zoomOut() override;
    void zoomTo(int pSize) override;

private slots:
    void updateUi();

    void updateMarginLineNumbersWidth();

    void checkCanSelectAll();

    void updateCursorPosition(int pLine, int pColumn);
};

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
