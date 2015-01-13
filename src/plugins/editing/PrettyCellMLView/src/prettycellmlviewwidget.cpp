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
// Pretty CellML view widget
//==============================================================================

#include "cellmlsupportplugin.h"
#include "corecliutils.h"
#include "corecellmleditingwidget.h"
#include "editorlistwidget.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "prettycellmlviewcellmltoprettycellmlconverter.h"
#include "prettycellmlviewlexer.h"
#include "prettycellmlviewwidget.h"
#include "qscintillawidget.h"
#include "settings.h"
#include "viewerwidget.h"

//==============================================================================

#include "ui_prettycellmlviewwidget.h"

//==============================================================================

#include <QLabel>
#include <QKeyEvent>
#include <QLayout>
#include <QMetaType>
#include <QSettings>
#include <QTimer>
#include <QVariant>

//==============================================================================

namespace OpenCOR {
namespace PrettyCellMLView {

//==============================================================================

PrettyCellmlViewWidget::PrettyCellmlViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::PrettyCellmlViewWidget),
    mNeedLoadingSettings(true),
    mSettingsGroup(QString()),
    mEditingWidget(0),
    mEditingWidgets(QMap<QString, CoreCellMLEditing::CoreCellmlEditingWidget *>()),
    mEditingWidgetsSha1(QMap<QString, QString>()),
    mSuccessfulConversions(QMap<QString, bool>())
{
    // Set up the GUI

    mGui->setupUi(this);
}

//==============================================================================

PrettyCellmlViewWidget::~PrettyCellmlViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void PrettyCellmlViewWidget::loadSettings(QSettings *pSettings)
{
    // Normally, we would retrieve the editing widget's settings, but
    // mEditingWidget is not set at this stage. So, instead, we keep track of
    // our settings' group and load them when initialising ourselves (see
    // initialize())...

    mSettingsGroup = pSettings->group();
}

//==============================================================================

void PrettyCellmlViewWidget::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);
    // Note: our view is such that our settings are actually saved when calling
    //       finalize() on the last file...
}

//==============================================================================

void PrettyCellmlViewWidget::retranslateUi()
{
    // Retranslate all our editing widgets

    foreach (CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget, mEditingWidgets)
        editingWidget->retranslateUi();
}

//==============================================================================

bool PrettyCellmlViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file, i.e. whether we have an
    // editing widget for it

    return mEditingWidgets.value(pFileName);
}

//==============================================================================

