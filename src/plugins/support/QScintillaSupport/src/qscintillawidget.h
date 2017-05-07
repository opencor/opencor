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

#include "qscintillasupportglobal.h"

//==============================================================================

#include <QFont>
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

class QSCINTILLASUPPORT_EXPORT QScintillaWidget : public QsciScintilla
{
    Q_OBJECT

public:
    explicit QScintillaWidget(QsciLexer *pLexer, QWidget *pParent);

    QMenu * contextMenu() const;
    void setContextMenu(const QList<QAction *> &pContextMenuActions);

    void cursorPosition(int &pLine, int &pColumn);

    virtual void setCursorPosition(int pLine, int pColumn);
    virtual void setLexer(QsciLexer *pLexer);

    int currentPosition() const;
    void setCurrentPosition(const int &pCurrentPosition);

    QString contents() const;
    void setContents(const QString &pContents,
                     const bool &pKeepHistory = false);

    int contentsSize() const;

    QString textInRange(const int &pStartRange, const int &pEndRange) const;
    int findTextInRange(const int &pStartRange, const int &pEndRange,
                        const QString &pText, const bool &pRegularExpression,
                        const bool &pCaseSensitive, const bool &pWholeWordsOnly) const;

    QString wordAt(const int &pLine, const int &pColumn) const;

    void selectWordAt(const int &pLine, const int &pColumn);

    bool isSelectAllAvailable() const;

    void del();

    void resetUndoHistory();

    QLabel * cursorPositionWidget() const;
    QLabel * editingModeWidget() const;

    QString eolString() const;

    QColor backgroundColor(const int &pStyle);
    void setBackgroundColor(const int &pStyle, const QColor &pBackgroundColor);

    QColor foregroundColor(const int &pStyle);
    void setForegroundColor(const int &pStyle, const QColor &pForegroundColor);

    int zoomLevel() const;

    static QString checkString(const QString &pString);

protected:
    virtual void changeEvent(QEvent *pEvent);
    virtual void contextMenuEvent(QContextMenuEvent *pEvent);
    virtual void dragEnterEvent(QDragEnterEvent *pEvent);
    virtual bool event(QEvent *pEvent);
    virtual void focusInEvent(QFocusEvent *pEvent);
    virtual void focusOutEvent(QFocusEvent *pEvent);
    virtual void keyPressEvent(QKeyEvent *pEvent);
    virtual void wheelEvent(QWheelEvent *pEvent);

private:
    QFont mFont;

    QMenu *mContextMenu;

    bool mCanSelectAll;

    bool mInsertMode;

    QLabel *mCursorPositionWidget;
    QLabel *mEditingModeWidget;

    void updateColors();

signals:
    void canSelectAll(const bool &pCanSelectAll);

    void keyPressed(QKeyEvent *pEvent, bool &pHandled);

private slots:
    void updateUi();

    void checkCanSelectAll();

    void cursorPositionChanged(const int &pLine, const int &pColumn);
};

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
