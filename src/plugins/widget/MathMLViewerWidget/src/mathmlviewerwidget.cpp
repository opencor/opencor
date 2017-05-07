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
// MathML Viewer widget
//==============================================================================

#include "corecliutils.h"
#include "coreguiutils.h"
#include "i18ninterface.h"
#include "mathmlviewerwidget.h"

//==============================================================================

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
namespace MathMLViewerWidget {

//==============================================================================

static QMap<QString, QString> GreekSymbols;

//==============================================================================

MathmlViewerWidget::MathmlViewerWidget(QWidget *pParent) :
    Widget(pParent),
    mMathmlDocument(QwtMathMLDocument()),
    mOneOverMathmlDocumentWidth(0),
    mOneOverMathmlDocumentHeight(0),
    mContents(QString()),
    mError(false)
{
    // Populate our table of Greek symbols, if needed

    if (GreekSymbols.isEmpty()) {
        // Upper case

        GreekSymbols.insert("ALPHA", "Α");
        GreekSymbols.insert("BETA", "Β");
        GreekSymbols.insert("GAMMA", "Γ");
        GreekSymbols.insert("DELTA", "Δ");
        GreekSymbols.insert("EPSILON", "Ε");
        GreekSymbols.insert("ZETA", "Ζ");
        GreekSymbols.insert("ETA", "Η");
        GreekSymbols.insert("THETA", "Θ");
        GreekSymbols.insert("IOTA", "Ι");
        GreekSymbols.insert("KAPPA", "Κ");
        GreekSymbols.insert("LAMBDA", "Λ");
        GreekSymbols.insert("MU", "Μ");
        GreekSymbols.insert("NU", "Ν");
        GreekSymbols.insert("XI", "Ξ");
        GreekSymbols.insert("OMICRON", "Ο");
        GreekSymbols.insert("PI", "Π");
        GreekSymbols.insert("RHO", "Ρ");
        GreekSymbols.insert("SIGMA", "Σ");
        GreekSymbols.insert("TAU", "Τ");
        GreekSymbols.insert("UPSILON", "Υ");
        GreekSymbols.insert("PHI", "Φ");
        GreekSymbols.insert("CHI", "Χ");
        GreekSymbols.insert("PSI", "Ψ");
        GreekSymbols.insert("OMEGA", "Ω");

        // Lower case

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
    mCopyToClipboardAction = Core::newAction(this);

    connect(mOptimiseFontSizeAction, SIGNAL(toggled(bool)),
            this, SLOT(update()));

    connect(mSubscriptsAction, SIGNAL(toggled(bool)),
            this, SLOT(updateMathmlViewerWidget()));
    connect(mGreekSymbolsAction, SIGNAL(toggled(bool)),
            this, SLOT(updateMathmlViewerWidget()));
    connect(mDigitGroupingAction, SIGNAL(toggled(bool)),
            this, SLOT(updateMathmlViewerWidget()));

    connect(mCopyToClipboardAction, SIGNAL(triggered(bool)),
            this, SLOT(copyToClipboard()));

    mContextMenu->addAction(mOptimiseFontSizeAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mSubscriptsAction);
    mContextMenu->addAction(mGreekSymbolsAction);
    mContextMenu->addAction(mDigitGroupingAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mCopyToClipboardAction);

    // Retranslate ourselves, so that our actions are properly initialised

    retranslateUi();
}

//==============================================================================

static const auto SettingsMathmlViewerWidgetOptimiseFontSizeEnabled = QStringLiteral("MathmlViewerWidgetOptimiseFontSizeEnabled");
static const auto SettingsMathmlViewerWidgetSubscriptsEnabled       = QStringLiteral("MathmlViewerWidgetSubscriptsEnabled");
static const auto SettingsMathmlViewerWidgetGreekSymbolsEnabled     = QStringLiteral("MathmlViewerWidgetGreekSymbolsEnabled");
static const auto SettingsMathmlViewerWidgetDigitGroupingEnabled    = QStringLiteral("MathmlViewerWidgetDigitGroupingEnabled");

//==============================================================================

void MathmlViewerWidget::loadSettings(QSettings *pSettings)
{
    // Retrieve our settings

    setOptimiseFontSize(pSettings->value(SettingsMathmlViewerWidgetOptimiseFontSizeEnabled, true).toBool());
    setSubscripts(pSettings->value(SettingsMathmlViewerWidgetSubscriptsEnabled, true).toBool());
    setGreekSymbols(pSettings->value(SettingsMathmlViewerWidgetGreekSymbolsEnabled, true).toBool());
    setDigitGrouping(pSettings->value(SettingsMathmlViewerWidgetDigitGroupingEnabled, true).toBool());
}

//==============================================================================

void MathmlViewerWidget::saveSettings(QSettings *pSettings) const
{
    // Keep track of our settings

    pSettings->setValue(SettingsMathmlViewerWidgetOptimiseFontSizeEnabled, optimiseFontSize());
    pSettings->setValue(SettingsMathmlViewerWidgetSubscriptsEnabled, subscripts());
    pSettings->setValue(SettingsMathmlViewerWidgetGreekSymbolsEnabled, greekSymbols());
    pSettings->setValue(SettingsMathmlViewerWidgetDigitGroupingEnabled, digitGrouping());
}

//==============================================================================

void MathmlViewerWidget::retranslateUi()
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
    I18nInterface::retranslateAction(mCopyToClipboardAction, tr("Copy to Clipboard"),
                                     tr("Copy the contents of the MathML viewer to the clipboard"));

    // Retranslate our contents
    // Note: we do this because we may be displaying numbers using digit
    //       grouping, this respecting the current locale...

    setContents(mContents);
}

//==============================================================================

void MathmlViewerWidget::updateSettings(MathmlViewerWidget *pMathmlViewerWidget)
{
    // Make sure that we are given another widget

    if (!pMathmlViewerWidget)
        return;

    // Update our MathML viewer settings

    setOptimiseFontSize(pMathmlViewerWidget->optimiseFontSize());
    setSubscripts(pMathmlViewerWidget->subscripts());
    setGreekSymbols(pMathmlViewerWidget->greekSymbols());
    setDigitGrouping(pMathmlViewerWidget->digitGrouping());
}

//==============================================================================

QString MathmlViewerWidget::contents() const
{
    // Return our contents

    return mContents;
}

//==============================================================================

void MathmlViewerWidget::setContents(const QString &pContents)
{
    // Make sure that we are not trying to set the same contents

    if (   !pContents.compare(mContents)
        &&  ((pContents.isEmpty() && !mError) || !pContents.isEmpty())) {
        return;
    }

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
        else if (pContents.isEmpty())
            mError = !mMathmlDocument.setContent(QString());
        else
            mError = true;
    }

