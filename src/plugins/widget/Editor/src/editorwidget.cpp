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
// Editor widget
//==============================================================================

#include "editorfindreplacewidget.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "guiutils.h"
#include "qscintillawidget.h"

//==============================================================================

#include "ui_editorwidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QFrame>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace Editor {

//==============================================================================

EditorWidget::EditorWidget(const QString &pContents, const bool &pReadOnly,
                           QsciLexer *pLexer, QWidget *pParent) :
    Core::Widget(pParent),
    mGui(new Ui::EditorWidget),
    mCurrentLine(0),
    mCurrentColumn(0),
    mFindReplaceVisible(false)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our editor and find/replace widget

    mEditor = new QScintillaSupport::QScintillaWidget(pLexer, this);
    mSeparator = Core::newLineWidget(this);
    mFindReplace = new EditorFindReplaceWidget(this);

    // Set our contents and our read-only state

    setContents(pContents);
    setReadOnly(pReadOnly);

    // Disable the focus policy on our editor so that tabbing only works within
    // our find/replace widget

    mEditor->setFocusPolicy(Qt::NoFocus);

    // Forward some signals that are emitted by our editor

    connect(mEditor, SIGNAL(SCN_ZOOM()),
            this, SLOT(zoomLevelChanged()));

    connect(mEditor, SIGNAL(cursorPositionChanged(int, int)),
            this, SIGNAL(cursorPositionChanged(const int &, const int &)));

    connect(mEditor, SIGNAL(SCN_MODIFIED(int, int, const char *, int, int, int, int, int, int, int)),
            this, SIGNAL(textChanged()));

    connect(mEditor, SIGNAL(copyAvailable(bool)),
            this, SIGNAL(copyAvailable(const bool &)));

    connect(mEditor, SIGNAL(canUndo(const bool &)),
            this, SIGNAL(canUndo(const bool &)));
    connect(mEditor, SIGNAL(canRedo(const bool &)),
            this, SIGNAL(canRedo(const bool &)));

    connect(mEditor, SIGNAL(canSelectAll(const bool &)),
            this, SIGNAL(canSelectAll(const bool &)));

    // Keep track of our position within our editor

    connect(mEditor, SIGNAL(cursorPositionChanged(int, int)),
            this, SLOT(keepTrackOfCursorPosition(const int &, const int &)));

    // Keep track of whenever a key is being pressed in our editor or
    // find/replace widget

    connect(mEditor, SIGNAL(keyPressed(QKeyEvent *, bool &)),
            this, SLOT(editorKeyPressed(QKeyEvent *, bool &)));
    connect(mFindReplace, SIGNAL(keyPressed(QKeyEvent *, bool &)),
            this, SLOT(findReplaceKeyPressed(QKeyEvent *, bool &)));

    // Keep track of whenever the find text changes

    connect(mFindReplace, SIGNAL(findTextChanged(const QString &)),
            this, SLOT(findTextChanged(const QString &)));
    connect(mFindReplace, SIGNAL(canFindReplace(const bool &)),
            this, SIGNAL(canFindReplace(const bool &)));

    // Keep track of the triggering of some actions in our find/replace widget

    connect(mFindReplace, SIGNAL(findPreviousRequested()),
            this, SLOT(findPrevious()));
    connect(mFindReplace, SIGNAL(findNextRequested()),
            this, SLOT(findNext()));

    connect(mFindReplace, SIGNAL(replaceRequested()),
            this, SLOT(replace()));
    connect(mFindReplace, SIGNAL(replaceAndFindRequested()),
            this, SLOT(replaceAndFind()));
    connect(mFindReplace, SIGNAL(replaceAllRequested()),
            this, SLOT(replaceAll()));

    // Add our editor and find/replace widgets to our layout

    mGui->layout->addWidget(mEditor);
    mGui->layout->addWidget(mSeparator);
    mGui->layout->addWidget(mFindReplace);

    // Make our editor our focus proxy

    setFocusProxy(mEditor);

    // Initially hide our find/replace widget

    hideFindReplace();
}

//==============================================================================