void PrettyCellmlViewWidget::initialize(const QString &pFileName,
                                        const bool &pUpdate)
{
    // Retrieve the editing widget associated with the given file, if any

    CoreCellMLEditing::CoreCellmlEditingWidget *newEditingWidget = mEditingWidgets.value(pFileName);

    if (!newEditingWidget) {
        // No editing widget exists for the given file, so create one and
        // populate it with its pretty CellML version

        PrettyCellMLViewCellmlToPrettyCellmlConverter converter(pFileName);
        bool successfulConversion = converter.execute();

        newEditingWidget = new CoreCellMLEditing::CoreCellmlEditingWidget(converter.output(),
                                                                          !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                                          new PrettyCellmlViewLexer(this),
                                                                          parentWidget());

        if (!successfulConversion) {
            // The conversion wasn't successful, so make the editor read-only
            // (since its contents is that of the file itself) and add a couple
            // of messages to our editor list

            newEditingWidget->editor()->setReadOnly(true);
            // Note: CoreEditingPlugin::filePermissionsChanged() will do the
            //       same as above, but this will take a wee bit of time
            //       while we want it done straightaway...

            newEditingWidget->editorList()->addItem(EditorList::EditorListItem::Error,
                                                    converter.errorLine(),
                                                    converter.errorColumn(),
                                                    Core::formatErrorMessage(converter.errorMessage(), false)+".");

            newEditingWidget->editorList()->addItem(EditorList::EditorListItem::Hint,
                                                    tr("You might want to use the Raw (CellML) view to edit the file."));
        }

        // Keep track of our editing widget (and of whether the conversion was
        // successful) and add it to ourselves

        mEditingWidgets.insert(pFileName, newEditingWidget);
        mEditingWidgetsSha1.insert(pFileName, Core::sha1(newEditingWidget->editor()->contents()));
        mSuccessfulConversions.insert(pFileName, successfulConversion);

        layout()->addWidget(newEditingWidget);

        // Add support for some key mappings to our editor

        connect(newEditingWidget->editor()->editor(), SIGNAL(keyPressed(QKeyEvent *, bool &)),
                this, SLOT(editorKeyPressed(QKeyEvent *, bool &)));
    }

    // Update our editing widget, if required

    if (pUpdate) {
        CoreCellMLEditing::CoreCellmlEditingWidget *oldEditingWidget = mEditingWidget;

        mEditingWidget = newEditingWidget;

        // Load our settings, if needed, or reset our editing widget using the
        // 'old' one

        if (mNeedLoadingSettings) {
            QSettings settings(SettingsOrganization, SettingsApplication);

            settings.beginGroup(mSettingsGroup);
                newEditingWidget->loadSettings(&settings);
            settings.endGroup();

            mNeedLoadingSettings = false;
        } else {
            newEditingWidget->updateSettings(oldEditingWidget);
        }

        // Select the first issue, if any, with the current file
        // Note: we use a single shot to give time to the setting up of the
        //       editing widget to complete...

        if (!mSuccessfulConversions.value(pFileName))
            QTimer::singleShot(0, this, SLOT(selectFirstItemInEditorList()));

        // Show/hide our editing widgets

        newEditingWidget->show();

        if (oldEditingWidget && (newEditingWidget != oldEditingWidget))
            oldEditingWidget->hide();

        // Set our focus proxy to our 'new' editing widget and make sure that
        // the latter immediately gets the focus
        // Note: if we were not to immediately give the focus to our 'new'
        //       editing widget, then the central widget would give the focus to
        //       our 'old' editing widget (see CentralWidget::updateGui()),
        //       which is clearly not what we want...

        setFocusProxy(newEditingWidget->editor());

        newEditingWidget->editor()->setFocus();
    } else {
        // Hide our 'new' editing widget

        newEditingWidget->hide();
    }
}

//==============================================================================

void PrettyCellmlViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget) {
        // There is an editing widget for the given file name, so save our
        // settings and reset our memory of the current editing widget, if
        // needed

        if (mEditingWidget == editingWidget) {
            QSettings settings(SettingsOrganization, SettingsApplication);

            settings.beginGroup(mSettingsGroup);
                editingWidget->saveSettings(&settings);
            settings.endGroup();

            mNeedLoadingSettings = true;
            mEditingWidget = 0;
        }

        // Delete the editor and remove it from our list

        delete editingWidget;

        mEditingWidgets.remove(pFileName);
    }
}

//==============================================================================

void PrettyCellmlViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed
    // Note: if the view for the given file is not the active view, then to call
    //       finalize() and then initialize() would activate the contents of the
    //       view (but the file tab would still point to the previously active
    //       file). However, we want to the 'old' file to remain the active one,
    //       hence the extra argument we pass to initialize()...

    if (contains(pFileName)) {
        bool update = mEditingWidget == mEditingWidgets.value(pFileName);

        finalize(pFileName);

        if (CellMLSupport::isCellmlFile(pFileName))
            initialize(pFileName, update);
    }
}

//==============================================================================

void PrettyCellmlViewWidget::fileRenamed(const QString &pOldFileName,
                                         const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);
    QString editingWidgetSha1 = mEditingWidgetsSha1.value(pOldFileName);
    bool successfulConversion = mSuccessfulConversions.value(pOldFileName);

    if (editingWidget) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgetsSha1.insert(pNewFileName, editingWidgetSha1);
        mSuccessfulConversions.insert(pNewFileName, successfulConversion);

        mEditingWidgets.remove(pOldFileName);
        mEditingWidgetsSha1.remove(pOldFileName);
        mSuccessfulConversions.remove(pOldFileName);
    }
}

