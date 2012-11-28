//==============================================================================
// Viewer widget
//==============================================================================

#include "viewerwidget.h"

//==============================================================================

#include <QPainter>
#include <QPaintEvent>

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

ViewerWidget::ViewerWidget(QWidget *pParent) :
    Widget(pParent),
    mMmlDocument(QtMmlDocument()),
    mOneOverMmlDocumentWidth(0),
    mOneOverMmlDocumentHeight(0),
    mContent(QString())
{
    // Initialise the font of our MathML document

    mMmlDocument.setFontName(QtMmlWidget::NormalFont, "Times New Roman");

//---GRY--- THE BELOW CODE IS JUST FOR TESTING THE WIDGET...
setContent("<math xmlns=\"http://www.w3.org/1998/Math/MathML\"><mrow><msub><mi>i</mi><mi>Na</mi></msub><mo>=</mo><mfrac><mrow><msub><mi>g</mi><mi>Na</mi></msub><mo>&middot;</mo><msup><mi>m</mi><mn>3</mn></msup><mo>&middot;</mo><mi>h</mi><mo>&middot;</mo><msub><mi>Na</mi><mi>o</mi></msub><mo>&middot;</mo><mfrac><msup><mi>F</mi><mn>2</mn></msup><mrow><mi>R</mi><mo>&middot;</mo><mi>T</mi></mrow></mfrac><mo>&middot;</mo><mo>(</mo><mrow><msup><mi>e</mi><mrow><mo>(</mo><mi>V</mi><mo>-</mo><msub><mi>E</mi><mn>Na</mn></msub><mo>)</mo><mo>&middot;</mo><mfrac><mrow><mi>F</mi></mrow><mrow><mi>R</mi><mo>&middot;</mo><mi>T</mi></mrow></mfrac></mrow></msup><mo>-</mo><mn>1</mn></mrow><mo>)</mo></mrow><mrow><msup><mi>e</mi><mrow><mi>V</mi><mo>&middot;</mo><mfrac><mrow><mi>F</mi></mrow><mrow><mi>R</mi><mo>&middot;</mo><mi>T</mi></mrow></mfrac></mrow></msup><mo>-</mo><mn>1</mn></mrow></mfrac><mo>&middot;</mo><mi>V</mi></mrow></math>");
}

//==============================================================================

bool ViewerWidget::setContent(const QString &pContent, QString *pErrorMsg,
                              int *pErrorLine, int *pErrorColumn)
{
    // Keep track of the the MathML equation

    mContent = pContent;

    // Make sure that the base font size is set to 100 pixels and then set the
    // MathML equation
    // Note: when setting the equation, QtMmlDocument recomputes the equation's
    //       layout. Now, because we want the equation to be rendered as
    //       optimally as possible, we use a big font size, so that when we
    //       actually need to render the equation (see paintEvent()), we can
    //       based on the size of the widget use an optimal font size...

    mMmlDocument.setBaseFontPointSize(100);

    bool res = mMmlDocument.setContent(pContent, pErrorMsg, pErrorLine, pErrorColumn);

    // Keep track of the size of the big version of the rendered equation

    QSize mmlDocumentSize = mMmlDocument.size();

    mOneOverMmlDocumentWidth  = 1.0/mmlDocumentSize.width();
    mOneOverMmlDocumentHeight = 1.0/mmlDocumentSize.height();

    // We are all done, so just return the result of setContent()

    return res;
}

//==============================================================================

QSize ViewerWidget::sizeHint() const
{
    // Suggest a default size for the file browser widget
    // Note: this is critical if we want a docked widget, with a file browser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.1);
}

//==============================================================================

void ViewerWidget::paintEvent(QPaintEvent *pEvent)
{
    QPainter painter(this);

    // Set the base font size value
    // Note: to go for 100% of the 'optimal' size might result in the edges of
    //       the equation being clipped, hence we go for 93% of the 'optimal'
    //       size...

    mMmlDocument.setBaseFontPointSize(qRound(93*qMin(mOneOverMmlDocumentWidth*width(),
                                                     mOneOverMmlDocumentHeight*height())));

    // Clear the background

    painter.fillRect(pEvent->rect(), QColor(palette().color(QPalette::Base)));

    // Render the equation

    QSize mmlDocumentSize = mMmlDocument.size();

    mMmlDocument.paint(&painter, QPoint(0.5*(width()-mmlDocumentSize.width()),
                                        0.5*(height()-mmlDocumentSize.height())));

    // Accept the event

    pEvent->accept();
}

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
