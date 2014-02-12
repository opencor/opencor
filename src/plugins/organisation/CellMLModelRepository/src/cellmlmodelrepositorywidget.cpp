/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// CellML Model Repository widget
//==============================================================================

#include "cellmlmodelrepositorywidget.h"

//==============================================================================

#include <QDesktopServices>
#include <QFile>
#include <QIODevice>
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

    setStyleSheet("QWebView {"
                  "    margin: 1px;"
                  "}");
    // Note: not sure why, but no matter how many pixels are specified for the
    //       margin, no margin actually exists, but it addresses the issue with
    //       the border drawn by drawBorderIfDocked, so...

    // Prevent objects from being dropped on us

    setAcceptDrops(false);

    // Have links opened in the user's browser rather than in our list

    page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    // Some connections

    connect(this, SIGNAL(linkClicked(const QUrl &)),
            this, SLOT(openLink(const QUrl &)));

    connect(page(), SIGNAL(selectionChanged()),
            this, SLOT(selectionChanged()));

    // Retrieve the output template

    QFile cellmlModelRepositoryWidgetOutputFile(":CellMLModelRepository_output");

    cellmlModelRepositoryWidgetOutputFile.open(QIODevice::ReadOnly);

    mOutputTemplate = cellmlModelRepositoryWidgetOutputFile.readAll();

    cellmlModelRepositoryWidgetOutputFile.close();

    // Let people know that there is nothing to copy initially

    emit copyTextEnabled(false);
}

//==============================================================================

QSize CellmlModelRepositoryWidget::sizeHint() const
{
    // Suggest a default size for the CellML Model Repository widget
    // Note: this is critical if we want a docked widget, with a CellML Model
    //       Repository widget on it, to have a decent size when docked to the
    //       main window...

    return defaultSize(0.15);
}

//==============================================================================

void CellmlModelRepositoryWidget::paintEvent(QPaintEvent *pEvent)
{
    // Default handling of the event

    QWebView::paintEvent(pEvent);

    // Draw a border

    drawBorder(
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
               true, true, true, true,
#elif defined(Q_OS_MAC)
               true, false, true, false,
#else
    #error Unsupported platform
#endif
               true, false, false, false
              );
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

void CellmlModelRepositoryWidget::selectionChanged()
{
    // The text selection has changed, so let the user know whether some text is
    // now selected

    emit copyTextEnabled(!selectedText().isEmpty());
}

//==============================================================================

}   // namespace CellMLModelRepository
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
