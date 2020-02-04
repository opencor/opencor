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
// CellML Text view widget
//==============================================================================
// Note: this view and the Raw CellML view work in the same way when it comes to
//       updating the viewer (e.g. see the XSL transformation)...
//==============================================================================

#include "cellmleditingviewwidget.h"
#include "cellmltextviewconverter.h"
#include "cellmltextviewlexer.h"
#include "cellmltextviewparser.h"
#include "cellmltextviewwidget.h"
#include "corecliutils.h"
#include "coreguiutils.h"
#include "editorlistwidget.h"
#include "editorwidgeteditorwidget.h"
#include "filemanager.h"
#include "mathmlviewerwidget.h"
#include "qscintillawidget.h"

//==============================================================================

#include <QDir>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QSettings>
#include <QTimer>

//==============================================================================

#include "qscintillabegin.h"
    #include "Qsci/qscilexerxml.h"
#include "qscintillaend.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewWidgetData::CellmlTextViewWidgetData(CellmlTextViewWidgetEditingWidget *pEditingWidget,
                                                   const QString &pSha1,
                                                   bool pValid,
                                                   CellMLSupport::CellmlFile::Version pCellmlVersion,
                                                   const QDomNode &pDocumentationNode,
                                                   const QDomDocument &pRdfNodes) :
    mEditingWidget(pEditingWidget),
    mSha1(pSha1),
    mValid(pValid),
    mCellmlVersion(pCellmlVersion),
    mDocumentationNode(pDocumentationNode),
    mRdfNodes(pRdfNodes)
{
}

//==============================================================================

CellmlTextViewWidgetData::~CellmlTextViewWidgetData()
{
    // Delete some internal objects

    delete mEditingWidget;
}

//==============================================================================

void CellmlTextViewWidgetData::retranslateUi()
{
    // Retranslate our editing widget

    mEditingWidget->retranslateUi();
}

//==============================================================================

CellmlTextViewWidgetEditingWidget * CellmlTextViewWidgetData::editingWidget() const
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

