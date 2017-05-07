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
#include "editorwidgetfindreplacewidget.h"
#include "qscintillawidget.h"

//==============================================================================

#include <QFrame>
#include <QKeyEvent>
#include <QLabel>
#include <QRegularExpression>
#include <QSettings>
#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace EditorWidget {

//==============================================================================

EditorWidget::EditorWidget(const QString &pContents, const bool &pReadOnly,
                           QsciLexer *pLexer, QWidget *pParent) :
    Core::Widget(pParent),
    mCurrentLine(0),
    mCurrentColumn(0),
    mFindReplaceVisible(false),
    mReadOnlyStyles(QIntList())
{
    // Create and set our vertical layout

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->setContentsMargins(QMargins());
    layout->setSpacing(0);

    setLayout(layout);

    // Create our editor and find/replace widget

    mEditor = new QScintillaSupport::QScintillaWidget(pLexer, this);
    mSeparator = Core::newLineWidget(this);
    mFindReplace = new EditorWidgetFindReplaceWidget(this);

    // Check which styles can have their background changed when making them
    // read-only or writable

    QColor baseColor = Core::baseColor();

    for (int i = 0; i < QsciScintillaBase::STYLE_MAX; ++i) {
        if (mEditor->backgroundColor(i) == baseColor)
            mReadOnlyStyles << i;
    }

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

    connect(mEditor, SIGNAL(textChanged()),
            this, SIGNAL(textChanged()));

    connect(mEditor, SIGNAL(copyAvailable(bool)),
            this, SIGNAL(copyAvailable(const bool &)));

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

    layout->addWidget(mEditor);
    layout->addWidget(mSeparator);
    layout->addWidget(mFindReplace);

    // Make our editor our focus proxy

    setFocusProxy(mEditor);

    // Initially hide our find/replace widget

    setFindReplaceVisible(false);
}

//==============================================================================

static const auto SettingsEditorWidgetZoomLevel = QStringLiteral("EditorWidgetZoomLevel");

//==============================================================================

void EditorWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve our settings

    setZoomLevel(pSettings->value(SettingsEditorWidgetZoomLevel, 0).toInt());
}

//==============================================================================

void EditorWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our settings

    pSettings->setValue(SettingsEditorWidgetZoomLevel, zoomLevel());
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

    if (!pEditorWidget || (pEditorWidget == this))
        return;

    // Update our zoom level

    setZoomLevel(pEditorWidget->zoomLevel());

    // Show/hide our find/replace widget

    setFindReplaceVisible(pEditorWidget->findReplaceIsVisible());

    // Update the find/replace widget itself
    // Note: we must inactivate (and then reactivate) our find/replace widget
    //       otherwise opening a new file or switching to another will result in
    //       the find text being automatically searched (see the
    //       findTextChanged() slot)...

    mFindReplace->setActive(false);
        mFindReplace->updateFrom(pEditorWidget->findReplace());
    mFindReplace->setActive(true);
}

//==============================================================================

