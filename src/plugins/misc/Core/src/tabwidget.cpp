#include "tabwidget.h"

#include <QApplication>
#include <QPainter>
#include <QPaintEvent>

namespace OpenCOR {
namespace Core {

TabWidget::TabWidget(QWidget *pParent) :
    QTabWidget(pParent),
    CommonWidget(pParent)
{
    // Set some properties
    // Note: we give a strong focus policy to the tab widget so that it can also
    //       get focus by being clicked on

    setMovable(true);
    setTabsClosable(true);
    setFocusPolicy(Qt::StrongFocus);

    // Logo settings

    mLogo.load(":logo");

    mBackgroundBrush.setStyle(Qt::SolidPattern);
    mBackgroundBrush.setColor(QImage(":logo").pixel(0, 0));

    // A connection to handle the change of tab

    connect(this, SIGNAL(currentChanged(int)),
            this, SLOT(tabChanged(const int &)));
    connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)),
            this, SLOT(getFocus(QWidget *, QWidget *)));
}

void TabWidget::paintEvent(QPaintEvent *pEvent)
{
    // Display the CellML logo in place of the tab widget itself, in case the
    // tab widget doesn't have any tab associated with it

    if (!count()) {
        // There are no tabs, so display our logo after having filled the widget
        // with the logo's background colour

        QPainter paint(this);

        paint.fillRect(QRect(0, 0, width(), height()), mBackgroundBrush);

        int logoWidth  = mLogo.width();
        int logoHeight = mLogo.height();

        if ((mLogo.width() >= width()) || (mLogo.height() >= height())) {
            // The logo doesn't fit within the widget, so determine what its
            // size should be for it to fit

            if (height()) {
                // The widget has a non-zero height, so...

                if (width()/height() > logoWidth/logoHeight) {
                    // The height of the widget is to dictate the size of the
                    // logo

                    logoHeight = height();
                    logoWidth  = logoHeight*mLogo.width()/mLogo.height();
                } else {
                    // The width of the widget is to dictate the size of the
                    // logo

                    logoWidth  = width();
                    logoHeight = logoWidth*mLogo.height()/mLogo.width();
                }
            } else {
                // The widget has a zero height, so...

                logoHeight = 0;
            }
        }

        paint.drawPixmap(QRect(0.5*(width()-logoWidth),
                               0.5*(height()-logoHeight),
                               logoWidth, logoHeight),
                         mLogo);

        // Accept the event

        pEvent->accept();
    } else {
        // There are tabs, so revert to the default paint handler

        QTabWidget::paintEvent(pEvent);
    }
}

void TabWidget::tabChanged(const int &pIndex) const
{
    // A new tab has been selected, so give the focus to its widget

    QWidget *crtWidget = widget(pIndex);

    if (crtWidget)
        crtWidget->setFocus();
}

void TabWidget::getFocus(QWidget *, QWidget *pNew) const
{
    // The tab widget (or a part of it) has just received the focus and, here,
    // we want to take advantage of this to give the focus to the widget of the
    // active tab as a result of the user clicking on the active tab (since this
    // won't emit the currentChanged signal). In our case, this means we are
    // after pNew being of QTabBar type and that, more importantly, its parent
    // is this tab widget

    if (pNew && (pNew->parentWidget() == this))
        // The tab widget didn't have the focus, but the user has just clicked
        // on the tab widget's active tab, thus giving the focus to the tab
        // widget, so now we need to give the focus to the active tab's widget

        tabChanged(currentIndex());
}

} }
