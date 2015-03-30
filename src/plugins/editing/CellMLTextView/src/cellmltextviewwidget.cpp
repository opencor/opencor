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
// Note: this view and the Raw CellML view work in the same way when it comes to
//       updating the viewer (e.g. see the XSL transformation)...
//==============================================================================

#include "cellmlfilemanager.h"
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
#include "xsltransformer.h"

//==============================================================================

#include "ui_cellmltextviewwidget.h"

//==============================================================================

#include <QLabel>
#include <QKeyEvent>
#include <QLayout>
#include <QMetaType>
#include <QRegularExpression>
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
                                                   const CellMLSupport::CellmlFile::Version &pCellmlVersion,
                                                   QDomDocument pRdfNodes) :
    mEditingWidget(pEditingWidget),
    mSha1(pSha1),
    mValid(pValid),
    mCellmlVersion(pCellmlVersion),
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

CellMLSupport::CellmlFile::Version CellmlTextViewWidgetData::cellmlVersion() const
{
    // Return our CellML version

    return mCellmlVersion;
}

//==============================================================================

QDomDocument CellmlTextViewWidgetData::rdfNodes() const
{
    // Return our RDF nodes

    return mRdfNodes;
}

//==============================================================================

void CellmlTextViewWidgetData::setSha1(const QString &pSha1)
{
    // Set our SHA-1 value

    mSha1 = pSha1;
}

//==============================================================================

