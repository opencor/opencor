#include "commandviewerwidget.h"

CommandViewerWidget::CommandViewerWidget(QWidget *pParent) :
    QtMmlWidget(pParent)
{
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

void CommandViewerWidget::retranslateUi()
{
    // Nothing to do for now...
}

void CommandViewerWidget::defaultSettings()
{
    // Nothing to do for now...
}

void CommandViewerWidget::loadSettings(const QSettings &, const QString &)
{
    // Nothing to do for now...
}

void CommandViewerWidget::saveSettings(QSettings &, const QString &)
{
    // Nothing to do for now...
}

QSize CommandViewerWidget::sizeHint() const
{
    // Suggest a default size for the file browser widget
    // Note: this is critical if we want a docked widget, with a file browser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.1);
}
