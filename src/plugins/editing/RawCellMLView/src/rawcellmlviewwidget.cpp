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

#include "corecellmleditingwidget.h"
#include "editorwidget.h"
#include "filemanager.h"
#include "rawcellmlviewwidget.h"
#include "viewerwidget.h"
#include "xsltransformer.h"

//==============================================================================

#include "ui_rawcellmlviewwidget.h"

//==============================================================================

#include <QDesktopWidget>
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
    mGui(new Ui::RawCellmlViewWidget),
    mEditingWidget(0),
    mEditingWidgets(QMap<QString, CoreCellMLEditing::CoreCellmlEditingWidget *>()),
    mEditingWidgetSizes(QIntList()),
    mEditorZoomLevel(0),
    mViewerOptimiseFontSizeEnabled(true),
    mViewerSubscriptsEnabled(true),
    mViewerGreekSymbolsEnabled(true),
    mViewerDigitGroupingEnabled(true),
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

static const auto SettingsEditingWidgetSizes            = QStringLiteral("EditingWidgetSizes");
static const auto SettingsEditorZoomLevel               = QStringLiteral("EditorZoomLevel");
static const auto SettingsViewerOptimiseFontSizeEnabled = QStringLiteral("ViewerOptimiseFontSizeEnabled");
static const auto SettingsViewerSubscriptsEnabled       = QStringLiteral("ViewerSubscriptsEnabled");
static const auto SettingsViewerGreekSymbolsEnabled     = QStringLiteral("ViewerGreekSymbolsEnabled");
static const auto SettingsViewerDigitGroupingEnabled    = QStringLiteral("ViewerDigitGroupingEnabled");

//==============================================================================

void RawCellmlViewWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve the editing widget's sizes and the editor's zoom level
    // Note #1: the viewer's default height is 19% of the desktop's height while
    //          that of the editor is as big as it can be...
    // Note #2: because the editor's default height is much bigger than that of
    //          our raw CellML view widget, the viewer's default height will
    //          effectively be less than 19% of the desktop's height, but that
    //          doesn't matter at all...

    QVariantList defaultEditingWidgetSizes = QVariantList() << 0.19*qApp->desktop()->screenGeometry().height()
                                                            << qApp->desktop()->screenGeometry().height();

    mEditingWidgetSizes = qVariantListToIntList(pSettings->value(SettingsEditingWidgetSizes, defaultEditingWidgetSizes).toList());
    mEditorZoomLevel = pSettings->value(SettingsEditorZoomLevel, 0).toInt();

    // Retrieve the editing widget's viewer settings

    mViewerOptimiseFontSizeEnabled = pSettings->value(SettingsViewerOptimiseFontSizeEnabled, true).toBool();
    mViewerSubscriptsEnabled = pSettings->value(SettingsViewerSubscriptsEnabled, true).toBool();
    mViewerGreekSymbolsEnabled = pSettings->value(SettingsViewerGreekSymbolsEnabled, true).toBool();
    mViewerDigitGroupingEnabled = pSettings->value(SettingsViewerDigitGroupingEnabled, true).toBool();
}

//==============================================================================

void RawCellmlViewWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of the editing widget's sizes and the editor's zoom level

    pSettings->setValue(SettingsEditingWidgetSizes, qIntListToVariantList(mEditingWidgetSizes));
    pSettings->setValue(SettingsEditorZoomLevel, mEditorZoomLevel);

    // Keep track of the editing widget's viewer settings

    pSettings->setValue(SettingsViewerOptimiseFontSizeEnabled, mViewerOptimiseFontSizeEnabled);
    pSettings->setValue(SettingsViewerSubscriptsEnabled, mViewerSubscriptsEnabled);
    pSettings->setValue(SettingsViewerGreekSymbolsEnabled, mViewerGreekSymbolsEnabled);
    pSettings->setValue(SettingsViewerDigitGroupingEnabled, mViewerDigitGroupingEnabled);
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

