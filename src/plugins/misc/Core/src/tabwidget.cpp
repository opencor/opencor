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

    mLogoRenderer.load(pLogoFileName);

    mLogoWidth  = mLogoRenderer.defaultSize().width();
    mLogoHeight = mLogoRenderer.defaultSize().height();

    mLogoOneOverWidth  = 1.0/mLogoWidth;
    mLogoOneOverHeight = 1.0/mLogoHeight;

    mLogoWidthOverHeight = (double) mLogoWidth/mLogoHeight;
    mLogoHeightOverWidth = (double) mLogoHeight/mLogoWidth;

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

        // Paint the widget's background

        int widgetWidth  = width();
        int widgetHeight = height();

        painter.fillRect(QRect(0, 0, widgetWidth, widgetHeight),
                         palette().color(QPalette::Base));

        // Determine what the dimensions of the logo should be, based on the
        // widget's current dimensions

        const int padding = 30;

        int logoWidth  = mLogoWidth;
        int logoHeight = mLogoHeight;

        bool needResizeWidth  = false;
        bool needResizeHeight = false;

        if (widgetWidth < mLogoWidth+padding) {
            if (widgetHeight < mLogoHeight+padding)
                needResizeWidth =   widgetWidth*mLogoOneOverWidth
                                  < widgetHeight*mLogoOneOverHeight;
            else
                needResizeWidth = true;

            needResizeHeight = !needResizeWidth;
        } else {
            needResizeHeight = widgetHeight < mLogoHeight+padding;
        }

        if (needResizeWidth) {
            logoWidth  = widgetWidth-padding;
            logoHeight = logoWidth*mLogoHeightOverWidth;
        } else if (needResizeHeight) {
            logoHeight = widgetHeight-padding;
            logoWidth  = logoHeight*mLogoWidthOverHeight;
        }

        // Render the logo itself

        mLogoRenderer.render(&painter,
                             QRect(0.5*(widgetWidth-logoWidth),
                                   0.5*(widgetHeight-logoHeight),
                                   logoWidth, logoHeight));

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
