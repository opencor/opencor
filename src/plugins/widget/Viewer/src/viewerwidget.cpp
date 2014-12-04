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
// Viewer widget
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "i18ninterface.h"
#include "viewerwidget.h"

//==============================================================================

#include <Qt>
#include <QtMath>

//==============================================================================

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QCursor>
#include <QDomDocument>
#include <QIcon>
#include <QMap>
#include <QMenu>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QPoint>
#include <QRectF>
#include <QRegularExpression>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

static QMap<QString, QString> GreekSymbols;

//==============================================================================

ViewerWidget::ViewerWidget(QWidget *pParent) :
    Widget(pParent),
    mMathmlDocument(QwtMathMLDocument()),
    mOneOverMathmlDocumentWidth(0),
    mOneOverMathmlDocumentHeight(0),
    mContents(QString()),
    mError(false)
{
    // Populate our table of Greek symbols, if needed

    if (GreekSymbols.isEmpty()) {
        GreekSymbols.insert("alpha", "α");
        GreekSymbols.insert("beta", "β");
        GreekSymbols.insert("gamma", "γ");
        GreekSymbols.insert("delta", "δ");
        GreekSymbols.insert("epsilon", "ε");
        GreekSymbols.insert("zeta", "ζ");
        GreekSymbols.insert("eta", "η");
        GreekSymbols.insert("theta", "θ");
        GreekSymbols.insert("iota", "ι");
        GreekSymbols.insert("kappa", "κ");
        GreekSymbols.insert("lambda", "λ");
        GreekSymbols.insert("mu", "μ");
        GreekSymbols.insert("nu", "ν");
        GreekSymbols.insert("xi", "ξ");
        GreekSymbols.insert("omicron", "ο");
        GreekSymbols.insert("pi", "π");
        GreekSymbols.insert("rho", "ρ");
        GreekSymbols.insert("sigma", "σ");
        GreekSymbols.insert("tau", "τ");
        GreekSymbols.insert("upsilon", "υ");
        GreekSymbols.insert("phi", "φ");
        GreekSymbols.insert("chi", "χ");
        GreekSymbols.insert("psi", "ψ");
        GreekSymbols.insert("omega", "ω");
    }

    // Create our context menu

    mContextMenu = new QMenu(this);

    mOptimiseFontSizeAction = newAction();
    mSubscriptsAction = newAction();
    mGreekSymbolsAction = newAction();
    mDigitGroupingAction = newAction();
    mCopyToClipboardAction = new QAction(this);

    connect(mOptimiseFontSizeAction, SIGNAL(toggled(bool)),
            this, SLOT(update()));

    connect(mSubscriptsAction, SIGNAL(toggled(bool)),
            this, SLOT(updateViewer()));
    connect(mGreekSymbolsAction, SIGNAL(toggled(bool)),
            this, SLOT(updateViewer()));
    connect(mDigitGroupingAction, SIGNAL(toggled(bool)),
            this, SLOT(updateViewer()));

    connect(mCopyToClipboardAction, SIGNAL(triggered(bool)),
            this, SLOT(copyToClipboard()));

    mContextMenu->addAction(mOptimiseFontSizeAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mSubscriptsAction);
    mContextMenu->addAction(mGreekSymbolsAction);
    mContextMenu->addAction(mDigitGroupingAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyToClipboardAction);

    // We want a context menu

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    // Retranslate ourselves, so that our actions are properly initialised

    retranslateUi();
}

//==============================================================================

static const auto SettingsViewerOptimiseFontSizeEnabled = QStringLiteral("ViewerOptimiseFontSizeEnabled");
static const auto SettingsViewerSubscriptsEnabled       = QStringLiteral("ViewerSubscriptsEnabled");
static const auto SettingsViewerGreekSymbolsEnabled     = QStringLiteral("ViewerGreekSymbolsEnabled");
static const auto SettingsViewerDigitGroupingEnabled    = QStringLiteral("ViewerDigitGroupingEnabled");

//==============================================================================

void ViewerWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve our settings

    setOptimiseFontSize(pSettings->value(SettingsViewerOptimiseFontSizeEnabled, true).toBool());
    setSubscripts(pSettings->value(SettingsViewerSubscriptsEnabled, true).toBool());
    setGreekSymbols(pSettings->value(SettingsViewerGreekSymbolsEnabled, true).toBool());
    setDigitGrouping(pSettings->value(SettingsViewerDigitGroupingEnabled, true).toBool());
}

//==============================================================================

void ViewerWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our settings

    pSettings->setValue(SettingsViewerOptimiseFontSizeEnabled, optimiseFontSize());
    pSettings->setValue(SettingsViewerSubscriptsEnabled, subscripts());
    pSettings->setValue(SettingsViewerGreekSymbolsEnabled, greekSymbols());
    pSettings->setValue(SettingsViewerDigitGroupingEnabled, digitGrouping());
}

//==============================================================================

void ViewerWidget::retranslateUi()
{
    // Retranslate our actions

    I18nInterface::retranslateAction(mOptimiseFontSizeAction, tr("Optimise Font Size"),
                                     tr("Optimise the font size to get the best fit possible"));
    I18nInterface::retranslateAction(mSubscriptsAction, tr("Subscripts"),
                                     tr("Use '_' to specify a subscript"));
    I18nInterface::retranslateAction(mGreekSymbolsAction, tr("Greek Symbols"),
                                     tr("Replace 'alpha' with 'α', 'beta' with 'β', etc."));
    I18nInterface::retranslateAction(mDigitGroupingAction, tr("Digit Grouping"),
                                     tr("Group the digits of a number in groups of thousands"));
    I18nInterface::retranslateAction(mCopyToClipboardAction, tr("Copy To Clipboard"),
                                     tr("Copy the contents of the viewer to the clipboard"));

    // Retranslate our contents
    // Note: we do this because we may be displaying numbers using digit
    //       grouping, this respecting the current locale...

    setContents(mContents);
}

//==============================================================================

void ViewerWidget::updateSettings(ViewerWidget *pViewerWidget)
{
    // Make sure that we are given another widget

    if (!pViewerWidget)
        return;

    // Update our viewer settings

    setOptimiseFontSize(pViewerWidget->optimiseFontSize());
    setSubscripts(pViewerWidget->subscripts());
    setGreekSymbols(pViewerWidget->greekSymbols());
    setDigitGrouping(pViewerWidget->digitGrouping());
}

//==============================================================================

QString ViewerWidget::contents() const
{
    // Return our contents

    return mContents;
}

//==============================================================================

void ViewerWidget::setContents(const QString &pContents)
{
    // Try to set our contents to our MathML document
    // Note: we don't check whether pContents has the same value as mContents
    //       since we would also need to check the value of mError and we don't
    //       know about it...

    mContents = pContents;

    if (subscripts() || greekSymbols() || digitGrouping()) {
        mError = !mMathmlDocument.setContent(processedContents());
    } else {
        // Clean up the given contents, if possible, before setting it

        QDomDocument domDocument;

        if (domDocument.setContent(pContents))
            mError = !mMathmlDocument.setContent(domDocument.toString(-1));
        else
            mError = true;
    }

    if (mError) {
        // An error occurred, but consider it only as an actual error if our
        // contents is not empty

        mError = pContents.size();
    } else {
        // Everything went fine, so determine (the inverse of) the size of our
        // contents when rendered using a font size of 100 points
        // Note: when setting the contents, QwtMathMLDocument recomputes its
        //       layout. Now, because we want the contents to be rendered as
        //       optimally as possible, we use a big font size, so that when we
        //       actually need to render the contents (see paintEvent()), we can
        //       do so optimally...

        mMathmlDocument.setBaseFontPointSize(100);

        QSizeF mathmlDocumentSize = mMathmlDocument.size();

        mOneOverMathmlDocumentWidth  = 1.0/mathmlDocumentSize.width();
        mOneOverMathmlDocumentHeight = 1.0/mathmlDocumentSize.height();
    }

    // Update ourselves

    update();
}

