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

#include <QPainter>
#include <QPaintEvent>
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
    mOptimiseFontSize(true)
{
    // Customise the family of our font

    QFont newFont = font();

    newFont.setFamily("Times New Roman");

    setFont(newFont);

//---GRY--- THE BELOW CODE IS JUST FOR TESTING THE WIDGET...
setContent("<math><mrow><msub><mi>i</mi><mi>Na</mi></msub><mo>=</mo><mfrac><mrow><msub><mi>g</mi><mi>Na</mi></msub><mo>&middot;</mo><msup><mi>m</mi><mn>3</mn></msup><mo>&middot;</mo><mi>h</mi><mo>&middot;</mo><msub><mi>Na</mi><mi>o</mi></msub><mo>&middot;</mo><mfrac><msup><mi>F</mi><mn>2</mn></msup><mrow><mi>R</mi><mo>&middot;</mo><mi>T</mi></mrow></mfrac><mo>&middot;</mo><mo>(</mo><mrow><msup><mi>e</mi><mrow><mo>(</mo><mi>V</mi><mo>-</mo><msub><mi>E</mi><mn>Na</mn></msub><mo>)</mo><mo>&middot;</mo><mfrac><mrow><mi>F</mi></mrow><mrow><mi>R</mi><mo>&middot;</mo><mi>T</mi></mrow></mfrac></mrow></msup><mo>-</mo><mn>1</mn></mrow><mo>)</mo></mrow><mrow><msup><mi>e</mi><mrow><mi>V</mi><mo>&middot;</mo><mfrac><mrow><mi>F</mi></mrow><mrow><mi>R</mi><mo>&middot;</mo><mi>T</mi></mrow></mfrac></mrow></msup><mo>-</mo><mn>1</mn></mrow></mfrac><mo>&middot;</mo><mi>V</mi></mrow></math>");

// Basic elements

//setContent("<math><mrow><mrow><mi>x</mi><mo>+</mo><mi>y</mi></mrow><mo>=</mo><mn>2</mn></mrow></math>");

// Fraction: mfrac

//setContent("<math><mfrac><mrow><mi>x</mi><mo>-</mo><mn>1</mn></mrow><mn>100</mn></mfrac></math>");
//setContent("<math><mfrac linethickness=\"150\"><mrow><mi>x</mi><mo>-</mo><mn>1</mn></mrow><mn>100</mn></mfrac></math>");
//setContent("<math><mfrac linethickness=\"0.00001\"><mrow><mi>x</mi><mo>-</mo><mn>1</mn></mrow><mn>100</mn></mfrac></math>");
//setContent("<math><mfrac linethickness=\"0\"><mrow><mi>x</mi><mo>-</mo><mn>1</mn></mrow><mn>100</mn></mfrac></math>");

// Radicals: msqrt and mroot

//setContent("<math><msqrt><mi>x</mi><mo>+</mo><mi>y</mi></msqrt></math>");
//setContent("<math><mroot><mi>x</mi><mn>3</mn></mroot></math>");

// Subscripts and superscripts: msub, msup and msubsup

//setContent("<math><msub><mi>x</mi><mi>i</mi></msub></math>");
//setContent("<math><msup><mi>x</mi><mi>j</mi></msup></math>");
//setContent("<math><msubsup><mi>x</mi><mi>i</mi><mi>j</mi></msubsup></math>");

// Underscripts and overscripts: munder, mover and munderover

//setContent("<math><munder><mi>x</mi><mo>&#9472;</mo></munder></math>");
//setContent("<math><mover><mi>v</mi><mo>&#8594;</mo></mover></math>");
//setContent("<math><munderover><mi>x</mi><mi>a</mi><mi>b</mi></munderover></math>");

// The ubiquitous mo element

//setContent("<math><mrow><munderover><mo>&#8747;</mo><mn>-1</mn><mn>+1</mn></munderover><mfrac><mrow><mi>d</mi><mi>x</mi></mrow><mi>x</mi></mfrac></mrow></math>");
//setContent("<math><mrow><mi>x</mi><munder><mo>&#8594;</mo><mtext>maps to</mtext></munder><mi>y</mi></mrow></math>");

// Matrices

//setContent("<math><mrow><mo>[</mo><mtable><mtr><mtd><mn>1</mn></mtd><mtd><mn>0</mn></mtd><mtd><mn>0</mn></mtd></mtr><mtr><mtd><mn>0</mn></mtd><mtd><mn>1</mn></mtd><mtd><mn>0</mn></mtd></mtr><mtr><mtd><mn>0</mn></mtd><mtd><mn>0</mn></mtd><mtd><mn>1</mn></mtd></mtr></mtable><mo>]</mo></mrow></math>");

// Some equations

//setContent("<math><mrow><mo>{</mo><mtable><mtr><mtd><mrow><mrow><mrow><mn>2</mn><mo>&#8290;</mo><mi>x</mi></mrow><mo>+</mo><mi>y</mi></mrow><mo>=</mo><mn>8</mn></mrow></mtd></mtr><mtr><mtd><mrow><mrow><mi>x</mi><mo>+</mo><mi>y</mi></mrow><mo>=</mo><mn>6</mn></mrow></mtd></mtr></mtable></mrow></math>");
//setContent("<math><mtable side=\"left\"><mlabeledtr><mtd><mtext>Gauss' law</mtext></mtd><mtd><mrow><mrow><mo>&#8711;</mo><mo>&#8729;</mo><mi mathvariant=\"normal\">E</mi></mrow><mo>=</mo><mfrac><mi>&#961;</mi><msub><mi>&#949;</mi><mn>0</mn></msub></mfrac></mrow></mtd></mlabeledtr><mlabeledtr><mtd><mtext>Gauss's law for magnetism</mtext></mtd><mtd><mrow><mrow><mo>&#8711;</mo><mo>&#8729;</mo><mi mathvariant=\"normal\">B</mi></mrow><mo>=</mo><mn>0</mn></mrow></mtd></mlabeledtr></mtable></math>");

//setContent("<math><mstyle mathbackground=\"yellow\" mathcolor=\"navy\" mathsize=\"16pt\" mathvariant=\"bold\"><mrow><mi>x</mi><mo>+</mo><mi>y</mi></mrow><mo>=</mo><mn mathcolor=\"red\">2</mn></mstyle></math>");
//setContent("<math><mstyle mathbackground=\"yellow\" mathcolor=\"navy\"><mrow><mi>x</mi><mo>+</mo><mi>y</mi></mrow><mo>=</mo><mn mathcolor=\"red\">2</mn></mstyle></math>");

//setContent("<math><mfrac><mrow><mi> x </mi><mo> + </mo><mi> y </mi><mo> + </mo><mi> z </mi></mrow><mrow><mi> x </mi><mphantom><mo form=\"infix\"> + </mo><mi> y </mi></mphantom><mo> + </mo><mi> z </mi></mrow></mfrac></math>");
}

