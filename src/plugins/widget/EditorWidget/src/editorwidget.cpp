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
#include <QSettings>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

EditorWidget::EditorWidget(const QString &pContents, bool pReadOnly,
                           QsciLexer *pLexer, QWidget *pParent) :
    Core::Widget(pParent)
{
    // Create and set our vertical layout

    auto layout = new QVBoxLayout(this);

    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);

    setLayout(layout);

    // Create our editor and find/replace widget

    mSeparator = Core::newLineWidget(this);
    mFindReplace = new EditorWidgetFindReplaceWidget(this);

    mEditor = new EditorWidgetEditorWidget(pLexer, mFindReplace, this);

    // Set our contents and our read-only state

    setContents(pContents);
    setReadOnly(pReadOnly);

    // Disable the focus policy on our editor so that tabbing only works within
    // our find/replace widget

    mEditor->setFocusPolicy(Qt::NoFocus);

    // Customise our word wrap feature

    mEditor->setWrapIndentMode(QsciScintilla::WrapIndentSame);
    mEditor->setWrapVisualFlags(QsciScintilla::WrapFlagInMargin);

    // Forward some signals that are emitted by our editor
    // Note: we cannot use the new connect() syntax since the signal is located
    //       in our QScintilla plugin and that we don't know anything about
    //       it...

    connect(mEditor, SIGNAL(cursorPositionChanged(int, int)),
            this, SIGNAL(cursorPositionChanged(int, int)));

    connect(mEditor, SIGNAL(textChanged()),
            this, SIGNAL(textChanged()));

    connect(mEditor, SIGNAL(copyAvailable(bool)),
            this, SIGNAL(copyAvailable(bool)));

    connect(mEditor, SIGNAL(canSelectAll(bool)),
            this, SIGNAL(canSelectAll(bool)));

    // Keep track of whenever the find text changes

    connect(mFindReplace, &EditorWidgetFindReplaceWidget::findTextChanged,
            mEditor, &EditorWidgetEditorWidget::findTextChanged);
    connect(mFindReplace, &EditorWidgetFindReplaceWidget::canFindReplace,
            this, &EditorWidget::canFindReplace);

    // Keep track of the triggering of some actions in our find/replace widget

    connect(mFindReplace, &EditorWidgetFindReplaceWidget::findNextRequested,
            mEditor, &EditorWidgetEditorWidget::findNext);
    connect(mFindReplace, &EditorWidgetFindReplaceWidget::findPreviousRequested,
            mEditor, &EditorWidgetEditorWidget::findPrevious);

    connect(mFindReplace, &EditorWidgetFindReplaceWidget::replaceRequested,
            mEditor, QOverload<>::of(&EditorWidgetEditorWidget::replace));
    connect(mFindReplace, &EditorWidgetFindReplaceWidget::replaceAndFindRequested,
            mEditor, &EditorWidgetEditorWidget::replaceAndFind);
    connect(mFindReplace, &EditorWidgetFindReplaceWidget::replaceAllRequested,
            mEditor, &EditorWidgetEditorWidget::replaceAll);

    connect(mFindReplace, &EditorWidgetFindReplaceWidget::searchOptionsChanged,
            mEditor, &EditorWidgetEditorWidget::highlightAll);

    // Add our editor and find/replace widgets to our layout

    layout->addWidget(mEditor);
    layout->addWidget(mSeparator);
    layout->addWidget(mFindReplace);

    // Make our editor our focus proxy

    setFocusProxy(mEditor);

    // Initially hide our find/replace widget

    setFindReplaceVisible(false);
}

//==============================================================================

static const char *SettingsEditorWidgetWordWrap  = "EditorWidgetWordWrap";
static const char *SettingsEditorWidgetZoomLevel = "EditorWidgetZoomLevel";

//==============================================================================

void EditorWidget::loadSettings(QSettings &pSettings)
{
    // Retrieve our settings

    setWordWrap(pSettings.value(SettingsEditorWidgetWordWrap, false).toBool());
    setZoomLevel(pSettings.value(SettingsEditorWidgetZoomLevel, 0).toInt());
}