//==============================================================================

bool ViewerWidget::error() const
{
    // Return whether there is an error

    return mError;
}

//==============================================================================

void ViewerWidget::setError(const bool &pError)
{
    // Keep track of whether there is an error

    if (mContents.isEmpty() && (pError == mError))
        return;

    mContents = QString();
    mError = pError;

    // Update ourselves

    update();
}

//==============================================================================

bool ViewerWidget::optimiseFontSize() const
{
    // Return whether we optimise our font size

    return mOptimiseFontSizeAction->isChecked();
}

//==============================================================================

void ViewerWidget::setOptimiseFontSize(const bool &pOptimiseFontSize)
{
    // Keep track of whether we should optimise our font size

    if (pOptimiseFontSize == optimiseFontSize())
        return;

    mOptimiseFontSizeAction->setChecked(pOptimiseFontSize);
}

//==============================================================================

bool ViewerWidget::subscripts() const
{
    // Return whether we use subscripts

    return mSubscriptsAction->isChecked();
}

//==============================================================================

void ViewerWidget::setSubscripts(const bool &pSubscripts)
{
    // Keep track of whether we use subscripts

    if (pSubscripts == subscripts())
        return;

    mSubscriptsAction->setChecked(pSubscripts);
}

//==============================================================================

bool ViewerWidget::greekSymbols() const
{
    // Return whether we use Greek symbols

    return mGreekSymbolsAction->isChecked();
}

//==============================================================================

void ViewerWidget::setGreekSymbols(const bool &pGreekSymbols)
{
    // Keep track of whether we use Greek symbols

    if (pGreekSymbols == greekSymbols())
        return;

    mGreekSymbolsAction->setChecked(pGreekSymbols);
}

//==============================================================================

bool ViewerWidget::digitGrouping() const
{
    // Return whether we do digit grouping

    return mDigitGroupingAction->isChecked();
}

//==============================================================================

void ViewerWidget::setDigitGrouping(const bool &pDigitGrouping)
{
    // Keep track of whether we do digit grouping

    if (pDigitGrouping == digitGrouping())
        return;

    mDigitGroupingAction->setChecked(pDigitGrouping);
}

//==============================================================================

void ViewerWidget::paintEvent(QPaintEvent *pEvent)
{
    // Make sure that we have a width and height

    QRectF rect = pEvent->rect();

    if (!rect.width() || !rect.height())
        return;

    // Clear our background

    QPainter painter(this);
    QColor backgroundColor = QColor(palette().color(QPalette::Base));

    painter.fillRect(rect, backgroundColor);

    // Render our contents or show a warning sign, depending on whether our
    // contents is valid

    if (mError) {
        // We want to show that there is an error, so render a stretched warning
        // icon in our center

        QIcon icon = QIcon(":/oxygen/status/task-attention.png");
        QSize iconSize = icon.availableSizes().first();

        int painterRectWidth = iconSize.width();
        int painterRectHeight = iconSize.height();

        if (rect.width() < rect.height())
            painterRectHeight *= rect.height()/rect.width();
        else
            painterRectWidth *= rect.width()/rect.height();

        if (painterRectHeight % 2)
            ++painterRectHeight;

        if (painterRectWidth % 2)
            ++painterRectWidth;

        QRect painterRect = QRect(0, 0, painterRectWidth, painterRectHeight);
        // Note: both painterRectWidth and painterRectHeight have even values.
        //       This ensures that when we get resized, then the warning icon
        //       remains nicely centered (while it would otherwise shifts to the
        //       left/right or top/bottom, depending on our width or height,
        //       respectively...

        painter.setWindow(painterRect);

        icon.paint(&painter, painterRect);
    } else {
        // Customise our MathML document
        // Note: to go for 100% of the 'optimal' font size might result in the
        //       edges of the contents being clipped on Windows (compared to
        //       Linux and OS X) or in some cases on Linux and OS X (e.g. if the
        //       contents includes a square root), hence we go for 75% of the
        //       'optimal' font size instead...

        mMathmlDocument.setBackgroundColor(backgroundColor);
        mMathmlDocument.setForegroundColor(QColor(palette().color(QPalette::Text)));

        mMathmlDocument.setBaseFontPointSize(optimiseFontSize()?
                                                 qRound(75.0*qMin(mOneOverMathmlDocumentWidth*width(),
                                                                  mOneOverMathmlDocumentHeight*height())):
                                                 font().pointSize());

        // Render our contents

        QSizeF mathmlDocumentSize = mMathmlDocument.size();

        mMathmlDocument.paint(&painter, QPointF(0.5*(rect.width()-mathmlDocumentSize.width()),
                                                0.5*(rect.height()-mathmlDocumentSize.height())));
    }

    // Enable/disable our copy to clipboard action

    mCopyToClipboardAction->setEnabled(!mContents.isEmpty() && !mError);

    // Accept the event

    pEvent->accept();
}

