#include "fileorganiserwidget.h"

#include <QPaintEvent>

FileOrganiserWidget::FileOrganiserWidget(QWidget *pParent) :
    QWidget(pParent),
    CommonWidget(pParent)
{
}

QSize FileOrganiserWidget::sizeHint() const
{
    // Suggest a default size for the file organiser widget
    // Note: this is critical if we want a docked widget, with a file organiser
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

void FileOrganiserWidget::paintEvent(QPaintEvent *pEvent)
{
    // Default handling of the event

    QWidget::paintEvent(pEvent);

    // Draw a border in case we are docked

    drawBorderIfDocked();

    // Accept the event

    pEvent->accept();
}