void CellmlTextViewWidgetData::setSha1(const QString &pSha1)
{
    // Set our SHA-1 value

    mSha1 = pSha1;
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

void CellmlTextViewWidgetData::setCellmlVersion(CellMLSupport::CellmlFile::Version pCellmlVersion)
{
    // Set our CellML version value

    mCellmlVersion = pCellmlVersion;
}

//==============================================================================

QDomNode CellmlTextViewWidgetData::documentationNode() const
{
    // Return our documentation node

    return mDocumentationNode;
}

//==============================================================================

QDomDocument CellmlTextViewWidgetData::rdfNodes() const
{
    // Return our RDF nodes

    return mRdfNodes;
}

//==============================================================================

QString CellmlTextViewWidgetData::fileContents() const
{
    // Return our (physical) file contents

    return mFileContents;
}

//==============================================================================

void CellmlTextViewWidgetData::setFileContents(const QString &pFileContents)
{
    // Set our (physical) file contents

    mFileContents = pFileContents;
}

//==============================================================================

QString CellmlTextViewWidgetData::convertedFileContents() const
{
    // Return our converted (physical) file contents

    return mConvertedFileContents;
}

//==============================================================================

void CellmlTextViewWidgetData::setConvertedFileContents(const QString &pConvertedFileContents)
{
    // Set our converted (physical) file contents

    mConvertedFileContents = pConvertedFileContents;
}

//==============================================================================

CellmlTextViewWidgetEditingWidget::CellmlTextViewWidgetEditingWidget(const QString &pContents,
                                                                     bool pReadOnly,
                                                                     QsciLexer *pLexer,
                                                                     QWidget *pParent) :
    CellMLEditingView::CellmlEditingViewWidget(pContents, pReadOnly, pLexer, pParent)
{
}

//==============================================================================

static const char *SingleLineCommentString = "//";
static const int SingleLineCommentLength   = 2;

//==============================================================================

static const char *StartMultilineCommentString = "/*";
static const char *EndMultilineCommentString   = "*/";
static const int StartMultilineCommentLength   = 2;
static const int EndMultilineCommentLength     = 2;

//==============================================================================

bool CellmlTextViewWidgetEditingWidget::handleEditorKeyPressEvent(QKeyEvent *pEvent)
{
    // Some key combinations from our editor

    if (   (pEvent->modifiers() == Qt::ControlModifier)
        && (pEvent->key() == Qt::Key_Slash)) {
        // We want to (un)comment the selected text, if any, or the current
        // line, so start by retrieving the position of our cursor within our
        // editor

        EditorWidget::EditorWidget *editor = editorWidget();
        int line, column;

        editor->cursorPosition(line, column);

        // Check whether some text is selected

        if (editor->hasSelectedText()) {
            // Some text is selected, so check whether full lines are selected
            // or 'random' text

            static const QString Null = "\0";

            int lineFrom, columnFrom, lineTo, columnTo;

            editor->selection(lineFrom, columnFrom, lineTo, columnTo);

            int selectedTextEndPosition = editor->position(lineTo, columnTo);
            QString editorEolString = editor->eolString();

            if (   (columnFrom == 0)
                && (   (columnTo == 0)
                    || (selectedTextEndPosition == editor->length())
                    || (editor->textInRange(selectedTextEndPosition, selectedTextEndPosition+editorEolString.length()) == editorEolString)
                    || (editor->textInRange(selectedTextEndPosition, selectedTextEndPosition+1) == Null))) {
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
                    QString text;
                    int iMax = (columnTo != 0)?lineTo:lineTo-1;

                    // Determine whether we should be commenting or uncommenting
                    // the lines

                    for (int i = lineFrom; i <= iMax; ++i) {
                        text = editor->text(i).trimmed();

                        commentLine = commentLine || (!text.isEmpty() && !text.startsWith(SingleLineCommentString));
                    }

                    // (Un)comment the lines as one 'big' action

                    editor->beginUndoAction();

                    for (int i = lineFrom; i <= iMax; ++i) {
                        commentOrUncommentLine(editor, i, commentLine);
                    }

                    editor->endUndoAction();

                    // Prepare ourselves for reselecting the lines

                    columnTo += (columnTo != 0)?
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

                columnTo += (commentSelectedText?1:-1)*(StartMultilineCommentLength+int(lineFrom == lineTo)*EndMultilineCommentLength);
            }

            // Reselect the text/lines

            if ((line == lineFrom) && (column == columnFrom)) {
                editor->setSelection(lineTo, columnTo, lineFrom, columnFrom);
            } else {
                editor->setSelection(lineFrom, columnFrom, lineTo, columnTo);
            }
        } else {
            // No text is selected, so simply (un)comment the current line

            bool commentLine = !editor->text(line).trimmed().startsWith(SingleLineCommentString);

            if (commentOrUncommentLine(editor, line, commentLine)) {
                if (commentLine) {
                    // We commented the line, so our position will be fine
                    // unless we were at the beginning of the line, in which
                    // case we need to update it

                    if (column == 0) {
                        editor->setCursorPosition(line, column+SingleLineCommentLength);
                    }
                } else {
                    // We uncommented the line, so go back to our original
                    // position (since uncommenting the line will have shifted
                    // it a bit)

                    editor->setCursorPosition(line, column-SingleLineCommentLength);
                }
            }
        }

        pEvent->accept();

        return true;
    }

    return false;
}

//==============================================================================

bool CellmlTextViewWidgetEditingWidget::commentOrUncommentLine(EditorWidget::EditorWidget *pEditor,
                                                               int pLineNumber,
                                                               bool pCommentLine)
{
    // (Un)comment the current line

    QString line = pEditor->text(pLineNumber).trimmed();

    if (line.isEmpty()) {
        return false;
    }

    // We are not dealing with an empty line, so we can (un)comment it

    if (pCommentLine) {
        pEditor->insertText(SingleLineCommentString, pLineNumber, 0);
    } else {
        // Uncomment the line, should it be commented

        if (line.startsWith(SingleLineCommentString)) {
            pEditor->removeText(pEditor->findTextInRange(pEditor->position(pLineNumber, 0),
                                                         pEditor->contentsSize(), SingleLineCommentString,
                                                         false, false, false),
                                SingleLineCommentLength);
        }
    }

    return true;
}

//==============================================================================

CellmlTextViewWidget::CellmlTextViewWidget(QWidget *pParent) :
    ViewWidget(pParent)
{
    // Create our MathML converter and create a connection to retrieve the
    // result of its MathML conversions

    connect(&mMathmlConverter, &Core::MathmlConverter::done,
            this, &CellmlTextViewWidget::mathmlConversionDone);
}

//==============================================================================

void CellmlTextViewWidget::loadSettings(QSettings &pSettings)
{
    // Normally, we would retrieve the editing widget's settings, but
    // mEditingWidget is not set at this stage. So, instead, we keep track of
    // our settings' group and load them when initialising ourselves (see
    // initialize())...

    mSettingsGroup = pSettings.group();
}

//==============================================================================

void CellmlTextViewWidget::saveSettings(QSettings &pSettings) const
{
    Q_UNUSED(pSettings)
    // Note: our view is such that our settings are actually saved when calling
    //       finalize() on the last file...
}

//==============================================================================

void CellmlTextViewWidget::retranslateUi()
{
    // Retranslate all of our data

    for (auto data : mData) {
        data->retranslateUi();
    }
}

//==============================================================================

void CellmlTextViewWidget::initialize(const QString &pFileName, bool pUpdate)
{
    // Retrieve the editing widget associated with the given file, if any

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    if (data == nullptr) {
        // No editing widget exists for the given file, so generate a CellML
        // text version of the given CellML file

        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        QByteArray fileContents;

        Core::readFile(pFileName, fileContents);

        bool fileIsEmpty = fileContents.trimmed().isEmpty();
        bool successfulConversion = fileIsEmpty?true:mConverter.execute(fileContents);

        // Create an editing widget for the given CellML file

        auto editingWidget = new CellmlTextViewWidgetEditingWidget(fileIsEmpty?QString():mConverter.output(),
                                                                   !fileManagerInstance->isReadableAndWritable(pFileName),
                                                                   nullptr, parentWidget());

        // Add the warnings, if any, that were generated by the converter

        if (!fileIsEmpty && mConverter.hasWarnings()) {
            for (const auto &warning : mConverter.warnings()) {
                editingWidget->editorListWidget()->addItem(EditorWidget::EditorListItem::Type::Warning,
                                                           successfulConversion?
                                                               -1:
                                                               warning.lineNumber(),
                                                           successfulConversion?
                                                               -1:
                                                               warning.columnNumber(),
                                                           successfulConversion?
                                                               QString("[%1:%2] %3").arg(warning.lineNumber())
                                                                                    .arg(warning.columnNumber())
                                                                                    .arg(warning.message()):
                                                               warning.message().arg(QString()));
            }
        }

        // Populate our editing widget with the CellML Text version of the
        // given CellML file

        if (successfulConversion) {
            // The conversion was successful, so we can apply our CellML Text
            // lexer to our editor

            auto lexer = new CellmlTextViewLexer(this);

            editingWidget->editorWidget()->setLexer(lexer);

            // Update our viewer whenever necessary
            // Note: normally, we would update our viewer when the text has
            //       changed or the cursor position has changed. However, when
            //       it comes to the text being changed, we need to make sure
            //       that the styling is done, hence checking for the lexer to
            //       be done...

            connect(lexer, &CellmlTextViewLexer::done,
                    this, &CellmlTextViewWidget::updateViewer);
            connect(editingWidget->editorWidget(), &EditorWidget::EditorWidget::cursorPositionChanged,
                    this, &CellmlTextViewWidget::updateViewer);
        } else {
            // The conversion wasn't successful, so make the editor read-only
            // (since its contents is that of the file itself) and add a couple
            // of messages to our editor list

            editingWidget->editorWidget()->setReadOnly(true);
            // Note: EditingViewPlugin::filePermissionsChanged() will do the
            //       same as above, but this will take a wee bit of time while
            //       we want it done straightaway...

            editingWidget->editorListWidget()->addItem(EditorWidget::EditorListItem::Type::Error,
                                                       mConverter.errorLine(),
                                                       mConverter.errorColumn(),
                                                       tr("%1.").arg(Core::formatMessage(mConverter.errorMessage(), false)));
            editingWidget->editorListWidget()->addItem(EditorWidget::EditorListItem::Type::Hint,
                                                       tr("You might want to use the Raw (CellML) view to edit the file."));

            // Apply an XML lexer to our editor

            editingWidget->editorWidget()->setLexer(new QsciLexerXML(this));
        }

        // Keep track of our editing widget (and of whether the conversion was
        // successful)

        CellMLSupport::CellmlFile::Version cellmlVersion = fileIsEmpty?
                                                               CellMLSupport::CellmlFile::Version::Cellml_1_0:
                                                               CellMLSupport::CellmlFile::fileVersion(pFileName);

        data = new CellmlTextViewWidgetData(editingWidget,
                                            Core::sha1(editingWidget->editorWidget()->contents()),
                                            successfulConversion,
                                            cellmlVersion,
                                            fileIsEmpty?
                                                QDomNode():
                                                mConverter.documentationNode(),
                                            fileIsEmpty?
                                                QDomDocument(QString()):
                                                mConverter.rdfNodes());

        mData.insert(pFileName, data);
    }

    // Update our editing widget, if required

    CellmlTextViewWidgetEditingWidget *newEditingWidget = data->editingWidget();

    if (pUpdate) {
        CellmlTextViewWidgetEditingWidget *oldEditingWidget = mEditingWidget;

        mEditingWidget = newEditingWidget;

        // Load our settings, if needed, or reset our editing widget using the
        // 'old' one

        if (mNeedLoadingSettings) {
            QSettings settings;

            settings.beginGroup(mSettingsGroup);

            newEditingWidget->loadSettings(settings);

            mNeedLoadingSettings = false;
        } else {
            newEditingWidget->updateSettings(oldEditingWidget);
        }

        // Update our viewer, if everything is fine, or select the first issue
        // with the current file

        if (data->isValid()) {
            updateViewer();
        } else {
            // Note: we use a single shot to give time to the setting up of the
            //       editing widget to complete...

            mEditorLists << newEditingWidget->editorListWidget();

            QTimer::singleShot(0, this, &CellmlTextViewWidget::selectFirstItemInEditorList);
        }

        // Set our focus proxy to our 'new' editing widget and make sure that
        // the latter immediately gets the focus
        // Note: if we were not to immediately give the focus to our 'new'
        //       editing widget, then the central widget would give the focus to
        //       our 'old' editing widget (see CentralWidget::updateGui()),
        //       which is clearly not what we want...

        setFocusProxy(newEditingWidget->editorWidget());

        newEditingWidget->editorWidget()->setFocus();
    } else {
        // Hide our 'new' editing widget

        newEditingWidget->hide();
    }
}

//==============================================================================

void CellmlTextViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    if (data != nullptr) {
        // There is an editing widget for the given file name, so save our
        // settings and reset our memory of the current editing widget, if
        // needed

        CellmlTextViewWidgetEditingWidget *editingWidget = data->editingWidget();

        if (mEditingWidget == editingWidget) {
            QSettings settings;

            settings.beginGroup(mSettingsGroup);

            editingWidget->saveSettings(settings);

            mNeedLoadingSettings = true;
            mEditingWidget = nullptr;
        }

        // Delete the editor and remove it from our list

        delete data;

        mData.remove(pFileName);
    }
}

