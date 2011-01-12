#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QNetworkReply>
#include <QWebView>

class HelpWidget;

class QHelpEngine;

class HelpNetworkReply : public QNetworkReply
{
public:
    HelpNetworkReply(const QNetworkRequest& request,
                     const QByteArray& fileData,
                     const QString& mimeType);

    inline virtual void abort()
        { /* Nothing to do... */ }
    virtual qint64 bytesAvailable() const
        { return data.length()+QNetworkReply::bytesAvailable(); }

protected:
    virtual qint64 readData(char *data, qint64 maxlen);

private:
    QByteArray data;
    qint64 origLen;
};

class HelpNetworkAccessManager : public QNetworkAccessManager
{
public:
    HelpNetworkAccessManager(QHelpEngine *engine, QObject *parent);

protected:
    virtual QNetworkReply *createRequest(Operation operation,
                                         const QNetworkRequest& request,
                                         QIODevice *outgoingData = 0);

private:
    QHelpEngine *helpEngine;
};

class HelpWidget : public QWebView
{
    Q_OBJECT

public:
    HelpWidget(QHelpEngine *engine, QWidget *parent = 0);

    inline void zoomIn(qreal range = 1.0)
        { setTextSizeMultiplier(textSizeMultiplier()+0.1*range); }
    inline void zoomOut(qreal range = 1.0)
        { setTextSizeMultiplier(qMax(0.0, textSizeMultiplier()-0.1*range)); }

protected:
    virtual void wheelEvent(QWheelEvent*);

    void mouseReleaseEvent(QMouseEvent *event);

private:
    QHelpEngine *helpEngine;
};

#endif
