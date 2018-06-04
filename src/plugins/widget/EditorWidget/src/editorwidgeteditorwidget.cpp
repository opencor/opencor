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

#include "coreguiutils.h"
#include "editorwidget.h"
#include "editorwidgeteditorwidget.h"
#include "editorwidgetfindreplacewidget.h"
#include "editorwidgetscrollbar.h"

//==============================================================================

#include <QKeyEvent>
#include <QMainWindow>

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

EditorWidgetEditorWidget::EditorWidgetEditorWidget(QsciLexer *pLexer,
                                                   EditorWidgetFindReplaceWidget *pFindReplace,
                                                   EditorWidget *pParent) :
    QScintillaSupport::QScintillaWidget(pLexer, pParent),
    mOwner(pParent),
    mFindReplace(pFindReplace),
    mReadOnlyStyles(QIntList()),
    mHighlightedLines(QIntList()),
    mTexts(QStringList())
{
    // Use our own vertical scroll bar for our editor, so that we can show the
    // position of our highlighting

    mVerticalScrollBar = new EditorWidgetScrollBar(this);

    replaceVerticalScrollBar(mVerticalScrollBar);

    // Check which styles can have their background changed when making them
    // read-only or writable

    QColor baseColor = Core::baseColor();

    for (int i = 0; i < QsciScintillaBase::STYLE_MAX; ++i) {
        if (backgroundColor(i) == baseColor)
            mReadOnlyStyles << i;
    }

    // Define and customise an indicator for our highlighting

    static const QColor HihghlightingColor = QColor(255, 239, 11, 69);

    mHighlightIndicatorNumber = indicatorDefine(QsciScintilla::StraightBoxIndicator);

    setIndicatorForegroundColor(HihghlightingColor, mHighlightIndicatorNumber);

    // Update our vertical scroll bar whenever our cursor position changes
    // Note: we cannot use the new connect() syntax since the signal is located
    //       in our QScintilla plugin and that we don't know anything about
    //       it...

    connect(this, SIGNAL(cursorPositionChanged(int, int)),
            mVerticalScrollBar, SLOT(update()));
}

//==============================================================================

void EditorWidgetEditorWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Hide our corresponding find/replace widget, if needed

    if (    mFindReplace->isVisible()
        && !(pEvent->modifiers() & Qt::ShiftModifier)
        && !(pEvent->modifiers() & Qt::ControlModifier)
        && !(pEvent->modifiers() & Qt::AltModifier)
        && !(pEvent->modifiers() & Qt::MetaModifier)
        &&  (pEvent->key() == Qt::Key_Escape)) {
        mOwner->setFindReplaceVisible(false);

        pEvent->accept();
    } else {
        // Get any derived class a chance to handle our editor's key press
        // event, if they want

        if (mOwner->handleEditorKeyPressEvent(pEvent)) {
            pEvent->accept();

            return;
        }

        // Default handling of the event

        QScintillaSupport::QScintillaWidget::keyPressEvent(pEvent);

        // Update our highlighting all if our find/replace widget is visible and
        // has some find text and we didn't press one of the navigation keys,
        // clear our highlighting if we pressed any key and our find/replace
        // widget is not visible, forward the pressing of the Esc key but only
        // on macOS (so we can exit full-screen mode, if needed)
        // Note #1: regarding highlighting all, we need to do that in case we
        //          have some highlighting and then add some text, in which case
        //          the new text might indeed need highlighting and our vertical
        //          scroll bar updating...
        // Note #2: if our find/replace widget is visible and has some find
        //          text, but that we press one of the navigation keys then we
        //          don't need and shouldn't do highlighting all since it will
        //          lose any selection that we might have...

        if (    mFindReplace->isVisible() && !mFindReplace->findText().isEmpty()
            && !(pEvent->modifiers() & Qt::ShiftModifier)
            && !(pEvent->modifiers() & Qt::ControlModifier)
            && !(pEvent->modifiers() & Qt::AltModifier)
            && !(pEvent->modifiers() & Qt::MetaModifier)
            &&  (pEvent->key() != Qt::Key_Tab) && (pEvent->key() != Qt::Key_Backtab)
            &&  (pEvent->key() != Qt::Key_Up) && (pEvent->key() != Qt::Key_Down)
            &&  (pEvent->key() != Qt::Key_Left) && (pEvent->key() != Qt::Key_Right)
            &&  (pEvent->key() != Qt::Key_Home) && (pEvent->key() != Qt::Key_End)
            &&  (pEvent->key() != Qt::Key_PageUp) && (pEvent->key() != Qt::Key_PageDown)) {
            highlightAll();

            pEvent->accept();
#ifdef Q_OS_MAC
        } else if (   !(pEvent->modifiers() & Qt::ShiftModifier)
                   && !(pEvent->modifiers() & Qt::ControlModifier)
                   && !(pEvent->modifiers() & Qt::AltModifier)
                   && !(pEvent->modifiers() & Qt::MetaModifier)
                   &&  (pEvent->key() == Qt::Key_Escape)) {
            QCoreApplication::sendEvent(Core::mainWindow(), pEvent);
#endif
        } else if (!mFindReplace->isVisible()) {
            clearHighlighting();

            pEvent->accept();
        } else {
            pEvent->accept();
        }
    }
}

