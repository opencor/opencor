#include "tabwidget.h"

#include <QApplication>
#include <QPainter>
#include <QPaintEvent>

namespace OpenCOR {
namespace Core {

TabWidget::TabWidget(const QString &pLogoFileName, QWidget *pParent) :
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

    mLogo.load(pLogoFileName);

    mLogoBackgroundColor = QImage(pLogoFileName).pixel(0, 0);

    mLogoWidth  = mLogo.width();
    mLogoHeight = mLogo.height();

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
        // There are no tabs, so display our logo

        QPainter painter(this);

        // Paint the widget with the logo's background colour

        int widgetWidth  = width();
        int widgetHeight = height();

        painter.fillRect(QRect(0, 0, widgetWidth, widgetHeight),
                         mLogoBackgroundColor);

        // Draw the logo itself

        painter.drawPixmap(QRect(0.5*(widgetWidth-mLogoWidth),
                                 0.5*(widgetHeight-mLogoHeight),
                                 mLogoWidth, mLogoHeight),
                           mLogo);

#ifndef Q_WS_MAC
        // Draw a border around the widget
        // Note #1: the border actually consists of two borders. A 'dark' outer
        //          border and a 'light' inner border. Note the way the border
        //          coordinates were adjusted to get the right effect...
        // Note #2: the border doesn't look good on Mac OS X, so...

        QPen pen = painter.pen();

        pen.setColor(palette().color(QPalette::Button));

        painter.setPen(pen);

        QRect border = rect();

        border.adjust(0, 0, -1, 0);

        painter.drawRect(border);

        pen.setColor(palette().color(QPalette::Midlight));

        painter.setPen(pen);

        border.adjust(1, 1, -1, -1);

        painter.drawRect(border);
#endif

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
