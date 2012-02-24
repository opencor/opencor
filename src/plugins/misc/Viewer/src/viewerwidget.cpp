//==============================================================================
// Viewer widget
//==============================================================================

#include "viewerwidget.h"

//==============================================================================

#include <QPaintEvent>

//==============================================================================

namespace OpenCOR {
namespace Viewer {

//==============================================================================

ViewerWidget::ViewerWidget(const QString &pName, QWidget *pParent) :
    QtMmlWidget(pParent),
    CommonWidget(pName, this, pParent)
{
    // Create a test MathML widget and set its base font point size to 100, so
    // that we can use that as a benchmark for what the 'ideal' dimensions of
    // the MathML widget should be (see resizeEvent below)

    mTestViewerWidget = new QtMmlWidget;

    mTestViewerWidget->setBaseFontPointSize(100);

    // Set the background role in such a way that the background color is always
    // that of the system's base colour

    setBackgroundRole(QPalette::Base);

    // Have the background filled automatically

    setAutoFillBackground(true);

    // Set a font that we know works on Windows, Linux and Mac OS X

    setFontName(NormalFont, "Times New Roman");

//---GRY--- THE BELOW CODE IS JUST FOR TESTING THE COMPONENT...
setContent("<math xmlns=\"http://www.w3.org/1998/Math/MathML\"><mrow><msub><mi>i</mi><mi>Na</mi></msub><mo>=</mo><mfrac><mrow><msub><mi>g</mi><mi>Na</mi></msub><mo>·</mo><msup><mi>m</mi><mn>3</mn></msup><mo>·</mo><mi>h</mi><mo>·</mo><msub><mi>Na</mi><mi>o</mi></msub><mo>·</mo><mfrac><msup><mi>F</mi><mn>2</mn></msup><mrow><mi>R</mi><mo>·</mo><mi>T</mi></mrow></mfrac><mo>·</mo><mo>(</mo><mrow><msup><mi>e</mi><mrow><mo>(</mo><mi>V</mi><mo>-</mo><msub><mi>E</mi><mn>Na</mn></msub><mo>)</mo><mo>·</mo><mfrac><mrow><mi>F</mi></mrow><mrow><mi>R</mi><mo>·</mo><mi>T</mi></mrow></mfrac></mrow></msup><mo>-</mo><mn>1</mn></mrow><mo>)</mo></mrow><mrow><msup><mi>e</mi><mrow><mi>V</mi><mo>·</mo><mfrac><mrow><mi>F</mi></mrow><mrow><mi>R</mi><mo>·</mo><mi>T</mi></mrow></mfrac></mrow></msup><mo>-</mo><mn>1</mn></mrow></mfrac><mo>·</mo><mi>V</mi></mrow></math>");
}

//==============================================================================

ViewerWidget::~ViewerWidget()
{
    delete mTestViewerWidget;
}

//==============================================================================

bool ViewerWidget::setContent(const QString &pContent, QString *pErrorMsg,
                              int *pErrorLine, int *pErrorColumn)
{
    // Set the MathML equation

    mTestViewerWidget->setContent(pContent);

    return QtMmlWidget::setContent(pContent, pErrorMsg,
                                   pErrorLine, pErrorColumn);
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
    // Default handling of the event

    QtMmlWidget::paintEvent(pEvent);

    // Draw a border in case we are docked

    drawBorderIfDocked();
}

//==============================================================================

void ViewerWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    QtMmlWidget::resizeEvent(pEvent);

    // Retrieve the 'optimal' dimensions of the MathML equation (which was
    // rendered using our mTestViewerWidget object)
    // Note: to skip the mTestViewerWidget object (to save a bit of memory) and
    //       use the current object to compute the 'optimal' dimensions is not
    //       good enough. For having tried it, this worked fine when resizing
    //       the object vertically (the equation would resize accordingly), but
    //       not horizontally (the equation would just not resize), so...

    QSize testViewerWidgetSize = mTestViewerWidget->sizeHint();

    setBaseFontPointSize(qRound(93*qMin((double) width()/testViewerWidgetSize.width(),
                                        (double) height()/testViewerWidgetSize.height())));
    // Note: to go for 100% of the 'optimal' size may result in the edges of
    //       the equation being clipped, hence we go for 93% of the 'optimal'
    //       size...
}

//==============================================================================

}   // namespace Viewer
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
