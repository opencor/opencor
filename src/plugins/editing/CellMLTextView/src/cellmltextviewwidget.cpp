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
#include "editorwidget.h"
#include "filemanager.h"
#include "mathmlviewerwidget.h"
#include "qscintillawidget.h"

//==============================================================================

#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QSettings>
#include <QTimer>

//==============================================================================

#include "Qsci/qscilexerxml.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLTextView {

//==============================================================================

CellmlTextViewWidgetData::CellmlTextViewWidgetData(CellMLEditingView::CellmlEditingViewWidget *pEditingWidget,
                                                   const QString &pSha1,
                                                   const bool &pValid,
                                                   const CellMLSupport::CellmlFile::Version &pCellmlVersion,
                                                   QDomDocument pRdfNodes) :
    mEditingWidget(pEditingWidget),
    mSha1(pSha1),
    mValid(pValid),
    mCellmlVersion(pCellmlVersion),
    mRdfNodes(pRdfNodes),
    mFileContents(QString()),
    mConvertedFileContents(QString())
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

CellMLEditingView::CellmlEditingViewWidget * CellmlTextViewWidgetData::editingWidget() const
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

void CellmlTextViewWidgetData::setCellmlVersion(const CellMLSupport::CellmlFile::Version &pCellmlVersion)
{
    // Set our CellML version value

    mCellmlVersion = pCellmlVersion;
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

CellmlTextViewWidget::CellmlTextViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mNeedLoadingSettings(true),
    mSettingsGroup(QString()),
    mEditingWidget(0),
    mData(QMap<QString, CellmlTextViewWidgetData *>()),
    mConverter(CellMLTextViewConverter()),
    mParser(CellmlTextViewParser()),
    mEditorLists(QList<EditorWidget::EditorListWidget *>()),
    mPresentationMathmlEquations(QMap<QString, QString>()),
    mContentMathmlEquation(QString())
{
    // Create our MathML converter and create a connection to retrieve the
    // result of its MathML conversions

    connect(&mMathmlConverter, SIGNAL(done(const QString &, const QString &)),
            this, SLOT(mathmlConversionDone(const QString &, const QString &)));
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
    // Retranslate all of our data

    foreach (CellmlTextViewWidgetData *data, mData)
        data->retranslateUi();
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

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    if (!data) {
        // No editing widget exists for the given file, so generate a CellML
        // text version of the given CellML file

        Core::FileManager *fileManagerInstance = Core::FileManager::instance();
        QString fileContents;

        Core::readFileContentsFromFile(pFileName, fileContents);

        bool fileIsEmpty = fileContents.trimmed().isEmpty();
        bool successfulConversion = fileIsEmpty?true:mConverter.execute(fileContents);

        // Create an editing widget for the given CellML file

        CellMLEditingView::CellmlEditingViewWidget *editingWidget = new CellMLEditingView::CellmlEditingViewWidget(fileIsEmpty?QString():mConverter.output(),
                                                                                                                   !fileManagerInstance->isReadableAndWritable(pFileName),
                                                                                                                   0, parentWidget());

        // Add the warnings, if any, that were generated by the converter

        if (!fileIsEmpty && mConverter.hasWarnings()) {
            foreach (const CellMLTextViewConverterWarning &warning, mConverter.warnings()) {
                editingWidget->editorListWidget()->addItem(EditorWidget::EditorListItem::Warning,
                                                           successfulConversion?-1:warning.line(),
                                                           successfulConversion?
                                                               QString("[%1] %2").arg(QString::number(warning.line()), warning.message()):
                                                               warning.message().arg(QString()));
            }
        }

        // Populate our editing widget with the CellML Text version of the
        // given CellML file

        if (successfulConversion) {
            // The conversion was successful, so we can apply our CellML Text
            // lexer to our editor

            editingWidget->editorWidget()->editor()->setLexer(new CellmlTextViewLexer(this));

            // Update our viewer whenever necessary

            connect(editingWidget->editorWidget(), SIGNAL(textChanged()),
                    this, SLOT(updateViewer()));
            connect(editingWidget->editorWidget(), SIGNAL(cursorPositionChanged(const int &, const int &)),
                    this, SLOT(updateViewer()));
        } else {
            // The conversion wasn't successful, so make the editor read-only
            // (since its contents is that of the file itself) and add a couple
            // of messages to our editor list

            editingWidget->editorWidget()->setReadOnly(true);
            // Note: EditingViewPlugin::filePermissionsChanged() will do the
            //       same as above, but this will take a wee bit of time while
            //       we want it done straightaway...

            editingWidget->editorListWidget()->addItem(EditorWidget::EditorListItem::Error,
                                                       mConverter.errorLine(),
                                                       mConverter.errorColumn(),
                                                       tr("%1.").arg(Core::formatMessage(mConverter.errorMessage(), false)));
            editingWidget->editorListWidget()->addItem(EditorWidget::EditorListItem::Hint,
                                                       tr("You might want to use the Raw (CellML) view to edit the file."));

            // Apply an XML lexer to our editor

            editingWidget->editorWidget()->editor()->setLexer(new QsciLexerXML(this));
        }

        // Keep track of our editing widget (and of whether the conversion was
        // successful)

        CellMLSupport::CellmlFile::Version cellmlVersion = fileIsEmpty?
                                                               CellMLSupport::CellmlFile::Cellml_1_0:
                                                               CellMLSupport::CellmlFile::version(pFileName);

        data = new CellmlTextViewWidgetData(editingWidget,
                                            Core::sha1(editingWidget->editorWidget()->contents()),
                                            successfulConversion,
                                            cellmlVersion,
                                            fileIsEmpty?QDomDocument(QString()):mConverter.rdfNodes());

        mData.insert(pFileName, data);

        // Add support for some key mappings to our editor

        connect(editingWidget->editorWidget()->editor(), SIGNAL(keyPressed(QKeyEvent *, bool &)),
                this, SLOT(editorKeyPressed(QKeyEvent *, bool &)));
    }

    // Update our editing widget, if required

    CellMLEditingView::CellmlEditingViewWidget *newEditingWidget = data->editingWidget();

    if (pUpdate) {
        CellMLEditingView::CellmlEditingViewWidget *oldEditingWidget = mEditingWidget;

        mEditingWidget = newEditingWidget;

        // Load our settings, if needed, or reset our editing widget using the
        // 'old' one

        if (mNeedLoadingSettings) {
            QSettings settings;

            settings.beginGroup(mSettingsGroup);
                newEditingWidget->loadSettings(&settings);
            settings.endGroup();

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

            QTimer::singleShot(0, this, SLOT(selectFirstItemInEditorList()));
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

    if (data) {
        // There is an editing widget for the given file name, so save our
        // settings and reset our memory of the current editing widget, if
        // needed

        CellMLEditingView::CellmlEditingViewWidget *editingWidget = data->editingWidget();

        if (mEditingWidget == editingWidget) {
            QSettings settings;

            settings.beginGroup(mSettingsGroup);
                editingWidget->saveSettings(&settings);
            settings.endGroup();

            mNeedLoadingSettings = true;
            mEditingWidget = 0;
        }

        // Delete the editor and remove it from our list

        delete data;

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

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    if (data) {
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

    if (data) {
        mData.insert(pNewFileName, data);
        mData.remove(pOldFileName);
    }
}

//==============================================================================

EditorWidget::EditorWidget * CellmlTextViewWidget::editorWidget(const QString &pFileName) const
{
    // Return the requested editor widget

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    return data?data->editingWidget()->editorWidget():0;
}

//==============================================================================

QWidget * CellmlTextViewWidget::widget(const QString &pFileName)
{
    // Return the requested (editing) widget

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    return data?data->editingWidget():0;
}

//==============================================================================

bool CellmlTextViewWidget::isEditorWidgetUseable(const QString &pFileName) const
{
    // Return whether the requested editor widget is useable

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    return data?data->isValid():false;
}

//==============================================================================

bool CellmlTextViewWidget::isEditorWidgetContentsModified(const QString &pFileName) const
{
    // Return whether the contents of the requested editor widget has been
    // modified

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    if (data) {
        if (Core::FileManager::instance()->isModified(pFileName)) {
            // The given file is considered as modified, so we need to retrieve
            // the contents of its physical version

            QString fileContents;

            Core::readFileContentsFromFile(pFileName, fileContents);

            // Check whether we already know about that file contents and, if
            // not, determine its converted version (and keep track of it)

            if (fileContents.compare(data->fileContents())) {
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

            return Core::sha1(data->editingWidget()->editorWidget()->contents()).compare(Core::sha1(data->convertedFileContents()));
        } else {
            // The given file is not considered as modified, so simply compare
            // the SHA-1 value of our editor's contents with our internal one

            return Core::sha1(data->editingWidget()->editorWidget()->contents()).compare(data->sha1());
        }
    } else {
        return false;
    }
}

//==============================================================================

bool CellmlTextViewWidget::saveFile(const QString &pOldFileName,
                                    const QString &pNewFileName,
                                    bool &pNeedFeedback)
{
    // Save the given file

    CellmlTextViewWidgetData *data = mData.value(pOldFileName);

    if (data) {
        // Parse the contents of the editor and, if successful, serialise its
        // corresponding DOM document, making sure that we include any metadata
        // that was in the original CellML file

        if (parse(pOldFileName)) {
            // Check whether we need a higher version of CellML to save the file
            // and, if so, ask the user whether it's OK to use that higher
            // version

            if (   (data->cellmlVersion() != CellMLSupport::CellmlFile::Unknown)
                && (mParser.cellmlVersion() > data->cellmlVersion())
                && (Core::questionMessageBox(tr("Save File"),
                                             tr("<strong>%1</strong> requires features that are not present in %2 and should therefore be saved as a %3 file. Do you want to proceed?").arg(pNewFileName,
                                                                                                                                                                                            CellMLSupport::CellmlFile::versionAsString(data->cellmlVersion()),
                                                                                                                                                                                            CellMLSupport::CellmlFile::versionAsString(mParser.cellmlVersion()))) == QMessageBox::No)) {
                pNeedFeedback = false;

                return false;
            }

            data->setCellmlVersion(mParser.cellmlVersion());

            // Add the metadata to our DOM document

            QDomDocument domDocument = mParser.domDocument();
            QDomElement domElement = domDocument.documentElement();

            for (QDomElement childElement = data->rdfNodes().firstChildElement();
                 !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
                domElement.appendChild(childElement.cloneNode());
            }

            // Serialise our DOM document

            if (Core::writeFileContentsToFile(pNewFileName, Core::serialiseDomDocument(domDocument))) {
                // We could serialise our DOM document, so update our SHA-1
                // value

                data->setSha1(Core::sha1(data->editingWidget()->editorWidget()->contents()));

                mData.insert(pOldFileName, data);

                return true;
            } else {
                return false;
            }
        } else {
            // The parsing was unsuccessful, so ask the user whether s/he wants
            // to save the contents of the view to a text file

            if (Core::questionMessageBox(tr("Save File"),
                                         tr("<strong>%1</strong> could not be saved. Do you want to save the contents of the view to a text file?").arg(pNewFileName)) == QMessageBox::Yes) {
                QString fileName = Core::getSaveFileName(tr("Save File"),
                                                         Core::newFileName(pNewFileName, "txt"));

                if (!fileName.isEmpty())
                    Core::writeFileContentsToFile(fileName, data->editingWidget()->editorWidget()->contents());
            }

            pNeedFeedback = false;

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

    if (mEditingWidget) {
        return QList<QWidget *>() << mEditingWidget->editorWidget()->cursorPositionWidget()
                                  << mEditingWidget->editorWidget()->editingModeWidget();
    } else {
        return QList<QWidget *>();
    }
}

//==============================================================================

void CellmlTextViewWidget::reformat(const QString &pFileName)
{
    // Reformat the contents of the given file's editor

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    if (data && parse(pFileName, true)) {
        EditorWidget::EditorWidget *editor = data->editingWidget()->editorWidget();
        int cursorLine;
        int cursorColumn;

        editor->cursorPosition(cursorLine, cursorColumn);

        mConverter.execute(Core::serialiseDomDocument(mParser.domDocument()));

        editor->setContents(mConverter.output(), true);
        editor->setCursorPosition(cursorLine, cursorColumn);
    }
}

//==============================================================================

bool CellmlTextViewWidget::validate(const QString &pFileName)
{
    // Validate the given file

    if (mData.contains(pFileName)) {
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

bool CellmlTextViewWidget::commentOrUncommentLine(QScintillaSupport::QScintillaWidget *pEditorWidget,
                                                  const int &pLineNumber,
                                                  const bool &pCommentLine)
{
    // (Un)comment the current line

    QString line = pEditorWidget->text(pLineNumber).trimmed();

    if (line.isEmpty())
        return false;

    // We are not dealing with an empty line, so we can (un)comment it

    if (pCommentLine) {
        pEditorWidget->insertAt(SingleLineCommentString, pLineNumber, 0);
    } else {
        // Uncomment the line, should it be commented

        if (line.startsWith(SingleLineCommentString)) {
            int commentLineNumber, commentColumnNumber;

            pEditorWidget->lineIndexFromPosition(pEditorWidget->findTextInRange(pEditorWidget->positionFromLineIndex(pLineNumber, 0),
                                                                                pEditorWidget->contentsSize(), SingleLineCommentString,
                                                                                false, false, false),
                                                 &commentLineNumber, &commentColumnNumber);

            pEditorWidget->setSelection(commentLineNumber, commentColumnNumber,
                                        commentLineNumber, commentColumnNumber+SingleLineCommentLength);
            pEditorWidget->removeSelectedText();
        }
    }

    return true;
}

//==============================================================================

bool CellmlTextViewWidget::parse(const QString &pFileName,
                                 const bool &pOnlyErrors)
{
    // Parse the given file, should it exist

    CellmlTextViewWidgetData *data = mData.value(pFileName);

    if (data) {
        CellMLEditingView::CellmlEditingViewWidget *editingWidget = data->editingWidget();

        editingWidget->editorListWidget()->clear();

        bool res = mParser.execute(editingWidget->editorWidget()->contents(),
                                   data->cellmlVersion());

        // Add the messages that were generated by the parser, if any, and
        // select the first one of them

        foreach (const CellmlTextViewParserMessage &message, mParser.messages()) {
            if (   !pOnlyErrors
                || (message.type() == CellmlTextViewParserMessage::Error)) {
                editingWidget->editorListWidget()->addItem((message.type() == CellmlTextViewParserMessage::Error)?
                                                               EditorWidget::EditorListItem::Error:
                                                               EditorWidget::EditorListItem::Warning,
                                                           message.line(), message.column(),
                                                           message.message());
            }
        }

        selectFirstItemInEditorList(editingWidget->editorListWidget());

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

                    // Determine whether we should be commenting or uncommenting
                    // the lines

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

            if (commentOrUncommentLine(editor, lineNumber, commentLine)) {
                if (commentLine) {
                    // We commented the line, so our position will be fine
                    // unless we were at the beginning of the line, in which
                    // case we need to update it

                    if (!columnNumber)
                        editor->QsciScintilla::setCursorPosition(lineNumber, columnNumber+SingleLineCommentLength);
                } else {
                    // We uncommented the line, so go back to our original
                    // position (since uncommenting the line will have shifted
                    // it a bit)

                    editor->QsciScintilla::setCursorPosition(lineNumber, columnNumber-SingleLineCommentLength);
                }
            }
        }

        pHandled = true;
    } else {
        pHandled = false;
    }
}

//==============================================================================

QString CellmlTextViewWidget::partialStatement(const int &pPosition,
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
             && (editor->styleAt(prevAsPos) != CellmlTextViewLexer::Keyword));

    int prevSemiColonPos = pPosition;

    do {
        prevSemiColonPos = editor->findTextInRange(prevSemiColonPos, 0, SemiColonTag, false, false, false);
    } while (   (prevSemiColonPos != -1)
             && (editor->styleAt(prevSemiColonPos) != CellmlTextViewLexer::Default));

    // Look for "as" and ";" after the given position

    int nextAsPos = pPosition-AsTag.length();

    do {
        nextAsPos = editor->findTextInRange(nextAsPos+1, editorContentsSize, AsTag, false, true, true);
    } while (   (nextAsPos != -1)
             && (editor->styleAt(nextAsPos) != CellmlTextViewLexer::Keyword));

    int nextSemiColonPos = pPosition-SemiColonTag.length();

    do {
        nextSemiColonPos = editor->findTextInRange(nextSemiColonPos+1, editorContentsSize, SemiColonTag, false, false, false);
    } while (   (nextSemiColonPos != -1)
             && (editor->styleAt(nextSemiColonPos) != CellmlTextViewLexer::Default));

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

    QString currentStatement = QString();
    QString localCurrentStatement;

    CellmlTextViewParser parser;

    forever {
        localCurrentStatement = partialStatement(pPosition-1, localFromPosition, localToPosition);

        currentStatement = localCurrentStatement+currentStatement;

        pPosition = localFromPosition;

        if (parser.execute(localCurrentStatement, false)) {
            if (parser.statementType() == CellmlTextViewParser::PiecewiseSel)
                break;
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

    QString currentStatement = QString();
    QString localCurrentStatement;

    CellmlTextViewParser parser;

    forever {
        localCurrentStatement = partialStatement(pPosition, localFromPosition, localToPosition);

        currentStatement += localCurrentStatement;

        pPosition = localToPosition;

        if (parser.execute(localCurrentStatement, false)) {
            if (parser.statementType() == CellmlTextViewParser::PiecewiseEndSel)
                break;
        } else {
            break;
        }
    }

    return currentStatement;
}

//==============================================================================

QString CellmlTextViewWidget::statement(const int &pPosition) const
{
    // Retrieve the (partial) statement around the given position

    int fromPosition;
    int toPosition;

    QString currentStatement = partialStatement(pPosition, fromPosition, toPosition);

    // Check, using our CellML Text parser, whether our (partial) statement
    // contains something that we can recognise

    CellmlTextViewParser parser;

    if (parser.execute(currentStatement, false)) {
        if (parser.statementType() == CellmlTextViewParser::PiecewiseSel) {
            // We are at the beginning of a piecewise statement, so retrieve its
            // end

            currentStatement += endOfPiecewiseStatement(toPosition);
        } else if (   (parser.statementType() == CellmlTextViewParser::PiecewiseCase)
                   || (parser.statementType() == CellmlTextViewParser::PiecewiseOtherwise)) {
            // We are in the middle of a piecewise statement, so retrieve both
            // its beginning and end

            currentStatement = beginningOfPiecewiseStatement(fromPosition)+currentStatement+endOfPiecewiseStatement(toPosition);
        } else if (parser.statementType() == CellmlTextViewParser::PiecewiseEndSel) {
            // We are at the beginning of a piecewise statement, so retrieve its
            // beginning

            currentStatement = beginningOfPiecewiseStatement(fromPosition)+currentStatement;
        }

        // Skip spaces and comments to determine the real start of our current
        // statement

        EditorWidget::EditorWidget *editor = mEditingWidget->editorWidget();
        int shift = 0;
        int style;

        forever {
            style = editor->styleAt(fromPosition);

            if (   (style == CellmlTextViewLexer::SingleLineComment)
                || (style == CellmlTextViewLexer::MultilineComment)
                || currentStatement[shift].isSpace()) {
                ++fromPosition;
                ++shift;
            } else {
                break;
            }
        }

        // Make sure that we are within our current statement

        return ((pPosition >= fromPosition) && (pPosition < toPosition))?
                   editor->textInRange(fromPosition, toPosition):
                   QString();
    } else {
        // Our current statement doesn't contain something that we can recognise

        return QString();
    }
}

//==============================================================================

void CellmlTextViewWidget::updateViewer()
{
    // Make sure that we still have an editing widget (i.e. it hasn't been
    // closed since the signal was emitted)

    if (!mEditingWidget)
        return;

    // Retrieve the statement, if any, around our current position

    QString currentStatement = statement(mEditingWidget->editorWidget()->currentPosition());

    // Update the contents of our viewer

    if (currentStatement.isEmpty()) {
        // There is no statement, so clear our viewer

        mContentMathmlEquation = QString();

        mEditingWidget->mathmlViewer()->setContents(QString());
    } else {
        // There is a statement, so try to parse it

        bool res = mParser.execute(currentStatement);

        if (res) {
            // The parsing was successful, so retrieve the Content MathML
            // version of our statement and check whether it's the same as our
            // previous one

            QString contentMathmlEquation =  "<math xmlns=\""+CellMLSupport::MathmlNamespace+"\">"
                                            +Core::cleanContentMathml(Core::serialiseDomDocument(mParser.domDocument()))
                                            +"</math>";

            if (contentMathmlEquation.compare(mContentMathmlEquation)) {
                // It's a different one, so check whether we have already
                // retrieved its Presentation MathML version

                mContentMathmlEquation = contentMathmlEquation;

                QString presentationMathmlEquation = mPresentationMathmlEquations.value(contentMathmlEquation);

                if (!presentationMathmlEquation.isEmpty())
                    mEditingWidget->mathmlViewer()->setContents(presentationMathmlEquation);
                else
                    mMathmlConverter.convert(contentMathmlEquation);
            }
        } else {
            // The parsing wasn't successful

            mContentMathmlEquation = QString();

            mEditingWidget->mathmlViewer()->setError(true);
        }
    }
}

//==============================================================================

void CellmlTextViewWidget::selectFirstItemInEditorList(EditorWidget::EditorListWidget *pEditorList)
{
    // Select the first item in the given editor list

    if (pEditorList) {
        pEditorList->selectFirstItem();
    } else {
        // We came here through a single shot (see initialize()), so rely on the
        // contents of mEditorLists by selecting the first item of the first
        // first editor list, assuming it's still current (i.e. it's still
        // referenced in mData)

        EditorWidget::EditorListWidget *editorList = mEditorLists.first();

        mEditorLists.removeFirst();

        foreach (CellmlTextViewWidgetData *data, mData.values()) {
            if (data->editingWidget()->editorListWidget() == editorList) {
                editorList->selectFirstItem();

                break;
            }
        }
    }
}

//==============================================================================

void CellmlTextViewWidget::mathmlConversionDone(const QString &pContentMathml,
                                                const QString &pPresentationMathml)
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

    if (!pContentMathml.compare(mContentMathmlEquation))
        mEditingWidget->mathmlViewer()->setContents(pPresentationMathml);

    mPresentationMathmlEquations.insert(pContentMathml, pPresentationMathml);
}

//==============================================================================

}   // namespace CellMLTextView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
