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

#include "coreguiutils.h"
#include "filemanager.h"
#include "qscintillawidget.h"

//==============================================================================

#include <QDragEnterEvent>
#include <QLabel>
#include <QMenu>
#include <QMimeData>

//==============================================================================

#include "qscintillabegin.h"
    #include "Qsci/qscilexer.h"
#include "qscintillaend.h"

//==============================================================================

namespace OpenCOR {
namespace QScintillaWidget {

//==============================================================================

QScintillaWidget::QScintillaWidget(QsciLexer *pLexer, QWidget *pParent) :
    QsciScintilla(pParent)
{
    // Customise ourselves

    setAutoIndent(true);
    setBraceMatching(QsciScintilla::SloppyBraceMatch);
    setCaretLineVisible(true);
    setFolding(QsciScintilla::BoxedTreeFoldStyle);
    setFrameShape(QFrame::NoFrame);
    setIndentationsUseTabs(false);
    setMarginLineNumbers(SC_MARGIN_NUMBER, true);
    setMatchedBraceBackgroundColor(Qt::white);
    setMatchedBraceForegroundColor(Qt::red);
    setTabWidth(4);
    setUtf8(true);

    // Set our font

#if defined(Q_OS_WIN)
    mFont = QFont("Lucida Console", 11);
#elif defined(Q_OS_LINUX)
    mFont = QFont("Monospace", 9);
#else
    mFont = QFont("Monaco", 12);
#endif

    setFont(mFont);
    setMarginsFont(mFont);

    // Use the given lexer

    setLexer(pLexer);

    // Force the use of UNIX EOL mode
    // Note: by default QScintilla will use EolWindows on Windows and EolUnix on
    //       Linux and macOS. However, the fact that it uses EolWindows on
    //       Windows can cause problems on that platform (with files not using a
    //       Windows EOL mode), so to be safe we use EolUnix on all platforms...

    setEolMode(EolUnix);

    // Initialise our colours by 'updating' them

    updateColors();

    // Clear some key mappings inherited from QsciScintilla
    // Note #1: indeed, QsciScintilla handles some shortcuts (e.g. Ctrl+L),
    //          which we don't want to see handled (e.g. Ctrl+L is used by
    //          QsciScintilla to delete the current line while we want OpenCOR
    //          to be able to use it to (un)lock the current file)...
    // Note #2: even though we are clearing those key mappings, we must also
    //          bypass QsciScintilla's handling of event() (see below). Indeed,
    //          not to do so would mean that if, for example, the user was to
    //          press Ctrl+L, then nothing would happen while we would have
    //          expected the current file to be (un)locked...

    int ctrlKey = 65536*SCMOD_CTRL;
    int shiftKey = 65536*SCMOD_SHIFT;

    SendScintilla(SCI_CLEARCMDKEY, ctrlKey+'/');
    SendScintilla(SCI_CLEARCMDKEY, ctrlKey+'D');
    SendScintilla(SCI_CLEARCMDKEY, ctrlKey+'L');
    SendScintilla(SCI_CLEARCMDKEY, ctrlKey+shiftKey+'L');
    SendScintilla(SCI_CLEARCMDKEY, ctrlKey+'R');
    SendScintilla(SCI_CLEARCMDKEY, ctrlKey+'T');

    // Add support for Specials (Unicode block)
    // Note #1: see issue #709 for more information...
    // Note #2: make sure that specials() supports the below representations...

    SendScintilla(SCI_SETREPRESENTATION, "\xef\xbf\xb9", "IAA");
    SendScintilla(SCI_SETREPRESENTATION, "\xef\xbf\xba", "IAS");
    SendScintilla(SCI_SETREPRESENTATION, "\xef\xbf\xbb", "IAT");
    SendScintilla(SCI_SETREPRESENTATION, "\xef\xbf\xbc", "OBJ");

    // Empty context menu by default

    mContextMenu = new QMenu(this);

    // Create our two labels to show our cursor position and editing mode

    mCursorPositionWidget = new QLabel(this);
    mEditingModeWidget = new QLabel(this);

    // Keep track of various changes

    connect(this, &QScintillaWidget::SCN_UPDATEUI,
            this, &QScintillaWidget::updateUi);

    connect(this, &QScintillaWidget::textChanged,
            this, &QScintillaWidget::updateMarginLineNumbersWidth);

    connect(this, &QScintillaWidget::selectionChanged,
            this, &QScintillaWidget::checkCanSelectAll);
    connect(this, &QScintillaWidget::textChanged,
            this, &QScintillaWidget::checkCanSelectAll);

    connect(this, &QScintillaWidget::cursorPositionChanged,
            this, &QScintillaWidget::updateCursorPosition);
}

//==============================================================================

QMenu * QScintillaWidget::contextMenu() const
{
    // Return our context menu

    return mContextMenu;
}

//==============================================================================

void QScintillaWidget::setContextMenu(const QList<QAction *> &pContextMenuActions)
{
    // Set our context menu

    mContextMenu->clear();

    for (auto action : pContextMenuActions) {
        mContextMenu->addAction(action);
    }
}

//==============================================================================

void QScintillaWidget::cursorPosition(int &pLine, int &pColumn)
{
    // Retrieve our cursor position

    getCursorPosition(&pLine, &pColumn);
}

//==============================================================================

void QScintillaWidget::setCursorPosition(int pLine, int pColumn)
{
    // Make sure that the line and column numbers make sense

    pLine = qMin(qMax(0, pLine), lines()-1);
    pColumn = qMin(qMax(0, pColumn), text(pLine).size()-1);

    // Make sure that the line is not within a folded block

    ensureLineVisible(pLine);

    // Center ourselves around the given line

    SendScintilla(SCI_LINESCROLL, 0, pLine-firstVisibleLine()-(SendScintilla(SCI_LINESONSCREEN)/2));

    // Set our cursor position

    QsciScintilla::setCursorPosition(pLine, pColumn);
}

//==============================================================================

void QScintillaWidget::setLexer(QsciLexer *pLexer)
{
    // Set our font for the given lexer, if any

    if (pLexer != nullptr) {
        pLexer->setFont(mFont);
    }

    QsciScintilla::setLexer(pLexer);
}

//==============================================================================

int QScintillaWidget::currentPosition() const
{
    // Return our current position

    return int(SendScintilla(SCI_GETCURRENTPOS));
}

//==============================================================================

void QScintillaWidget::setCurrentPosition(int pCurrentPosition)
{
    // Set our current position

    SendScintilla(SCI_SETCURRENTPOS, pCurrentPosition);
}

//==============================================================================

QString QScintillaWidget::contents() const
{
    // Return our contents

    return text();
}

//==============================================================================

void QScintillaWidget::setContents(const QString &pContents,
                                   bool pEmptyUndoBuffer)
{
    // Set our contents and empty our undo buffer, if required

    bool readOnly = isReadOnly();

    if (readOnly) {
        setReadOnly(false);
    }

    SendScintilla(SCI_SETTEXT, ScintillaBytesConstData(textAsBytes(pContents)));

    if (pEmptyUndoBuffer) {
        SendScintilla(SCI_EMPTYUNDOBUFFER);
    }

    setReadOnly(readOnly);
}

//==============================================================================

int QScintillaWidget::contentsSize() const
{
    // Return the size of our contents

    return int(SendScintilla(SCI_GETLENGTH));
}

//==============================================================================

QString QScintillaWidget::textInRange(int pStartRange, int pEndRange) const
{
    // Retrieve and return the text in the given range, after making sure that
    // the given range makes sense

    int maxRange = contentsSize();

    if (   (pStartRange < 0) || (pStartRange >= maxRange)
        || (pEndRange < 0) || (pEndRange >= maxRange)
        || (pStartRange >= pEndRange)) {
        return {};
    }

    auto text = new char[pEndRange-pStartRange+1] {};

    SendScintilla(SCI_GETTEXTRANGE, pStartRange, pEndRange, text);

    QString res = QString(text);

    delete[] text;

    return res;
}

//==============================================================================

int QScintillaWidget::findTextInRange(int pStartRange, int pEndRange,
                                      const QString &pText,
                                      bool pRegularExpression,
                                      bool pCaseSensitive,
                                      bool pWholeWordsOnly) const
{
    // Keep track of the start and end of the current target

    int crtTargetStart = int(SendScintilla(SCI_GETTARGETSTART));
    int crtTargetEnd = int(SendScintilla(SCI_GETTARGETEND));

    // Find and return the position, if any, of the given text in the given
    // range

    SendScintilla(SCI_SETSEARCHFLAGS,
                   (pRegularExpression?SCFIND_REGEXP:0)
                  +(pCaseSensitive?SCFIND_MATCHCASE:0)
                  +(pWholeWordsOnly?SCFIND_WHOLEWORD:0));

    SendScintilla(SCI_SETTARGETSTART, pStartRange);
    SendScintilla(SCI_SETTARGETEND, pEndRange);

    QByteArray byteArray = pText.toUtf8();

    int res = int(SendScintilla(SCI_SEARCHINTARGET, uintptr_t(byteArray.length()), byteArray.constData()));

    // Retrieve the start and end of the current target

    SendScintilla(SCI_SETTARGETSTART, crtTargetStart);
    SendScintilla(SCI_SETTARGETEND, crtTargetEnd);

    return res;
}

//==============================================================================

QString QScintillaWidget::wordAt(int pLine, int pColumn) const
{
    // Return the word, if any, at the given line/column

    return wordAtLineIndex(pLine, pColumn);
}

//==============================================================================

void QScintillaWidget::selectWordAt(int pLine, int pColumn)
{
    // Select the word, if any, at the given line/column

    auto position = ulong(positionFromLineIndex(pLine, pColumn));

    int startPosition = int(SendScintilla(SCI_WORDSTARTPOSITION, position, 1));
    int endPosition = int(SendScintilla(SCI_WORDENDPOSITION, position, 1));

    if (endPosition-startPosition > 0) {
        SendScintilla(SCI_SETSEL, ulong(startPosition), endPosition);
    }
}

//==============================================================================

bool QScintillaWidget::isSelectAllAvailable() const
{
    // Return whether we can select all the text

    return mCanSelectAll;
}

//==============================================================================

void QScintillaWidget::del()
{
    // Delete the selected text, if any

    SendScintilla(SCI_CLEAR);
}

//==============================================================================

void QScintillaWidget::resetUndoHistory()
{
    // Reset our undo history

    SendScintilla(SCI_EMPTYUNDOBUFFER);
}

//==============================================================================

QLabel * QScintillaWidget::cursorPositionWidget() const
{
    // Return our cursort position widget

    return mCursorPositionWidget;
}

//==============================================================================

QLabel * QScintillaWidget::editingModeWidget() const
{
    // Return our editing mode widget

    return mEditingModeWidget;
}

//==============================================================================

QString QScintillaWidget::eolString() const
{
    // Return the end of line we use

    switch (eolMode()) {
    case EolWindows:
        return "\r\n";
    case EolUnix:
        return "\n";
    case EolMac:
        return "\r";
    }

    return {};
    // Note: we can't reach this point, but without it we may, at compilation
    //       time, be told that not all control paths return a value...
}

//==============================================================================

QColor QScintillaWidget::backgroundColor(int pStyle)
{
    // Return the background color for the given style

    return QRgb(SendScintilla(SCI_STYLEGETBACK, pStyle));
}

//==============================================================================

void QScintillaWidget::setBackgroundColor(int pStyle,
                                          const QColor &pBackgroundColor)
{
    // Set the background color for the given style

    SendScintilla(SCI_STYLESETBACK, ulong(pStyle), pBackgroundColor);
}

//==============================================================================

QColor QScintillaWidget::foregroundColor(int pStyle)
{
    // Return the foreground color for the given style

    return QRgb(SendScintilla(SCI_STYLEGETFORE, pStyle));
}

//==============================================================================

void QScintillaWidget::setForegroundColor(int pStyle,
                                          const QColor &pForegroundColor)
{
    // Set the foreground color for the given style

    SendScintilla(SCI_STYLESETFORE, ulong(pStyle), pForegroundColor);
}

//==============================================================================

int QScintillaWidget::zoomLevel() const
{
    // Return our zoom level

    return int(SendScintilla(SCI_GETZOOM));
}

//==============================================================================

bool QScintillaWidget::handleChanges() const
{
    // Return our changes should be handled

    return mHandleChanges;
}

//==============================================================================

void QScintillaWidget::setHandleChanges(bool pHandleChanges)
{
    // Set whether we should handle changes

    mHandleChanges = pHandleChanges;

    // Emit a few signals, if we should handle changes
    // Note: this is to ensure that our owner is up-to-date with whatever we
    //       have done when changes weren't handled...

    if (pHandleChanges) {
        int line;
        int column;

        cursorPosition(line, column);

        emit textChanged();
        emit cursorPositionChanged(line, column);
    }
}

//==============================================================================

QString QScintillaWidget::specials(const QString &pString)
{
    // Check whether the given string is one of the Specials (Unicode block)
    // Note: see the customised representation of the Specials (Unicode block)
    //       in the constructor...

    static const QString IAA = "\xef\xbf\xb9";
    static const QString IAS = "\xef\xbf\xba";
    static const QString IAT = "\xef\xbf\xbb";
    static const QString OBJ = "\xef\xbf\xbc";

    if (pString == IAA) {
        return "IAA";
    }

    if (pString == IAS) {
        return "IAS";
    }

    if (pString == IAT) {
        return "IAT";
    }

    if (pString == OBJ) {
        return "OBJ";
    }

    return pString;
}

//==============================================================================

void QScintillaWidget::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    QsciScintilla::changeEvent(pEvent);