//==============================================================================

void CellmlTextViewWidget::fileSaved(const QString &pFileName)
{
    // The given file has been saved, so consider it reloaded, but only if it
    // has a corresponding widget that is invisible

    QWidget *crtWidget = widget(pFileName);

    if ((crtWidget != nullptr) && !crtWidget->isVisible()) {
        fileReloaded(pFileName);
    }
}

//==============================================================================

void CellmlTextViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed
    // Note: if the view for the given file is not the active view, then to call
    //       finalize() and then initialize() will activate the contents of the
    //       view (but the file tab will still point to the previously active
    //       file). However, we want the 'old' file to remain the active one,
    //       hence the extra argument we pass to initialize()...

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    if (data != nullptr) {
        bool update = mEditingWidget == data->editingWidget();

        finalize(pFileName);
        initialize(pFileName, update);
    }
}

//==============================================================================

void CellmlTextViewWidget::fileRenamed(const QString &pOldFileName,
                                       const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    CellmlTextViewWidgetData *data = mData.value(pOldFileName);

    if (data != nullptr) {
        mData.insert(pNewFileName, data);
        mData.remove(pOldFileName);
    }
}

//==============================================================================

EditorWidget::EditorWidget * CellmlTextViewWidget::editorWidget(const QString &pFileName) const
{
    // Return the requested editor widget

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    return (data != nullptr)?data->editingWidget()->editorWidget():nullptr;
}