QScintillaSupport::QScintillaWidget * EditorWidget::editor() const
{
    // Return our editor

    return mEditor;
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

void EditorWidget::setCursorPosition(const int &pLine, const int &pColumn)
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

void EditorWidget::setContents(const QString &pContents,
                               const bool &pKeepHistory)
{
    // Set the contents of our editor

    mEditor->setContents(pContents, pKeepHistory);
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

    // Update our background colour

    QColor baseColor = Core::baseColor();
    QColor lockedColor = Core::lockedColor(baseColor);
    QColor newBackgroundColor = pReadOnly?lockedColor:baseColor;

    foreach (const int &readOnlyStyle, mReadOnlyStyles)
        mEditor->setBackgroundColor(readOnlyStyle, newBackgroundColor);
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
                                  const bool &pRegularExpression,
                                  const bool &pCaseSensitive,
                                  const bool &pWholeWordsOnly) const
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

EditorWidgetFindReplaceWidget * EditorWidget::findReplace()
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

void EditorWidget::setFindReplaceVisible(const bool &pVisible)
{
    // Set our find text, if we are to show our find/replace widget
    // Note: if we are over a word, then we want it to become our find text, but
    //       if we make it so then a call to findText() will be triggered if the
    //       find text is different. Clearly, we don't want this to happen,
    //       hence we inactivate and then reactivate our find/replace widget...

    if (pVisible) {
        QString crtWord = mEditor->wordAt(mCurrentLine, mCurrentColumn);

        if (!crtWord.isEmpty()) {
            mFindReplace->setActive(false);
                mEditor->selectWordAt(mCurrentLine, mCurrentColumn);

                mFindReplace->setFindText(crtWord);
            mFindReplace->setActive(true);
        } else {
            mFindReplace->selectFindText();
        }
    }

    // Show/hide our find/replace widget

    mFindReplaceVisible = pVisible;

    mSeparator->setVisible(pVisible);
    mFindReplace->setVisible(pVisible);

    // Give the focus to our find/replace widget or to our editor, depending on
    // the case

    if (pVisible)
        mFindReplace->setFocus();
    else
        mEditor->setFocus();
}

//==============================================================================

int EditorWidget::styleAt(const int &pPosition) const
{
    // Return the style used at the given position

    return mEditor->SendScintilla(QsciScintilla::SCI_GETSTYLEAT,
                                  mEditor->text().left(pPosition).toUtf8().length());
}

//==============================================================================

bool EditorWidget::findPrevious()
{
    // Find the previous occurrence of the text in our editor

    int oldPosition = mEditor->currentPosition();

    mEditor->setCurrentPosition(oldPosition-mEditor->selectedText().length());
    mEditor->getCursorPosition(&mCurrentLine, &mCurrentColumn);

    bool res = findText(mFindReplace->findText(), false);

    if (!res)
        mEditor->setCurrentPosition(oldPosition);

    return res;
}

//==============================================================================

bool EditorWidget::findNext()
{
    // Find the next occurrence of the text in our editor

    mEditor->getCursorPosition(&mCurrentLine, &mCurrentColumn);

    return findText(mFindReplace->findText(), true);
}

//==============================================================================

void EditorWidget::replace()
{
    // Replace the current text

    if (mFindReplace->useRegularExpression()) {
        // The find/replace is done using a regular expression, so only replace
        // the currently selected text if it matches the regular expression

        if (QRegularExpression(mFindReplace->findText(),
                               mFindReplace->isCaseSensitive()?
                                   QRegularExpression::NoPatternOption:
                                   QRegularExpression::CaseInsensitiveOption).match(mEditor->selectedText()).hasMatch()) {
            mEditor->replace(mFindReplace->replaceText());
        }
    } else {
        // The find/replace is done using a simple match, which may be case
        // sensitive and/or may require whole words

        // Make sure that the currently selected text is a whole word, should
        // this be requested

        QString crtSelectedText = mEditor->selectedText();
        int crtLine;
        int crtColumn;

        mEditor->getCursorPosition(&crtLine, &crtColumn);

        if (   mFindReplace->searchWholeWordsOnly()
            && crtSelectedText.compare(mEditor->wordAt(crtLine, crtColumn))) {
            return;
        }

        // Replace the currently selected text if we have a match

        if (!crtSelectedText.compare(mFindReplace->findText(),
                                         mFindReplace->isCaseSensitive()?
                                             Qt::CaseSensitive:
                                             Qt::CaseInsensitive)) {
            mEditor->replace(mFindReplace->replaceText());
        }
    }
}

//==============================================================================

void EditorWidget::replaceAndFind()
{
    // Replace the current text and the find the next occurence of the text

    replace();
    findNext();
}

//==============================================================================

void EditorWidget::replaceAll()
{
    // Replace all the occurences of the text
    // Note: the original plan was to call mEditor->findFirst() the first time
    //       round and with wrapping disabled, and then mEditor->findNext()
    //       until we have found all occurrences, but for some reasons this can
    //       result in some occurrences being missed, hence the way we do it
    //       below...

    // Keep track of the first visible line and of our position

    int origFirstVisibleLine = mEditor->firstVisibleLine();
    int origLine;
    int origColumn;

    mEditor->getCursorPosition(&origLine, &origColumn);

    // Go to the beginning of the of the editor

    mEditor->QsciScintilla::setCursorPosition(0, 0);

    // Replace all occurrences

    int oldLine = 0;
    int oldColumn = 0;
    int newLine;
    int newColumn;

    while (findNext()) {
        // Retrieve our new position

        mEditor->getCursorPosition(&newLine, &newColumn);

        // Make sure that our new position is not 'before' our old one

        if (   (newLine < oldLine)
            || ((newLine == oldLine) && (newColumn < oldColumn))) {
            break;
        }

        // Our new position is fine, so replace the occurrence

        mEditor->replace(mFindReplace->replaceText());

        // Get ready for the next occurrence

        oldLine = newLine;
        oldColumn = newColumn;
    }

    // Reset the first visible line and go to our original position, after
    // having corrected it, if needed

    mEditor->setFirstVisibleLine(origFirstVisibleLine);

    origLine = qMin(origLine, mEditor->lines()-1);
    origColumn = qMin(origColumn, mEditor->lineLength(origLine)-1);

    mEditor->QsciScintilla::setCursorPosition(origLine, origColumn);
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
        setFindReplaceVisible(false);

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
        setFindReplaceVisible(false);

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
                              mFindReplace->useRegularExpression(),
                              mFindReplace->isCaseSensitive(),
                              mFindReplace->searchWholeWordsOnly(),
                              true, pForward, mCurrentLine, mCurrentColumn);
}

//==============================================================================

void EditorWidget::findTextChanged(const QString &pText)
{
    // The find text has changed, so look for its first occurrence in our
    // editor, should there be some text to search otherwise go back to our
    // original position

    if (pText.isEmpty()) {
        mEditor->setSelection(mCurrentLine, mCurrentColumn,
                              mCurrentLine, mCurrentColumn);
    } else {
        findText(pText, true);
    }
}

//==============================================================================

}   // namespace EditorWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
