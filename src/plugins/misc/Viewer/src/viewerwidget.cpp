//==============================================================================
// Viewer widget
//==============================================================================

#include "viewerwidget.h"

//==============================================================================

#include <QPainter>
#include <QPaintEvent>

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

    QSize mathmlDocumentSize = mMathmlDocument.size();

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

    painter.fillRect(pEvent->rect(), QColor(palette().color(QPalette::Base)));

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

    QSize mathmlDocumentSize = mMathmlDocument.size();

    mMathmlDocument.paint(&painter, QPoint(0.5*(width()-mathmlDocumentSize.width()),
                                           0.5*(height()-mathmlDocumentSize.height())));

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