void RawCellmlViewWidget::initialize(const QString &pFileName)
{
    // Retrieve the editing widget associated with the given file, if any

    CoreCellMLEditing::CoreCellmlEditingWidget *oldEditingWidget = mEditingWidget;

    mEditingWidget = mEditingWidgets.value(pFileName);

    if (!mEditingWidget) {
        // No editing widget exists for the given file, so create one

        QString fileContents;

        Core::readTextFromFile(pFileName, fileContents);

        mEditingWidget = new CoreCellMLEditing::CoreCellmlEditingWidget(fileContents,
                                                                        !Core::FileManager::instance()->isReadableAndWritable(pFileName),
                                                                        new QsciLexerXML(this),
                                                                        parentWidget());

        // Keep track of our editing widget's sizes when moving the splitter and
        // of changes to our editor's zoom level

        connect(mEditingWidget, SIGNAL(splitterMoved(int, int)),
                this, SLOT(splitterMoved()));

        connect(mEditingWidget->editor(), SIGNAL(zoomLevelChanged(const int &)),
                this, SLOT(zoomLevelChanged(const int &)));

        // Update our viewer whenever necessary

        connect(mEditingWidget->editor(), SIGNAL(textChanged()),
                this, SLOT(updateViewer()));
        connect(mEditingWidget->editor(), SIGNAL(cursorPositionChanged(const int &, const int &)),
                this, SLOT(updateViewer()));

        // Keep track of our editing widget's viewer settings

        connect(mEditingWidget->viewer(), SIGNAL(optimiseFontSizeChanged(const bool &)),
                this, SLOT(optimiseFontSizeChanged(const bool &)));
        connect(mEditingWidget->viewer(), SIGNAL(subscriptsChanged(const bool &)),
                this, SLOT(subscriptsChanged(const bool &)));
        connect(mEditingWidget->viewer(), SIGNAL(greekSymbolsChanged(const bool &)),
                this, SLOT(greekSymbolsChanged(const bool &)));
        connect(mEditingWidget->viewer(), SIGNAL(digitGroupingChanged(const bool &)),
                this, SLOT(digitGroupingChanged(const bool &)));

        // Keep track of our editing widget and add it to ourselves

        mEditingWidgets.insert(pFileName, mEditingWidget);

        layout()->addWidget(mEditingWidget);
    }

    // Set our current editing widget's viewer settings

    mEditingWidget->viewer()->setOptimiseFontSize(mViewerOptimiseFontSizeEnabled);
    mEditingWidget->viewer()->setSubscripts(mViewerSubscriptsEnabled);
    mEditingWidget->viewer()->setGreekSymbols(mViewerGreekSymbolsEnabled);
    mEditingWidget->viewer()->setDigitGrouping(mViewerDigitGroupingEnabled);

    // Show/hide our editing widgets and adjust our sizes

    foreach (CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget, mEditingWidgets)
        if (editingWidget == mEditingWidget) {
            // This is the editing widget we are after, so show it and update
            // its size, zoom level and find/replace widget

            editingWidget->setSizes(mEditingWidgetSizes);
            editingWidget->editor()->setZoomLevel(mEditorZoomLevel);

            if (oldEditingWidget)
                editingWidget->editor()->updateFindReplaceFrom(oldEditingWidget->editor());

            editingWidget->show();
        } else {
            // Not the editing widget we are after, so hide it

            editingWidget->hide();
        }

    // Set our focus proxy to our 'new' editing widget and make sure that the
    // latter immediately gets the focus
    // Note: if we were not to immediately give our 'new' editing widget the
    //       focus, then the central widget would give the focus to our 'old'
    //       editing widget (see CentralWidget::updateGui()), so...

    setFocusProxy(mEditingWidget->editor());

    mEditingWidget->editor()->setFocus();
}

//==============================================================================

void RawCellmlViewWidget::finalize(const QString &pFileName)
{
    // Remove the editing widget, should there be one for the given file

    CoreCellMLEditing::CoreCellmlEditingWidget *editingWidget  = mEditingWidgets.value(pFileName);

    if (editingWidget) {
        // There is an editor for the given file name, so delete it and remove
        // it from our list

        delete editingWidget;

        mEditingWidgets.remove(pFileName);

        // Reset our memory of the current editor, if needed

        if (editingWidget == mEditingWidget)
            mEditingWidget = 0;
    }
}

//==============================================================================

void RawCellmlViewWidget::fileReloaded(const QString &pFileName)
{
    // The given file has been reloaded, so reload it, should it be managed

    if (contains(pFileName)) {
        finalize(pFileName);
        initialize(pFileName);
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

void RawCellmlViewWidget::cleanUpXml(const QDomNode &pDomNode) const
{
    // Go through the node's children and remove all unrecognisable attributes

    for (int i = 0, iMax = pDomNode.childNodes().count(); i < iMax; ++i) {
        QDomNode domNode = pDomNode.childNodes().at(i);
        QDomNamedNodeMap domNodeAttributes = domNode.attributes();

        QStringList attributeNames = QStringList();

        for (int j = 0, jMax = domNodeAttributes.count(); j < jMax; ++j) {
            QString attributeName = domNodeAttributes.item(j).nodeName();

            if (attributeName.contains(":"))
                attributeNames << attributeName;
        }

        foreach (const QString &attributeName, attributeNames)
            domNodeAttributes.removeNamedItem(attributeName);

        cleanUpXml(domNode);
    }
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
            // We are not within a child node, so...

            return QString();
        }
    } else {
        // No DOM representation of the given Content MathML block could be
        // retrieved, so...

        return QString();
    }
}

//==============================================================================

void RawCellmlViewWidget::splitterMoved()
{
    // The splitter has moved, so keep track of the editing widget's sizes

    mEditingWidgetSizes = qobject_cast<CoreCellMLEditing::CoreCellmlEditingWidget *>(sender())->sizes();
}

//==============================================================================

void RawCellmlViewWidget::zoomLevelChanged(const int &pZoomLevel)
{
    // One of our editors had its zoom level changed, so keep track of the new
    // zoom level

    mEditorZoomLevel = pZoomLevel;
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
                // Our current position is not within a Content MathML equation,
                // so...

                mContentMathmlEquation = QString();

                mEditingWidget->viewer()->setContents(mContentMathmlEquation);
            }
        }
    } else {
        // We couldn't find any Content MathML block, so...

        mContentMathmlEquation = QString();

        mEditingWidget->viewer()->setContents(mContentMathmlEquation);
    }
}

//==============================================================================

void RawCellmlViewWidget::optimiseFontSizeChanged(const bool &pEnabled)
{
    // Keep track of our editing widget's viewer settings

    mViewerOptimiseFontSizeEnabled = pEnabled;
}

//==============================================================================

void RawCellmlViewWidget::subscriptsChanged(const bool &pEnabled)
{
    // Keep track of our editing widget's viewer settings

    mViewerSubscriptsEnabled = pEnabled;
}

//==============================================================================

void RawCellmlViewWidget::greekSymbolsChanged(const bool &pEnabled)
{
    // Keep track of our editing widget's viewer settings

    mViewerGreekSymbolsEnabled = pEnabled;
}

//==============================================================================

void RawCellmlViewWidget::digitGroupingChanged(const bool &pEnabled)
{
    // Keep track of our editing widget's viewer settings

    mViewerDigitGroupingEnabled = pEnabled;
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
