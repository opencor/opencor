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

#include "Qsci/qscilexer.h"

//==============================================================================

namespace OpenCOR {
namespace QScintillaSupport {

//==============================================================================

QScintillaWidget::QScintillaWidget(QsciLexer *pLexer, QWidget *pParent) :
    QsciScintilla(pParent),
    mCanSelectAll(false),
    mInsertMode(true)
{
    // Customise ourselves

    setAutoIndent(true);
    setBraceMatching(QsciScintilla::SloppyBraceMatch);
    setCaretLineVisible(true);
    setFolding(QsciScintilla::BoxedTreeFoldStyle);
    setFrameShape(QFrame::NoFrame);
    setIndentationsUseTabs(false);
    setMarginWidth(SC_MARGIN_NUMBER, 0);
    setMatchedBraceBackgroundColor(Qt::white);
    setMatchedBraceForegroundColor(Qt::red);
    setTabWidth(4);
    setUtf8(true);

    // Set our font

#if defined(Q_OS_WIN)
    mFont = QFont("Lucida Console", 11);
#elif defined(Q_OS_LINUX)
    mFont = QFont("Monospace", 9);
#elif defined(Q_OS_MAC)
    mFont = QFont("Monaco", 12);
#else
    #error Unsupported platform
#endif

    setFont(mFont);

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

    SendScintilla(SCI_CLEARCMDKEY, (SCMOD_CTRL << 16)+'/');
    SendScintilla(SCI_CLEARCMDKEY, (SCMOD_CTRL << 16)+'D');
    SendScintilla(SCI_CLEARCMDKEY, (SCMOD_CTRL << 16)+'L');
    SendScintilla(SCI_CLEARCMDKEY, (SCMOD_CTRL << 16)+(SCMOD_SHIFT << 16)+'L');
    SendScintilla(SCI_CLEARCMDKEY, (SCMOD_CTRL << 16)+'R');
    SendScintilla(SCI_CLEARCMDKEY, (SCMOD_CTRL << 16)+'T');

    // Add support for Specials (Unicode block)
    // Note #1: see issue #709 for more information...
    // Note #2: make sure that checkString() supports the below
    //          representations...

    SendScintilla(SCI_SETREPRESENTATION, "\xef\xbf\xb9", "IAA");
    SendScintilla(SCI_SETREPRESENTATION, "\xef\xbf\xba", "IAS");
    SendScintilla(SCI_SETREPRESENTATION, "\xef\xbf\xbb", "IAT");
    SendScintilla(SCI_SETREPRESENTATION, "\xef\xbf\xbc", "OBJ");

    // Empty context menu by default

    mContextMenu = new QMenu(this);

    // Create our two labels to show our cursor position and editing mode

    mCursorPositionWidget = new QLabel(this);
    mEditingModeWidget = new QLabel(this);

    // Keep track of the change to the UI

    connect(this, SIGNAL(SCN_UPDATEUI(int)),
            this, SLOT(updateUi()));

    // Keep track of changes to our editor that may affect our ability to select
    // all of its text

    connect(this, SIGNAL(selectionChanged()),
            this, SLOT(checkCanSelectAll()));
    connect(this, SIGNAL(textChanged()),
            this, SLOT(checkCanSelectAll()));

    // Keep track of the change in the cursor position

    connect(this, SIGNAL(cursorPositionChanged(int, int)),
            this, SLOT(cursorPositionChanged(const int &, const int &)));
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

    foreach (QAction *action, pContextMenuActions)
        mContextMenu->addAction(action);
}

//==============================================================================

void QScintillaWidget::cursorPosition(int &pLine, int &pColumn)
{
    // Retrieve our cursor position

    QsciScintilla::getCursorPosition(&pLine, &pColumn);
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

    SendScintilla(SCI_LINESCROLL, 0, pLine-firstVisibleLine()-0.5*SendScintilla(SCI_LINESONSCREEN));

    // Set our cursor position

    QsciScintilla::setCursorPosition(pLine, pColumn);
}

//==============================================================================

void QScintillaWidget::setLexer(QsciLexer *pLexer)
{
    // Set our font for the given lexer, if any

    if (pLexer)
        pLexer->setFont(mFont);

    QsciScintilla::setLexer(pLexer);
}

//==============================================================================

int QScintillaWidget::currentPosition() const
{
    // Return our current position

    return SendScintilla(SCI_GETCURRENTPOS);
}

//==============================================================================

void QScintillaWidget::setCurrentPosition(const int &pCurrentPosition)
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
                                   const bool &pKeepHistory)
{
    // Set our contents and keep our history, if required

    if (pKeepHistory) {
        bool readOnly = isReadOnly();

        if (readOnly)
            setReadOnly(false);

        SendScintilla(SCI_SETTEXT, ScintillaBytesConstData(textAsBytes(pContents)));

        setReadOnly(readOnly);
    } else {
        setText(pContents);
    }
}

//==============================================================================

int QScintillaWidget::contentsSize() const
{
    // Return the size of our contents

    return SendScintilla(SCI_GETLENGTH);
}

//==============================================================================

QString QScintillaWidget::textInRange(const int &pStartRange,
                                      const int &pEndRange) const
{
    // Retrieve and return the text in the given range, after making sure that
    // the given range makes sense

    int maxRange = contentsSize();

    if (   (pStartRange < 0) || (pStartRange >= maxRange)
        || (pEndRange < 0) || (pEndRange >= maxRange)
        || (pStartRange >= pEndRange)) {
        return QString();
    }

    char *text = new char[pEndRange-pStartRange+1];

    SendScintilla(SCI_GETTEXTRANGE, pStartRange, pEndRange, text);

    QString res = QString(text);

    delete[] text;

    return res;
}

//==============================================================================

int QScintillaWidget::findTextInRange(const int &pStartRange,
                                      const int &pEndRange,
                                      const QString &pText,
                                      const bool &pRegularExpression,
                                      const bool &pCaseSensitive,
                                      const bool &pWholeWordsOnly) const
{
    // Keep track of the start and end of the current target

    int crtTargetStart = SendScintilla(SCI_GETTARGETSTART);
    int crtTargetEnd = SendScintilla(SCI_GETTARGETEND);

    // Find and return the position, if any, of the given text within the given
    // range

    SendScintilla(SCI_SETSEARCHFLAGS,
                   (pRegularExpression?SCFIND_REGEXP:0)
                  +(pCaseSensitive?SCFIND_MATCHCASE:0)
                  +(pWholeWordsOnly?SCFIND_WHOLEWORD:0));

    SendScintilla(SCI_SETTARGETSTART, pStartRange);
    SendScintilla(SCI_SETTARGETEND, pEndRange);

    QByteArray byteArray = pText.toUtf8();

    int res = SendScintilla(SCI_SEARCHINTARGET, byteArray.length(), byteArray.constData());

    // Retrieve the start and end of the current target

    SendScintilla(SCI_SETTARGETSTART, crtTargetStart);
    SendScintilla(SCI_SETTARGETEND, crtTargetEnd);

    return res;
}

//==============================================================================

QString QScintillaWidget::wordAt(const int &pLine, const int &pColumn) const
{
    // Return the current word, if any

    return wordAtLineIndex(pLine, pColumn);
}

//==============================================================================

void QScintillaWidget::selectWordAt(const int &pLine, const int &pColumn)
{
    // Return the current word, if any

    int position = positionFromLineIndex(pLine, pColumn);

    int startPosition = SendScintilla(SCI_WORDSTARTPOSITION, position, true);
    int endPosition = SendScintilla(SCI_WORDENDPOSITION, position, true);

    if (endPosition-startPosition > 0) {
        setSelection(SendScintilla(SCI_LINEFROMPOSITION, startPosition),
                     SendScintilla(SCI_GETCOLUMN, startPosition),
                     SendScintilla(SCI_LINEFROMPOSITION, endPosition),
                     SendScintilla(SCI_GETCOLUMN, endPosition));
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

    return QString();
    // Note: we can't reach this point, but without it we may be told that not
    //       all control paths return a value...
}

//==============================================================================

QColor QScintillaWidget::backgroundColor(const int &pStyle)
{
    // Return the background color for the given style

    return SendScintilla(SCI_STYLEGETBACK, pStyle);
}

//==============================================================================

void QScintillaWidget::setBackgroundColor(const int &pStyle,
                                          const QColor &pBackgroundColor)
{
    // Set the background color for the given style

    SendScintilla(SCI_STYLESETBACK, pStyle, pBackgroundColor);
}

//==============================================================================

QColor QScintillaWidget::foregroundColor(const int &pStyle)
{
    // Return the foreground color for the given style

    return SendScintilla(SCI_STYLEGETFORE, pStyle);
}

//==============================================================================

void QScintillaWidget::setForegroundColor(const int &pStyle,
                                          const QColor &pForegroundColor)
{
    // Set the foreground color for the given style

    SendScintilla(SCI_STYLESETFORE, pStyle, pForegroundColor);
}

//==============================================================================

int QScintillaWidget::zoomLevel() const
{
    // Return our zoom level

    return SendScintilla(SCI_GETZOOM);
}

//==============================================================================

QString QScintillaWidget::checkString(const QString &pString)
{
    // Check whether the given string is one of the Specials (Unicode block)
    // Note: see the customised representation of the Specials (Unicode block)
    //       in the constructor...

    if (!pString.compare("\xef\xbf\xb9"))
        return "IAA";
    else if (!pString.compare("\xef\xbf\xba"))
        return "IAS";
    else if (!pString.compare("\xef\xbf\xbb"))
        return "IAT";
    else if (!pString.compare("\xef\xbf\xbc"))
        return "OBJ";
    else
        return pString;
}

//==============================================================================

void QScintillaWidget::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    QsciScintilla::changeEvent(pEvent);

    // Check whether the palette has changed and if so then update our colors

    if (pEvent->type() == QEvent::PaletteChange)
        updateColors();
}

//==============================================================================

void QScintillaWidget::contextMenuEvent(QContextMenuEvent *pEvent)
{
    // Show our context menu or QsciScintilla's one, if we don't have one

    if (mContextMenu->isEmpty())
        QsciScintilla::contextMenuEvent(pEvent);
    else
        mContextMenu->exec(pEvent->globalPos());
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

    if (!pEvent->mimeData()->hasFormat(Core::FileSystemMimeType))
        pEvent->acceptProposedAction();
    else
        pEvent->ignore();
}

//==============================================================================

bool QScintillaWidget::event(QEvent *pEvent)
{
    // Bypass QsciScintilla's handling of event()
    // Note: see the note on the clearing of some key mappings in the contructor
    //       above...

    return QsciScintillaBase::event(pEvent);
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
    // Let people know that a key has been pressed

    bool handled = false;

    emit keyPressed(pEvent, handled);

    // Carry on as normal, if the event wasn't handled

    if (handled) {
        // Accept the event

        pEvent->accept();
    } else {
        // Reset the font size, if needed

        if (   !(pEvent->modifiers() & Qt::ShiftModifier)
            &&  (pEvent->modifiers() & Qt::ControlModifier)
            && !(pEvent->modifiers() & Qt::AltModifier)
            && !(pEvent->modifiers() & Qt::MetaModifier)) {
            if (pEvent->key() == Qt::Key_0) {
                zoomTo(0);

                // Accept the event

                pEvent->accept();
            } else if (pEvent->key() == Qt::Key_Equal) {
                zoomIn();

                // Accept the event

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
}

//==============================================================================

void QScintillaWidget::wheelEvent(QWheelEvent *pEvent)
{
    // Increasing/decrease the font size, if needed

    if (pEvent->modifiers() == Qt::ControlModifier) {
        int delta = pEvent->delta();

        if (delta > 0)
            zoomIn();
        else if (delta < 0)
            zoomOut();

        pEvent->accept();
    } else {
        QsciScintilla::wheelEvent(pEvent);
    }
}

//==============================================================================

void QScintillaWidget::updateUi()
{
    // Update our editing mode, if needed

    bool newInsertMode = !overwriteMode();

    if (   (newInsertMode != mInsertMode)
        || mEditingModeWidget->text().isEmpty()) {
        mInsertMode = newInsertMode;

        mEditingModeWidget->setText(mInsertMode?"INS":"OVR");
    }
}

//==============================================================================

void QScintillaWidget::checkCanSelectAll()
{
    // Check whether we can select all the text

    bool newCanSelectAll = !text().isEmpty() && selectedText().compare(text());

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

    setCaretLineBackgroundColor(qRgba(red*255, green*255, blue*255, caretLineBackgroundColor.alpha()));
}

//==============================================================================

void QScintillaWidget::cursorPositionChanged(const int &pLine,
                                             const int &pColumn)
{
    // Update our cursor position

    mCursorPositionWidget->setText(QString("Line: %1, Col: %2").arg(QString::number(pLine+1),
                                                                    QString::number(pColumn+1)));
}

//==============================================================================

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
