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
    mHighlightedLines(QIntList())
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
        emit hideFindReplaceWidgetRequested();
        mOwner->setFindReplaceVisible(false);

        pEvent->accept();
    } else {
        // Default handling of the event

        QScintillaSupport::QScintillaWidget::keyPressEvent(pEvent);

        // Update our highlighting all if our find/replace widget is visible and
        // has some find text and we didn't press one of the navigation keys,
        // otherwise send the event to our main window, in case it needs to
        // handle some key event (e.g. handle Esc to exit full-screen mode on
        // macOS)
        // Note #1: regarding highlighting all, we need to do that in case we
        //          have some highlighting and then add some text, in which case
        //          the new text might indeed need highlighting and our vertical
        //          scroll bar updating...
        // Note #2: if our find/replace widget is visible and has some find
        //          test, but that we press one of the navigation keys then we
        //          don't need and shouldn't do highlighting all since it will
        //          lose any selection that we might have...

        if (    mFindReplace->isVisible() && !mFindReplace->findText().isEmpty()
            && !(pEvent->modifiers() & Qt::ShiftModifier)
            && !(pEvent->modifiers() & Qt::ControlModifier)
            && !(pEvent->modifiers() & Qt::AltModifier)
            && !(pEvent->modifiers() & Qt::MetaModifier)
            &&  (pEvent->key() != Qt::Key_Up) &&  (pEvent->key() != Qt::Key_Down)
            &&  (pEvent->key() != Qt::Key_Left) && (pEvent->key() != Qt::Key_Right)
            &&  (pEvent->key() != Qt::Key_Home) && (pEvent->key() != Qt::Key_End)
            &&  (pEvent->key() != Qt::Key_PageUp) && (pEvent->key() != Qt::Key_PageDown)) {
            highlightAll();
        } else {
            QCoreApplication::sendEvent(Core::mainWindow(), pEvent);
        }
    }
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
        int lastLine, lastIndex;

        lineIndexFromPosition(text().length(), &lastLine, &lastIndex);
        clearIndicatorRange(0, 0, lastLine, lastIndex, mHighlightIndicatorNumber);

        mHighlightedLines = QIntList();

        mVerticalScrollBar->update();
    }
}

//==============================================================================

void EditorWidgetEditorWidget::addHighlighting(int pFromLine, int pFromColumn,
                                               int pToLine, int pToColumn)
{
    // Clear the current highlighting

    fillIndicatorRange(pFromLine, pFromColumn, pToLine, pToColumn,
                       mHighlightIndicatorNumber);

    if (!mHighlightedLines.contains(pFromLine)) {
        mHighlightedLines << pFromLine;

        mVerticalScrollBar->update();
    }
}

//==============================================================================

void EditorWidgetEditorWidget::doHighlightAllOrReplaceAll(bool pHighlightAll)
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

    // Keep track of the first visible line, of our current line/column, and of
    // the position of our scrollbars

    int crtFirstVisibleLine = firstVisibleLine();
    int line;
    int column;

    getCursorPosition(&line, &column);

    int horizontalScrollBarPosition = horizontalScrollBar()->value();
    int verticalScrollBarPosition = verticalScrollBar()->value();

    // Go to the beginning of our editor

    QsciScintilla::setCursorPosition(0, 0);

    // Hihghlight/replace all the occurences of the text

    int firstLine = -1;
    int firstColumn = -1;
    int crtLine;
    int crtColumn;

    while (findNext()) {
        // Retrieve our new position

        getCursorPosition(&crtLine, &crtColumn);

        // Check whether we are back to our first line/column, in case we are
        // trying to highlight all the occurrences of the text

        if (    pHighlightAll
            && (crtLine == firstLine) && (crtColumn == firstColumn)) {
            break;
        }

        // Our new position is fine, so highlight/replace the occurrence of the
        // text

        if (pHighlightAll)
            addHighlighting(crtLine, crtColumn-mFindReplace->findText().length(), crtLine, crtColumn);
        else
            QScintillaSupport::QScintillaWidget::replace(mFindReplace->replaceText());

        // Initialise our first line/column, if needed

        if ((firstLine == -1) && (firstColumn == -1)) {
            firstLine = crtLine;
            firstColumn = crtColumn;
        }
    }

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

    doHighlightAllOrReplaceAll(false);
}

//==============================================================================

void EditorWidgetEditorWidget::highlightAll()
{
    // Highlight all the occurences of the text

    doHighlightAllOrReplaceAll(true);
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
        // Look for the first occurrence of the given text in our editor, but
        // first highlight all the occurrences of the given text

        highlightAll();

        findText(pText, true);
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