//==============================================================================

void EditorWidget::saveSettings(QSettings &pSettings) const
{
    // Keep track of our settings

    pSettings.setValue(SettingsEditorWidgetWordWrap, wordWrap());
    pSettings.setValue(SettingsEditorWidgetZoomLevel, zoomLevel());
}

//==============================================================================

void EditorWidget::retranslateUi()
{
    // Retranslate our find/replace widget

    mFindReplace->retranslateUi();
}

//==============================================================================

void EditorWidget::updateSettings(EditorWidget *pEditorWidget)
{
    // Make sure that we are given another widget

    if ((pEditorWidget == nullptr) || (pEditorWidget == this)) {
        return;
    }

    // Update our word wrap mode and zoom level

    setWordWrap(pEditorWidget->wordWrap());
    setZoomLevel(pEditorWidget->zoomLevel());

    // Show/hide our find/replace widget

    setFindReplaceVisible(pEditorWidget->isFindReplaceVisible(), false);

    // Update the find/replace widget itself
    // Note: we must inactivate (and then reactivate) our find/replace widget
    //       otherwise opening a new file or switching to another will result in
    //       the find text being automatically searched (see the
    //       findTextChanged() slot)...

    mFindReplace->setActive(false);
        mFindReplace->updateFrom(pEditorWidget->mFindReplace);
    mFindReplace->setActive(true);
}

//==============================================================================

bool EditorWidget::handleEditorKeyPressEvent(QKeyEvent *pEvent)
{
    Q_UNUSED(pEvent)

    // By default, we don't handle our editor's key press event

    return false;
}

//==============================================================================

void EditorWidget::setLexer(QsciLexer *pLexer)
{
    // Set our editor's lexer

    mEditor->setLexer(pLexer);
}

//==============================================================================

QMenu * EditorWidget::contextMenu() const
{
    // Return the context menu of our editor

    return mEditor->contextMenu();
}

//==============================================================================

void EditorWidget::setContextMenu(const QList<QAction *> &pContextMenuActions)
{
    // Set the context menu of our editor

    mEditor->setContextMenu(pContextMenuActions);
}

//==============================================================================

void EditorWidget::cursorPosition(int &pLine, int &pColumn)
{
    // Retrieve our cursor position

    mEditor->cursorPosition(pLine, pColumn);
}

//==============================================================================

int EditorWidget::length() const
{
    // Retrieve our length

    return mEditor->length();
}

//==============================================================================

QString EditorWidget::text(int pLine) const
{
    // Retrieve the given line of text

    return mEditor->text(pLine);
}

//==============================================================================

void EditorWidget::selection(int &pLineFrom, int &pColumnFrom, int &pLineTo,
                             int &pColumnTo)
{
    // Retrieve our current selection

    mEditor->getSelection(&pLineFrom, &pColumnFrom, &pLineTo, &pColumnTo);
}

//==============================================================================

void EditorWidget::setSelection(int pLineFrom, int pColumnFrom, int pLineTo,
                                int pColumnTo)
{
    // Set the current selection

    mEditor->setSelection(pLineFrom, pColumnFrom, pLineTo, pColumnTo);
}

//==============================================================================

int EditorWidget::position(int pLine, int pColumn)
{
    // Retrieve our position from the given line/column

    return mEditor->positionFromLineIndex(pLine, pColumn);
}

//==============================================================================

void EditorWidget::setCursorPosition(int pLine, int pColumn)
{
    // Set our cursor position

    mEditor->setCursorPosition(pLine, pColumn);
}

//==============================================================================

int EditorWidget::currentPosition() const
{
    // Return the current position of our editor

    return mEditor->currentPosition();
}

//==============================================================================

QString EditorWidget::contents() const
{
    // Return the contents of our editor

    return mEditor->contents();
}

//==============================================================================

void EditorWidget::setContents(const QString &pContents, bool pEmptyUndoBuffer)
{
    // Set the contents of our editor

    mEditor->setContents(pContents, pEmptyUndoBuffer);
}

