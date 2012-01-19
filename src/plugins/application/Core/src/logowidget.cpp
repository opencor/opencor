//==============================================================================
// Logo widget
//==============================================================================

#include "logowidget.h"

//==============================================================================

#include "ui_logowidget.h"

//==============================================================================

#include <QPainter>
#include <QPaintEvent>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

LogoWidget::LogoWidget(QWidget *pParent) :
    QWidget(pParent),
    mUi(new Ui::LogoWidget)
{
    // Set up the UI

    mUi->setupUi(this);

    // Logo settings

    static const QString logoResourceName = ":logo";

    mLogo.load(logoResourceName);

    mLogoWidth  = mLogo.width();
    mLogoHeight = mLogo.height();

    // Set the background colour to that of the logo's

    QPalette pal = palette();

    pal.setColor(QPalette::Window, QImage(logoResourceName).pixel(0, 0));

    setPalette(pal);

    // Have the background filled automatically

    setAutoFillBackground(true);
}

//==============================================================================

LogoWidget::~LogoWidget()
{
    // Delete some internal objects

    delete mUi;
}

//==============================================================================

void LogoWidget::paintEvent(QPaintEvent *pEvent)
{
    // Display our logo, in case no file is being managed

    QPainter painter(this);

    // Paint the widget with the logo's background colour

    int widgetWidth  = width();
    int widgetHeight = height();

    // Draw the logo itself

    painter.drawPixmap(QRect(0.5*(widgetWidth-mLogoWidth),
                             0.5*(widgetHeight-mLogoHeight),
                             mLogoWidth, mLogoHeight),
                       mLogo);

#ifndef Q_WS_MAC
    // Draw a border around the widget

    QPen pen = painter.pen();

    pen.setColor(qApp->palette().color(QPalette::Midlight));

    painter.setPen(pen);

    QRect border = rect();

    border.adjust(0, 0, -1, -1);

    painter.drawRect(border);
#endif

    // Accept the event

    pEvent->accept();
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
