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
// Raw CellML view widget
//==============================================================================

#include "cellmlfilemanager.h"
#include "corecellmleditingwidget.h"
#include "editorlistwidget.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "rawcellmlviewwidget.h"
#include "settings.h"
#include "viewerwidget.h"
#include "xsltransformer.h"

//==============================================================================

#include "ui_rawcellmlviewwidget.h"

//==============================================================================

#include <QApplication>
#include <QDomDocument>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
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
    mGui(new Ui::RawCellmlViewWidget),
    mNeedLoadingSettings(true),
    mSettingsGroup(QString()),
    mEditingWidget(0),
    mEditingWidgets(QMap<QString, CoreCellMLEditing::CoreCellmlEditingWidget *>()),
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

RawCellmlViewWidget::~RawCellmlViewWidget()
{
    // Delete the GUI

    delete mGui;

    // Stop our XSL transformer
    // Note: we don't need to delete it since it will be done as part of its
    //       thread being stopped...

    mXslTransformer->stop();
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

    foreach (CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget, mEditingWidgets)
        editingWidget->retranslateUi();
}

//==============================================================================

bool RawCellmlViewWidget::contains(const QString &pFileName) const
{
    // Return whether we know about the given file, i.e. whether we have an
    // editing widget for it

    return mEditingWidgets.value(pFileName);
}

//==============================================================================

