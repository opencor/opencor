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
    QScintillaWidget::QScintillaWidget(pLexer, pParent),
    mOwner(pParent),
    mFindReplace(pFindReplace)
{
    // Use our own vertical scroll bar for our editor, so that we can show the
    // position of our highlighting

    mVerticalScrollBar = new EditorWidgetScrollBar(this);

    replaceVerticalScrollBar(mVerticalScrollBar);

    // Check which styles can have their background changed when making them
    // read-only or writable

    QColor baseColor = Core::baseColor();

    for (int i = 0; i < QsciScintillaBase::STYLE_MAX; ++i) {
        if (backgroundColor(i) == baseColor) {
            mReadOnlyStyles << i;
        }
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

    if (   mFindReplace->isVisible()
        && (pEvent->modifiers() == Qt::NoModifier)
        && (pEvent->key() == Qt::Key_Escape)) {
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

        QScintillaWidget::QScintillaWidget::keyPressEvent(pEvent);

        // Update our highlighting all if our find/replace widget is visible and
        // has some find text and we didn't press one of the navigation keys,
        // clear our highlighting if we pressed any key (except any of our
        // modifiers) and our find/replace widget is not visible, forward the
        // pressing of the Esc key but only on macOS (so we can exit full-screen
        // mode, if needed)
        // Note #1: regarding highlighting all, we need to do that in case we
        //          have some highlighting and then add some text, in which case
        //          the new text might indeed need highlighting and our vertical
        //          scroll bar updating...
        // Note #2: if our find/replace widget is visible and has some find
        //          text, but that we press one of the navigation keys then we
        //          don't need and shouldn't do highlighting all since it will
        //          lose any selection that we might have...
        // Note #3: regarding navigation keys, we don't need to test for the
        //          arrow keys since in that case pEvent->modifiers() will be
        //          equal to Qt::KeypadModifier and, here, we are testing for
        //          Qt::NoModifier...

        if (   mFindReplace->isVisible() && !mFindReplace->findText().isEmpty()
            && (pEvent->modifiers() == Qt::NoModifier)
            && (pEvent->key() != Qt::Key_Home) && (pEvent->key() != Qt::Key_End)
            && (pEvent->key() != Qt::Key_PageUp) && (pEvent->key() != Qt::Key_PageDown)) {
            highlightAll();

            pEvent->accept();
#ifdef Q_OS_MAC
        } else if (   (pEvent->modifiers() == Qt::NoModifier)
                   && (pEvent->key() == Qt::Key_Escape)) {
            QCoreApplication::sendEvent(Core::mainWindow(), pEvent);
#endif
        } else if (pEvent->modifiers() != Qt::NoModifier) {
            pEvent->accept();
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

    QScintillaWidget::QScintillaWidget::mouseDoubleClickEvent(pEvent);

    // Check whether something has been selected and, if so, highlight all of
    // its occurrences
    // Note: we must inactivate (and then reactivate) our find/replace widget
    //       before setting its find text otherwise we will be highlighting
    //       things twice...

    if (hasSelectedText()) {
        mFindReplace->setActive(false);
            mFindReplace->setFindText(selectedText());
        mFindReplace->setActive(true);

        highlightAll();
    }
}

//==============================================================================

void EditorWidgetEditorWidget::mousePressEvent(QMouseEvent *pEvent)
{
    // Default handling of the event

    QScintillaWidget::QScintillaWidget::mousePressEvent(pEvent);

    // Clear any highlighting that we might have, if our find/replace widget is
    // not visible

    if (!mFindReplace->isVisible()) {
        clearHighlighting();
    }
}

//==============================================================================

void EditorWidgetEditorWidget::setReadOnly(bool pReadOnly)
{
    // Default handling of the method

    QScintillaWidget::QScintillaWidget::setReadOnly(pReadOnly);

    // Update our background colour

    for (auto readOnlyStyle : mReadOnlyStyles) {
        setBackgroundColor(readOnlyStyle,
                           pReadOnly?
                               Core::lockedColor(Core::baseColor()):
                               Core::baseColor());
    }
}

//==============================================================================

QIntSet EditorWidgetEditorWidget::highlightedLines() const
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

        mHighlightedLines = QIntSet();

        mVerticalScrollBar->update();
    }
}

//==============================================================================

void EditorWidgetEditorWidget::processAll(Action pAction)
{
    // Some pre-processing, based on the given action

    QByteArray findText = mFindReplace->findText().toUtf8();

    if (pAction == Action::HighlightAll) {
        // Clear any previous hihghlighting

        clearHighlighting();

        // No point in carrying on, if the find text is empty or only consists
        // of spaces and the like

        if (findText.trimmed().isEmpty()) {
            return;
        }
    } else {
        // Stop tracking our changes and consider our action as being one big
        // action

        setHandleChanges(false);

        beginUndoAction();
    }

    // Keep track of the current selection

    int selectionStart = int(SendScintilla(SCI_GETSELECTIONSTART));
    int selectionEnd = int(SendScintilla(SCI_GETSELECTIONEND));
    int selectionShift = 0;

    // Specify our search flags

    SendScintilla(SCI_SETSEARCHFLAGS,
                  ulong( (mFindReplace->useRegularExpression()?SCFIND_REGEXP:0)
                        |(mFindReplace->isCaseSensitive()?SCFIND_MATCHCASE:0)
                        |(mFindReplace->searchWholeWordsOnly()?SCFIND_WHOLEWORD:0)));

    // Hihghlight/replace all the occurences of the text

    const char *rawFindText = findText.constData();
    int findTextLen = findText.length();
    int findTextPos = text().length();
    uint replaceCommand = mFindReplace->useRegularExpression()?
                              SCI_REPLACETARGETRE:
                              SCI_REPLACETARGET;
    QByteArray replaceText = mFindReplace->replaceText().toUtf8();
    const char *rawReplaceText = replaceText.constData();
    auto rawReplaceTextLen = uintptr_t(strlen(rawReplaceText));
    int selFoundTextLen = 0;

    forever {
        // Find the first occurrence of the given text, going backward from the
        // given text position
        // Note: by going backward, we don't have to worry about the size of the
        //       text, in case bits of it have been replaced...

        SendScintilla(SCI_SETTARGETSTART, findTextPos);
        SendScintilla(SCI_SETTARGETEND, 0);

        findTextPos = int(SendScintilla(SCI_SEARCHINTARGET, uintptr_t(findTextLen), rawFindText));

        if (findTextPos == -1) {
            // No more occurrences, so just leave our loop

            break;
        }

        // Determine the length of our found text
        // Note: we cannot and must not use findTextLen since we may be finding
        //       text using a regular expression...

        int foundTextLen = int(SendScintilla(SCI_GETTARGETEND))-findTextPos;

        // Either highlight or replace our found text

        if (pAction == Action::HighlightAll) {
            SendScintilla(SCI_SETINDICATORCURRENT, mHighlightIndicatorNumber);
            SendScintilla(SCI_INDICATORFILLRANGE, ulong(findTextPos), foundTextLen);

            int line = int(SendScintilla(SCI_LINEFROMPOSITION, findTextPos));

            mHighlightedLines << line;
        } else {
            if (findTextPos < selectionStart) {
                selectionShift += replaceText.length()-foundTextLen;
            } else if (findTextPos == selectionStart) {
                selFoundTextLen = foundTextLen;
            }

            SendScintilla(SCI_SETTARGETSTART, findTextPos);
            SendScintilla(SCI_SETTARGETEND, findTextPos+foundTextLen);

            SendScintilla(replaceCommand, rawReplaceTextLen, rawReplaceText);
        }
    }

    // Some post-processing, based on the given action

    if (pAction == Action::HighlightAll) {
        // Get our vertical scroll-bar to update itself, if we have some lines
        // to highlight

        if (!mHighlightedLines.isEmpty()) {
            mVerticalScrollBar->update();
        }
    } else {
        // Reselect what used to be selected and re-enable the tracking of
        // changes, as well as let Scintilla know that we are done with our big
        // action

        endUndoAction();

        SendScintilla(SCI_SETSELECTIONSTART, selectionStart+selectionShift);
        SendScintilla(SCI_SETSELECTIONEND,  selectionEnd+selectionShift
                                           +((selFoundTextLen != 0)?
                                                 replaceText.length()-selFoundTextLen:
                                                 0));

        setHandleChanges(true);
    }
}

//==============================================================================

void EditorWidgetEditorWidget::highlightAll()
{
    // Highlight all the occurences of the text

    processAll(Action::HighlightAll);
}

//==============================================================================

void EditorWidgetEditorWidget::replaceAll()
{
    // Replace all the occurences of the text

    processAll(Action::ReplaceAll);
}

//==============================================================================

void EditorWidgetEditorWidget::updateLineColumn()
{
    // Update our line/column

    lineIndexFromPosition(currentPosition()-selectedText().length(),
                          &mLine, &mColumn);
}

//==============================================================================

bool EditorWidgetEditorWidget::findNext()
{
    // Find the next occurrence of the text in our editor

    return findText(mFindReplace->findText(), true, false);
}

//==============================================================================

bool EditorWidgetEditorWidget::findPrevious()
{
    // Find the previous occurrence of the text in our editor

    int oldPosition = currentPosition();

    setCurrentPosition(oldPosition-selectedText().length());

    bool res = findText(mFindReplace->findText(), false, false);

    if (!res) {
        setCurrentPosition(oldPosition);
    }

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

        QsciScintilla::setCursorPosition(mLine, mColumn); // NOLINT(bugprone-parent-virtual-call)
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
    // If there is only one text left in mTexts, then highlight of its
    // occurrences select its first occurrence
    // Note: this works with findTextChanged() and addresses the case where a
    //       user types something to search, in which case we don't want to
    //       highlight all the occurrences of the text at every key stroke since
    //       it could make things really slow in some cases (e.g. looking for a
    //       frequent text in a long file)...

    QString text = mTexts.first();

    mTexts.removeFirst();

    if (mTexts.isEmpty()) {
        highlightAll();

        findText(text);
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
            QScintillaWidget::QScintillaWidget::replace(mFindReplace->replaceText());
        }
    } else {
        // The find/replace is done using a simple match, which may be case
        // sensitive and/or may require whole words

        // Make sure that the currently selected text is a whole word, should
        // this be requested

        QString crtSelectedText = selectedText();
        int line;
        int column;

        cursorPosition(line, column);

        if (   mFindReplace->searchWholeWordsOnly()
            && (crtSelectedText != wordAt(line, column))) {
            return;
        }

        // Replace the currently selected text if we have a match

        if (crtSelectedText.compare(mFindReplace->findText(),
                                    mFindReplace->isCaseSensitive()?
                                        Qt::CaseSensitive:
                                        Qt::CaseInsensitive) == 0) {
            QScintillaWidget::QScintillaWidget::replace(mFindReplace->replaceText());
        }
    }
}

