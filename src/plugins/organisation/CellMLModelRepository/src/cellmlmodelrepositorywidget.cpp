//==============================================================================
// CellML Model Repository widget
//==============================================================================

#include "cellmlmodelrepositorywidget.h"

//==============================================================================

#include <QDesktopServices>
#include <QFile>
#include <QPaintEvent>

//==============================================================================

namespace OpenCOR {
namespace CellMLModelRepository {

//==============================================================================

CellmlModelRepositoryWidget::CellmlModelRepositoryWidget(QWidget *pParent) :
    QWebView(pParent),
    CommonWidget(pParent)
{
    // Add a small margin to the widget, so that no visual trace of the border
    // drawn by drawBorderIfDocked is left when scrolling

    setStyleSheet("QWebView { margin: 1px; }");
    // Note: not sure why, but no matter how many pixels are specified for the
    //       margin, no margin actually exists, but it addresses the issue with
    //       the border drawn by drawBorderIfDocked, so...

    // Prevent objects from being dropped on us

    setAcceptDrops(false);

    // Have links opened in the user's browser rather than in our list

    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    connect(this, SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(openLink(const QUrl &)));

    // Retrieve the output template

    QFile cellmlModelRepositoryWidgetOutputFile(":cellmlModelRepositoryWidgetOutput");

    cellmlModelRepositoryWidgetOutputFile.open(QIODevice::ReadOnly);

    mOutputTemplate = QString(cellmlModelRepositoryWidgetOutputFile.readAll());

    cellmlModelRepositoryWidgetOutputFile.close();
}

//==============================================================================

QSize CellmlModelRepositoryWidget::sizeHint() const
{
    // Suggest a default size for the CellML Model Repository widget
    // Note: this is critical if we want a docked widget, with a CellML Model
    //       Repository widget on it, to have a decent size when docked to the
    //       main window

    return defaultSize(0.15);
}

//==============================================================================

void CellmlModelRepositoryWidget::changeEvent(QEvent *pEvent)
{
    // Default handling of the event

    QWebView::changeEvent(pEvent);

    // Check whether the palette has changed and if so then (re)initialise the
    // colour to be used for the border when docked

    if (pEvent->type() == QEvent::PaletteChange)
        initBorderColor();
}

//==============================================================================

void CellmlModelRepositoryWidget::paintEvent(QPaintEvent *pEvent)
{
    // Default handling of the event

    QWebView::paintEvent(pEvent);

    // Draw a border in case we are docked

    drawBorderIfDocked(true, true, false, false, false);
}

//==============================================================================

void CellmlModelRepositoryWidget::output(const QString &pOutput)
{
    // Set the page to contain pOutput using our output template

    setHtml(mOutputTemplate.arg(pOutput));
}

//==============================================================================

void CellmlModelRepositoryWidget::openLink(const QUrl &pUrl)
{
    // Open the link in the user's browser

    QDesktopServices::openUrl(pUrl);
}

//==============================================================================

}   // namespace CellMLModelRepository
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
