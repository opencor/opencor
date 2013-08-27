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
// Help widget
//==============================================================================

#ifndef HELPWIDGET_H
#define HELPWIDGET_H

//==============================================================================

#include "commonwidget.h"

//==============================================================================

#include <QNetworkReply>
#include <QWebView>

//==============================================================================

class QHelpEngine;

//==============================================================================

namespace OpenCOR {
namespace Help {

//==============================================================================

class HelpWidget;

//==============================================================================

class HelpNetworkReply : public QNetworkReply
{
public:
    explicit HelpNetworkReply(const QNetworkRequest &pRequest,
                              const QByteArray &pData,
                              const QString &pMimeType);

    virtual void abort();
    virtual qint64 bytesAvailable() const;

protected:
    virtual qint64 readData(char *pData, qint64 pMaxlen);

private:
    QByteArray mData;
    qint64 mOrigLen;
};

//==============================================================================

class HelpNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit HelpNetworkAccessManager(QHelpEngine *pHelpEngine,
                                      QObject *pParent);

protected:
    virtual QNetworkReply * createRequest(Operation pOperation,
                                          const QNetworkRequest &pRequest,
                                          QIODevice *pOutgoingData = 0);

private:
    QHelpEngine *mHelpEngine;

    QString mErrorMsgTemplate;
};

//==============================================================================

class HelpPage : public QWebPage
{
public:
    explicit HelpPage(QObject *pParent);

protected:
    virtual bool acceptNavigationRequest(QWebFrame*,
                                         const QNetworkRequest &pRequest,
                                         QWebPage::NavigationType);

private:
    QMap<QString, QString> mFileNames;
};

//==============================================================================

class HelpWidget : public QWebView, public Core::CommonWidget
{
    Q_OBJECT

public:
    explicit HelpWidget(QHelpEngine *pHelpEngine, const QUrl &pHomePage,
                        QWidget *pParent = 0);

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
    virtual void paintEvent(QPaintEvent *pEvent);

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

}   // namespace Help
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
