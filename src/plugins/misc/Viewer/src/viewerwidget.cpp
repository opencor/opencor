//==============================================================================
// Viewer widget
//==============================================================================

#include "viewerwidget.h"

//==============================================================================

#include <QEvent>

//==============================================================================

#include "qwt_mathml_text_engine.h"
#include "qwt_mml_document.h"

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

ViewerWidget::ViewerWidget(QWidget *pParent) :
    QwtTextLabel(pParent),
    CommonWidget(pParent),
    mPaintEventLevel(0),
    mOptimiseFontSize(true)
{
    // Make sure that MathML support is enabled

    QwtText::setTextEngine(QwtText::MathMLText, new QwtMathMLTextEngine());

    // Customise our background

    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);

//---GRY--- THE BELOW CODE IS JUST FOR TESTING THE WIDGET...
//setContent("<math><mrow><msub><mi>i</mi><mi>Na</mi></msub><mo>=</mo><mfrac><mrow><msub><mi>g</mi><mi>Na</mi></msub><mo>&middot;</mo><msup><mi>m</mi><mn>3</mn></msup><mo>&middot;</mo><mi>h</mi><mo>&middot;</mo><msub><mi>Na</mi><mi>o</mi></msub><mo>&middot;</mo><mfrac><msup><mi>F</mi><mn>2</mn></msup><mrow><mi>R</mi><mo>&middot;</mo><mi>T</mi></mrow></mfrac><mo>&middot;</mo><mo>(</mo><mrow><msup><mi>e</mi><mrow><mo>(</mo><mi>V</mi><mo>-</mo><msub><mi>E</mi><mn>Na</mn></msub><mo>)</mo><mo>&middot;</mo><mfrac><mrow><mi>F</mi></mrow><mrow><mi>R</mi><mo>&middot;</mo><mi>T</mi></mrow></mfrac></mrow></msup><mo>-</mo><mn>1</mn></mrow><mo>)</mo></mrow><mrow><msup><mi>e</mi><mrow><mi>V</mi><mo>&middot;</mo><mfrac><mrow><mi>F</mi></mrow><mrow><mi>R</mi><mo>&middot;</mo><mi>T</mi></mrow></mfrac></mrow></msup><mo>-</mo><mn>1</mn></mrow></mfrac><mo>&middot;</mo><mi>V</mi></mrow></math>");

    // Some presentation MathML that we use as test
    // Note #1: taken from http://www.xmlmind.com/tutorials/MathML/...
    // Note #2: http://www.mathmlcentral.com/Tools/FromMathML.jsp can be used to
    //          test...

    // Basic elements

    setContent("<math><mrow><mrow><mi>x</mi><mo>+</mo><mi>y</mi></mrow><mo>=</mo><mn>2</mn></mrow></math>");

    // Fraction: mfrac

//    setContent("<math><mfrac><mrow><mi>x</mi><mo>-</mo><mn>1</mn></mrow><mn>100</mn></mfrac></math>");

    // Radicals: msqrt and mroot

//    setContent("<math><msqrt><mi>x</mi><mo>+</mo><mi>y</mi></msqrt></math>");
//    setContent("<math><mroot><mi>x</mi><mn>3</mn></mroot></math>");

    // Subscripts and superscripts: msub, msup and msubsup

//    setContent("<math><msub><mi>x</mi><mi>i</mi></msub></math>");
//    setContent("<math><msup><mi>x</mi><mi>j</mi></msup></math>");
//    setContent("<math><msubsup><mi>x</mi><mi>i</mi><mi>j</mi></msubsup></math>");

    // Underscripts and overscripts: munder, mover and munderover

//    setContent("<math><munder><mi>x</mi><mo>&#9472;</mo></munder></math>");
//    setContent("<math><mover><mi>v</mi><mo>&#8594;</mo></mover></math>");
//    setContent("<math><munderover><mi>x</mi><mi>a</mi><mi>b</mi></munderover></math>");

    // The ubiquitous mo element

//    setContent("<math><mrow><munderover><mo>&#8747;</mo><mn>-1</mn><mn>+1</mn></munderover><mfrac><mrow><mi>d</mi><mi>x</mi></mrow><mi>x</mi></mfrac></mrow></math>");
//    setContent("<math><mrow><mi>x</mi><munder><mo>&#8594;</mo><mtext>maps to</mtext></munder><mi>y</mi></mrow></math>");

    // Matrices

//    setContent("<math><mrow><mo>[</mo><mtable><mtr><mtd><mn>1</mn></mtd><mtd><mn>0</mn></mtd><mtd><mn>0</mn></mtd></mtr><mtr><mtd><mn>0</mn></mtd><mtd><mn>1</mn></mtd><mtd><mn>0</mn></mtd></mtr><mtr><mtd><mn>0</mn></mtd><mtd><mn>0</mn></mtd><mtd><mn>1</mn></mtd></mtr></mtable><mo>]</mo></mrow></math>");

    // Some equations