CellmlTextViewWidget::CellmlTextViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mGui(new Ui::CellmlTextViewWidget),
    mNeedLoadingSettings(true),
    mSettingsGroup(QString()),
    mEditingWidget(0),
    mData(QMap<QString, CellmlTextViewWidgetData>()),
    mConverter(CellMLTextViewConverter()),
    mParser(CellmlTextViewParser()),
    mEditorLists(QList<EditorList::EditorListWidget *>()),
    mPresentationMathmlEquations(QMap<QString, QString>()),
    mContentMathmlEquation(QString())
{
    // Set up the GUI

    mGui->setupUi(this);

    // Create our XSL transformer and create a connection to retrieve the result
    // of its XSL transformations

    mXslTransformer = new Core::XslTransformer();

    connect(mXslTransformer, SIGNAL(done(const QString &, const QString &)),
            this, SLOT(xslTransformationDone(const QString &, const QString &)));
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

    CellmlTextViewWidgetData data = mData.value(pFileName);
    CoreCellMLEditing::CoreCellmlEditingWidget *newEditingWidget = data.editingWidget();

    if (!newEditingWidget) {
        // No editing widget exists for the given file, so generate a CellML
        // text version of the given CellML file

        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        QString fileContents;

        Core::readTextFromFile(pFileName, fileContents);

        bool fileIsEmpty = fileContents.trimmed().isEmpty();
        bool successfulConversion = fileIsEmpty?true:mConverter.execute(fileContents);

        // Create an editing widget for the given CellML file

        newEditingWidget = new CoreCellMLEditing::CoreCellmlEditingWidget(fileIsEmpty?QString():mConverter.output(),
                                                                          !fileManagerInstance->isReadableAndWritable(pFileName),
                                                                          0, parentWidget());

        // Add the warnings, if any, that were generated by the converter

        if (!fileIsEmpty && mConverter.hasWarnings()) {
            foreach (const CellMLTextViewConverterWarning &warning, mConverter.warnings()) {
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

            // Update our viewer whenever necessary

            connect(newEditingWidget->editor(), SIGNAL(textChanged()),
                    this, SLOT(updateViewer()));
            connect(newEditingWidget->editor(), SIGNAL(cursorPositionChanged(const int &, const int &)),
                    this, SLOT(updateViewer()));
        } else {
            // The conversion wasn't successful, so make the editor read-only
            // (since its contents is that of the file itself) and add a couple
            // of messages to our editor list

            newEditingWidget->editor()->setReadOnly(true);
            // Note: CoreEditingPlugin::filePermissionsChanged() will do the
            //       same as above, but this will take a wee bit of time
            //       while we want it done straightaway...

            newEditingWidget->editorList()->addItem(EditorList::EditorListItem::Error,
                                                    mConverter.errorLine(),
                                                    mConverter.errorColumn(),
                                                    Core::formatMessage(mConverter.errorMessage(), false)+".");

            newEditingWidget->editorList()->addItem(EditorList::EditorListItem::Hint,
                                                    tr("You might want to use the Raw (CellML) view to edit the file."));

            // Apply an XML lexer to our editor

            newEditingWidget->editor()->editor()->setLexer(new QsciLexerXML(this));
        }

        // Keep track of our editing widget (and of whether the conversion was
        // successful) and add it to ourselves

        CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);
        CellMLSupport::CellmlFile::Version cellmlVersion = fileIsEmpty?
                                                               CellMLSupport::CellmlFile::Cellml_1_0:
                                                               CellMLSupport::CellmlFile::version(cellmlFile);

        mData.insert(pFileName,
                     CellmlTextViewWidgetData(newEditingWidget,
                                              Core::sha1(newEditingWidget->editor()->contents()),
                                              successfulConversion,
                                              cellmlVersion,
                                              fileIsEmpty?QDomDocument(QString()):mConverter.rdfNodes()));

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

        // Update our viewer

        if (data.isValid())
            updateViewer();

        // Select the first issue, if any, with the current file
        // Note: we use a single shot to give time to the setting up of the
        //       editing widget to complete...

        if (!data.isValid()) {
            mEditorLists << mEditingWidget->editorList();

            QTimer::singleShot(0, this, SLOT(selectFirstItemInEditorList()));
        }

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

    CellmlTextViewWidgetData data = mData.value(pOldFileName);
    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = data.editingWidget();

    if (editingWidget) {
        // Parse the contents of the editor and, if successful, serialise its
        // corresponding DOM document, making sure that we include any metadata
        // that was in the original CellML file

        if (parse(pOldFileName)) {
            // Let the user know if we had to use a higher version of CellML

            if (mParser.cellmlVersion() > data.cellmlVersion()) {
                editingWidget->editorList()->addItem(EditorList::EditorListItem::Information,
                                                     -1, -1,
                                                     tr("The CellML file requires features that are not present in %1 and was therefore saved as a %2 file.").arg(CellMLSupport::CellmlFile::versionAsString(data.cellmlVersion()),
                                                                                                                                                                   CellMLSupport::CellmlFile::versionAsString(mParser.cellmlVersion())));
            }

            // Serialise the file

            QDomDocument domDocument = mParser.domDocument();

            domDocument.firstChildElement().appendChild(data.rdfNodes().firstChildElement().cloneNode());

            if (Core::writeTextToFile(pNewFileName, qDomDocumentToString(domDocument))) {
                // We could serialise the file, so update our SHA-1 value

                data.setSha1(Core::sha1(editingWidget->editor()->contents()));

                mData.insert(pOldFileName, data);

                return true;
            } else {
                return false;
            }
        } else {
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

void CellmlTextViewWidget::reformat(const QString &pFileName)
{
    // Reformat the contents of the given file's editor

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mData.value(pFileName).editingWidget();

    if (editingWidget && parse(pFileName, true)) {
        int cursorLine;
        int cursorColumn;

        editingWidget->editor()->cursorPosition(cursorLine, cursorColumn);

        mConverter.execute(qDomDocumentToString(mParser.domDocument()));

        editingWidget->editor()->setContentsWithHistory(mConverter.output());
        editingWidget->editor()->setCursorPosition(cursorLine, cursorColumn);
    }
}

//==============================================================================

bool CellmlTextViewWidget::validate(const QString &pFileName)
{
    // Validate the given file

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mData.value(pFileName).editingWidget();

    if (editingWidget) {
        // To validate currently consists of trying to parse the contents of the
        // editor

        return parse(pFileName);
    } else {
        // The file doesn't exist, so it can't be validated

        return false;
    }
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
                                                                        pEditor->contentsSize(), SingleLineCommentString,
                                                                        false, false, false),
                                               &commentLineNumber, &commentColumnNumber);

                pEditor->setSelection(commentLineNumber, commentColumnNumber,
                                      commentLineNumber, commentColumnNumber+SingleLineCommentLength);
                pEditor->removeSelectedText();
            }
        }
    }
}

