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
// Raw CellML view widget
//==============================================================================
// Note: this view and the CellML Text view work in the same way when it comes
//       to updating the viewer (e.g. see the XSL transformation)...
//==============================================================================

#include "cellmlfilemanager.h"
#include "cellmleditingviewwidget.h"
#include "corecliutils.h"
#include "editorlistwidget.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "mathmlviewerwidget.h"
#include "rawcellmlviewwidget.h"

//==============================================================================

#include <QDomDocument>
#include <QLabel>
#include <QLayout>
#include <QMetaType>
#include <QSettings>
#include <QVariant>

//==============================================================================

#include "Qsci/qscilexerxml.h"

//==============================================================================

namespace OpenCOR {
namespace RawCellMLView {

//==============================================================================

RawCellmlViewWidget::RawCellmlViewWidget(QWidget *pParent) :
    ViewWidget(pParent),
    mNeedLoadingSettings(true),
    mSettingsGroup(QString()),
    mEditingWidget(0),
    mEditingWidgets(QMap<QString, CellMLEditingView::CellmlEditingViewWidget *>()),
    mPresentationMathmlEquations(QMap<QString, QString>()),
    mContentMathmlEquation(QString())
{
    // Create our MathML converter and create a connection to retrieve the
    // result of its MathML conversions

    connect(&mMathmlConverter, SIGNAL(done(const QString &, const QString &)),
            this, SLOT(mathmlConversionDone(const QString &, const QString &)));
}

//==============================================================================

void RawCellmlViewWidget::loadSettings(QSettings *pSettings)
{
    // Normally, we would retrieve the editing widget's settings, but
    // mEditingWidget is not set at this stage. So, instead, we keep track of
    // our settings' group and load them when initialising ourselves (see
    // initialize())...

    mSettingsGroup = pSettings->group();
}

//==============================================================================

void RawCellmlViewWidget::saveSettings(QSettings *pSettings) const
{
    Q_UNUSED(pSettings);
    // Note: our view is such that our settings are actually saved when calling
    //       finalize() on the last file...
}

//==============================================================================

void RawCellmlViewWidget::retranslateUi()
{
    // Retranslate all our editing widgets

    foreach (CellMLEditingView::CellmlEditingViewWidget *editingWidget, mEditingWidgets)
        editingWidget->retranslateUi();
}

//==============================================================================

bool RawCellmlViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file

    return mEditingWidgets.contains(pFileName);
}

//==============================================================================

void RawCellmlViewWidget::initialize(const QString &pFileName,
                                     const bool &pUpdate)
{
    // Retrieve the editing widget associated with the given file, if any

    CellMLEditingView::CellmlEditingViewWidget *newEditingWidget = mEditingWidgets.value(pFileName);

    if (!newEditingWidget) {
        // No editing widget exists for the given file, so create one

        QString fileContents;

        Core::readFileContentsFromFile(pFileName, fileContents);

        newEditingWidget = new CellMLEditingView::CellmlEditingViewWidget(fileContents,
                                                                          !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                                          new QsciLexerXML(this),
                                                                          parentWidget());

        // Update our viewer whenever necessary

        connect(newEditingWidget->editorWidget(), SIGNAL(textChanged()),
                this, SLOT(updateViewer()));
        connect(newEditingWidget->editorWidget(), SIGNAL(cursorPositionChanged(const int &, const int &)),
                this, SLOT(updateViewer()));

        // Keep track of our editing widget

        mEditingWidgets.insert(pFileName, newEditingWidget);
    }

    // Update our editing widget, if required

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

        // Update our viewer

        updateViewer();

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

void RawCellmlViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CellMLEditingView::CellmlEditingViewWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget) {
        // There is an editing widget for the given file name, so save our
        // settings and reset our memory of the current editing widget, if
        // needed

        if (mEditingWidget == editingWidget) {
            QSettings settings;

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

void RawCellmlViewWidget::fileReloaded(const QString &pFileName)
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
        initialize(pFileName, update);
    }
}

//==============================================================================

void RawCellmlViewWidget::fileRenamed(const QString &pOldFileName,
                                      const QString &pNewFileName)
{
    // The given file has been renamed, so update our editing widgets mapping

    CellMLEditingView::CellmlEditingViewWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    if (editingWidget) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgets.remove(pOldFileName);
    }
}

//==============================================================================

EditorWidget::EditorWidget * RawCellmlViewWidget::editorWidget(const QString &pFileName) const
{
    // Return the requested editor widget

    CellMLEditingView::CellmlEditingViewWidget *editingWidget = mEditingWidgets.value(pFileName);

    return editingWidget?editingWidget->editorWidget():0;
}

