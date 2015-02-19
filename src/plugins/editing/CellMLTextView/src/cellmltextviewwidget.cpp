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
// CellML Text view widget
//==============================================================================

#include "cellmlsupportplugin.h"
#include "cellmltextviewconverter.h"
#include "cellmltextviewlexer.h"
#include "cellmltextviewparser.h"
#include "cellmltextviewwidget.h"
#include "corecliutils.h"
#include "corecellmleditingwidget.h"
#include "editorlistwidget.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "qscintillawidget.h"
#include "settings.h"
#include "viewerwidget.h"

//==============================================================================

#include "ui_cellmltextviewwidget.h"

//==============================================================================

#include <QLabel>
#include <QKeyEvent>
#include <QLayout>
#include <QMetaType>
#include <QSettings>
#include <QTimer>
#include <QVariant>

//==============================================================================

#include "Qsci/qscilexerxml.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewWidgetData::CellmlTextViewWidgetData(CoreCellMLEditing::CoreCellmlEditingWidget *pEditingWidget,
                                                   const QString &pSha1,
                                                   const bool &pValid,
                                                   QDomDocument pRdfNodes) :
    mEditingWidget(pEditingWidget),
    mSha1(pSha1),
    mValid(pValid),
    mRdfNodes(pRdfNodes)
{
}

//==============================================================================

CoreCellMLEditing::CoreCellmlEditingWidget * CellmlTextViewWidgetData::editingWidget() const
{
    // Return our editing widget

    return mEditingWidget;
}

//==============================================================================

QString CellmlTextViewWidgetData::sha1() const
{
    // Return our SHA-1 value

    return mSha1;
}

//==============================================================================

bool CellmlTextViewWidgetData::isValid() const
{
    // Return whether we are valid

    return mValid;
}

//==============================================================================

QDomDocument CellmlTextViewWidgetData::rdfNodes() const
{
    // Return our RDF nodes

    return mRdfNodes;
}

//==============================================================================

CellmlTextViewWidget::CellmlTextViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::CellmlTextViewWidget),
    mNeedLoadingSettings(true),
    mSettingsGroup(QString()),
    mEditingWidget(0),
    mData(QMap<QString, CellmlTextViewWidgetData>())
{
    // Set up the GUI

    mGui->setupUi(this);
}

//==============================================================================

CellmlTextViewWidget::~CellmlTextViewWidget()
{
    // Delete the GUI

    delete mGui;
}

//==============================================================================

void CellmlTextViewWidget::loadSettings(QSettings *pSettings)
{
    // Normally, we would retrieve the editing widget's settings, but
    // mEditingWidget is not set at this stage. So, instead, we keep track of
    // our settings' group and load them when initialising ourselves (see
    // initialize())...

    mSettingsGroup = pSettings->group();
}

//==============================================================================

void CellmlTextViewWidget::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);
    // Note: our view is such that our settings are actually saved when calling
    //       finalize() on the last file...
}

//==============================================================================

void CellmlTextViewWidget::retranslateUi()
{
    // Retranslate all our editing widgets

    foreach (const CellmlTextViewWidgetData &data, mData)
        data.editingWidget()->retranslateUi();
}

//==============================================================================

bool CellmlTextViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file

    return mData.contains(pFileName);
}

//==============================================================================

void CellmlTextViewWidget::initialize(const QString &pFileName,
                                      const bool &pUpdate)
{
    // Retrieve the editing widget associated with the given file, if any

    CoreCellMLEditing::CoreCellmlEditingWidget *newEditingWidget = mData.value(pFileName).editingWidget();

    if (!newEditingWidget) {
        // No editing widget exists for the given file, so generate a CellML
        // text version of the given CellML file

        static CellMLTextViewConverter converter;
        bool successfulConversion = converter.execute(pFileName);

        // Create an editing widget for the given CellML file

        newEditingWidget = new CoreCellMLEditing::CoreCellmlEditingWidget(converter.output(),
                                                                          !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                                          0, parentWidget());

        // Add the warnings, if any, that were generated by the converter

        if (converter.hasWarnings()) {
            foreach (const CellMLTextViewConverterWarning &warning, converter.warnings()) {
                newEditingWidget->editorList()->addItem(EditorList::EditorListItem::Warning,
                                                        successfulConversion?-1:warning.line(),
                                                        successfulConversion?
                                                            QString("[%1] ").arg(warning.line())+warning.message().arg(" "+tr("in the original CellML file")):
                                                            warning.message().arg(QString()));
            }
        }

        // Populate our editing widget with the CellML text version of the
        // given CellML file

        if (successfulConversion) {
            // The conversion was successful, so we can apply our CellML text
            // lexer to our editor

            newEditingWidget->editor()->editor()->setLexer(new CellmlTextViewLexer(this));
        } else {
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
                                                    Core::formatMessage(converter.errorMessage(), false)+".");

            newEditingWidget->editorList()->addItem(EditorList::EditorListItem::Hint,
                                                    tr("You might want to use the Raw (CellML) view to edit the file."));

            // Apply an XML lexer to our editor

            newEditingWidget->editor()->editor()->setLexer(new QsciLexerXML(this));
        }

        // Keep track of our editing widget (and of whether the conversion was
        // successful) and add it to ourselves

        mData.insert(pFileName,
                     CellmlTextViewWidgetData(newEditingWidget,
                                              Core::sha1(newEditingWidget->editor()->contents()),
                                              successfulConversion,
                                              converter.rdfNodes()));

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

        if (!mData.value(pFileName).isValid())
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

void CellmlTextViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mData.value(pFileName).editingWidget();

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

        mData.remove(pFileName);
    }
}

