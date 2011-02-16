#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include "commonwidget.h"

#include <QNetworkReply>
#include <QWebView>

class HelpWidget;

class QHelpEngine;

class HelpNetworkReply : public QNetworkReply
{
public:
    HelpNetworkReply(const QNetworkRequest &pRequest, const QByteArray &pData,
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
    HelpNetworkAccessManager(QHelpEngine *pHelpEngine, QObject *pParent);

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
    HelpPage(QHelpEngine *pHelpEngine, QObject *pParent);

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
    HelpWidget(QHelpEngine *pHelpEngine, const QUrl &pHomePage,
               QWidget *pParent = 0);

    virtual void retranslateUi();

    virtual void defaultSettings();

    virtual void loadSettings(const QSettings &pSettings, const QString &pKey);
    virtual void saveSettings(QSettings &pSettings, const QString &pKey);

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

#endif