//==============================================================================

QWidget * RawCellmlViewWidget::widget(const QString &pFileName)
{
    // Return the requested (editing) widget

    return mEditingWidgets.value(pFileName);
}

//==============================================================================

QList<QWidget *> RawCellmlViewWidget::statusBarWidgets() const
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

void RawCellmlViewWidget::reformat(const QString &pFileName)
{
    // Reformat the contents of the given file's editor

    CellMLEditingView::CellmlEditingViewWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget && validate(pFileName, true)) {
        int cursorLine;
        int cursorColumn;

        editingWidget->editorWidget()->cursorPosition(cursorLine, cursorColumn);

        QDomDocument domDocument;

        domDocument.setContent(editingWidget->editorWidget()->contents());

        editingWidget->editorWidget()->setContents(Core::serialiseDomDocument(domDocument), true);
        editingWidget->editorWidget()->setCursorPosition(cursorLine, cursorColumn);
    }
}

//==============================================================================

bool RawCellmlViewWidget::validate(const QString &pFileName,
                                   const bool &pOnlyErrors) const
{
    // Validate the given file

    CellMLEditingView::CellmlEditingViewWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget) {
        // Clear the list of CellML issues

        EditorWidget::EditorListWidget *editorList = editingWidget->editorListWidget();

        editorList->clear();

        // Retrieve the list of CellML issues, if any

        CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);
        CellMLSupport::CellmlFileIssues cellmlFileIssues;

        bool res = cellmlFile->isValid(editingWidget->editorWidget()->contents(), cellmlFileIssues, true);

        // Warn the user about the CellML issues being maybe for a (in)direclty
        // imported CellML file, should we be dealing with a CellML 1.1 file

        int nbOfReportedIssues = 0;

        foreach (const CellMLSupport::CellmlFileIssue &cellmlFileIssue, cellmlFileIssues) {
            nbOfReportedIssues +=    !pOnlyErrors
                                  ||  (cellmlFileIssue.type() == CellMLSupport::CellmlFileIssue::Error);
        }

        if (   (cellmlFile->version() != CellMLSupport::CellmlFile::Cellml_1_0)
            && cellmlFile->model() && cellmlFile->model()->imports()->length()
            && nbOfReportedIssues) {
            editorList->addItem(EditorWidget::EditorListItem::Information,
                                (nbOfReportedIssues == 1)?
                                    tr("The issue reported below may be related to this CellML file or to one of its (in)directly imported CellML files."):
                                    tr("The issues reported below may be related to this CellML file and/or to one or several of its (in)directly imported CellML files."));
        }

        // Add whatever issue there may be to our list and select the first one
        // of them

        foreach (const CellMLSupport::CellmlFileIssue &cellmlFileIssue, cellmlFileIssues) {
            if (   !pOnlyErrors
                || (cellmlFileIssue.type() == CellMLSupport::CellmlFileIssue::Error)) {
                editorList->addItem((cellmlFileIssue.type() == CellMLSupport::CellmlFileIssue::Error)?
                                        EditorWidget::EditorListItem::Error:
                                        EditorWidget::EditorListItem::Warning,
                                    cellmlFileIssue.line(),
                                    cellmlFileIssue.column(),
                                    qPrintable(cellmlFileIssue.formattedMessage()));
            }
        }

        editorList->selectFirstItem();

        return res;
    } else {
        // The file doesn't exist, so it can't be validated

        return false;
    }
}

//==============================================================================

