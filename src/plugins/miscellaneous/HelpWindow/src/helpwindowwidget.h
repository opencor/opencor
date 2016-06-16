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
// Help window widget
//==============================================================================

#pragma once

//==============================================================================

#include "commonwidget.h"
#include "webviewerwidget.h"

//==============================================================================

#include <QNetworkReply>

//==============================================================================

class QHelpEngine;

//==============================================================================

namespace OpenCOR {
namespace HelpWindow {

//==============================================================================

class HelpWindowNetworkReply : public QNetworkReply
{
public:
    explicit HelpWindowNetworkReply(const QNetworkRequest &pRequest,
                                    const QByteArray &pData,
                                    const QString &pMimeType);

    virtual void abort();
    virtual qint64 bytesAvailable() const;

protected:
    virtual qint64 readData(char *pData, qint64 pMaxlen);

private:
    QByteArray mData;
};

//==============================================================================

class HelpWindowNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit HelpWindowNetworkAccessManager(QHelpEngine *pHelpEngine,
                                            QObject *pParent);

protected:
    virtual QNetworkReply * createRequest(Operation pOperation,
                                          const QNetworkRequest &pRequest,
                                          QIODevice *pOutgoingData = 0);

private:
    QHelpEngine *mHelpEngine;

    QString mErrorMessageTemplate;
};

//==============================================================================

class HelpWindowPage : public QWebPage
{
public:
    explicit HelpWindowPage(QObject *pParent);

protected:
    virtual bool acceptNavigationRequest(QWebFrame *pFrame,
                                         const QNetworkRequest &pRequest,
                                         QWebPage::NavigationType pType);

private:
    QMap<QString, QString> mFileNames;
};

//==============================================================================

class HelpWindowWidget : public WebViewerWidget::WebViewerWidget,
                         public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit HelpWindowWidget(QHelpEngine *pHelpEngine, const QUrl &pHomePage,
                              QWidget *pParent);

    virtual void retranslateUi();

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings) const;

    void goToHomePage();

    void resetZoom();

    void zoomIn();
    void zoomOut();

protected:
    virtual QSize sizeHint() const;

    virtual void mouseReleaseEvent(QMouseEvent *pEvent);
    virtual void wheelEvent(QWheelEvent *pEvent);

private:
    QHelpEngine *mHelpEngine;

    QUrl mHomePage;

    int mZoomLevel;

    void emitZoomRelatedSignals();

    void setZoomLevel(const int &pZoomLevel);

Q_SIGNALS:
    void notHomePage(const bool &pNotHomePage);

    void backEnabled(const bool &pEnabled);
    void forwardEnabled(const bool &pEnabled);

    void copyTextEnabled(const bool &pEnabled);

    void notDefaultZoomLevel(const bool &pEnabled);
    void zoomOutEnabled(const bool &pEnabled);

private Q_SLOTS:
    void urlChanged(const QUrl &pUrl);

    void selectionChanged();

    void documentChanged();
};

//==============================================================================

}   // namespace HelpWindow
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
