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

#include "coreguiutils.h"
#include "filemanager.h"
#include "qscintillawidget.h"

//==============================================================================

#include <Qt>

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

    setCaretLineVisible(true);
    setFrameShape(QFrame::NoFrame);
    setMarginWidth(SC_MARGIN_NUMBER, 0);
    setUtf8(true);

    // Associate a lexer to our Scintilla editor, should one be provided
    // Note: the default font family and size come from Qt Creator...

#if defined(Q_OS_WIN)
    mFont = QFont("Courier", 10);
#elif defined(Q_OS_LINUX)
    mFont = QFont("Monospace", 9);
#elif defined(Q_OS_MAC)
    mFont = QFont("Monaco", 12);
#else
    #error Unsupported platform
#endif

    if (pLexer) {
        // A lexer was provided, so specify its fonts and associate it with our
        // Scintilla editor

        pLexer->setFont(mFont);

        setLexer(pLexer);

        // Specify the type of tree folding to be used (some lexers may indeed
        // use that feature)

        setFolding(QsciScintilla::BoxedTreeFoldStyle);
    } else {
        // No lexer was provided, so simply specify a default font family and
        // size for our Scintilla editor

        setFont(mFont);
    }

    // Force the use of UNIX EOL mode
    // Note: by default QScintilla will use EolWindows on Windows and EolUnix on
    //       Linux and OS X. However, the fact that it uses EolWindows on
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

    SendScintilla(SCI_CLEARCMDKEY, (SCMOD_CTRL << 16)+'D');
    SendScintilla(SCI_CLEARCMDKEY, (SCMOD_CTRL << 16)+'L');
    SendScintilla(SCI_CLEARCMDKEY, (SCMOD_CTRL << 16)+(SCMOD_SHIFT << 16)+'L');

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

void QScintillaWidget::setCursorPosition(int pLine, int pColumn)
{
    // Make sure that the line is not within a folded block

    ensureLineVisible(pLine);

    // Center ourselves around the given line

    SendScintilla(SCI_LINESCROLL, 0, pLine-firstVisibleLine()-0.5*SendScintilla(SCI_LINESONSCREEN));

    // Set our cursor position

    QsciScintilla::setCursorPosition(pLine, pColumn);
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

void QScintillaWidget::setContents(const QString &pContents)
{
    // Set our contents

    setText(pContents);
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
    // Retrieve and return the text in the given range, making sure that the
    // given range makes sense

    int maxRange = contentsSize();

    if (   (pStartRange < 0) || (pStartRange >= maxRange)
        || (pEndRange < 0) || (pEndRange >= maxRange)
        || (pStartRange >= pEndRange))
        return QString();

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
                                      const bool &pCaseSensitive) const
{
    // Keep track of the start and end of the current target

    int currentTargetStart = SendScintilla(SCI_GETTARGETSTART);
    int currentTargetEnd = SendScintilla(SCI_GETTARGETEND);

    // Find and return the position, if any, of the given text within the given
    // range

    SendScintilla(SCI_SETSEARCHFLAGS, pCaseSensitive?SCFIND_MATCHCASE:0);

    SendScintilla(SCI_SETTARGETSTART, pStartRange);
    SendScintilla(SCI_SETTARGETEND, pEndRange);

    QByteArray byteArray = pText.toUtf8();

    int res = SendScintilla(SCI_SEARCHINTARGET, byteArray.length(), byteArray.constData());

    // Retrieve the start and end of the current target

    SendScintilla(SCI_SETTARGETSTART, currentTargetStart);
    SendScintilla(SCI_SETTARGETEND, currentTargetEnd);

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

    if (endPosition-startPosition > 0)
        setSelection(SendScintilla(SCI_LINEFROMPOSITION, startPosition),
                     SendScintilla(SCI_GETCOLUMN, startPosition),
                     SendScintilla(SCI_LINEFROMPOSITION, endPosition),
                     SendScintilla(SCI_GETCOLUMN, endPosition));
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
    case EolUnix:
        return "\n";
    case EolMac:
        return "\r";
    default:   // EolWindows
        return "\r\n";
    }
}

//==============================================================================

void QScintillaWidget::setBackgroundColor(const int &pStyle,
                                          const QColor &pBackgroundColor)
{
    // Set the background color for the given style

    SendScintilla(SCI_STYLESETBACK, pStyle, pBackgroundColor);
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
    // Note: this is not (currently?) needed on Windows and OS X, but if we
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

    bool newCanSelectAll = text().size() && selectedText().compare(text());

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

    qreal r = caretLineBackgroundColor.redF();
    qreal g = caretLineBackgroundColor.greenF();
    qreal b = caretLineBackgroundColor.blueF();

    while ((r < Threshold) || (g < Threshold) || (b < Threshold)) {
        r = 0.5*(r+1.0);
        g = 0.5*(g+1.0);
        b = 0.5*(b+1.0);
    }

    setCaretLineBackgroundColor(qRgba(r*255, g*255, b*255, caretLineBackgroundColor.alpha()));
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