QString RawCellmlViewWidget::retrieveContentMathmlEquation(const QString &pContentMathmlBlock,
                                                           const int &pPosition) const
{
    // Retrieve a DOM representation of the given Content MathML block

    QDomDocument domDocument;

    if (domDocument.setContent(pContentMathmlBlock, true)) {
        // Look for the child node within which our position is located, if any

        QDomElement domElement = domDocument.documentElement();
        QDomElement foundChildElement = QDomElement();

        for (QDomElement childElement = domElement.firstChildElement();
             !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
            // Retrieve the start position of the current child node
            // Note: it needs to be corrected since the line and column numbers
            //       we are getting for the current child node correspond to the
            //       position of ">" in, say, "<apply ...>", while we need the
            //       position of "<"...

            int childNodeStartPosition;

            Core::stringLineColumnAsPosition(pContentMathmlBlock,
                                             mEditingWidget->editorWidget()->eolString(),
                                             childElement.lineNumber(),
                                             childElement.columnNumber(),
                                             childNodeStartPosition);

            childNodeStartPosition = pContentMathmlBlock.lastIndexOf("<"+childElement.localName(), childNodeStartPosition);

            // Retrieve the end position of the current child node

            int childNodeEndPosition = -1;

            if (childElement != domElement.lastChildElement()) {
                // We are not dealing with the last child node, so update the
                // position from which we are to look for the closing tag, which
                // here must be the start position of the next child node and
                // not the end of the given Content MathML block

                QDomElement nextChildElement = childElement.nextSiblingElement();

                Core::stringLineColumnAsPosition(pContentMathmlBlock,
                                                 mEditingWidget->editorWidget()->eolString(),
                                                 nextChildElement.lineNumber(),
                                                 nextChildElement.columnNumber(),
                                                 childNodeEndPosition);
            }

            childNodeEndPosition = pContentMathmlBlock.lastIndexOf("</"+childElement.localName()+">", childNodeEndPosition)+2+childElement.localName().length();

            // Check whether our position is within the start and end positions
            // of the current child node

            if ((pPosition >= childNodeStartPosition) && (pPosition <= childNodeEndPosition)) {
                foundChildElement = childElement;

                break;
            }
        }

        // Check whether our position is within a child node

        if (!foundChildElement.isNull()) {
            // We are within a childe node, so remove all the other child nodes
            // and return the string representation of the resulting DOM
            // document

            for (QDomElement childElement = domElement.firstChildElement();
                 childElement != foundChildElement;
                 childElement = domElement.firstChildElement()) {
                domElement.removeChild(childElement);
            }

            for (QDomElement childElement = domElement.lastChildElement();
                 childElement != foundChildElement;
                 childElement = domElement.lastChildElement()) {
                domElement.removeChild(childElement);
            }

            return domDocument.toString(-1);
        } else {
            // We are not within a child node

            return QString();
        }
    } else {
        // No DOM representation of the given Content MathML block could be
        // retrieved

        return QString();
    }
}

//==============================================================================

void RawCellmlViewWidget::updateViewer()
{
    // Make sure that we still have an editing widget (i.e. it hasn't been
    // closed since the signal was emitted)

    if (!mEditingWidget)
        return;

    // Retrieve the Content MathML block around our current position, if any

    static const QString StartMathTag = "<math ";
    static const QString EndMathTag = "</math>";

    EditorWidget::EditorWidget *editor = mEditingWidget->editorWidget();
    int crtPosition = editor->currentPosition();

    int crtStartMathTagPos = editor->findTextInRange(crtPosition+StartMathTag.length(), 0, StartMathTag, false, true, false);
    int prevEndMathTagPos = editor->findTextInRange(crtPosition, 0, EndMathTag, false, true, false);
    int crtEndMathTagPos = editor->findTextInRange(crtPosition-EndMathTag.length()+1, editor->contentsSize(), EndMathTag, false, true, false);

    bool foundMathmlBlock = true;
    bool hasContentMathmlEquation = true;

    if (   (crtStartMathTagPos >= 0) && (crtEndMathTagPos >= 0)
        && (crtStartMathTagPos <= crtPosition)
        && (crtPosition <= crtEndMathTagPos+EndMathTag.length()-1)) {
        if (   (prevEndMathTagPos >= 0)
            && (prevEndMathTagPos > crtStartMathTagPos)
            && (prevEndMathTagPos < crtPosition)) {
            foundMathmlBlock = false;
        }
    } else {
        foundMathmlBlock = false;
    }

    if (foundMathmlBlock) {
        // Retrieve the Content MathML block

        QString contentMathmlBlock = editor->textInRange(crtStartMathTagPos, crtEndMathTagPos+EndMathTag.length());

        // Make sure that we have a valid Content MathML block
        // Note: indeed, our Content MathML block may not be valid, in which
        //       case cleaning it up will result in an empty string...

        if (Core::cleanContentMathml(contentMathmlBlock).isEmpty()) {
            mContentMathmlEquation = QString();

            mEditingWidget->mathmlViewer()->setError(true);
        } else {
            // A Content MathML block contains 0+ child nodes, so extract and
            // clean up the one, if any, at our current position

            QString contentMathmlEquation = Core::cleanContentMathml(retrieveContentMathmlEquation(contentMathmlBlock, crtPosition-crtStartMathTagPos));

            // Check whether we have got a Content MathML equation

            if (!contentMathmlEquation.isEmpty()) {
                // Now, check whether our Content MathML equation is the same as
                // our previous one

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
                hasContentMathmlEquation = false;
            }
        }
    }

    if (!foundMathmlBlock || !hasContentMathmlEquation) {
        mContentMathmlEquation = QString();

        mEditingWidget->mathmlViewer()->setContents(QString());
    }
}

//==============================================================================

void RawCellmlViewWidget::mathmlConversionDone(const QString &pContentMathml,
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

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
