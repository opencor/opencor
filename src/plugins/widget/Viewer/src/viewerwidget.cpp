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

#include "viewerwidget.h"

//==============================================================================

#include <Qt>

//==============================================================================

#include <QAction>
#include <QCursor>
#include <QDomDocument>
#include <QIcon>
#include <QMenu>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QPoint>
#include <QRectF>

//==============================================================================

#include "qwt_mathml_text_engine.h"

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

ViewerWidget::ViewerWidget(QWidget *pParent) :
    Widget(pParent),
    mMathmlDocument(QwtMathMLDocument()),
    mOneOverMathmlDocumentWidth(0),
    mOneOverMathmlDocumentHeight(0),
    mContents(QString()),
    mError(false)
{
    // Create our context menu

    mContextMenu = new QMenu(this);

    mOptimiseFontSizeAction = newAction(this);
    mSubscriptsAction = newAction(this);
    mGreekSymbolsAction = newAction(this);
    mDigitGroupingAction = newAction(this);

    connect(mOptimiseFontSizeAction, SIGNAL(triggered()),
            this, SLOT(update()));
    connect(mOptimiseFontSizeAction, SIGNAL(toggled(bool)),
            this, SIGNAL(optimiseFontSizeChanged(const bool &)));

    connect(mSubscriptsAction, SIGNAL(triggered()),
            this, SLOT(updateContents()));
    connect(mSubscriptsAction, SIGNAL(toggled(bool)),
            this, SIGNAL(subscriptsChanged(const bool &)));

    connect(mGreekSymbolsAction, SIGNAL(triggered()),
            this, SLOT(updateContents()));
    connect(mGreekSymbolsAction, SIGNAL(toggled(bool)),
            this, SIGNAL(greekSymbolsChanged(const bool &)));

    connect(mDigitGroupingAction, SIGNAL(triggered()),
            this, SLOT(updateContents()));
    connect(mDigitGroupingAction, SIGNAL(toggled(bool)),
            this, SIGNAL(digitGroupingChanged(const bool &)));

    mContextMenu->addAction(mOptimiseFontSizeAction);
    mContextMenu->addSeparator();
    mContextMenu->addAction(mSubscriptsAction);
    mContextMenu->addAction(mGreekSymbolsAction);
    mContextMenu->addAction(mDigitGroupingAction);

    // We want a context menu

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(showCustomContextMenu(const QPoint &)));

    // Retranslate ourselves, so that our action is properly set

    retranslateUi();
}

//==============================================================================

void ViewerWidget::retranslateUi()
{
    // Retranslate our actions

    mOptimiseFontSizeAction->setText(tr("Optimise Font Size"));
    mSubscriptsAction->setText(tr("Subscripts"));
    mGreekSymbolsAction->setText(tr("Greek Symbols"));
    mDigitGroupingAction->setText(tr("Digit Grouping"));
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
    // Set our contents

    if (!pContents.compare(mContents))
        return;

    // Try to set our contents to our MathML document

    mContents = pContents;

    if (mMathmlDocument.setContent(pContents)) {
        mError = false;

        // Process and reset our contents, if needed

        if (subscripts() || greekSymbols() || digitGrouping())
            mMathmlDocument.setContent(processedContents());

        // Determine (the inverse of) the size of our contents when rendered
        // using a font size of 100 points
        // Note: when setting the contents, QwtMathMLDocument recomputes its
        //       layout. Now, because we want the contents to be rendered as
        //       optimally as possible, we use a big font size, so that when we
        //       actually need to render the contents (see paintEvent()), we can
        //       do so optimally...

        mMathmlDocument.setBaseFontPointSize(100);

        QSizeF mathmlDocumentSize = mMathmlDocument.size();

        mOneOverMathmlDocumentWidth  = 1.0/mathmlDocumentSize.width();
        mOneOverMathmlDocumentHeight = 1.0/mathmlDocumentSize.height();
    } else {
        mError = pContents.size();
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

    if (pError == mError)
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

    // Let people know about the new value

    emit optimiseFontSizeChanged(pOptimiseFontSize);

    // Update ourselves

    update();
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

    // Let people know about the new value

    emit subscriptsChanged(pSubscripts);

    // Update ourselves

    update();
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

    // Let people know about the new value

    emit greekSymbolsChanged(pGreekSymbols);

    // Update ourselves

    update();
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

    // Let people know about the new value

    emit digitGroupingChanged(pDigitGrouping);

    // Update ourselves

    update();
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

        QIcon icon = QIcon(":Viewer_warning");
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

QAction * ViewerWidget::newAction(QObject *pParent)
{
    // Create and return a checkable and checked action

    QAction *res = new QAction(pParent);

    res->setCheckable(true);
    res->setChecked(true);

    return res;
}

//==============================================================================

void ViewerWidget::processNode(const QDomNode &pDomNode) const
{
    // Go through the node's children and process them

    for (int i = 0, iMax = pDomNode.childNodes().count(); i < iMax; ++i) {
        QDomNode domNode = pDomNode.childNodes().at(i);

        // Check whether we want to use Greek symbols and/or subscripts and
        // whether the node's parent is an mi element

        if (    (subscripts() || greekSymbols())
            && !domNode.parentNode().nodeName().compare("mi")) {
            QString domNodeValue = domNode.nodeValue();
qDebug(">>> domNodeValue: %s", qPrintable(domNodeValue));
        }

        // Check whether we want to do digit grouping and whether the node's
        // parent is an mn element

        if (digitGrouping() && !domNode.parentNode().nodeName().compare("mn")) {
            // We want to do digit grouping and whether the node's parent is an
            // mn element, so check whether the value of that element is a valid
            // number

            bool domNodeValueValid;
            QString domNodeValue = domNode.nodeValue();

            domNodeValue.toDouble(&domNodeValueValid);

            if (domNodeValueValid) {
                // The number is valid, so do digit grouping on it

                int exponentPos = domNodeValue.indexOf("e", 0, Qt::CaseInsensitive);
                int decimalPointPos = domNodeValue.indexOf(".");

                if (   (decimalPointPos != -1)
                    && ((exponentPos == -1) || (decimalPointPos < exponentPos))) {
                    QString beforeDecimalPoint = domNodeValue.left(decimalPointPos);
                    domNodeValue = domNodeValue.right(domNodeValue.length()-decimalPointPos);
                    bool maybeDigit = true;
                    int nbOfDigits = -1;

                    for (int i = beforeDecimalPoint.length()-1; i >= 0; --i) {
                        if (maybeDigit && beforeDecimalPoint[i].isDigit()) {
                            if (++nbOfDigits == 3) {
                                domNodeValue = ","+domNodeValue;

                                nbOfDigits = 0;
                            }
                        } else {
                            maybeDigit = false;
                        }

                        domNodeValue = beforeDecimalPoint[i]+domNodeValue;
                    }

                    domNode.setNodeValue(domNodeValue);
                }
            }
        }

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

void ViewerWidget::updateContents()
{
    // Update our contents and therefore ourselves (through setContents())

    QString contents = mContents;

    mContents = QString();

    setContents(contents);
}

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