void EditorWidget::retranslateUi()
{
    // Retranslate our find/replace widget

    mFindReplace->retranslateUi();
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

void EditorWidget::setContents(const QString &pContents)
{
    // Set the contents of our editor

    mEditor->setContents(pContents);
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

void EditorWidget::setReadOnly(const bool &pReadOnly)
{
    // Set the read-only mode of our editor and find/replace widget

    mEditor->setReadOnly(pReadOnly);
    mFindReplace->setReadOnly(pReadOnly);

    // Update our background

    QColor backgroundColor = pReadOnly?Core::lockedColor(Core::baseColor()):Core::baseColor();

    for (int i = 0; i < QsciScintillaBase::STYLE_MAX; ++i)
        mEditor->setBackgroundColor(i, backgroundColor);
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

QString EditorWidget::textInRange(const int &pStartRange,
                                  const int &pEndRange) const
{
    // Retrieve and return the text in the given range in our editor, making
    // sure that the given range makes sense

    return mEditor->textInRange(pStartRange, pEndRange);
}

//==============================================================================

int EditorWidget::findTextInRange(const int &pStartRange, const int &pEndRange,
                                  const QString &pText,
                                  const bool &pCaseSensitive) const
{
    // Find and return the position, if any, of the given text within the given
    // range in our editor

    return mEditor->findTextInRange(pStartRange, pEndRange, pText, pCaseSensitive);
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

void EditorWidget::resetUndoHistory()
{
    // Reset the undo history of our editor

    mEditor->resetUndoHistory();
}

//==============================================================================

QLabel * EditorWidget::cursorPositionWidget() const
{
    // Return the cursort position widget of our editor

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

void EditorWidget::setBackgroundColor(const int &pStyle,
                                      const QColor &pBackgroundColor)
{
    // Set the background color for the given style in our editor

    mEditor->setBackgroundColor(pStyle, pBackgroundColor);
}

//==============================================================================

void EditorWidget::setForegroundColor(const int &pStyle,
                                      const QColor &pForegroundColor)
{
    // Set the foreground color for the given style in our editor

    mEditor->setForegroundColor(pStyle, pForegroundColor);
}

//==============================================================================

int EditorWidget::zoomLevel() const
{
    // Return the zoom level of our editor

    return mEditor->zoomLevel();
}

//==============================================================================

void EditorWidget::setZoomLevel(const int &pZoomLevel)
{
    // Set the zoom level of our editor

    return mEditor->zoomTo(pZoomLevel);
}

//==============================================================================

void EditorWidget::updateFindReplaceFrom(EditorWidget *pEditor)
{
    // Make sure that we have a given editor

    if (!pEditor)
        return;

    // Show/hide our find/replace widget

    if (pEditor->findReplaceIsVisible())
        showFindReplace();
    else
        hideFindReplace();

    // Update the find/replace widget itself

    mFindReplace->updateFrom(pEditor->findReplace());
}

//==============================================================================

EditorFindReplaceWidget * EditorWidget::findReplace()
{
    // Return our find/replace widget

    return mFindReplace;
}

//==============================================================================

bool EditorWidget::findReplaceIsVisible() const
{
    // Return whether our find/replace widget is visible

    return mFindReplaceVisible;
}

//==============================================================================

void EditorWidget::showFindReplace()
{
    // Set our find text
    // Note: if we are over a word, then we want it to become our find text, but
    //       by then it means that a call to findText() will be triggered if the
    //       find text gets changed. We clearly don't want this to happen, hence
    //       we disconnect and then reconnect things...

    QString currentWord = mEditor->wordAt(mCurrentLine, mCurrentColumn);

    if (!currentWord.isEmpty()) {
        disconnect(mFindReplace, SIGNAL(findTextChanged(const QString &)),
                   this, SLOT(findTextChanged(const QString &)));

        mEditor->selectWordAt(mCurrentLine, mCurrentColumn);

        mFindReplace->setFindText(currentWord);

        connect(mFindReplace, SIGNAL(findTextChanged(const QString &)),
                this, SLOT(findTextChanged(const QString &)));
    } else {
        mFindReplace->selectFindText();
    }

    // Show our find/replace widget

    mFindReplaceVisible = true;

    mSeparator->setVisible(true);
    mFindReplace->setVisible(true);

    // Give the focus to our find/replace widget and hide our editor's caret

    mFindReplace->setFocus();
}

//==============================================================================

void EditorWidget::hideFindReplace()
{
    // Hide our find/replace widget

    mFindReplaceVisible = false;

    mSeparator->setVisible(false);
    mFindReplace->setVisible(false);

    // Give the focus (back) to our editor and show its caret

    mEditor->setFocus();
}

//==============================================================================

bool EditorWidget::findPrevious()
{
    // Find the previous occurrence of the text in our editor

    int oldPosition = mEditor->currentPosition();

    mEditor->setCurrentPosition(oldPosition-mEditor->selectedText().length());

    mCurrentLine = mEditor->currentLine();
    mCurrentColumn = mEditor->currentColumn();

    bool res = findText(mFindReplace->findText(), false);

    if (!res)
        mEditor->setCurrentPosition(oldPosition);

    return res;
}

//==============================================================================

bool EditorWidget::findNext()
{
    // Find the next occurrence of the text in our editor

    mCurrentLine = mEditor->currentLine();
    mCurrentColumn = mEditor->currentColumn();

    return findText(mFindReplace->findText(), true);
}

//==============================================================================

void EditorWidget::replace()
{
    // Replace the current text

qDebug(">>> EditorWidget::replace()...");
}

//==============================================================================

void EditorWidget::replaceAndFind()
{
    // Replace the current text and the find the next occurence of the text

qDebug(">>> EditorWidget::replaceAndFind()...");
}

//==============================================================================

void EditorWidget::replaceAll()
{
    // Replace all the texts

qDebug(">>> EditorWidget::replaceAll()...");
}

//==============================================================================

void EditorWidget::zoomLevelChanged()
{
    // Let people know that the zoom level has changed

    emit zoomLevelChanged(zoomLevel());
}

//==============================================================================

void EditorWidget::keepTrackOfCursorPosition(const int &pLine,
                                             const int &pColumn)
{
    // Keep track of our new position within our editor

    if (mEditor->hasFocus()) {
        mCurrentLine = pLine;
        mCurrentColumn = pColumn;
    }
}

//==============================================================================

void EditorWidget::editorKeyPressed(QKeyEvent *pEvent, bool &pHandled)
{
    // Some key combinations from our editor

    if (   !(pEvent->modifiers() & Qt::ShiftModifier)
        && !(pEvent->modifiers() & Qt::ControlModifier)
        && !(pEvent->modifiers() & Qt::AltModifier)
        && !(pEvent->modifiers() & Qt::MetaModifier)
        &&  (pEvent->key() == Qt::Key_Escape)) {
        hideFindReplace();

        pHandled = true;
    } else {
        pHandled = false;
    }
}

//==============================================================================

void EditorWidget::findReplaceKeyPressed(QKeyEvent *pEvent, bool &pHandled)
{
    // Some key combinations from our find/replace widget

    if (   !(pEvent->modifiers() & Qt::ShiftModifier)
        && !(pEvent->modifiers() & Qt::ControlModifier)
        && !(pEvent->modifiers() & Qt::AltModifier)
        && !(pEvent->modifiers() & Qt::MetaModifier)
        &&  (pEvent->key() == Qt::Key_Escape)) {
        hideFindReplace();

        pHandled = true;
    } else if (   !(pEvent->modifiers() & Qt::ShiftModifier)
               && !(pEvent->modifiers() & Qt::ControlModifier)
               && !(pEvent->modifiers() & Qt::AltModifier)
               && !(pEvent->modifiers() & Qt::MetaModifier)
               &&  (   (pEvent->key() == Qt::Key_Return)
                    || (pEvent->key() == Qt::Key_Enter))) {
        if (mFindReplace->findEditHasFocus()) {
            findNext();

            pHandled = true;
        } else if (mFindReplace->replaceEditHasFocus()) {
            replaceAndFind();

            pHandled = true;
        } else {
            pHandled = false;
        }
    } else {
        pHandled = false;
    }
}

//==============================================================================

bool EditorWidget::findText(const QString &pText, const bool &pForward)
{
    // Find the previous/next occurrence of the given text

    return mEditor->findFirst(pText,
                              mFindReplace->regularExpression(),
                              mFindReplace->caseSensitive(),
                              mFindReplace->wholeWordsOnly(),
                              true, pForward, mCurrentLine, mCurrentColumn);
}

//==============================================================================

void EditorWidget::findTextChanged(const QString &pText)
{
    // The find text has changed, so look for its first occurrence in our
    // editor, should there be some text to search otherwise go back to our
    // original position

    if (pText.isEmpty())
        mEditor->setSelection(mCurrentLine, mCurrentColumn,
                              mCurrentLine, mCurrentColumn);
    else
        findText(pText, true);
}

//==============================================================================

}   // namespace Editor
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