//==============================================================================

void EditorWidgetEditorWidget::mouseDoubleClickEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QScintillaSupport::QScintillaWidget::mouseDoubleClickEvent(pEvent);

    // Check whether something has been selected and, if so, highlight all of
    // its occurrences
    // Note #1: we must inactivate (and then reactivate) our find/replace widget
    //          before setting its find text otherwise we will be highlighting
    //          things twice...
    // Note #2: we need to (re)select text since highlighting all of its
    //          occurrences will clear the selection...

    if (hasSelectedText()) {
        int line;
        int column;

        getCursorPosition(&line, &column);

        mFindReplace->setActive(false);
            mFindReplace->setFindText(selectedText());
        mFindReplace->setActive(true);

        highlightAll();
        selectWordAt(line, column);
    }
}

//==============================================================================

void EditorWidgetEditorWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QScintillaSupport::QScintillaWidget::mousePressEvent(pEvent);

    // Clear any highlighting that we might have, if our find/replace widget is
    // not visible

    if (!mFindReplace->isVisible())
        clearHighlighting();
}

//==============================================================================

void EditorWidgetEditorWidget::setReadOnly(bool pReadOnly)
{
    // Default handling of the method

    QScintillaSupport::QScintillaWidget::setReadOnly(pReadOnly);

    // Update our background colour

    foreach (int readOnlyStyle, mReadOnlyStyles) {
        setBackgroundColor(readOnlyStyle,
                           pReadOnly?
                               Core::lockedColor(Core::baseColor()):
                               Core::baseColor());
    }
}

//==============================================================================

QIntList EditorWidgetEditorWidget::highlightedLines() const
{
    // Return our highlighted lines

    return mHighlightedLines;
}

//==============================================================================

void EditorWidgetEditorWidget::clearHighlighting()
{
    // Clear the current highlighting

    if (!mHighlightedLines.isEmpty()) {
        SendScintilla(SCI_SETINDICATORCURRENT, mHighlightIndicatorNumber);
        SendScintilla(SCI_INDICATORCLEARRANGE, 0, text().length());

        mHighlightedLines = QIntList();

        mVerticalScrollBar->update();
    }
}

//==============================================================================