//==============================================================================

QWidget * CellmlTextViewWidget::widget(const QString &pFileName)
{
    // Return the requested (editing) widget

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    return (data != nullptr)?data->editingWidget():nullptr;
}

//==============================================================================

bool CellmlTextViewWidget::isEditorWidgetUseable(const QString &pFileName) const
{
    // Return whether the requested editor widget is useable

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    return (data != nullptr)?data->isValid():false;
}

//==============================================================================

bool CellmlTextViewWidget::isEditorWidgetContentsModified(const QString &pFileName) const
{
    // Return whether the contents of the requested editor widget has been
    // modified

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    if (data != nullptr) {
        if (Core::FileManager::instance()->isModified(pFileName)) {
            // The given file is considered modified, so we need to retrieve the
            // contents of its physical version

            QString fileContents;

            Core::readFile(pFileName, fileContents);

            // Check whether we already know about that file contents and, if
            // not, determine its converted version (and keep track of it)

            if (fileContents != data->fileContents()) {
                CellMLTextViewConverter converter;
                bool fileIsEmpty = fileContents.trimmed().isEmpty();
                bool successfulConversion = fileIsEmpty?true:converter.execute(fileContents);

                if (successfulConversion) {
                    data->setFileContents(fileContents);
                    data->setConvertedFileContents(fileIsEmpty?QString():converter.output());
                } else {
                    // We couldn't convert the physical version of the given
                    // file, which means that we are definitely different

                    return true;
                }
            }

            return Core::sha1(data->editingWidget()->editorWidget()->contents()) != Core::sha1(data->convertedFileContents());
        }

        // The given file is not considered modified, so simply compare the
        // SHA-1 value of our editor's contents with our internal one

        return Core::sha1(data->editingWidget()->editorWidget()->contents()) != data->sha1();
    }

    return false;
}

