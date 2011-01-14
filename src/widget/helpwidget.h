#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QNetworkReply>
#include <QWebView>

class HelpWidget;

class QHelpEngine;

class HelpNetworkReply : public QNetworkReply
{
public:
    HelpNetworkReply(const QNetworkRequest& pRequest, const QByteArray& pData,
                     const QString& pMimeType);

    inline virtual void abort()
        { /* Nothing to do... */ }
    virtual qint64 bytesAvailable() const
        { return mData.length()+QNetworkReply::bytesAvailable(); }

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
                                         const QNetworkRequest& pRequest,
                                         QIODevice *pOutgoingData = 0);

private:
    QHelpEngine *mHelpEngine;
    QString mErrorMsgTemplate;

    QString errorMsg(const QString& pErrorMsg);
};

class HelpPage : public QWebPage
{
public:
    HelpPage(QHelpEngine *pHelpEngine, QObject *pParent);

protected:
    virtual bool acceptNavigationRequest(QWebFrame*,
                                         const QNetworkRequest& pRequest,
                                         QWebPage::NavigationType);

private:
    QHelpEngine *mHelpEngine;

    friend class HelpWidget;
};

class HelpWidget : public QWebView
{
    Q_OBJECT

public:
    HelpWidget(QHelpEngine *pHelpEngine, const QUrl& pHomepage,
               QWidget *pParent = 0);

    void resetAll();

    void gotoHomepage();

    int minimumZoomLevel();
    int defaultZoomLevel();

    void resetZoom();

    void zoomIn();
    void zoomOut();

    void setZoomLevel(const int& pZoomLevel);
    int zoomLevel();

protected:
    void mouseReleaseEvent(QMouseEvent *pEvent);
    void wheelEvent(QWheelEvent *pEvent);

private:
    QUrl mHomepage;

    int mZoomLevel;

private Q_SLOTS:
    void actionChanged();

Q_SIGNALS:
    void backAvailable(bool pAvailable);
    void forwardAvailable(bool pAvailable);

    void zoomLevelChanged(int pZoomLevel);
};

#endif