    // Check whether the palette has changed and if so then update our colors

    if (pEvent->type() == QEvent::PaletteChange) {
        updateColors();
    }
}

//==============================================================================

void QScintillaWidget::contextMenuEvent(QContextMenuEvent *pEvent)
{
    // Show our context menu or QsciScintilla's one, if we don't have one

    if (mContextMenu->isEmpty()) {
        QsciScintilla::contextMenuEvent(pEvent);
    } else {
        mContextMenu->exec(pEvent->globalPos());
    }
}

//==============================================================================

void QScintillaWidget::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // Accept the proposed action for the event, but only if we are not dropping
    // URIs
    // Note: this is not (currently?) needed on Windows and macOS, but if we
    //       don't have that check on Linux, then to drop some files on our
    //       Scintilla editor will result in the text/plain version of the data
    //       (e.g. file:///home/me/myFile) to be inserted in the text...

    if (!pEvent->mimeData()->hasFormat(Core::FileSystemMimeType)) {
        pEvent->acceptProposedAction();
    } else {
        pEvent->ignore();
    }
}

//==============================================================================

bool QScintillaWidget::event(QEvent *pEvent)
{
    // Bypass QsciScintilla's handling of event()
    // Note: see the note on the clearing of some key mappings in the
    //       constructor above...

    return QsciScintillaBase::event(pEvent); // NOLINT(bugprone-parent-virtual-call)
}