//==============================================================================

Editor::EditorWidget * PrettyCellmlViewWidget::editor(const QString &pFileName) const
{
    // Return the requested editor

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    return editingWidget?editingWidget->editor():0;
}

//==============================================================================

bool PrettyCellmlViewWidget::isEditorUseable(const QString &pFileName) const
{
    // Return whether the requested editor is useable

    return mSuccessfulConversions.value(pFileName);
}

//==============================================================================

bool PrettyCellmlViewWidget::isEditorContentsModified(const QString &pFileName) const
{
    // Return whether the contents of the requested editor has been modified

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    return editingWidget?Core::sha1(editingWidget->editor()->contents()).compare(mEditingWidgetsSha1.value(pFileName)):false;
}

//==============================================================================

QList<QWidget *> PrettyCellmlViewWidget::statusBarWidgets() const
{
    // Return our status bar widgets

    if (mEditingWidget)
        return QList<QWidget *>() << mEditingWidget->editor()->cursorPositionWidget()
                                  << mEditingWidget->editor()->editingModeWidget();
    else
        return QList<QWidget *>();
}

//==============================================================================

static const auto Comment = QStringLiteral("//");
static const int CommentLength = Comment.length();

//==============================================================================

static const auto StartComment = QStringLiteral("/*");
static const auto EndComment = QStringLiteral("*/");
static const int StartCommentLength = StartComment.length();
static const int EndCommentLength = EndComment.length();

//==============================================================================

void PrettyCellmlViewWidget::commentOrUncommentLine(QScintillaSupport::QScintillaWidget *pEditor,
                                                    const int &pLineNumber,
                                                    const bool &pCommentLine)
{
    // (Un)comment the current line

    QString line = pEditor->text(pLineNumber).trimmed();

    if (!line.isEmpty()) {
        // We are not dealing with an empty line, so we can (un)comment it

        if (pCommentLine) {
            pEditor->insertAt(Comment, pLineNumber, 0);
        } else {
            // Uncomment the line, should it be commented

            if (line.startsWith(Comment)) {
                int commentLineNumber, commentColumnNumber;

                pEditor->lineIndexFromPosition(pEditor->findTextInRange(pEditor->positionFromLineIndex(pLineNumber, 0),
                                                                        pEditor->contentsSize(), Comment),
                                               &commentLineNumber, &commentColumnNumber);

                pEditor->setSelection(commentLineNumber, commentColumnNumber,
                                      commentLineNumber, commentColumnNumber+CommentLength);
                pEditor->removeSelectedText();
            }
        }
    }
}

//==============================================================================