void EditorWidgetEditorWidget::doHighlightReplaceAll(bool pHighlightAll)
{
    // Highlight/replace all the occurences of the text
    // Note: the original plan was to call findFirst() the first time round and
    //       with wrapping disabled, and then findNext() until we had found all
    //       the occurrences, but for some reasons this can result in some
    //       occurrences being missed, hence the way we do it below...

    // Clear any previous hihghlighting, in case we want to highlight all the
    // occurences of the text

    if (pHighlightAll)
        clearHighlighting();

    // Carry on, but only if there is really something to highlight, in case we
    // want to highlight all the occurences of the text

    QString findText = mFindReplace->findText();

    if (pHighlightAll && findText.trimmed().isEmpty())
        return;

    // Keep track of the first visible line, of our current line/column, and of
    // the position of our scrollbars

    int crtFirstVisibleLine = firstVisibleLine();
    int line;
    int column;

    getCursorPosition(&line, &column);

    int horizontalScrollBarPosition = horizontalScrollBar()->value();
    int verticalScrollBarPosition = verticalScrollBar()->value();

    // Stop tracking changes if we are replacing all

    if (!pHighlightAll)
        trackChanges(false);

    // Go to the beginning of our editor

    QsciScintilla::setCursorPosition(0, 0);

    // Hihghlight/replace all the occurences of the text

    int origPosition = -1;
    int crtPosition;
    int findTextLength = findText.length();
    QString replaceText = mFindReplace->replaceText();
    bool regularExpression = mFindReplace->useRegularExpression();
    bool caseSensitive = mFindReplace->isCaseSensitive();
    bool wholeWordsOnly = mFindReplace->searchWholeWordsOnly();

    line = column = 0;

    while (findFirst(findText, regularExpression, caseSensitive, wholeWordsOnly,
                     true, true, line, column)) {
        // Retrieve our current position

        crtPosition = currentPosition();

        // Check whether we are back to our original position
        // Note: we want to do this both when highlighting all (obviously), but
        //       also when replacing all in case we were to replace, say, "abc"
        //       with "abcd"...

        if (crtPosition == origPosition)
            break;

        // Our new position is fine, so highlight/replace the occurrence of the
        // text

        if (pHighlightAll) {
            int fromLine;
            int fromColumn;

            lineIndexFromPosition(crtPosition-findTextLength, &fromLine, &fromColumn);

            SendScintilla(SCI_SETINDICATORCURRENT, mHighlightIndicatorNumber);
            SendScintilla(SCI_INDICATORFILLRANGE, crtPosition-findTextLength, findTextLength);

            if (!mHighlightedLines.contains(fromLine))
                mHighlightedLines << fromLine;
        } else {
            QScintillaSupport::QScintillaWidget::replace(replaceText);
        }

        // Initialise our first line/column, if needed

        if (origPosition == -1)
            origPosition = crtPosition;

        // Retrieve our new line/column

        getCursorPosition(&line, &column);
    }

    // Get our vertical scroll-bar to update itself if we are highlighting all
    // and we have some lines to highlight

    if (pHighlightAll && !mHighlightedLines.isEmpty())
        mVerticalScrollBar->update();

    // Re-enable the tracking of changes if we are replacing all

    if (!pHighlightAll)
        trackChanges(true);

    // Go back to our original first visible line, position (after having
    // corrected it, but only if we replaced all the occurrences of the text)
    // and scroll bar positions

    setFirstVisibleLine(crtFirstVisibleLine);

    if (!pHighlightAll) {
        line = qMin(line, lines()-1);
        column = qMin(column, lineLength(line)-1);
    }

    QsciScintilla::setCursorPosition(line, column);

    horizontalScrollBar()->setValue(horizontalScrollBarPosition);
    verticalScrollBar()->setValue(verticalScrollBarPosition);

    // Clear any previous hihghlighting, in case we replaced all the occurences
    // of the text

    if (!pHighlightAll)
        clearHighlighting();
}

//==============================================================================

void EditorWidgetEditorWidget::replaceAll()
{
    // Replace all the occurences of the text

    doHighlightReplaceAll(false);
}

//==============================================================================

void EditorWidgetEditorWidget::highlightAll()
{
    // Highlight all the occurences of the text

    doHighlightReplaceAll(true);
}

//==============================================================================

bool EditorWidgetEditorWidget::findNext()
{
    // Find the next occurrence of the text in our editor

    return findText(mFindReplace->findText(), true);
}

//==============================================================================

bool EditorWidgetEditorWidget::findPrevious()
{
    // Find the previous occurrence of the text in our editor

    int oldPosition = currentPosition();

    setCurrentPosition(oldPosition-selectedText().length());

    bool res = findText(mFindReplace->findText(), false);

    if (!res)
        setCurrentPosition(oldPosition);

    return res;
}

//==============================================================================

void EditorWidgetEditorWidget::findTextChanged(const QString &pText)
{
    // The find text has changed, so look for its first occurrence in our
    // editor, should there be some text to search otherwise go back to our
    // original position

    if (pText.isEmpty()) {
        clearHighlighting();

        setSelection(0, 0, 0, 0);
    } else {
        // Add the given text to our list of texts to highlight and find

        mTexts << pText;

        // Highlight all the occurrences of the text and find its first
        // occurrence, but through a single shot (see the note in
        // highlightAllAndFind())

        QTimer::singleShot(0, this, &EditorWidgetEditorWidget::highlightAllAndFind);
    }
}

//==============================================================================