//==============================================================================

void EditorWidgetEditorWidget::replaceAndFind()
{
    // Replace the current text and then find its next occurence

    replace();
    findNext();
}

//==============================================================================

bool EditorWidgetEditorWidget::findText(const QString &pText, bool pForward,
                                        bool pFirstTime)
{
    // Find the previous/next occurrence of the given text

    int line;
    int column;

    if (pFirstTime) {
        line = mLine;
        column = mColumn;
    } else {
        cursorPosition(line, column);
    }

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

    QScintillaWidget::QScintillaWidget::cut();

    // Update our highlighting all if our find/replace widget is visible and
    // has some find text

    if (mFindReplace->isVisible() && !mFindReplace->findText().isEmpty()) {
        highlightAll();
    }
}

//==============================================================================

void EditorWidgetEditorWidget::paste()
{
    // Default handling of the method

    QScintillaWidget::QScintillaWidget::paste();

    // Update our highlighting all if our find/replace widget is visible and
    // has some find text

    if (mFindReplace->isVisible() && !mFindReplace->findText().isEmpty()) {
        highlightAll();
    }
}

//==============================================================================

void EditorWidgetEditorWidget::del()
{
    // Default handling of the method

    QScintillaWidget::QScintillaWidget::del();

    // Update our highlighting all if our find/replace widget is visible and
    // has some find text

    if (mFindReplace->isVisible() && !mFindReplace->findText().isEmpty()) {
        highlightAll();
    }
}

//==============================================================================

void EditorWidgetEditorWidget::undo()
{
    // Default handling of the method

    QScintillaWidget::QScintillaWidget::undo();

    // Update our highlighting all if our find/replace widget is visible and
    // has some find text

    if (mFindReplace->isVisible() && !mFindReplace->findText().isEmpty()) {
        highlightAll();
    }
}

//==============================================================================

void EditorWidgetEditorWidget::redo()
{
    // Default handling of the method

    QScintillaWidget::QScintillaWidget::redo();

    // Update our highlighting all if our find/replace widget is visible and
    // has some find text

    if (mFindReplace->isVisible() && !mFindReplace->findText().isEmpty()) {
        highlightAll();
    }
}

//==============================================================================

} // namespace EditorWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