//==============================================================================

void QScintillaWidget::focusInEvent(QFocusEvent *pEvent)
{
    // We are getting the focus, so make sure that our caret is visible

    setCaretWidth(1);

    // Default handling of the event

    QsciScintilla::focusInEvent(pEvent);
}

//==============================================================================

void QScintillaWidget::focusOutEvent(QFocusEvent *pEvent)
{
    // We are losing the focus, so hide our caret

    setCaretWidth(0);

    // Default handling of the event

    QsciScintilla::focusOutEvent(pEvent);
}

//==============================================================================

void QScintillaWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Increase/decrease/reset the font size, if needed

    if (   (pEvent->modifiers() == Qt::ControlModifier)
        || (pEvent->modifiers() == Qt::ControlModifier+Qt::KeypadModifier)) {
        if (pEvent->key() == Qt::Key_0) {
            zoomTo(0);

            pEvent->accept();
        } else if (   (pEvent->key() == Qt::Key_Plus)
                   || (pEvent->key() == Qt::Key_Equal)) {
            zoomIn();

            pEvent->accept();
        } else if (pEvent->key() == Qt::Key_Minus) {
            zoomOut();

            pEvent->accept();
        } else {
            // Default handling of the event

            QsciScintilla::keyPressEvent(pEvent);
        }
    } else {
        // Default handling of the event

        QsciScintilla::keyPressEvent(pEvent);
    }
}