//==============================================================================

QSize ViewerWidget::minimumSizeHint() const
{
    // Suggest a default minimum size for our viewer widget

    return defaultSize(0.03);
}

//==============================================================================

QSize ViewerWidget::sizeHint() const
{
    // Suggest a default size for our viewer widget
    // Note: this is critical if we want a docked widget, with a viewer widget
    //       on it, to have a decent size when docked to the main window...

    return defaultSize(0.1);
}

//==============================================================================

QAction * ViewerWidget::newAction()
{
    // Create and return a checkable and checked action

    QAction *res = Core::newAction(true, this);

    res->setChecked(true);

    return res;
}

//==============================================================================

QString ViewerWidget::greekSymbolize(const QString &pValue) const
{
    // Convert the given value into a Greek symbol, if possible

    return GreekSymbols.value(pValue.toLower(), pValue);
}

//==============================================================================

QDomNode ViewerWidget::newMiNode(const QDomNode &pDomNode,
                                 const QString &pValue) const
{
    // Create and return an mi element with a text child node, which value if
    // the one given

    QDomElement res = pDomNode.ownerDocument().createElement("mi");

    res.setAttribute("mathvariant", "italic");

    res.appendChild(pDomNode.ownerDocument().createTextNode(greekSymbols()?greekSymbolize(pValue):pValue));

    return res;
}

//==============================================================================

