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
    mPresentationMathmlEquations(QMap<QString, QString>())
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
            // its size and zoom level

            editingWidget->setSizes(mEditingWidgetSizes);
            editingWidget->editor()->setZoomLevel(mEditorZoomLevel);

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
        // If our DOM representation contains zero or one child node, then we
        // simply return its string representation

        QDomNode domNode = domDocument.documentElement();

        if (domNode.childNodes().count() <= 1)
            return domDocument.toString(-1);

        // Go through the different child nodes of our DOM representation and
        // determine (based on our line and column values) to which one we
        // 'belong'

        QDomNode prevChildNode = domNode.firstChild();

        while (domNode.childNodes().count() != 1) {
            // Retrieve the position of the current child node
            // Note: it needs to be corrected since the line and column numbers
            //       we are getting for the current child node correspond to the
            //       position of ">" in, say, "<apply ...>", while we need the
            //       position of "<"...

            QDomNode childNode = prevChildNode.nextSibling();
            int childNodePosition;

            Core::stringLineColumnAsPosition(pContentMathmlBlock,
                                             mEditingWidget->editor()->eolString(),
                                             childNode.lineNumber(),
                                             childNode.columnNumber(),
                                             childNodePosition);

            childNodePosition = pContentMathmlBlock.lastIndexOf("<"+childNode.nodeName(), childNodePosition);

            // Check where we are with respect to the position of the current
            // child node and remove the previous node or all the subsequent
            // ones, depending on the case

            if (pPosition >= childNodePosition) {
                domNode.removeChild(prevChildNode);

                prevChildNode = childNode;
            } else {
                while (domNode.childNodes().count() != 1)
                    domNode.removeChild(domNode.lastChild());

                break;
            }
        }

        // At this stage, our DOM document contains only the equation we are
        // after, so all we now need to do is return its string representation

        return domDocument.toString(-1);
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

    // Retrieve the new mathematical equation, if any, around our current
    // position

    static const QString StartMathTag = "<math ";
    static const QByteArray EndMathTag = "</math>";

    Editor::EditorWidget *editor = mEditingWidget->editor();
    int crtPos = editor->currentPosition();

    int crtStartMathTagPos = editor->findTextInRange(crtPos+StartMathTag.length(), 0, StartMathTag);
    int prevEndMathTagPos = editor->findTextInRange(crtPos, 0, EndMathTag);
    int crtEndMathTagPos = editor->findTextInRange(crtPos-EndMathTag.length()+1, editor->contentsSize(), EndMathTag);

    bool foundMathmlBlock = true;

    if (   (crtStartMathTagPos >= 0) && (crtEndMathTagPos >= 0)
        && (crtStartMathTagPos <= crtPos)
        && (crtPos <= crtEndMathTagPos+EndMathTag.length()-1)) {
        if (   (prevEndMathTagPos >= 0)
            && (prevEndMathTagPos > crtStartMathTagPos)
            && (prevEndMathTagPos < crtPos))
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
            mEditingWidget->viewer()->setError(true);
        } else {
            // A Content MathML block contains 0+ child nodes, so extract and
            // clean up the one, if any, at our current position

            QString contentMathmlEquation = cleanUpXml(retrieveContentMathmlEquation(contentMathmlBlock, crtPos-crtStartMathTagPos));

            // Check whether our Content MathML equation is the same as our
            // previous one

            if (!contentMathmlEquation.compare(mEditingWidget->viewer()->contents())) {
                // It's the same, so leave

                return;
            } else {
                // It's a different one, so check whether we have already
                // retrieved its Presentation MathML version

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
        }
    } else {
        mEditingWidget->viewer()->setContents(QString());
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
    // the Presentation MathML

    mEditingWidget->viewer()->setContents(pOutput);

    mPresentationMathmlEquations.insert(pInput, pOutput);
}

//==============================================================================

}   // namespace RawCellMLView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