//==============================================================================

void QScintillaWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Increasing/decrease the font size, if needed

    if (pEvent->modifiers() == Qt::ControlModifier) {
        int delta = pEvent->delta();

        if (delta > 0) {
            zoomIn();
        } else if (delta < 0) {
            zoomOut();
        }

        pEvent->accept();
    } else {
        QsciScintilla::wheelEvent(pEvent);
    }
}

//==============================================================================

void QScintillaWidget::zoomIn()
{
    // Zoom in the default way and then update our margin line numbers width

    QsciScintilla::zoomIn();

    updateMarginLineNumbersWidth();
}

//==============================================================================

void QScintillaWidget::zoomOut()
{
    // Zoom out the default way and then update our margin line numbers width

    QsciScintilla::zoomOut();

    updateMarginLineNumbersWidth();
}

//==============================================================================

void QScintillaWidget::zoomTo(int pSize)
{
    // Zoom to the default way and then update our margin line numbers width

    QsciScintilla::zoomTo(pSize);

    updateMarginLineNumbersWidth();
}

//==============================================================================

void QScintillaWidget::undo()
{
    // Undo the last action, but without handling changes since this may slow
    // things done (e.g. when the last action was a replacing all action)

    setHandleChanges(false);
        QsciScintilla::undo();
    setHandleChanges(true);
}