//==============================================================================

QString ViewerWidget::content() const
{
    // Return our content

    return mContent;
}

//==============================================================================

void ViewerWidget::setContent(const QString &pContent)
{
    // Set our content

    if (!pContent.compare(mContent))
        return;

    // Keep track of our content

    mContent = pContent;

    // Determine (the inverse of) the size of our content when rendered using a
    // font size of 100 points
    // Note: when setting the content, QwtMathMLDocument recomputes its layout.
    //       Now, because we want the content to be rendered as optimally as
    //       possible, we use a big font size, so that when we actually need to
    //       render the content (see paintEvent()), we can do so optimally...

    mMathmlDocument.setBaseFontPointSize(100);
    mMathmlDocument.setContent(mContent);

    QSizeF mathmlDocumentSize = mMathmlDocument.size();

    mOneOverMathmlDocumentWidth  = 1.0/mathmlDocumentSize.width();
    mOneOverMathmlDocumentHeight = 1.0/mathmlDocumentSize.height();
}

//==============================================================================

bool ViewerWidget::optimiseFontSize() const
{
    // Return whether we optimise our font size

    return mOptimiseFontSize;
}

//==============================================================================

void ViewerWidget::setOptimiseFontSize(const bool &pOptimiseFontSize)
{
    // Keep track of whether we should optimise our font size

    if (pOptimiseFontSize == mOptimiseFontSize)
        return;

    mOptimiseFontSize = pOptimiseFontSize;

    // Repaint ourselves

    repaint();
}

//==============================================================================

void ViewerWidget::paintEvent(QPaintEvent *pEvent)
{
    QPainter painter(this);

    // Clear our background

    QRectF rect = pEvent->rect();

    painter.fillRect(rect, QColor(palette().color(QPalette::Base)));

    // Set our base font size and font name
    // Note: to go for 100% of the 'optimal' font size might result in the edges
    //       of the content being clipped on Windows (compared to Linux and OS
    //       X) or in some cases on Linux and OS X (e.g. if the content includes
    //       a square root), hence we go for 75% of the 'optimal' font size
    //       instead...

    mMathmlDocument.setBaseFontPointSize(mOptimiseFontSize?
                                             qRound(75.0*qMin(mOneOverMathmlDocumentWidth*width(),
                                                              mOneOverMathmlDocumentHeight*height())):
                                             font().pointSize());
    mMathmlDocument.setFontName(QwtMathMLDocument::NormalFont, font().family());

    // Render our content

    QSizeF mathmlDocumentSize = mMathmlDocument.size();

    mMathmlDocument.paint(&painter, QPointF(0.5*(rect.width()-mathmlDocumentSize.width()),
                                            0.5*(rect.height()-mathmlDocumentSize.height())));

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

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
