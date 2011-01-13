#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QNetworkReply>
#include <QWebView>

class HelpWidget;

class QHelpEngine;

class HelpNetworkReply : public QNetworkReply
{
public:
    HelpNetworkReply(const QNetworkRequest& pRequest,
                     const QByteArray& pFileData,
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
    HelpNetworkAccessManager(QHelpEngine *pEngine, QObject *pParent);

protected:
    virtual QNetworkReply *createRequest(Operation pOperation,
                                         const QNetworkRequest& pRequest,
                                         QIODevice *pOutgoingData = 0);

private:
    QHelpEngine *mHelpEngine;
    QString mErrorMsg;

    QString errorMsg(const QString& pErrorMsg);
};

class HelpWidget : public QWebView
{
    Q_OBJECT

public:
    HelpWidget(QHelpEngine *pEngine, QWidget *pParent = 0);

    inline void zoomIn(const qreal& pRange = 1.0)
        { setTextSizeMultiplier(textSizeMultiplier()+0.1*pRange); }
    inline void zoomOut(const qreal& pRange = 1.0)
        { setTextSizeMultiplier(qMax(0.0, textSizeMultiplier()-0.1*pRange)); }

protected:
    void mouseReleaseEvent(QMouseEvent *pEvent);
    void wheelEvent(QWheelEvent *pEvent);
};

#endif