//==============================================================================

void CellmlTextViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed
    // Note: if the view for the given file is not the active view, then to call
    //       finalize() and then initialize() would activate the contents of the
    //       view (but the file tab would still point to the previously active
    //       file). However, we want to the 'old' file to remain the active one,
    //       hence the extra argument we pass to initialize()...

    if (contains(pFileName)) {
        bool update = mEditingWidget == mData.value(pFileName).editingWidget();

        finalize(pFileName);

        if (CellMLSupport::isCellmlFile(pFileName))
            initialize(pFileName, update);
    }
}

//==============================================================================

void CellmlTextViewWidget::fileRenamed(const QString &pOldFileName,
                                       const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    CellmlTextViewWidgetData data = mData.value(pOldFileName);

    if (data.editingWidget()) {
        mData.insert(pNewFileName, data);
        mData.remove(pOldFileName);
    }
}

//==============================================================================

Editor::EditorWidget * CellmlTextViewWidget::editor(const QString &pFileName) const
{
    // Return the requested editor

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mData.value(pFileName).editingWidget();

    return editingWidget?editingWidget->editor():0;
}

//==============================================================================

bool CellmlTextViewWidget::isEditorUseable(const QString &pFileName) const
{
    // Return whether the requested editor is useable

    return mData.value(pFileName).isValid();
}

//==============================================================================

bool CellmlTextViewWidget::isEditorContentsModified(const QString &pFileName) const
{
    // Return whether the contents of the requested editor has been modified

    CellmlTextViewWidgetData data = mData.value(pFileName);

    return data.editingWidget()?Core::sha1(data.editingWidget()->editor()->contents()).compare(data.sha1()):false;
}

//==============================================================================

bool CellmlTextViewWidget::saveFile(const QString &pOldFileName,
                                    const QString &pNewFileName)
{
    // Save the given file

    Editor::EditorWidget *currentEditor = editor(pOldFileName);

    if (currentEditor) {
        // Parse the contents of the editor and, if successful, serialise its
        // corresponding DOM document, making sure that we include any metadata
        // that was in the original CellML file

        static CellmlTextViewParser parser;

        mEditingWidget->editorList()->clear();

        bool successfulParsing = parser.execute(currentEditor->contents());

        if (successfulParsing) {
            QDomDocument domDocument = parser.domDocument();
            QDomDocument rdfNodes = mData.value(pOldFileName).rdfNodes();

            domDocument.appendChild(rdfNodes.cloneNode());

qDebug("---[SERIALISSATION]---");
qDebug("FROM: %s", qPrintable(pOldFileName));
qDebug("TO:   %s", qPrintable(pNewFileName));
qDebug("---------");
qDebug("%s", qPrintable(qDomDocumentToString(domDocument)));
qDebug("---------");

//        return Core::writeTextToFile(pNewFileName, qDomDocumentToString(domDocument));

//---GRY--- RETURN false FOR NOW UNTIL WE ARE DONE WITH THE SAVING OF A FILE...
            return false;
        } else {
            // Add the messages that were generated by the parser and select the
            // first one of them

            foreach (const CellmlTextViewParserMessage &message, parser.messages()) {
                mEditingWidget->editorList()->addItem(message.isError()?EditorList::EditorListItem::Error:EditorList::EditorListItem::Warning,
                                                      message.line(), message.column(),
                                                      message.message());
            }

            selectFirstItemInEditorList();

            return false;
        }
    } else {
        return false;
    }
}

//==============================================================================

QList<QWidget *> CellmlTextViewWidget::statusBarWidgets() const
{
    // Return our status bar widgets

    if (mEditingWidget)
        return QList<QWidget *>() << mEditingWidget->editor()->cursorPositionWidget()
                                  << mEditingWidget->editor()->editingModeWidget();
    else
        return QList<QWidget *>();
}

//==============================================================================

static const auto SingleLineCommentString = QStringLiteral("//");
static const int SingleLineCommentLength  = SingleLineCommentString.length();

//==============================================================================

