#include "mmlviewerwidget.h"

#include <QPaintEvent>
#include <QResizeEvent>

MmlViewerWidget::MmlViewerWidget(QWidget *pParent) :
    QtMmlWidget(pParent),
    CommonWidget(pParent)
{
    // Create a test MathML widget and set its base font point size to 100, so
    // that we can use that as a benchmark for what the 'ideal' dimensions of
    // the MathML widget should be (see resizeEvent below)

    testMmlWidget = new QtMmlWidget;

    testMmlWidget->setBaseFontPointSize(100);

    // Set the background to white

    QPalette pal = palette();

    pal.setColor(QPalette::Window, Qt::white);

    setPalette(pal);
    setAutoFillBackground(true);

    // Set a font that we know works on Windows, Linux and Mac OS X

    setFontName(NormalFont, "Times New Roman");

//---GRY--- Just for testing...

setContent("<math><mrow><msup><mi>x</mi><mn>2</mn></msup><mo>+</mo><mrow><mn>4</mn><mo></mo><mi>x</mi></mrow><mo>+</mo><mn>4</mn></mrow></math>");
}

MmlViewerWidget::~MmlViewerWidget()
{
    delete testMmlWidget;
}

void MmlViewerWidget::retranslateUi()
{
    // Nothing to do for now...
}

void MmlViewerWidget::defaultSettings()
{
    // Nothing to do for now...
}

void MmlViewerWidget::loadSettings(const QSettings &, const QString &)
{
    // Nothing to do for now...
}

void MmlViewerWidget::saveSettings(QSettings &, const QString &)
{
    // Nothing to do for now...
}

bool MmlViewerWidget::setContent(const QString &pContent, QString *pErrorMsg,
                                 int *pErrorLine, int *pErrorColumn)
{
    // Set the MathML equation

    testMmlWidget->setContent(pContent);

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
    QtMmlWidget::paintEvent(pEvent);

    // Draw a border in case we are docked

    drawBorderIfDocked();

    pEvent->accept();
}

void MmlViewerWidget::resizeEvent(QResizeEvent *pEvent)
{
    QtMmlWidget::resizeEvent(pEvent);

    // Render an optimal version (with regards to dimensions) of the MathML
    // equation

    QSize testMmlWidgetSize = testMmlWidget->sizeHint();

    setBaseFontPointSize(round(93*fmin((double) width()/testMmlWidgetSize.width(),
                                       (double) height()/testMmlWidgetSize.height())));
    // Note: to go for 100% of the 'optimal' size may result in the edges of
    //       the equation being clipped, hence we go for 93% of the 'optimal'
    //       size...

    pEvent->accept();
}
