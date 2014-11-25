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
// CellML annotation view metadata web view widget
//==============================================================================

#include "cellmlannotationviewmetadatawebviewwidget.h"

//==============================================================================

#include <QCursor>
#include <QEvent>
#include <QWebElement>
#include <QWebHitTestResult>

//==============================================================================

namespace OpenCOR {
namespace CellMLAnnotationView {

//==============================================================================

CellmlAnnotationViewMetadataWebViewWidget::CellmlAnnotationViewMetadataWebViewWidget(QWidget *pParent) :
    QWebView(pParent),
    mResettingCursor(false)
{
    // Customise ourselves

    setAcceptDrops(false);
    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

//==============================================================================

bool CellmlAnnotationViewMetadataWebViewWidget::event(QEvent *pEvent)
{
    // Override the change of the cursor when hovering some text

    if (mResettingCursor) {
        return true;
    } else if (    (pEvent->type() == QEvent::CursorChange)
        &&  (cursor().shape() == Qt::IBeamCursor)
        && !mResettingCursor) {
        mResettingCursor = true;

        setCursor(Qt::ArrowCursor);

        mResettingCursor = false;

        return true;
    } else {
        return QWebView::event(pEvent);
    }
}

//==============================================================================

void CellmlAnnotationViewMetadataWebViewWidget::retrieveLinkInformation(QString &pLink,
                                                                        QString &pTextContent)
{
    // Retrieve the link and text content values for the link, if any, below our
    // mouse pointer
    // Note: normally, one would want to handle the linkHovered() signal, but it
    //       may provide the wrong information. Indeed, say that you are over a
    //       link and then scroll down/up using your mouse wheel, and end up
    //       over another link and click it. Now, because your mouse didn't
    //       move, no linkHovered() message will have been emitted (and
    //       handled), which means that if we need that information to process
    //       the click, then we will have the wrong information...

    QWebHitTestResult hitTestResult = page()->mainFrame()->hitTestContent(mapFromGlobal(QCursor::pos()));
    QWebElement element = hitTestResult.element();

    while (!element.isNull() && element.tagName().compare("A"))
        element = element.parent();

    if (element.isNull()) {
        pLink = QString();
        pTextContent = QString();
    } else {
        pLink = element.attribute("href");
        pTextContent = hitTestResult.linkText();
    }
}

//==============================================================================

}   // namespace CellMLAnnotationView
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
