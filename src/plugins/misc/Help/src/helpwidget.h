#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include "commonwidget.h"

#include <QNetworkReply>
#include <QWebView>

class QHelpEngine;

namespace OpenCOR {

class HelpWidget;

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

class HelpNetworkAccessManager : public QNetworkAccessManager
{
public:
    explicit HelpNetworkAccessManager(QHelpEngine *pHelpEngine,
                                      QObject *pParent);

protected:
    virtual QNetworkReply *createRequest(Operation pOperation,
                                         const QNetworkRequest &pRequest,
                                         QIODevice *pOutgoingData = 0);

private:
    QHelpEngine *mHelpEngine;

    QString mErrorMsgTemplate;
};

class HelpPage : public QWebPage
{
public:
    explicit HelpPage(QHelpEngine *pHelpEngine, QObject *pParent);

protected:
    virtual bool acceptNavigationRequest(QWebFrame*,
                                         const QNetworkRequest &pRequest,
                                         QWebPage::NavigationType);

private:
    QHelpEngine *mHelpEngine;
};

class HelpWidget : public QWebView, public CommonWidget
{
    Q_OBJECT

public:
    explicit HelpWidget(const QString &pName, QHelpEngine *pHelpEngine,
                        const QUrl &pHomePage, QWidget *pParent);

    virtual void loadSettings(QSettings *pSettings);
    virtual void saveSettings(QSettings *pSettings);

    QUrl homePage();
    void gotoHomePage();

    bool isBackAvailable();
    bool isForwardAvailable();

    int minimumZoomLevel();
    int defaultZoomLevel();

    void resetZoom();

    void zoomIn();
    void zoomOut();

    void setZoomLevel(const int &pZoomLevel);
    int zoomLevel();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *pEvent);
    virtual void wheelEvent(QWheelEvent *pEvent);

    virtual QSize sizeHint() const;

    virtual void paintEvent(QPaintEvent *pEvent);

private:
    QUrl mHomePage;

    bool mBackAvailable;
    bool mForwardAvailable;

    int mZoomLevel;

Q_SIGNALS:
    void backAvailable(const bool &pAvailable);
    void forwardAvailable(const bool &pAvailable);

    void zoomLevelChanged(const int &pZoomLevel);

private Q_SLOTS:
    void webPageChanged();
};

}

#endif
