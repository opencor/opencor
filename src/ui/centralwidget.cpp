#include "centralwidget.h"

#include "ui_centralwidget.h"

#include <QPainter>
#include <QPaintEvent>

CentralWidget::CentralWidget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent),
    mUi(new Ui::CentralWidget)
{
    // Set up the UI

    mUi->setupUi(this);
}

CentralWidget::~CentralWidget()
{
    // Delete the UI

    delete mUi;
}

void CentralWidget::paintEvent(QPaintEvent *pEvent)
{
    // Display the CellML logo in the center of the widget, as some kind of a
    // background image

    QPixmap pixmap;

    pixmap.load(":cellmlLogo");

    QPainter paint(this);

    paint.drawPixmap(0.5*(width()-pixmap.width()),
                     0.5*(height()-pixmap.height()),
                     pixmap);

    // Have the rest of the painting done by the parent

    QWidget::paintEvent(pEvent);
}