    if (mError) {
        // An error occurred, but consider it only as an actual error if our
        // contents is not empty

        mError = !pContents.isEmpty();
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

        mOneOverMathmlDocumentWidth = 1.0/mathmlDocumentSize.width();
        mOneOverMathmlDocumentHeight = 1.0/mathmlDocumentSize.height();
    }

    // Update ourselves

    update();
}

//==============================================================================

bool MathmlViewerWidget::error() const
{
    // Return whether there is an error

    return mError;
}

//==============================================================================

void MathmlViewerWidget::setError(const bool &pError)
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

bool MathmlViewerWidget::optimiseFontSize() const
{
    // Return whether we optimise our font size

    return mOptimiseFontSizeAction->isChecked();
}

//==============================================================================

void MathmlViewerWidget::setOptimiseFontSize(const bool &pOptimiseFontSize)
{
    // Keep track of whether we should optimise our font size

    if (pOptimiseFontSize == optimiseFontSize())
        return;

    mOptimiseFontSizeAction->setChecked(pOptimiseFontSize);
}

//==============================================================================

bool MathmlViewerWidget::subscripts() const
{
    // Return whether we use subscripts

    return mSubscriptsAction->isChecked();
}

//==============================================================================

void MathmlViewerWidget::setSubscripts(const bool &pSubscripts)
{
    // Keep track of whether we use subscripts

    if (pSubscripts == subscripts())
        return;

    mSubscriptsAction->setChecked(pSubscripts);
}

//==============================================================================

bool MathmlViewerWidget::greekSymbols() const
{
    // Return whether we use Greek symbols

    return mGreekSymbolsAction->isChecked();
}

//==============================================================================

void MathmlViewerWidget::setGreekSymbols(const bool &pGreekSymbols)
{
    // Keep track of whether we use Greek symbols

    if (pGreekSymbols == greekSymbols())
        return;

    mGreekSymbolsAction->setChecked(pGreekSymbols);
}

//==============================================================================