void RawCellmlViewWidget::initialize(const QString &pFileName,
                                     const bool &pUpdate)
{
    // Retrieve the editing widget associated with the given file, if any

    CoreCellMLEditing::CoreCellmlEditingWidget *newEditingWidget = mEditingWidgets.value(pFileName);

    if (!newEditingWidget) {
        // No editing widget exists for the given file, so create one

        QString fileContents;

        Core::readTextFromFile(pFileName, fileContents);

        newEditingWidget = new CoreCellMLEditing::CoreCellmlEditingWidget(fileContents,
                                                                          !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                                          new QsciLexerXML(this),
                                                                          parentWidget());

        // Update our viewer whenever necessary

        connect(newEditingWidget->editor(), SIGNAL(textChanged()),
                this, SLOT(updateViewer()));
        connect(newEditingWidget->editor(), SIGNAL(cursorPositionChanged(const int &, const int &)),
                this, SLOT(updateViewer()));

        // Keep track of our editing widget and add it to ourselves

        mEditingWidgets.insert(pFileName, newEditingWidget);

        layout()->addWidget(newEditingWidget);
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

        updateViewer();

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

void RawCellmlViewWidget::finalize(const QString &pFileName)
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

void RawCellmlViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed
    // Note: if the view for the given file is not the active view, then to call
    //       call finalize() and then initialize() would activate the contents
    //       of the view (but the file tab would still point to the previously
    //       active file). However, we want to the 'old' file to remain the
    //       active one, hence the extra argument we pass to initialize()...

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

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mEditingWidgets.value(pOldFileName);

    if (editingWidget) {
        mEditingWidgets.insert(pNewFileName, editingWidget);
        mEditingWidgets.remove(pOldFileName);
    }
}

//==============================================================================

Editor::EditorWidget * RawCellmlViewWidget::editor(const QString &pFileName) const
{
    // Return the requested editor

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    return editingWidget?editingWidget->editor():0;
}

//==============================================================================

QList<QWidget *> RawCellmlViewWidget::statusBarWidgets() const
{
    // Return our status bar widgets

    if (mEditingWidget)
        return QList<QWidget *>() << mEditingWidget->editor()->cursorPositionWidget()
                                  << mEditingWidget->editor()->editingModeWidget();
    else
        return QList<QWidget *>();
}

//==============================================================================

void RawCellmlViewWidget::validate(const QString &pFileName) const
{
    // Validate the given file

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget = mEditingWidgets.value(pFileName);

    if (editingWidget) {
        // Clear the list of CellML issues

        EditorList::EditorListWidget *editorList = editingWidget->editorList();

        editorList->clear();

        // Retrieve the list of CellML issues, if any

        CellMLSupport::CellmlFile *cellmlFile = CellMLSupport::CellmlFileManager::instance()->cellmlFile(pFileName);
        CellMLSupport::CellmlFileIssues cellmlFileIssues;

        if (cellmlFile->isValid(pFileName, editingWidget->editor()->contents(), cellmlFileIssues)) {
            // There are no CellML issues, so the CellML file is valid

            QMessageBox::information(qApp->activeWindow(),
                                     tr("CellML Validation"),
                                     tr("The CellML file is valid."),
                                     QMessageBox::Ok);
        } else {
            // There are some CellML issues, so add them to our list and select
            // the first one

            foreach (const CellMLSupport::CellmlFileIssue &cellmlFileIssue, cellmlFileIssues)
                editorList->addItem((cellmlFileIssue.type() == CellMLSupport::CellmlFileIssue::Error)?
                                        EditorList::EditorListItem::Error:
                                        EditorList::EditorListItem::Warning,
                                    cellmlFileIssue.line(),
                                    cellmlFileIssue.column(),
                                    qPrintable(cellmlFileIssue.formattedMessage()));

            editorList->selectFirstItem();
        }
    }
}

//==============================================================================

void RawCellmlViewWidget::cleanUpXml(const QDomNode &pDomNode) const
{
    // Clean up the current node

    QDomNamedNodeMap domNodeAttributes = pDomNode.attributes();

    QStringList attributeNames = QStringList();

    for (int j = 0, jMax = domNodeAttributes.count(); j < jMax; ++j) {
        QString attributeName = domNodeAttributes.item(j).nodeName();

        if (attributeName.contains(":"))
            attributeNames << attributeName;
    }

    foreach (const QString &attributeName, attributeNames)
        domNodeAttributes.removeNamedItem(attributeName);

    // Go through the node's children, if any, and clean them up

    for (int i = 0, iMax = pDomNode.childNodes().count(); i < iMax; ++i)
        cleanUpXml(pDomNode.childNodes().at(i));
}

//==============================================================================

QString RawCellmlViewWidget::cleanUpXml(const QString &pMathml) const
{
    // Clean up and return the given XML string

    QDomDocument domDocument;

    if (domDocument.setContent(pMathml)) {
        QDomNode domNode = domDocument.documentElement();

        cleanUpXml(domNode);

        return domDocument.toString(-1);
    } else {
        return QString();
    }
}

//==============================================================================

QString RawCellmlViewWidget::retrieveContentMathmlEquation(const QString &pContentMathmlBlock,
                                                           const int &pPosition) const
{
    // Retrieve a DOM representation of the given Content MathML block

    QDomDocument domDocument;

    if (domDocument.setContent(pContentMathmlBlock)) {
        // Look for the child node within which our position is located, if any

        int childNodeIndex = -1;
        QDomNode domNode = domDocument.documentElement();

        for (int i = 0, iMax = domNode.childNodes().count(); i < iMax; ++i) {
            QDomNode childNode = domNode.childNodes().at(i);

            // Retrieve the start position of the current child node
            // Note: it needs to be corrected since the line and column numbers
            //       we are getting for the current child node correspond to the
            //       position of ">" in, say, "<apply ...>", while we need the
            //       position of "<"...

            int childNodeStartPosition;

            Core::stringLineColumnAsPosition(pContentMathmlBlock,
                                             mEditingWidget->editor()->eolString(),
                                             childNode.lineNumber(),
                                             childNode.columnNumber(),
                                             childNodeStartPosition);

            childNodeStartPosition = pContentMathmlBlock.lastIndexOf("<"+childNode.nodeName(), childNodeStartPosition);

            // Retrieve the end position of the current child node

            int childNodeEndPosition = -1;

            if (i < iMax-1) {
                // We are not dealing with the last child node, so update the
                // position from which we are to look for the closing tag, which
                // here must be the start position of the next child node and
                // not the end of the given Content MathML block

                QDomNode nextChildNode = domNode.childNodes().at(i+1);

                Core::stringLineColumnAsPosition(pContentMathmlBlock,
                                                 mEditingWidget->editor()->eolString(),
                                                 nextChildNode.lineNumber(),
                                                 nextChildNode.columnNumber(),
                                                 childNodeEndPosition);
            }

            childNodeEndPosition = pContentMathmlBlock.lastIndexOf("</"+childNode.nodeName()+">", childNodeEndPosition)+2+childNode.nodeName().length();

            // Check whether our position is within the start and end positions
            // of the current child node

            if ((pPosition >= childNodeStartPosition) && (pPosition <= childNodeEndPosition)) {
                childNodeIndex = i;

                break;
            }
        }

        // Check whether our position is within a child node

        if (childNodeIndex != -1) {
            // We are within a childe node, so remove all the other child nodes
            // and the string representation of the resulting DOM document

            for (int i = 0, iMax = domNode.childNodes().count()-1-childNodeIndex; i < iMax; ++i)
                domNode.removeChild(domNode.lastChild());

            for (int i = 0; i < childNodeIndex; ++i)
                domNode.removeChild(domNode.firstChild());

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
    static const QByteArray EndMathTag = "</math>";

    Editor::EditorWidget *editor = mEditingWidget->editor();
    int currentPosition = editor->currentPosition();

    int crtStartMathTagPos = editor->findTextInRange(currentPosition+StartMathTag.length(), 0, StartMathTag);
    int prevEndMathTagPos = editor->findTextInRange(currentPosition, 0, EndMathTag);
    int crtEndMathTagPos = editor->findTextInRange(currentPosition-EndMathTag.length()+1, editor->contentsSize(), EndMathTag);

    bool foundMathmlBlock = true;

    if (   (crtStartMathTagPos >= 0) && (crtEndMathTagPos >= 0)
        && (crtStartMathTagPos <= currentPosition)
        && (currentPosition <= crtEndMathTagPos+EndMathTag.length()-1)) {
        if (   (prevEndMathTagPos >= 0)
            && (prevEndMathTagPos > crtStartMathTagPos)
            && (prevEndMathTagPos < currentPosition))
            foundMathmlBlock = false;
    } else {
        foundMathmlBlock = false;
    }

    if (foundMathmlBlock) {
        // Retrieve the Content MathML block

        QString contentMathmlBlock = editor->textInRange(crtStartMathTagPos, crtEndMathTagPos+EndMathTag.length());

        // Make sure that we have a valid Content MathML block
        // Note: indeed, our Content MathML block may not be valid, in which
        //       case cleaning it up will result in an empty string...

        if (cleanUpXml(contentMathmlBlock).isEmpty()) {
            mContentMathmlEquation = QString();

            mEditingWidget->viewer()->setError(true);
        } else {
            // A Content MathML block contains 0+ child nodes, so extract and
            // clean up the one, if any, at our current position

            QString contentMathmlEquation = cleanUpXml(retrieveContentMathmlEquation(contentMathmlBlock, currentPosition-crtStartMathTagPos));

            // Check whether we have got a Content MathML equation

            if (!contentMathmlEquation.isEmpty()) {
                // Now, check whether our Content MathML equation is the same as
                // our previous one

                if (!contentMathmlEquation.compare(mContentMathmlEquation)) {
                    // It's the same, so leave

                    return;
                } else {
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
                // Our current position is not within a Content MathML equation

                mContentMathmlEquation = QString();

                mEditingWidget->viewer()->setContents(QString());
            }
        }
    } else {
        // We couldn't find any Content MathML block

        mContentMathmlEquation = QString();

        mEditingWidget->viewer()->setContents(QString());
    }
}

//==============================================================================

void RawCellmlViewWidget::xslTransformationDone(const QString &pInput,
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

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