//==============================================================================

void QScintillaWidget::redo()
{
    // Redo the last action, but without handling changes since this may slow
    // things done (e.g. when the last action was a replacing all action)

    setHandleChanges(false);
        QsciScintilla::redo();
    setHandleChanges(true);
}

//==============================================================================

void QScintillaWidget::updateUi()
{
    // Make sure that we are allowed to handle connections

    if (!mHandleChanges) {
        return;
    }

    // Update our editing mode, if needed

    bool newInsertMode = !overwriteMode();

    if (   (newInsertMode != mInsertMode)
        || mEditingModeWidget->text().isEmpty()) {
        mInsertMode = newInsertMode;

        mEditingModeWidget->setText(mInsertMode?"INS":"OVR");
    }
}

//==============================================================================

void QScintillaWidget::updateMarginLineNumbersWidth()
{
    // Make sure that we are allowed to handle connections

    if (!mHandleChanges) {
        return;
    }

    // Resize the margin line numbers width
    // Note: the +6 is to ensure that the margin line numbers width is indeed
    //       wide enough (there is clearly a 'problem' with the width computed
    //       by Scintilla)...

    setMarginWidth(SC_MARGIN_NUMBER,
                   int(SendScintilla(SCI_TEXTWIDTH, STYLE_LINENUMBER,
                                     ScintillaBytesConstData(textAsBytes(QString::number(lines()))))+6));
}

//==============================================================================

void QScintillaWidget::checkCanSelectAll()
{
    // Make sure that we are allowed to handle connections

    if (!mHandleChanges) {
        return;
    }

    // Check whether we can select all the text

    bool newCanSelectAll = !text().isEmpty() && (selectedText() != text());

    if (newCanSelectAll != mCanSelectAll) {
        mCanSelectAll = newCanSelectAll;

        emit canSelectAll(mCanSelectAll);
    }
}

//==============================================================================

void QScintillaWidget::updateColors()
{
    // Compute and set the background colour of our caret line

    static const qreal Threshold = 0.875;

    QColor caretLineBackgroundColor = Core::highlightColor();

    qreal red = caretLineBackgroundColor.redF();
    qreal green = caretLineBackgroundColor.greenF();
    qreal blue = caretLineBackgroundColor.blueF();

    while ((red < Threshold) || (green < Threshold) || (blue < Threshold)) {
        red = 0.5*(red+1.0);
        green = 0.5*(green+1.0);
        blue = 0.5*(blue+1.0);
    }

    setCaretLineBackgroundColor(qRgba(int(red*255), int(green*255), int(blue*255),
                                      caretLineBackgroundColor.alpha()));
}

//==============================================================================

void QScintillaWidget::updateCursorPosition(int pLine, int pColumn)
{
    // Make sure that we are allowed to handle connections

    if (!mHandleChanges) {
        return;
    }

    // Update our cursor position

    mCursorPositionWidget->setText(QString("Line: %1, Col: %2").arg(pLine+1)
                                                               .arg(pColumn+1));
}

//==============================================================================

} // namespace QScintillaWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