bool MathmlViewerWidget::digitGrouping() const
{
    // Return whether we do digit grouping

    return mDigitGroupingAction->isChecked();
}

//==============================================================================

void MathmlViewerWidget::setDigitGrouping(const bool &pDigitGrouping)
{
    // Keep track of whether we do digit grouping

    if (pDigitGrouping == digitGrouping())
        return;

    mDigitGroupingAction->setChecked(pDigitGrouping);
}

//==============================================================================

void MathmlViewerWidget::contextMenuEvent(QContextMenuEvent *pEvent)
{
    // Show our custom context menu

    mContextMenu->exec(pEvent->globalPos());
}

//==============================================================================

void MathmlViewerWidget::paintEvent(QPaintEvent *pEvent)
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

        static const QIcon WarningIcon = QIcon(":/oxygen/status/task-attention.png");

        static const int WarningIconWidth  = WarningIcon.availableSizes().first().width();
        static const int WarningIconHeight = WarningIcon.availableSizes().first().height();

        int painterRectWidth = WarningIconWidth;
        int painterRectHeight = WarningIconHeight;

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

        WarningIcon.paint(&painter, painterRect);
    } else {
        // Customise our MathML document
        // Note: to go for 100% of the 'optimal' font size might result in the
        //       edges of the contents being clipped on Windows (compared to
        //       Linux and macOS) or in some cases on Linux and macOS (e.g. if
        //       the contents includes a square root), hence we go for 75% of
        //       the 'optimal' font size instead...

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

QAction * MathmlViewerWidget::newAction()
{
    // Create and return a checkable and checked action

    QAction *res = Core::newAction(true, this);

    res->setChecked(true);

    return res;
}

//==============================================================================

QString MathmlViewerWidget::greekSymbolize(const QString &pValue) const
{
    // Convert the given value into a Greek symbol, if possible

    return GreekSymbols.value(pValue, pValue);
}

//==============================================================================

QDomElement MathmlViewerWidget::newMiNode(const QDomNode &pDomNode,
                                          const QString &pValue) const
{
    // Create and return an mi element with a text child node, which value is
    // the one given

    QDomElement res = pDomNode.ownerDocument().createElement("mi");

    res.setAttribute("mathvariant", "italic");

    res.appendChild(pDomNode.ownerDocument().createTextNode(greekSymbols()?greekSymbolize(pValue):pValue));

    return res;
}

//==============================================================================

void MathmlViewerWidget::processNode(const QDomNode &pDomNode) const
{
    // Go through the node's children and process them

    static const QString MiElement = "mi";
    static const QString MnElement = "mn";
    static const QString MoElement = "mo";

    static const QRegularExpression LeadingAndTrailingUnderscoresRegEx = QRegularExpression("(^_+|_+$)");
    static const QRegularExpression MultipleUnderscoresRegEx = QRegularExpression("_+");
    static const QRegularExpression NotUnderscoreRegEx = QRegularExpression("[^_]");

    bool processSubscripts = subscripts();
    bool processGreekSymbols = greekSymbols();
    bool processDigitGrouping = digitGrouping();

    for (QDomNode domNode = pDomNode.firstChild();
         !domNode.isNull(); domNode = domNode.nextSibling()) {
        bool processDomNode = true;

        // Check whether the current node has only one child of type text

        QDomNode childNode = domNode.firstChild();

        if ((domNode.childNodes().count() == 1) && (childNode.isText())) {
            // Check whether we want to use subscripts and/or Greek symbols and
            // the current node is an mi element, whether we want to do digit
            // grouping and the current node is an mn element, or whether we
            // are dealing with an mo element

            if (    (processSubscripts || processGreekSymbols)
                && !domNode.nodeName().compare(MiElement)) {
                // We want to use subscripts and/or Greek symbols and the
                // current node is an mi element, so check whether we want to
                // use subscripts

                QString childNodeValue = childNode.nodeValue();

                if (processSubscripts) {
                    // We want to use subscripts (and maybe also Greek symbols),
                    // so remove leading, trailing and duplicate underscores

                    childNodeValue.remove(LeadingAndTrailingUnderscoresRegEx);
                    childNodeValue.replace(MultipleUnderscoresRegEx, "_");

                    // Split the value of the child node using the underscore as
                    // a separator

                    QStringList domChildNodeSubValues = childNodeValue.split('_');

                    // Create a new node that is going to contain the subscript
                    // version of our current node
                    // Note: our calls to newMiNode() will handle the case where
                    //       we want to use Greek symbols...

                    int domChildNodeSubValuesCount = domChildNodeSubValues.count();

                    if (domChildNodeSubValuesCount >= 2) {
                        QDomElement newDomElement = domNode.ownerDocument().createElement("msub");

                        newDomElement.appendChild(newMiNode(domNode, domChildNodeSubValues[domChildNodeSubValuesCount-2]));
                        newDomElement.appendChild(newMiNode(domNode, domChildNodeSubValues[domChildNodeSubValuesCount-1]));

                        if (domChildNodeSubValuesCount > 2) {
                            for (int j = domChildNodeSubValuesCount-3; j >= 0; --j) {
                                QDomElement newerDomElement = domNode.ownerDocument().createElement("msub");

                                newerDomElement.appendChild(newMiNode(domNode, domChildNodeSubValues[j]));
                                newerDomElement.appendChild(newDomElement);

                                newDomElement = newerDomElement;
                            }
                        }

                        // Replace the current node with our new one

                        domNode.parentNode().replaceChild(newDomElement, domNode);

                        domNode = newDomElement;
                    } else if (processGreekSymbols) {
                        // There are no subscripts to be processed, but we want
                        // to use Greek symbols, so try to Greek symbolise our
                        // child node value

                        childNode.setNodeValue(greekSymbolize(childNodeValue));
                    }
                } else if (processGreekSymbols) {
                    // We want to use Greek symbols, so go through the value of
                    // the child node (from the end) and replace whatever can be
                    // replaced with Greek symbols
                    // Note: we start from the end because it makes the
                    //       algorithm faster (since replacing a string of
                    //       character with a Greek symbol will shorten the
                    //       value of our child node)...

                    int fromPos = childNodeValue.size();
                    int startPos;
                    int endPos;
                    QString domChildNodeSubValue;

                    while (fromPos != -1) {
                        endPos = childNodeValue.lastIndexOf(NotUnderscoreRegEx, fromPos);

                        if (endPos != -1) {
                            startPos = childNodeValue.lastIndexOf('_', endPos);

                            domChildNodeSubValue = childNodeValue.mid(startPos+1, endPos-startPos);

                            childNodeValue.replace(startPos+1, endPos-startPos, greekSymbolize(domChildNodeSubValue));

                            fromPos = startPos;
                        } else {
                            fromPos = -1;
                        }
                    }

                    childNode.setNodeValue(childNodeValue);
                }

                processDomNode = false;
            } else if (    processDigitGrouping
                       && !domNode.nodeName().compare(MnElement)) {
                // We want to do digit grouping and the current node is an mn
                // element, so we can go ahead

                childNode.setNodeValue(Core::digitGroupNumber(childNode.nodeValue()));

                processDomNode = false;
            } else if (!domNode.nodeName().compare(MoElement)) {
                // The current node is an mo element, so no need to process it
                // further

                processDomNode = false;
            }
        }

        // Process the current node itself, if needed

        if (processDomNode)
            processNode(domNode);
    }
}

//==============================================================================

QString MathmlViewerWidget::processedContents() const
{
    // Process and return our processed contents

    if (mContents.isEmpty())
        return QString();

    QDomDocument domDocument;

    if (domDocument.setContent(mContents)) {
        processNode(domDocument.documentElement());

        return domDocument.toString(-1);
    } else {
        // We should never reach this point (since we should only come here if
        // our contents is valid), but better be safe than sorry...

        return QString();
    }
}

//==============================================================================

void MathmlViewerWidget::updateMathmlViewerWidget()
{
    // Update our MathML viewer by 'updating' our contents

    QString contents = mContents;

    mContents = QString();

    setContents(contents);
}

//==============================================================================

void MathmlViewerWidget::copyToClipboard()
{
    // Copy our contents to the clipboard

    QSizeF mathmlDocumentSize = mMathmlDocument.size();
    int contentsWidth = qCeil(mathmlDocumentSize.width());
    int contentsHeight = qCeil(mathmlDocumentSize.height());
    QPixmap pixmap(contentsWidth, contentsHeight);

    render(&pixmap, QPoint(), QRegion(0, 0, contentsWidth, contentsHeight));

    QApplication::clipboard()->setPixmap(pixmap);
}

//==============================================================================

}   // namespace MathMLViewerWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