//==============================================================================

bool CellmlTextViewWidget::saveFile(const QString &pOldFileName,
                                    const QString &pNewFileName,
                                    bool &pNeedFeedback)
{
    // Save the given file

    CellmlTextViewWidgetData *data = mData.value(pOldFileName);

    if (data != nullptr) {
        // Parse the contents of the editor and, if successful, serialise its
        // corresponding DOM document, making sure that we include any metadata
        // that was in the original CellML file

        if (parse(pOldFileName)) {
            // Check whether we need a higher version of CellML to save the file
            // and, if so, ask the user whether it's OK to use that higher
            // version

            if (   (data->cellmlVersion() != CellMLSupport::CellmlFile::Version::Unknown)
                && (mParser.cellmlVersion() > data->cellmlVersion())
                && (Core::questionMessageBox(tr("Save File"),
                                             tr("<strong>%1</strong> requires features that are not present in %2 and should therefore be saved as a %3 file. Do you want to proceed?").arg(QDir::toNativeSeparators(pNewFileName),
                                                                                                                                                                                            CellMLSupport::CellmlFile::versionAsString(data->cellmlVersion()),
                                                                                                                                                                                            CellMLSupport::CellmlFile::versionAsString(mParser.cellmlVersion()))) == QMessageBox::No)) {
                pNeedFeedback = false;

                return false;
            }

            data->setCellmlVersion(mParser.cellmlVersion());

            // Add the documentation, if any, to our model element

            if (!data->documentationNode().isNull()) {
                mParser.modelElement().appendChild(data->documentationNode().cloneNode());
            }

            // Add the metadata to our DOM document

            QDomDocument domDocument = mParser.domDocument();
            QDomElement domElement = domDocument.documentElement();

            for (QDomElement childElement = data->rdfNodes().firstChildElement();
                 !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
                domElement.appendChild(childElement.cloneNode());
            }

            // Serialise our DOM document

            if (Core::writeFile(pNewFileName, Core::serialiseDomDocument(domDocument))) {
                // We could serialise our DOM document, so update our SHA-1
                // value

                data->setSha1(Core::sha1(data->editingWidget()->editorWidget()->contents()));

                mData.insert(pOldFileName, data);

                return true;
            }

            return false;
        }

        // The parsing was unsuccessful, so ask the user whether s/he wants to
        // save the contents of the view to a text file

        if (Core::questionMessageBox(tr("Save File"),
                                     tr("<strong>%1</strong> could not be saved. Do you want to save the contents of the view to a text file?").arg(QDir::toNativeSeparators(pNewFileName))) == QMessageBox::Yes) {
            QString fileName = Core::getSaveFileName(tr("Save"),
                                                     Core::newFileName(pNewFileName, "txt"));

            if (!fileName.isEmpty()) {
                Core::writeFile(fileName, data->editingWidget()->editorWidget()->contents());
            }
        }

        pNeedFeedback = false;

        return false;
    }

    return false;
}