void PrettyCellmlViewWidget::editorKeyPressed(QKeyEvent *pEvent, bool &pHandled)
{
    // Some key combinations from our editor

    if (   !(pEvent->modifiers() & Qt::ShiftModifier)
        &&  (pEvent->modifiers() & Qt::ControlModifier)
        && !(pEvent->modifiers() & Qt::AltModifier)
        && !(pEvent->modifiers() & Qt::MetaModifier)
        &&  (pEvent->key() == Qt::Key_Slash)) {
        // We want to (un)comment the selected text, if any, or the current line

        QScintillaSupport::QScintillaWidget *editor = qobject_cast<QScintillaSupport::QScintillaWidget *>(sender());

        // Retrieve the position of our cursor within the editor

        int lineNumber, columnNumber;

        editor->getCursorPosition(&lineNumber, &columnNumber);

        // Check whether some text is selected

        if (editor->hasSelectedText()) {
            // Some text is selected, so check whether full lines are selected
            // or 'random' text

            int lineFrom, columnFrom, lineTo, columnTo;

            editor->getSelection(&lineFrom, &columnFrom, &lineTo, &columnTo);

            int selectedTextEndPosition = editor->positionFromLineIndex(lineTo, columnTo);
            QString editorEolString = editor->eolString();

            if (    !columnFrom
                && (   !columnTo
                    ||  (selectedTextEndPosition == editor->length())
                    || !editor->textInRange(selectedTextEndPosition, selectedTextEndPosition+editorEolString.length()).compare(editorEolString)
                    || !editor->textInRange(selectedTextEndPosition, selectedTextEndPosition+1).compare("\0"))) {
                // The selected text consists of full lines, so check whether
                // it's surrounded by /* XXX */

                QString trimmedSelectedText = editor->selectedText().trimmed();

                if (   trimmedSelectedText.startsWith(StartComment)
                    && trimmedSelectedText.endsWith(EndComment)) {
                    // The full lines are surrounded by /* XXX */, so simply
                    // remove them

                    QString selectedText = editor->selectedText();

                    selectedText.remove(selectedText.indexOf(StartComment), StartCommentLength);
                    selectedText.remove(selectedText.indexOf(EndComment), EndCommentLength);

                    editor->replaceSelectedText(selectedText);

                    // Prepare ourselves for reselecting the lines

                    columnTo -= EndCommentLength;
                } else {
                    // The full lines are not surrounded by /* XXX */, so simply
                    // (un)comment them

                    bool commentLine = false;
                    QString line;
                    int iMax = columnTo?lineTo:lineTo-1;

                    // Determine whether we should be commenting or uncommenting the
                    // lines

                    for (int i = lineFrom; i <= iMax; ++i) {
                        line = editor->text(i).trimmed();

                        commentLine = commentLine || (!line.isEmpty() && !line.startsWith(Comment));
                    }

                    // (Un)comment the lines as one 'big' action

                    editor->beginUndoAction();

                    for (int i = lineFrom; i <= iMax; ++i)
                        commentOrUncommentLine(editor, i, commentLine);

                    editor->endUndoAction();

                    // Prepare ourselves for reselecting the lines

                    columnTo += columnTo?
                                    commentLine?CommentLength:-CommentLength:
                                    0;
                }
            } else {
                // The selected text doesn't consist of full lines, so simply
                // (un)comment it

                QString selectedText = editor->selectedText();
                bool commentSelectedText =    !selectedText.startsWith(StartComment)
                                           || !selectedText.endsWith(EndComment);

                if (commentSelectedText) {
                    // The selected text is not commented, so comment it

                    editor->replaceSelectedText(StartComment+selectedText+EndComment);
                } else {
                    // The selected text is commented, so uncomment it

                    editor->replaceSelectedText(selectedText.mid(StartCommentLength, selectedText.length()-StartCommentLength-EndCommentLength));
                }

                // Prepare ourselves for reselecting the text

                columnTo += (commentSelectedText?1:-1)*(StartCommentLength+(lineFrom == lineTo)*EndCommentLength);
            }

            // Reselect the text/lines

            if ((lineNumber == lineFrom) && (columnNumber == columnFrom))
                editor->setSelection(lineTo, columnTo, lineFrom, columnFrom);
            else
                editor->setSelection(lineFrom, columnFrom, lineTo, columnTo);
        } else {
            // No text is selected, so simply (un)comment the current line

            bool commentLine = !editor->text(lineNumber).trimmed().startsWith(Comment);

            commentOrUncommentLine(editor, lineNumber, commentLine);

            if (commentLine) {
                // We commented the line, so our position will be fine unless we
                // were at the beginning of the line, in which case we need to
                // update it

                if (!columnNumber)
                    editor->QsciScintilla::setCursorPosition(lineNumber, columnNumber+CommentLength);
            } else {
                // We uncommented the line, so go back to our original position
                // (since uncommenting the line will have shifted it a bit)

                editor->QsciScintilla::setCursorPosition(lineNumber, columnNumber-CommentLength);
            }
        }

        pHandled = true;
    } else {
        pHandled = false;
    }
}

//==============================================================================

void PrettyCellmlViewWidget::selectFirstItemInEditorList()
{
    // Select the first item in the current editor list

    mEditingWidget->editorList()->selectFirstItem();
}

//==============================================================================

}   // namespace PrettyCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
