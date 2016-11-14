/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Web Viewer widget
//==============================================================================

#include "webviewerwidget.h"

//==============================================================================

#include <QCursor>
#include <QDesktopServices>
#include <QEvent>
#include <QHelpEvent>
#include <QNetworkRequest>
#include <QToolTip>
#include <QWebElement>
#include <QWebHitTestResult>

//==============================================================================

namespace OpenCOR {
namespace WebViewerWidget {

//==============================================================================

WebViewerPage::WebViewerPage(WebViewerWidget *pParent) :
    QWebPage(pParent),
    mOwner(pParent)
{
}

//==============================================================================

bool WebViewerPage::acceptNavigationRequest(QWebFrame *pFrame,
                                            const QNetworkRequest &pRequest,
                                            QWebPage::NavigationType pType)
{
    Q_UNUSED(pFrame);
    Q_UNUSED(pType);

    // Ask our owner whether the URL should be handled by ourselves or just
    // opened the default way

    QUrl url = pRequest.url();
    QString urlScheme = url.scheme();

    if (mOwner->isUrlSchemeSupported(urlScheme)) {
        return true;
    } else {
        QDesktopServices::openUrl(url);

        return false;
    }
}

//==============================================================================

WebViewerWidget::WebViewerWidget(QWidget *pParent) :
    QWebView(pParent),
    mResettingCursor(false),
    mLinkToolTip(QString())
{
    // Use our own page

    setPage(new WebViewerPage(this));

    // Customise ourselves

    setAcceptDrops(false);
    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

//==============================================================================

void WebViewerWidget::setLinkToolTip(const QString &pLinkToolTip)
{
    // Set our link tool tip

    mLinkToolTip = pLinkToolTip;
}

//==============================================================================

bool WebViewerWidget::event(QEvent *pEvent)
{
    // Override the change of the cursor and tool tip when hovering a link

    if (mResettingCursor) {
        return true;
    } else if (    (pEvent->type() == QEvent::CursorChange)
               &&  (cursor().shape() == Qt::IBeamCursor)
               && !mResettingCursor) {
        mResettingCursor = true;

        setCursor(Qt::ArrowCursor);

        mResettingCursor = false;

        return true;
    } else if (pEvent->type() == QEvent::ToolTip) {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(pEvent);

        if (!mLinkToolTip.isEmpty()) {
            QToolTip::showText(helpEvent->globalPos(), mLinkToolTip);
        } else {
            QToolTip::hideText();

            pEvent->ignore();
        }

        return true;
    } else {
        return QWebView::event(pEvent);
    }
}

//==============================================================================

bool WebViewerWidget::isUrlSchemeSupported(const QString &pUrlScheme)
{
    Q_UNUSED(pUrlScheme);

    // By default, we support all URL schemes

    return true;
}

//==============================================================================

QWebElement WebViewerWidget::retrieveLinkInformation(QString &pLink,
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
    QWebElement res = hitTestResult.element();

    while (!res.isNull() && res.tagName().compare("A"))
        res = res.parent();

    if (res.isNull()) {
        pLink = QString();
        pTextContent = QString();
    } else {
        pLink = res.attribute("href");
        pTextContent = hitTestResult.linkText();
    }

    return res;
}

//==============================================================================

}   // namespace WebViewerWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