//==============================================================================

QList<QWidget *> CellmlTextViewWidget::statusBarWidgets() const
{
    // Return our status bar widgets

    if (mEditingWidget != nullptr) {
        return { mEditingWidget->editorWidget()->cursorPositionWidget(),
                 mEditingWidget->editorWidget()->editingModeWidget() };
    }

    return {};
}

//==============================================================================

void CellmlTextViewWidget::reformat(const QString &pFileName)
{
    // Reformat the contents of the given file's editor

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    if ((data != nullptr) && parse(pFileName, true)) {
        EditorWidget::EditorWidget *editor = data->editingWidget()->editorWidget();
        int line;
        int column;

        editor->cursorPosition(line, column);

        mConverter.execute(Core::serialiseDomDocument(mParser.domDocument()));

        editor->setContents(mConverter.output(), false);
        editor->setCursorPosition(line, column);
    }
}

//==============================================================================

bool CellmlTextViewWidget::validate(const QString &pFileName, QString &pExtra)
{
    // No extra information by default

    pExtra = QString();

    // Validate the given file

    if (mData.contains(pFileName)) {
        // To validate currently consists of trying to parse the contents of the
        // editor

        return parse(pFileName, pExtra);
    }

    // The file doesn't exist, so it can't be validated

    return false;
}

//==============================================================================