//    setContent("<math><mrow><mo>{</mo><mtable><mtr><mtd><mrow><mrow><mrow><mn>2</mn><mo>&#8290;</mo><mi>x</mi></mrow><mo>+</mo><mi>y</mi></mrow><mo>=</mo><mn>8</mn></mrow></mtd></mtr><mtr><mtd><mrow><mrow><mi>x</mi><mo>+</mo><mi>y</mi></mrow><mo>=</mo><mn>6</mn></mrow></mtd></mtr></mtable></mrow></math>");
//    setContent("<math><mtable side=\"left\"><mlabeledtr><mtd><mtext>Gauss' law</mtext></mtd><mtd><mrow><mrow><mo>&#8711;</mo><mo>&#8729;</mo><mi mathvariant=\"normal\">E</mi></mrow><mo>=</mo><mfrac><mi>&#961;</mi><msub><mi>&#949;</mi><mn>0</mn></msub></mfrac></mrow></mtd></mlabeledtr><mlabeledtr><mtd><mtext>Gauss's law for magnetism</mtext></mtd><mtd><mrow><mrow><mo>&#8711;</mo><mo>&#8729;</mo><mi mathvariant=\"normal\">B</mi></mrow><mo>=</mo><mn>0</mn></mrow></mtd></mlabeledtr></mtable></math>");

//    setContent("<math><mstyle mathbackground=\"yellow\" mathcolor=\"navy\" mathsize=\"16pt\"mathvariant=\"bold\"><mrow><mi>x</mi><mo>+</mo><mi>y</mi></mrow><mo>=</mo><mn mathcolor=\"red\">2</mn></mstyle></math>");

//    setContent("<math><mfrac><mrow><mi> x </mi><mo> + </mo><mi> y </mi><mo> + </mo><mi> z </mi></mrow><mrow><mi> x </mi><mphantom><mo form=\"infix\"> + </mo><mi> y </mi></mphantom><mo> + </mo><mi> z </mi></mrow></mfrac></math>");
}

//==============================================================================

QString ViewerWidget::content() const
{
    // Return our content

    return plainText();
}

//==============================================================================

void ViewerWidget::setContent(const QString &pContent)
{
    // Set our content

    if (!pContent.compare(plainText()))
        return;

    // Keep track of the content

    setText(pContent, QwtText::MathMLText);
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

    repaint();
}

//==============================================================================

bool ViewerWidget::event(QEvent *pEvent)
{
    // In paintEvent(), we may change the font size (in case we need to optimise
    // it), which means that another paint event will eventually get sent. Yet,
    // we don't want to handle those events, so we have mPaintEventLevel which
    // keeps track of our level of painting. mPaintEventLevel is manually
    // increased prior to calling setFont() and decreased here, if necessary
    // (i.e. whenever we receive QEvent::UpdateLater). If mPaintEventLevel is
    // equal to zero, then we just ask QwtTextLabel to process the event (as
    // well as any other event)...

    switch (pEvent->type()) {
    case QEvent::UpdateLater:
        if (mPaintEventLevel) {
            --mPaintEventLevel;

            return true;
        }
    default:
        // Default handling of the event

        return QwtTextLabel::event(pEvent);
    }
}

//==============================================================================

void ViewerWidget::paintEvent(QPaintEvent *pEvent)
{
    // Optimise the font size, if requested, so that the rendered content is as
    // big as possible

    int origFontSize = font().pointSize();

    if (mOptimiseFontSize) {
        // Determine the size the content would take if rendered using a font
        // size of 100 pt

        QwtMathMLDocument mathmlDocument;

        mathmlDocument.setBaseFontPointSize(100);
        mathmlDocument.setContent(plainText());

        QSize textSize = mathmlDocument.size();

        int fontSize = 75.0*qMin(double(width())/textSize.width(),
                                 double(height())/textSize.height());
        // Note: normally, we would multiply the lowest ratio by 100 pt, but we
        //       need to account for the fact that the lowest ratio may not be
        //       100% accurate (since the size of the rendered content is only
        //       approximated by using a font size of 100 pt), hence we give
        //       ourselves a safety net by using 75 pt instead...

        // Update the font, if possible, now that we have found its 'optimal'
        // size

        if (fontSize) {
            QFont newFont = font();

            newFont.setPointSize(fontSize);

            ++mPaintEventLevel;

            setFont(newFont);
        }
    }

    // Default handling of the event

    QwtTextLabel::paintEvent(pEvent);

    // Reset the font size, if needed

    if (origFontSize != font().pointSize()) {
        QFont newFont = font();

        newFont.setPointSize(origFontSize);

        ++mPaintEventLevel;

        setFont(newFont);
    }
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
