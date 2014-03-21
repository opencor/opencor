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

#include "editorwidget.h"
#include "qscintillawidget.h"

//==============================================================================

#include "ui_editorwidget.h"

//==============================================================================

#include <QVBoxLayout>

//==============================================================================

namespace OpenCOR {
namespace Editor {

//==============================================================================

EditorWidget::EditorWidget(const QString &pContents, const bool &pReadOnly,
                           QsciLexer *pLexer, QWidget *pParent) :
    QWidget(pParent),
    mGui(new Ui::EditorWidget)
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our editor

    mEditor = new QScintillaSupport::QScintillaWidget(pContents, pReadOnly,
                                                      pLexer, this);

    // Forward some signals that are emitted by our editor

    connect(mEditor, SIGNAL(SCN_ZOOM()),
            this, SLOT(zoomLevelChanged()));

    connect(mEditor, SIGNAL(cursorPositionChanged(int, int)),
            this, SIGNAL(cursorPositionChanged(const int &, const int &)));

    connect(mEditor, SIGNAL(textChanged()),
            this, SIGNAL(textChanged()));

    connect(mEditor, SIGNAL(copyAvailable(bool)),
            this, SIGNAL(copyAvailable(const bool &)));

    connect(mEditor, SIGNAL(canUndo(const bool &)),
            this, SIGNAL(canUndo(const bool &)));
    connect(mEditor, SIGNAL(canRedo(const bool &)),
            this, SIGNAL(canRedo(const bool &)));

    connect(mEditor, SIGNAL(canSelectAll(const bool &)),
            this, SIGNAL(canSelectAll(const bool &)));

    // Add our editor to our layout

    mGui->layout->addWidget(mEditor);
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
    // Set the read-only mode of our editor

    mEditor->setReadOnly(pReadOnly);
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

void EditorWidget::zoomLevelChanged()
{
    // Let people know that the zoom level has changed

    emit zoomLevelChanged(zoomLevel());
}

//==============================================================================

}   // namespace Editor
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