//==============================================================================

int EditorWidget::contentsSize() const
{
    // Return the size of the contents of our editor

    return mEditor->contentsSize();
}

//==============================================================================

bool EditorWidget::isReadOnly() const
{
    // Return whether our editor is in read-only mode

    return mEditor->isReadOnly();
}

//==============================================================================

void EditorWidget::setReadOnly(bool pReadOnly)
{
    // Set the read-only mode of our editor and find/replace widget

    mEditor->setReadOnly(pReadOnly);
    mFindReplace->setReadOnly(pReadOnly);
}

//==============================================================================

bool EditorWidget::hasSelectedText() const
{
    // Return whether our editor has some text selected

    return mEditor->hasSelectedText();
}

//==============================================================================

QString EditorWidget::selectedText() const
{
    // Return whether the selected text, if any, in our editor

    return mEditor->selectedText();
}

//==============================================================================

QString EditorWidget::textInRange(int pStartRange, int pEndRange) const
{
    // Retrieve and return the text in the given range in our editor, making
    // sure that the given range makes sense

    return mEditor->textInRange(pStartRange, pEndRange);
}

//==============================================================================

int EditorWidget::findTextInRange(int pStartRange, int pEndRange,
                                  const QString &pText, bool pRegularExpression,
                                  bool pCaseSensitive, bool pWholeWordsOnly) const
{
    // Find and return the position, if any, of the given text within the given
    // range in our editor

    return mEditor->findTextInRange(pStartRange, pEndRange, pText,
                                    pRegularExpression, pCaseSensitive,
                                    pWholeWordsOnly);
}

//==============================================================================

bool EditorWidget::isUndoAvailable() const
{
    // Return whether we can undo a change in our editor

    return mEditor->isUndoAvailable();
}

//==============================================================================

bool EditorWidget::isRedoAvailable() const
{
    // Return whether we can redo a change in our editor

    return mEditor->isRedoAvailable();
}

//==============================================================================

bool EditorWidget::isFindPreviousNextAvailable() const
{
    // Return whether we can find previous/next in our editor

    return mFindReplace->isFindPreviousNextAvailable();
}

//==============================================================================

bool EditorWidget::isSelectAllAvailable() const
{
    // Return whether we can select all the text in our editor

    return mEditor->isSelectAllAvailable();
}

//==============================================================================

void EditorWidget::cut()
{
    // Cut the selected text, if any, in our editor

    mEditor->cut();
}

//==============================================================================

void EditorWidget::copy()
{
    // Copy the selected text, if any, in our editor

    mEditor->copy();
}

//==============================================================================

void EditorWidget::paste()
{
    // Paste the text, if any available, in our editor

    mEditor->paste();
}

//==============================================================================

void EditorWidget::del()
{
    // Delete the selected text, if any, in our editor

    mEditor->del();
}

//==============================================================================

void EditorWidget::undo()
{
    // Undo the last change, if any, in our editor

    mEditor->undo();
}

//==============================================================================

void EditorWidget::redo()
{
    // Redo the last change, if any, in our editor

    mEditor->redo();
}

//==============================================================================

void EditorWidget::selectAll()
{
    // Select all the text, if any, in our editor

    mEditor->selectAll();
}

//==============================================================================

void EditorWidget::highlightAll()
{
    // Highlight all the occurences of the text, if any, in our editor

    mEditor->highlightAll();
}

//==============================================================================

bool EditorWidget::wordWrap() const
{
    // Return whether we word wrap the text

    return mEditor->wrapMode() == QsciScintilla::WrapWord;
}

//==============================================================================

void EditorWidget::setWordWrap(bool pWordWrap)
{
    // Word wrap (or not) the text

    mEditor->setWrapMode(pWordWrap?
                             QsciScintilla::WrapWord:
                             QsciScintilla::WrapNone);
}

//==============================================================================

void EditorWidget::resetUndoHistory()
{
    // Reset the undo history of our editor

    mEditor->resetUndoHistory();
}

//==============================================================================

