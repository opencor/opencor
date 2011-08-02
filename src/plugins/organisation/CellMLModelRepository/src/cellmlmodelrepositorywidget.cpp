#include "cellmlmodelrepositorywidget.h"

#include <QPaintEvent>

namespace OpenCOR {
namespace CellMLModelRepository {

CellmlModelRepositoryWidget::CellmlModelRepositoryWidget(const QString &pName,
                                                         QWidget *pParent) :
    QWebView(pParent),
    CommonWidget(pName, this, pParent)
{
    // Add a small margin to the widget, so that no visual trace of the border
    // drawn by drawBorderIfDocked is left when scrolling

    setStyleSheet("QWebView { margin: 1px; }");
    // Note: not sure why, but no matter how many pixels are specified for the
    //       margin, no margin actually exists, but it addresses the issue with
    //       border drawn by drawBorderIfDocked, so...

    // Load the CellML Model Repository page

    setUrl(QUrl("http://models.cellml.org/"));

    // Prevent objects from being dropped on us
    // Note: by default, QWebView allows for objects to be dropped on itself,
    //       so...

    setAcceptDrops(false);

    // Prevent the widget from taking over the scrolling of other widgets

    setFocusPolicy(Qt::NoFocus);
}

QSize CellmlModelRepositoryWidget::sizeHint() const
{
    // Suggest a default size for the PMR explorer widget
    // Note: this is critical if we want a docked widget, with a PMR explorer
    //       widget on it, to have a decent size when docked to the main window

    return defaultSize(0.15);
}

void CellmlModelRepositoryWidget::paintEvent(QPaintEvent *pEvent)
{
    // Default handling of the event

    QWebView::paintEvent(pEvent);

    // Draw a border in case we are docked

    drawBorderIfDocked();
}

} }
