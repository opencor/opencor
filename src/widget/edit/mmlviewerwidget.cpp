#include "mmlviewerwidget.h"

#include <QPaintEvent>

MmlViewerWidget::MmlViewerWidget(QWidget *pParent) :
    QtMmlWidget(pParent),
    CommonWidget(pParent)
{
    // Create a test MathML widget and set its base font point size to 100, so
    // that we can use that as a benchmark for what the 'ideal' dimensions of
    // the MathML widget should be (see resizeEvent below)

    mTestMmlWidget = new QtMmlWidget;

    mTestMmlWidget->setBaseFontPointSize(100);

    // Set the background to white

    QPalette pal = palette();

    pal.setColor(QPalette::Window, Qt::white);

    setPalette(pal);
    setAutoFillBackground(true);

    // Set a font that we know works on Windows, Linux and Mac OS X

    setFontName(NormalFont, "Times New Roman");
}

MmlViewerWidget::~MmlViewerWidget()
{
    delete mTestMmlWidget;
}

bool MmlViewerWidget::setContent(const QString &pContent, QString *pErrorMsg,
                                 int *pErrorLine, int *pErrorColumn)
{
    // Set the MathML equation

    mTestMmlWidget->setContent(pContent);

    return QtMmlWidget::setContent(pContent, pErrorMsg, pErrorLine, pErrorColumn);
}

QSize MmlViewerWidget::sizeHint() const
{
    // Suggest a default size for the file browser widget
    // Note: this is critical if we want a docked widget, with a file browser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.1);
}

void MmlViewerWidget::paintEvent(QPaintEvent *pEvent)
{
    // Default handling of the event

    QtMmlWidget::paintEvent(pEvent);

    // Draw a border in case we are docked

    drawBorderIfDocked();

    // Accept the event

    pEvent->accept();
}

void MmlViewerWidget::resizeEvent(QResizeEvent *pEvent)
{
    // Default handling of the event

    QtMmlWidget::resizeEvent(pEvent);

    // Retrieve the 'optimal' dimensions of the MathML equation (which was
    // rendered using our mTestMmlWidget object)
    // Note: to skip the mTestMmlWidget object (to save a bit of memory) and use
    //       the current object to compute the 'optimal' dimensions is not good
    //       enough. For having tried it, this worked fine when resizing the
    //       object vertically (the equation would resize accordingly), but not
    //       horizontally (the equation would just not resize), so...

    QSize testMmlWidgetSize = mTestMmlWidget->sizeHint();

    setBaseFontPointSize(round(93*fmin((double) width()/testMmlWidgetSize.width(),
                                       (double) height()/testMmlWidgetSize.height())));
    // Note: to go for 100% of the 'optimal' size may result in the edges of
    //       the equation being clipped, hence we go for 93% of the 'optimal'
    //       size...

    // Accept the event

    pEvent->accept();
}