void ViewerWidget::processNode(const QDomNode &pDomNode) const
{
    // Go through the node's children and process them

    for (int i = 0, iMax = pDomNode.childNodes().count(); i < iMax; ++i) {
        bool processDomNode = true;
        QDomNode domNode = pDomNode.childNodes().at(i);

        // Check whether the current node has only one child of type text

        if (   (domNode.childNodes().count() == 1)
            && (domNode.firstChild().nodeType() == QDomNode::TextNode)) {
            // Check whether we want to use subscripts and/or Greek symbols and
            // the current node is an mi element, or whether we want to do digit
            // grouping and the current node is an mn element

            if (    (subscripts() || greekSymbols())
                && !domNode.nodeName().compare("mi")) {
                // We want to use subscripts and/or Greek symbols and the
                // current node is an mi element, so check whether we want to
                // use subscripts

                QString domChildNodeValue = domNode.firstChild().nodeValue();

                if (subscripts()) {
                    // We want to use subscripts (and maybe also Greek symbols),
                    // so remove leading, trailing and duplicate underscores

                    domChildNodeValue.remove(QRegularExpression("^_+"));
                    domChildNodeValue.remove(QRegularExpression("_+$"));
                    domChildNodeValue.replace(QRegularExpression("_+"), "_");

                    // Split the value of the child node using the underscore as
                    // a separator

                    QStringList domChildNodeSubValues = domChildNodeValue.split("_");

                    // Create a new node that is going to contain the subscript
                    // version of our current node
                    // Note: our calls to newMiNode() will handle the case where
                    //       we want to use Greek symbols...

                    int domChildNodeSubValuesCount = domChildNodeSubValues.count();

                    if (domChildNodeSubValuesCount >= 2) {
                        QDomNode newDomNode = domNode.ownerDocument().createElement("msub");

                        newDomNode.appendChild(newMiNode(domNode, domChildNodeSubValues[domChildNodeSubValuesCount-2]));
                        newDomNode.appendChild(newMiNode(domNode, domChildNodeSubValues[domChildNodeSubValuesCount-1]));

                        if (domChildNodeSubValuesCount > 2)
                            for (int j = domChildNodeSubValuesCount-3; j >= 0; --j) {
                                QDomNode newerDomNode = domNode.ownerDocument().createElement("msub");

                                newerDomNode.appendChild(newMiNode(domNode, domChildNodeSubValues[j]));
                                newerDomNode.appendChild(newDomNode);

                                newDomNode = newerDomNode;
                            }

                        // Replace the current node with our new one

                        domNode.parentNode().replaceChild(newDomNode, domNode);
                    }
                } else if (greekSymbols()) {
                    // We want to use Greek symbols, so go through the value of
                    // the child node (from the end) and replace whatever can be
                    // replaced with Greek symbols
                    // Note: we start from the end because it makes the
                    //       algorithm (since replacing a string of character
                    //       with a Greek symbol will shorten the value of our
                    //       child node)...

                    int fromPos = domChildNodeValue.size();
                    int startPos;
                    int endPos;
                    QString domChildNodeSubValue;

                    while (fromPos != -1) {
                        endPos = domChildNodeValue.lastIndexOf(QRegularExpression("[^_]"), fromPos);

                        if (endPos != -1) {
                            startPos = domChildNodeValue.lastIndexOf(QRegularExpression("_"), endPos);

                            domChildNodeSubValue = domChildNodeValue.mid(startPos+1, endPos-startPos);

                            domChildNodeValue.replace(startPos+1, endPos-startPos, greekSymbolize(domChildNodeSubValue));

                            fromPos = startPos;
                        } else {
                            fromPos = -1;
                        }
                    }

                    domNode.firstChild().setNodeValue(domChildNodeValue);
                }

                processDomNode = false;
            } else if (    digitGrouping()
                       && !domNode.nodeName().compare("mn")) {
                // We want to do digit grouping and the current node is an mn
                // element, so we can go ahead

                domNode.firstChild().setNodeValue(Core::digitGroupNumber(domNode.firstChild().nodeValue()));

                processDomNode = false;
            }
        }

        // Process the current node itself, if needed

        if (processDomNode)
            processNode(domNode);
    }
}

//==============================================================================

QString ViewerWidget::processedContents() const
{
    // Process and return our processed contents

    if (mContents.isEmpty())
        return QString();

    QDomDocument domDocument;

    if (domDocument.setContent(mContents)) {
        QDomNode domNode = domDocument.documentElement();

        processNode(domNode);

        return domDocument.toString(-1);
    } else {
        // We should never reach this point (since we should only come here if
        // our contents is valid), but better be safe than sorry...

        return QString();
    }
}

//==============================================================================

void ViewerWidget::showCustomContextMenu(const QPoint &pPosition) const
{
    Q_UNUSED(pPosition);

    // Show our custom context menu

    mContextMenu->exec(QCursor::pos());
}

//==============================================================================

void ViewerWidget::updateViewer()
{
    // Update our viewer by 'updating' our contents

    QString contents = mContents;

    mContents = QString();

    setContents(contents);
}

//==============================================================================

void ViewerWidget::copyToClipboard()
{
    // Copy our contents to the clipboard

    QSizeF mathmlDocumentSize = mMathmlDocument.size();

    QApplication::clipboard()->setPixmap(grab().copy(qFloor(0.5*(width()-mathmlDocumentSize.width())),
                                                     qFloor(0.5*(height()-mathmlDocumentSize.height())),
                                                     qCeil(mathmlDocumentSize.width()),
                                                     qCeil(mathmlDocumentSize.height())));
}

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