QLabel * EditorWidget::cursorPositionWidget() const
{
    // Return the cursor position widget of our editor

    return mEditor->cursorPositionWidget();
}

//==============================================================================

QLabel * EditorWidget::editingModeWidget() const
{
    // Return the editing mode widget of our editor

    return mEditor->editingModeWidget();
}

//==============================================================================

QString EditorWidget::eolString() const
{
    // Return the end of line used by our editor

    return mEditor->eolString();
}

//==============================================================================

int EditorWidget::zoomLevel() const
{
    // Return the zoom level of our editor

    return mEditor->zoomLevel();
}

//==============================================================================

void EditorWidget::setZoomLevel(int pZoomLevel)
{
    // Set the zoom level of our editor

    return mEditor->zoomTo(pZoomLevel);
}

//==============================================================================

bool EditorWidget::isFindReplaceVisible() const
{
    // Return whether our find/replace widget is visible

    return mFindReplace->isVisible();
}

//==============================================================================

void EditorWidget::setFindReplaceVisible(bool pVisible, bool pSelectWord)
{
    // Set our find text and highlight all its occurrences, if we are to show
    // our find/replace widget
    // Note: if we are over a word, then we want it to become our find text
    //       (unless some text is already selected), but if we make it so then a
    //       call to findText() will be triggered if the find text is different.
    //       Clearly, we don't want this to happen, hence we inactivate and then
    //       reactivate our find/replace widget...

    if (pVisible) {
        QString selText = QString();

        if (hasSelectedText()) {
            selText = selectedText();
        } else if (pSelectWord) {
            int line;
            int column;

            mEditor->cursorPosition(line, column);

            selText = mEditor->wordAt(line, column);

            mEditor->selectWordAt(line, column);
        }

        mEditor->updateLineColumn();

        mFindReplace->setActive(false);
            mFindReplace->setFindText(selText);
        mFindReplace->setActive(true);

        mEditor->highlightAll();
    } else {
        mEditor->clearHighlighting();
    }

    // Show/hide our find/replace widget

    mSeparator->setVisible(pVisible);
    mFindReplace->setVisible(pVisible);

    // Give the focus to our find/replace widget or to our editor, depending on
    // the case

    if (pVisible) {
        mFindReplace->setFocus();
    } else {
        mEditor->setFocus();
    }
}

//==============================================================================

int EditorWidget::styleAt(int pPosition) const
{
    // Return the style used at the given position

    return int(mEditor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT,
                                      pPosition));
}

//==============================================================================

bool EditorWidget::findNext()
{
    // Find the next occurrence of the text in our editor

    return mEditor->findNext();
}

//==============================================================================

bool EditorWidget::findPrevious()
{
    // Find the previous occurrence of the text in our editor

    return mEditor->findPrevious();
}

//==============================================================================

void EditorWidget::insertText(const QString &pText, int pLine, int pColumn)
{
    // Insert the given given text at the given line/column

    mEditor->insertAt(pText, pLine, pColumn);
}

//==============================================================================

void EditorWidget::removeText(int pPosition, int pLength)
{
    // Select the text at the given position, and for the given length, and
    // remove it

    mEditor->SendScintilla(QsciScintilla::SCI_SETSEL, ulong(pPosition), pPosition+pLength);
    mEditor->removeSelectedText();
}

//==============================================================================

void EditorWidget::replaceSelectedText(const QString &pText)
{
    // Replace the current text with the given one

    mEditor->replaceSelectedText(pText);
}

//==============================================================================

void EditorWidget::replaceAndFind()
{
    // Replace the current text and then find its next occurence

    mEditor->replaceAndFind();
}

//==============================================================================

void EditorWidget::beginUndoAction()
{
    // Begin an undo action

    mEditor->beginUndoAction();
}

//==============================================================================

void EditorWidget::endUndoAction()
{
    // End an undo action

    mEditor->endUndoAction();
}

//==============================================================================

bool EditorWidget::handleEditorChanges() const
{
    // Return whether we should handle our editor's changes

    return mEditor->handleChanges();
}

//==============================================================================

} // namespace EditorWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
