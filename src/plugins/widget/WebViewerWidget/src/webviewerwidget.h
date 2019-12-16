/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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
} // namespace Core

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
    bool acceptNavigationRequest(QWebFrame *pFrame,
                                 const QNetworkRequest &pRequest,
                                 QWebPage::NavigationType pType) override;

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

    void loadSettings(QSettings &pSettings) override;
    void saveSettings(QSettings &pSettings) const override;

    QWebView * webView() const;
    Core::ProgressBarWidget * progressBarWidget() const;

    QWebElement retrieveLinkInformation(QString &pLink, QString &pTextContent);

    void setToolTip(const QString &pToolTip);

    virtual bool isUrlSchemeSupported(const QString &pUrlScheme);

    void setHomePage(const QString &pHomePage);
    QString homePage() const;

    void goToHomePage();

    void setZoomingEnabled(bool pZoomingEnabled);

    void setOverrideCursor(bool pOverrideCursor);

    void showProgressBar();
    void hideProgressBar();

    void showWebInspector();

protected:
    bool event(QEvent *pEvent) override;
    void wheelEvent(QWheelEvent *pEvent) override;

private:
    QString mToolTip;

    QString mHomePage;

    bool mZoomingEnabled = true;
    int mZoomLevel = -1;

    bool mOverrideCursor = false;
    bool mOverridingCursor = false;

    QWebView *mWebView;

    Core::ProgressBarWidget *mProgressBarWidget;
    Core::BorderedWidget *mProgressBarBorderedWidget;

    void emitZoomRelatedSignals();

    void setZoomLevel(int pZoomLevel);

signals:
    void homePage(bool pHomePage);

    void backEnabled(bool pEnabled);
    void forwardEnabled(bool pEnabled);

    void copyTextEnabled(bool pEnabled);

    void defaultZoomLevel(bool pDefault);
    void zoomingOutEnabled(bool pEnabled);

public slots:
    void resetZoom();

    void zoomIn();
    void zoomOut();

private slots:
    void urlChanged(const QUrl &pUrl);

    void selectionChanged();

    void pageChanged();

    void loadProgress(int pProgress);
    void loadFinished();
    void resetProgressBar();
};

//==============================================================================

} // namespace WebViewerWidget
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
