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

#include "filemanager.h"
#include "qscintillawidget.h"

//==============================================================================

#include <QDragEnterEvent>
#include <QMenu>
#include <QMimeData>

//==============================================================================

#include "Qsci/qscilexer.h"

//==============================================================================

namespace OpenCOR {
namespace QScintillaSupport {

//==============================================================================

void QScintillaWidget::constructor(const QString &pContents,
                                   const bool &pReadOnly, QsciLexer *pLexer)
{
    // Remove the frame around our Scintilla editor

    setFrameShape(QFrame::NoFrame);

    // Remove the margin number in our Scintilla editor

    setMarginWidth(SC_MARGIN_NUMBER, 0);

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

        // Specify the type of tree folding to be used. Some lexers may indeed
        // use that feature, so...

        setFolding(QsciScintilla::BoxedTreeFoldStyle);
    } else {
        // No lexer was provided, so simply specify a default font family and
        // size for our Scintilla editor

        setFont(mFont);
    }

    // Set the contents of our Scintilla editor and its read-only property

    setContents(pContents);
    setReadOnly(pReadOnly);

    // Empty context menu by default

    mContextMenu = new QMenu(this);

    // Can't undo/redo by default

    mCanUndo = false;
    mCanRedo = false;

    // Can't select all the text by default

    mCanSelectAll = false;

    // Keep track of changes to our editor that may affect our ability to select
    // all of its text
    // Note: we use the SCN_MODIFIED() signal rather than the textChanged()
    //       signal since the latter is only emitted when inserting or deleting
    //       some text...

    connect(this, SIGNAL(selectionChanged()),
            this, SLOT(checkCanSelectAll()));
    connect(this, SIGNAL(SCN_MODIFIED(int, int, const char *, int, int, int, int, int, int, int)),
            this, SLOT(checkCanSelectAll()));
}

//==============================================================================

QScintillaWidget::QScintillaWidget(const QString &pContents,
                                   const bool &pReadOnly,
                                   QsciLexer *pLexer, QWidget *pParent) :
    QsciScintilla(pParent)
{
    // Construct our object

    constructor(pContents, pReadOnly, pLexer);
}

//==============================================================================

QScintillaWidget::QScintillaWidget(QWidget *pParent) :
    QsciScintilla(pParent)
{
    // Construct our object

    constructor();
}

//==============================================================================

QScintillaWidget::~QScintillaWidget()
{
    // Delete some internal objects

    delete mContextMenu;
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

bool QScintillaWidget::isSelectAllAvailable() const
{
    // Return whether we can select all the text

    return mCanSelectAll;
}

//==============================================================================

void QScintillaWidget::resetUndoHistory()
{
    // Reset our undo history

    SendScintilla(SCI_EMPTYUNDOBUFFER);
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
    //       (e.g. file:///home/me/myFile) to be inserted in the text, so...

    if (!pEvent->mimeData()->hasFormat(Core::FileSystemMimeType))
        pEvent->acceptProposedAction();
    else
        pEvent->ignore();
}

//==============================================================================

bool QScintillaWidget::event(QEvent *pEvent)
{
    // Bypass QsciScintilla's handling of event()
    // Note: indeed, QsciScintilla handles some shortcuts (e.g. Ctrl+L), which
    //       we don't want it to handle (e.g. Ctrl+L is used by QsciScintilla to
    //       delete the current line while OpenCOR uses it to (un)lock the
    //       current file), so...

    return QsciScintillaBase::event(pEvent);
}

//==============================================================================

void QScintillaWidget::keyPressEvent(QKeyEvent *pEvent)
{
    // Reset the font size, if needed

    if (   !(pEvent->modifiers() & Qt::ShiftModifier)
        &&  (pEvent->modifiers() & Qt::ControlModifier)
        && !(pEvent->modifiers() & Qt::AltModifier)
        && !(pEvent->modifiers() & Qt::MetaModifier)
        &&  (pEvent->key() == Qt::Key_0)) {
        zoomTo(0);
    } else {
        // Default handling of the event

        QsciScintilla::keyPressEvent(pEvent);

        // Check whether the undo/redo is possible

        bool newCanUndo = isUndoAvailable();
        bool newCanRedo = isRedoAvailable();

        if (newCanUndo != mCanUndo) {
            mCanUndo = newCanUndo;

            emit canUndo(mCanUndo);
        }

        if (newCanRedo != mCanRedo) {
            mCanRedo = newCanRedo;

            emit canRedo(mCanRedo);
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

}   // namespace QScintillaSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