bool CellmlTextViewWidget::parse(const QString &pFileName, QString &pExtra,
                                 bool pOnlyErrors)
{
    // Parse the given file, should it exist

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    if (data != nullptr) {
        CellmlTextViewWidgetEditingWidget *editingWidget = data->editingWidget();

        editingWidget->editorListWidget()->clear();

        bool res = mParser.execute(editingWidget->editorWidget()->contents(),
                                   data->cellmlVersion());

        // Add the messages that were generated by the parser, if any, and
        // select the first one of them

        for (const auto &message : mParser.messages()) {
            if (   !pOnlyErrors
                || (message.type() == CellmlTextViewParserMessage::Type::Error)) {
                editingWidget->editorListWidget()->addItem((message.type() == CellmlTextViewParserMessage::Type::Error)?
                                                               EditorWidget::EditorListItem::Type::Error:
                                                               EditorWidget::EditorListItem::Type::Warning,
                                                           message.line(), message.column(),
                                                           message.message());
            }
        }

        editingWidget->editorListWidget()->selectFirstItem();

        // Provide some extra information in case, if we are dealing with a
        // CellML 1.0/1.1 files and are therefore using the CellML API

        if (   (data->cellmlVersion() == CellMLSupport::CellmlFile::Version::Cellml_1_0)
            || (data->cellmlVersion() == CellMLSupport::CellmlFile::Version::Cellml_1_1)) {
            pExtra = tr(R"(the <a href="https://github.com/cellmlapi/cellml-api/">CellML validation service</a> cannot be used in this view, so only validation against the <a href="https://opencor.ws/user/plugins/editing/CellMLTextView.html#CellML Text format">CellML Text format</a> was performed. For full CellML validation, you might want to use the Raw CellML view instead.)");
        }

        return res;
    }

    return false;
}

//==============================================================================

bool CellmlTextViewWidget::parse(const QString &pFileName, QString &pExtra)
{
    // Parse the given file

    return parse(pFileName, pExtra, false);
}

//==============================================================================

bool CellmlTextViewWidget::parse(const QString &pFileName, bool pOnlyErrors)
{
    // Parse the given file

    QString dummy;

    return parse(pFileName, dummy, pOnlyErrors);
}

//==============================================================================

bool CellmlTextViewWidget::isComment(int pPosition) const
{
    // Return whether we have a single or multiline comment at the given
    // position

    int style = mEditingWidget->editorWidget()->styleAt(pPosition);

    return    (style == int(CellmlTextViewLexer::Style::SingleLineComment))
           || (style == int(CellmlTextViewLexer::Style::MultilineComment));
}

//==============================================================================

QString CellmlTextViewWidget::partialStatement(int pPosition,
                                               int &pFromPosition,
                                               int &pToPosition) const
{
    // Retrieve the (partial) statement around the given position
    // Note: a (partial) statement is a piece of text that is bounded by either
    //       "as" or ";", and this at both ends...

    static const QString AsTag = "as";
    static const QString SemiColonTag = ";";

    EditorWidget::EditorWidget *editor = mEditingWidget->editorWidget();
    int editorContentsSize = editor->contentsSize();

    // Look for "as" and ";" before the given position

    int prevAsPos = pPosition;

    do {
        prevAsPos = editor->findTextInRange(prevAsPos, 0, AsTag, false, true, true);
    } while (   (prevAsPos != -1)
             && (editor->styleAt(prevAsPos) != int(CellmlTextViewLexer::Style::Keyword)));

    int prevSemiColonPos = pPosition;

    do {
        prevSemiColonPos = editor->findTextInRange(prevSemiColonPos, 0, SemiColonTag, false, false, false);
    } while (   (prevSemiColonPos != -1)
             && (editor->styleAt(prevSemiColonPos) != int(CellmlTextViewLexer::Style::Default)));

    // Look for "as" and ";" after the given position

    int nextAsPos = pPosition-AsTag.length();

    do {
        nextAsPos = editor->findTextInRange(nextAsPos+1, editorContentsSize, AsTag, false, true, true);
    } while (   (nextAsPos != -1)
             && (editor->styleAt(nextAsPos) != int(CellmlTextViewLexer::Style::Keyword)));

    int nextSemiColonPos = pPosition-SemiColonTag.length();

    do {
        nextSemiColonPos = editor->findTextInRange(nextSemiColonPos+1, editorContentsSize, SemiColonTag, false, false, false);
    } while (   (nextSemiColonPos != -1)
             && (editor->styleAt(nextSemiColonPos) != int(CellmlTextViewLexer::Style::Default)));

    nextAsPos = (nextAsPos == -1)?editorContentsSize:nextAsPos;
    nextSemiColonPos = (nextSemiColonPos == -1)?editorContentsSize:nextSemiColonPos;

    // Determine the start and end of our current statement

    pFromPosition = (prevAsPos > prevSemiColonPos)?
                        prevAsPos+AsTag.length():
                        prevSemiColonPos+SemiColonTag.length();
    pToPosition = (nextAsPos < nextSemiColonPos)?
                      nextAsPos+AsTag.length():
                      nextSemiColonPos+SemiColonTag.length();

    return editor->textInRange(pFromPosition, pToPosition);
}

//==============================================================================

QString CellmlTextViewWidget::beginningOfPiecewiseStatement(int &pPosition) const
{
    // Retrieve the beginning of a piecewise statement

    int localFromPosition;
    int localToPosition;

    QString currentStatement;
    QString localCurrentStatement;

    CellmlTextViewParser parser;

    forever {
        localCurrentStatement = partialStatement(pPosition-1, localFromPosition, localToPosition);

        currentStatement = localCurrentStatement+currentStatement;

        pPosition = localFromPosition;

        if (parser.execute(localCurrentStatement, false)) {
            if (parser.statement() == CellmlTextViewParser::Statement::PiecewiseSel) {
                break;
            }
        } else {
            break;
        }
    }

    return currentStatement;
}

//==============================================================================

QString CellmlTextViewWidget::endOfPiecewiseStatement(int &pPosition) const
{
    // Retrieve the end of a piecewise statement

    int localFromPosition;
    int localToPosition;

    QString currentStatement;
    QString localCurrentStatement;

    CellmlTextViewParser parser;

    forever {
        localCurrentStatement = partialStatement(pPosition, localFromPosition, localToPosition);

        currentStatement += localCurrentStatement;

        pPosition = localToPosition;

        if (parser.execute(localCurrentStatement, false)) {
            if (parser.statement() == CellmlTextViewParser::Statement::PiecewiseEndSel) {
                break;
            }
        } else {
            break;
        }
    }

    return currentStatement;
}

//==============================================================================

void CellmlTextViewWidget::updateViewer()
{
    // Make sure that we still have an editing widget (i.e. it hasn't been
    // closed since the signal was emitted) and that its editor allows us to
    // handle connections

    if (    (mEditingWidget == nullptr)
        || !mEditingWidget->editorWidget()->handleEditorChanges()) {
        return;
    }

    // Make sure that our current position is not within a comment

    int position = mEditingWidget->editorWidget()->currentPosition();
    QString currentStatement;

    if (!isComment(position)) {
        // Retrieve the (partial) statement around our current position

        int fromPosition;
        int toPosition;

        currentStatement = partialStatement(position, fromPosition, toPosition);

        // Check, using our CellML Text parser, whether our (partial) statement
        // contains something that we can recognise

        CellmlTextViewParser parser;

        if (parser.execute(currentStatement, false)) {
            if (parser.statement() == CellmlTextViewParser::Statement::PiecewiseSel) {
                // We are at the beginning of a piecewise statement, so retrieve
                // its end

                currentStatement += endOfPiecewiseStatement(toPosition);
            } else if (   (parser.statement() == CellmlTextViewParser::Statement::PiecewiseCase)
                       || (parser.statement() == CellmlTextViewParser::Statement::PiecewiseOtherwise)) {
                // We are in the middle of a piecewise statement, so retrieve
                // both its beginning and end

                currentStatement = beginningOfPiecewiseStatement(fromPosition)+currentStatement+endOfPiecewiseStatement(toPosition);
            } else if (parser.statement() == CellmlTextViewParser::Statement::PiecewiseEndSel) {
                // We are at the beginning of a piecewise statement, so retrieve
                // its beginning

                currentStatement = beginningOfPiecewiseStatement(fromPosition)+currentStatement;
            }

            // Skip spaces and comments to determine the real start of our
            // current statement

            int shift = 0;

            forever {
                if (isComment(fromPosition) || currentStatement[shift].isSpace()) {
                    fromPosition += QString(currentStatement[shift]).toUtf8().size();

                    ++shift;
                } else {
                    break;
                }
            }

            // Make sure that we are within our current statement

            currentStatement = ((position >= fromPosition) && (position < toPosition))?
                                   mEditingWidget->editorWidget()->textInRange(fromPosition, toPosition):
                                   QString();
        } else {
            // Our current statement doesn't contain something that we can
            // recognise

            currentStatement = QString();
        }
    }

    // Update the contents of our viewer

    if (currentStatement.isEmpty()) {
        // There is no statement, so clear our viewer

        mContentMathmlEquation = QString();

        mEditingWidget->mathmlViewer()->setContents({});
    } else {
        // There is a statement, so try to parse it

        bool res = mParser.execute(currentStatement);

        if (res) {
            // The parsing was successful, so retrieve the Content MathML
            // version of our statement and check whether it's the same as our
            // previous one

            QString contentMathmlEquation =  R"(<math xmlns=")"+CellMLSupport::MathmlNamespace+R"(">)"
                                            +Core::cleanContentMathml(Core::serialiseDomDocument(mParser.domDocument()))
                                            +"</math>";

            if (contentMathmlEquation != mContentMathmlEquation) {
                // It's a different one, so check whether we have already
                // retrieved its Presentation MathML version

                mContentMathmlEquation = contentMathmlEquation;

                QString presentationMathmlEquation = mPresentationMathmlEquations.value(contentMathmlEquation);

                if (!presentationMathmlEquation.isEmpty()) {
                    mEditingWidget->mathmlViewer()->setContents(presentationMathmlEquation);
                } else {
                    mMathmlConverter.convert(contentMathmlEquation);
                }
            }
        } else {
            // The parsing wasn't successful

            mContentMathmlEquation = QString();

            mEditingWidget->mathmlViewer()->setError(true);
        }
    }
}

//==============================================================================

void CellmlTextViewWidget::selectFirstItemInEditorList()
{
    // Rely on the contents of mEditorLists to select the first item of the
    // first editor list, assuming it's still current (i.e. it's still
    // referenced in mData)

    EditorWidget::EditorListWidget *editorList = mEditorLists.first();

    mEditorLists.removeFirst();

    for (auto data : mData) {
        if (data->editingWidget()->editorListWidget() == editorList) {
            editorList->selectFirstItem();

            break;
        }
    }
}

//==============================================================================

void CellmlTextViewWidget::mathmlConversionDone(const QString &pContentMathml,
                                                const QString &pPresentationMathml)
{
    // Make sure that we still have an editing widget (i.e. it hasn't been
    // closed since the signal was emitted)

    if (mEditingWidget == nullptr) {
        return;
    }

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

    if (pContentMathml == mContentMathmlEquation) {
        mEditingWidget->mathmlViewer()->setContents(pPresentationMathml);
    }

    mPresentationMathmlEquations.insert(pContentMathml, pPresentationMathml);
}

//==============================================================================

} // namespace CellMLTextView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