//==============================================================================

bool CellmlTextViewWidget::parse(const QString &pFileName,
                                 const bool &pOnlyErrors)
{
    // Parse the given file, should it exist

    CellmlTextViewWidgetData data = mData.value(pFileName);
    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = data.editingWidget();

    if (editingWidget) {
        editingWidget->editorList()->clear();

        bool res = mParser.execute(editingWidget->editor()->contents(),
                                   data.cellmlVersion());

        // Add the messages that were generated by the parser, if any, and
        // select the first one of them

        foreach (const CellmlTextViewParserMessage &message, mParser.messages()) {
            if (   !pOnlyErrors
                || (message.type() == CellmlTextViewParserMessage::Error)) {
                editingWidget->editorList()->addItem((message.type() == CellmlTextViewParserMessage::Error)?
                                                         EditorList::EditorListItem::Error:
                                                         EditorList::EditorListItem::Warning,
                                                     message.line(), message.column(),
                                                     message.message());
            }
        }

        selectFirstItemInEditorList(editingWidget->editorList());

        return res;
    } else {
        return false;
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

QString CellmlTextViewWidget::statement(const int &pPosition) const
{
    // Retrieve the statement around the given position
    // Note: a statement is a piece of text that is bounded by "as" or ";" at
    //       either of its ends...

    static const QString AsTag = "as";
    static const QString SemiColonTag = ";";

    Editor::EditorWidget *editor = mEditingWidget->editor();

    int prevAsPos = editor->findTextInRange(pPosition, 0, AsTag, false, true, true);
    int prevSemiColonPos = editor->findTextInRange(pPosition, 0, SemiColonTag, false, false, false);
    int nextSemiColonPos = editor->findTextInRange(pPosition-SemiColonTag.length()+1, editor->contentsSize(), SemiColonTag, false, false, false);

    int fromPos = (prevAsPos > prevSemiColonPos)?
                      prevAsPos+AsTag.length():
                      prevSemiColonPos+SemiColonTag.length();
    int toPos = nextSemiColonPos+SemiColonTag.length();

    // Skip spaces

    static const QString NonSpaceRegEx = "[^\\s]";

    fromPos = editor->findTextInRange(fromPos, editor->contentsSize(), NonSpaceRegEx, true, false, false);

    return ((pPosition >= fromPos) && (pPosition < toPos))?
               editor->textInRange(fromPos, toPos):
               QString();
}

//==============================================================================

void CellmlTextViewWidget::updateViewer()
{
//---GRY--- TO BE COMPLETED... TO START WITH, WE DO THIS ASSUMING THERE ARE NO
//          COMMENTS. THEN, WE WILL HAVE TO HANDLE THE CASE WHERE THERE ARE
//          COMMENTS, INCLUDING WITHIN AN EQUATION...
    // Make sure that we still have an editing widget (i.e. it hasn't been
    // closed since the signal was emitted)

    if (!mEditingWidget)
        return;

    // Retrieve the statement, if any, around our current position

    QString currentStatement = statement(mEditingWidget->editor()->currentPosition());
static int counter = 0;
qDebug("---[%05d]---", ++counter);
qDebug("[%s]", qPrintable(currentStatement));

    // Update the contents of our viewer

    if (currentStatement.isEmpty()) {
        // There is no statement, so clear our viewer

        mContentMathmlEquation = QString();

        mEditingWidget->viewer()->setContents(QString());
    } else {
        // There is a statement, so try to parse it

        bool res = mParser.execute(currentStatement);

        if (res) {
            // The parsing was successful, so retrieve the Content MathML
            // version of our statement and check whether it's the same as our
            // previous one

            QString contentMathmlEquation =  "<math xmlns=\"http://www.w3.org/1998/Math/MathML\">"
                                            +Core::cleanMathml(qDomDocumentToString(mParser.domDocument()))
                                            +"</math>";

            if (contentMathmlEquation.compare(mContentMathmlEquation)) {
                // It's a different one, so check whether we have already
                // retrieved its Presentation MathML version

                mContentMathmlEquation = contentMathmlEquation;

                QString presentationMathmlEquation = mPresentationMathmlEquations.value(contentMathmlEquation);

                if (!presentationMathmlEquation.isEmpty()) {
                    mEditingWidget->viewer()->setContents(presentationMathmlEquation);
                } else {
                    // We haven't already retrieved its Presentation MathML
                    // version, so do it now

                    static const QString CtopXsl = Core::resourceAsByteArray(":/web-xslt/ctop.xsl");

                    mXslTransformer->transform(contentMathmlEquation, CtopXsl);
                }
            }
        } else {
            // The parsing wasn't successful

            mContentMathmlEquation = QString();

            mEditingWidget->viewer()->setError(true);
        }
    }
}

//==============================================================================

QString CellmlTextViewWidget::endOfPiecewiseAssignment(Editor::EditorWidget *pEditor,
                                                       const int &pFromPosition)
{
    // Look for the end of a piecewise assignment in the given editor, starting
    // from the given position

    static const QString SemiColonTag = ";";
    static const QString NonSpaceRegEx = "[^\\s]";

    QString res = QString();
    int crtPosition = pFromPosition;
    QString crtStatement = QString();

    forever {
        // Look for the end of the current statement

        int semiColonPos = pEditor->findTextInRange(crtPosition, pEditor->contentsSize(), SemiColonTag, false, false, false);
qDebug(">>> semiColonPos: %d", semiColonPos);

        if (semiColonPos == -1) {
            break;
        } else {
            // Retrieve the current statement

            crtStatement = pEditor->textInRange(crtPosition, semiColonPos+SemiColonTag.length());
qDebug(">>> [%s]", qPrintable(crtStatement));

            res += crtStatement;

            // Skip spaces

            crtPosition = pEditor->findTextInRange(crtPosition, pEditor->contentsSize(), NonSpaceRegEx, true, false, false);

            // Check whether we are dealing with a keyword

            if (pEditor->styleAt(crtPosition) == CellmlTextViewLexer::Keyword) {
                // Check whether that keyword is "endsel", in which case we
                // would have found the end of our piecewise assignment

                static const QRegularExpression KeywordRegEx = QRegularExpression("\\w+");

                QString keyword = KeywordRegEx.match(crtStatement).captured(0);

                if (!keyword.compare("endsel"))
                    break;
            }

            crtPosition = semiColonPos+SemiColonTag.length();
        }
    }

    return res;
}

//==============================================================================

void CellmlTextViewWidget::selectFirstItemInEditorList(EditorList::EditorListWidget *pEditorList)
{
    // Select the first item in the given editor list

    if (pEditorList) {
        pEditorList->selectFirstItem();
    } else {
        // We came here through a single shot (see initialize()), so rely on the
        // contents of mEditorLists

        EditorList::EditorListWidget *editorList = mEditorLists.first();

        mEditorLists.removeFirst();

        editorList->selectFirstItem();
    }
}

//==============================================================================

void CellmlTextViewWidget::xslTransformationDone(const QString &pInput,
                                                 const QString &pOutput)
{
    // Make sure that we still have an editing widget (i.e. it hasn't been
    // closed since the signal was emitted)

    if (!mEditingWidget)
        return;

    // The XSL transformation is done, so update our viewer and keep track of
    // the mapping between the Content and Presentation MathML
    // Note: before setting the contents of our viewer, we need to make sure
    //       that pInput is still our current Content MathML equation. Indeed,
    //       say that updateViewer() gets called many times in a short period of
    //       time (e.g. as a result of replacing all the occurences of a
    //       particular string with another one) and that some of those calls
    //       don't require an XSL transformation, then we may end up in a case
    //       where pInput is not our current Content MathML equation anymore, in
    //       which case the contents of our viewer shouldn't be updated...

    if (!pInput.compare(mContentMathmlEquation))
        mEditingWidget->viewer()->setContents(pOutput);

    mPresentationMathmlEquations.insert(pInput, pOutput);
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
