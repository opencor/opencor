/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Web Viewer widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "webviewerwidgetglobal.h"

//==============================================================================

#include <QString>
#include <QWebPage>

//==============================================================================

class QWebView;

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace Core {
    class BorderedWidget;
    class ProgressBarWidget;
}   // namespace Core

//==============================================================================

namespace WebViewerWidget {

//==============================================================================

class WebViewerWidget;

//==============================================================================

class WebViewerPage : public QWebPage
{
public:
    explicit WebViewerPage(WebViewerWidget *pParent);

protected:
    virtual bool acceptNavigationRequest(QWebFrame *pFrame,
                                         const QNetworkRequest &pRequest,
                                         QWebPage::NavigationType pType);

private:
    WebViewerWidget *mOwner;
};

//==============================================================================

class WEBVIEWERWIDGET_EXPORT WebViewerWidget : public QWidget,
                                               public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit WebViewerWidget(QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    QWebView * webView() const;
    Core::ProgressBarWidget * progressBarWidget() const;

    QWebElement retrieveLinkInformation(QString &pLink, QString &pTextContent);

    void setToolTip(const QString &pToolTip);

    virtual bool isUrlSchemeSupported(const QString &pUrlScheme);

    void setHomePage(const QString &pHomePage);
    QString homePage() const;

    void goToHomePage();

    void setZoomingEnabled(const bool &pZoomingEnabled);

    void setOverrideCursor(const bool &pOverrideCursor);

    void showProgressBar();
    void hideProgressBar();

    void showWebInspector();

protected:
    virtual bool event(QEvent *pEvent);
    virtual void wheelEvent(QWheelEvent *pEvent);

private:
    QString mToolTip;

    QString mHomePage;

    bool mZoomingEnabled;
    int mZoomLevel;

    bool mOverrideCursor;
    bool mOverridingCursor;

    QWebView *mWebView;

    Core::ProgressBarWidget *mProgressBarWidget;
    Core::BorderedWidget *mProgressBarBorderedWidget;

    void emitZoomRelatedSignals();

    void setZoomLevel(const int &pZoomLevel);

signals:
    void homePage(const bool &pHomePage);

    void backEnabled(const bool &pEnabled);
    void forwardEnabled(const bool &pEnabled);

    void copyTextEnabled(const bool &pEnabled);

    void defaultZoomLevel(const bool &pDefault);
    void zoomingOutEnabled(const bool &pEnabled);

public slots:
    void resetZoom();

    void zoomIn();
    void zoomOut();

private slots:
    void urlChanged(const QUrl &pUrl);

    void selectionChanged();

    void pageChanged();

    void loadProgress(const int &pProgress);
    void loadFinished();
    void resetProgressBar();
};

//==============================================================================

}   // namespace WebViewerWidget
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