void EditorWidgetEditorWidget::highlightAllAndFind()
{
    // If there is only one text left in mHighlightAllAndFindTexts, then
    // highlight of its occurrences select its first occurrence
    // Note: this works with findTextChanged() and addresses the case where a
    //       user types something to search, in which case we don't want to
    //       highlight all the occurrences of the text at every key stroke since
    //       it could make things really slow in some cases (e.g. looking for a
    //       frequent text in a long file)...

    QString text = mTexts.first();

    mTexts.removeFirst();

    if (mTexts.isEmpty()) {
        highlightAll();

        findText(text, true);
    }
}

//==============================================================================

void EditorWidgetEditorWidget::replace()
{
    // Replace the current text

    if (mFindReplace->useRegularExpression()) {
        // The find/replace is done using a regular expression, so only replace
        // the currently selected text if it matches the regular expression

        if (QRegularExpression(mFindReplace->findText(),
                               mFindReplace->isCaseSensitive()?
                                   QRegularExpression::NoPatternOption:
                                   QRegularExpression::CaseInsensitiveOption).match(selectedText()).hasMatch()) {
            QScintillaSupport::QScintillaWidget::replace(mFindReplace->replaceText());
        }
    } else {
        // The find/replace is done using a simple match, which may be case
        // sensitive and/or may require whole words

        // Make sure that the currently selected text is a whole word, should
        // this be requested

        QString crtSelectedText = selectedText();
        int line;
        int column;

        getCursorPosition(&line, &column);

        if (   mFindReplace->searchWholeWordsOnly()
            && crtSelectedText.compare(wordAt(line, column))) {
            return;
        }

        // Replace the currently selected text if we have a match

        if (!crtSelectedText.compare(mFindReplace->findText(),
                                  mFindReplace->isCaseSensitive()?
                                      Qt::CaseSensitive:
                                      Qt::CaseInsensitive)) {
            QScintillaSupport::QScintillaWidget::replace(mFindReplace->replaceText());
        }
    }
}

//==============================================================================

void EditorWidgetEditorWidget::replaceAndFind()
{
    // Replace the current text and the find the next occurence of the text

    replace();
    findNext();
}

//==============================================================================

bool EditorWidgetEditorWidget::findText(const QString &pText, bool pForward)
{
    // Find the previous/next occurrence of the given text

    int line;
    int column;

    getCursorPosition(&line, &column);

    return findFirst(pText,
                     mFindReplace->useRegularExpression(),
                     mFindReplace->isCaseSensitive(),
                     mFindReplace->searchWholeWordsOnly(),
                     true, pForward, line, column);
}

//==============================================================================

void EditorWidgetEditorWidget::cut()
{
    // Default handling of the method

    QScintillaSupport::QScintillaWidget::cut();

    // Update our highlighting all if our find/replace widget is visible and
    // has some find text

    if (mFindReplace->isVisible() && !mFindReplace->findText().isEmpty())
        highlightAll();
}

//==============================================================================

void EditorWidgetEditorWidget::paste()
{
    // Default handling of the method

    QScintillaSupport::QScintillaWidget::paste();

    // Update our highlighting all if our find/replace widget is visible and
    // has some find text

    if (mFindReplace->isVisible() && !mFindReplace->findText().isEmpty())
        highlightAll();
}

//==============================================================================

void EditorWidgetEditorWidget::del()
{
    // Default handling of the method

    QScintillaSupport::QScintillaWidget::del();

    // Update our highlighting all if our find/replace widget is visible and
    // has some find text

    if (mFindReplace->isVisible() && !mFindReplace->findText().isEmpty())
        highlightAll();
}

//==============================================================================

void EditorWidgetEditorWidget::undo()
{
    // Default handling of the method

    QScintillaSupport::QScintillaWidget::undo();

    // Update our highlighting all if our find/replace widget is visible and
    // has some find text

    if (mFindReplace->isVisible() && !mFindReplace->findText().isEmpty())
        highlightAll();
}

//==============================================================================

void EditorWidgetEditorWidget::redo()
{
    // Default handling of the method

    QScintillaSupport::QScintillaWidget::redo();

    // Update our highlighting all if our find/replace widget is visible and
    // has some find text

    if (mFindReplace->isVisible() && !mFindReplace->findText().isEmpty())
        highlightAll();
}

//==============================================================================

}   // namespace EditorWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