static const auto StartMultilineCommentString = QStringLiteral("/*");
static const auto EndMultilineCommentString   = QStringLiteral("*/");
static const int StartMultilineCommentLength  = StartMultilineCommentString.length();
static const int EndMultilineCommentLength    = EndMultilineCommentString.length();

//==============================================================================

void CellmlTextViewWidget::commentOrUncommentLine(QScintillaSupport::QScintillaWidget *pEditor,
                                                  const int &pLineNumber,
                                                  const bool &pCommentLine)
{
    // (Un)comment the current line

    QString line = pEditor->text(pLineNumber).trimmed();

    if (!line.isEmpty()) {
        // We are not dealing with an empty line, so we can (un)comment it

        if (pCommentLine) {
            pEditor->insertAt(SingleLineCommentString, pLineNumber, 0);
        } else {
            // Uncomment the line, should it be commented

            if (line.startsWith(SingleLineCommentString)) {
                int commentLineNumber, commentColumnNumber;

                pEditor->lineIndexFromPosition(pEditor->findTextInRange(pEditor->positionFromLineIndex(pLineNumber, 0),
                                                                        pEditor->contentsSize(), SingleLineCommentString),
                                               &commentLineNumber, &commentColumnNumber);

                pEditor->setSelection(commentLineNumber, commentColumnNumber,
                                      commentLineNumber, commentColumnNumber+SingleLineCommentLength);
                pEditor->removeSelectedText();
            }
        }
    }
}

//==============================================================================

void CellmlTextViewWidget::editorKeyPressed(QKeyEvent *pEvent, bool &pHandled)
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

                if (   trimmedSelectedText.startsWith(StartMultilineCommentString)
                    && trimmedSelectedText.endsWith(EndMultilineCommentString)) {
                    // The full lines are surrounded by /* XXX */, so simply
                    // remove them

                    QString selectedText = editor->selectedText();

                    selectedText.remove(selectedText.indexOf(StartMultilineCommentString), StartMultilineCommentLength);
                    selectedText.remove(selectedText.indexOf(EndMultilineCommentString), EndMultilineCommentLength);

                    editor->replaceSelectedText(selectedText);

                    // Prepare ourselves for reselecting the lines

                    columnTo -= EndMultilineCommentLength;
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

                        commentLine = commentLine || (!line.isEmpty() && !line.startsWith(SingleLineCommentString));
                    }

                    // (Un)comment the lines as one 'big' action

                    editor->beginUndoAction();

                    for (int i = lineFrom; i <= iMax; ++i)
                        commentOrUncommentLine(editor, i, commentLine);

                    editor->endUndoAction();

                    // Prepare ourselves for reselecting the lines

                    columnTo += columnTo?
                                    commentLine?SingleLineCommentLength:-SingleLineCommentLength:
                                    0;
                }
            } else {
                // The selected text doesn't consist of full lines, so simply
                // (un)comment it

                QString selectedText = editor->selectedText();
                bool commentSelectedText =    !selectedText.startsWith(StartMultilineCommentString)
                                           || !selectedText.endsWith(EndMultilineCommentString);

                if (commentSelectedText) {
                    // The selected text is not commented, so comment it

                    editor->replaceSelectedText(StartMultilineCommentString+selectedText+EndMultilineCommentString);
                } else {
                    // The selected text is commented, so uncomment it

                    editor->replaceSelectedText(selectedText.mid(StartMultilineCommentLength, selectedText.length()-StartMultilineCommentLength-EndMultilineCommentLength));
                }

                // Prepare ourselves for reselecting the text

                columnTo += (commentSelectedText?1:-1)*(StartMultilineCommentLength+(lineFrom == lineTo)*EndMultilineCommentLength);
            }

            // Reselect the text/lines

            if ((lineNumber == lineFrom) && (columnNumber == columnFrom))
                editor->setSelection(lineTo, columnTo, lineFrom, columnFrom);
            else
                editor->setSelection(lineFrom, columnFrom, lineTo, columnTo);
        } else {
            // No text is selected, so simply (un)comment the current line

            bool commentLine = !editor->text(lineNumber).trimmed().startsWith(SingleLineCommentString);

            commentOrUncommentLine(editor, lineNumber, commentLine);

            if (commentLine) {
                // We commented the line, so our position will be fine unless we
                // were at the beginning of the line, in which case we need to
                // update it

                if (!columnNumber)
                    editor->QsciScintilla::setCursorPosition(lineNumber, columnNumber+SingleLineCommentLength);
            } else {
                // We uncommented the line, so go back to our original position
                // (since uncommenting the line will have shifted it a bit)

                editor->QsciScintilla::setCursorPosition(lineNumber, columnNumber-SingleLineCommentLength);
            }
        }

        pHandled = true;
    } else {
        pHandled = false;
    }
}

//==============================================================================

void CellmlTextViewWidget::selectFirstItemInEditorList()
{
    // Select the first item in the current editor list

    mEditingWidget->editorList()->selectFirstItem();
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
