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
// QScintillaWidget class
//==============================================================================

#ifndef QSCINTILLAWIDGET_H
#define QSCINTILLAWIDGET_H

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

    virtual void setCursorPosition(int pLine, int pColumn);
    virtual void setLexer(QsciLexer *pLexer);

    int currentPosition() const;
    void setCurrentPosition(const int &pCurrentPosition);

    QString contents() const;
    void setContents(const QString &pContents);

    int contentsSize() const;

    QString textInRange(const int &pStartRange, const int &pEndRange) const;
    int findTextInRange(const int &pStartRange, const int &pEndRange,
                        const QString &pText,
                        const bool &pCaseSensitive = true) const;

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

Q_SIGNALS:
    void canSelectAll(const bool &pCanSelectAll);

    void keyPressed(QKeyEvent *pEvent, bool &pHandled);

private Q_SLOTS:
    void updateUi();

    void checkCanSelectAll();

    void cursorPositionChanged(const int &pLine, const int &pColumn);
};

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
