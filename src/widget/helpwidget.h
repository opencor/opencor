#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QUrl>
#include <QAction>
#include <QWebView>
#include <QNetworkReply>

class MainWindow;
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

class HelpPage : public QWebPage
{
public:
    HelpPage(QHelpEngine *engine, QObject *parent);

protected:
    virtual bool acceptNavigationRequest(QWebFrame *frame,
                                         const QNetworkRequest& request,
                                         NavigationType type);

private:
    QHelpEngine *helpEngine;

    friend class HelpWidget;
};

class HelpWidget : public QWebView
{
    Q_OBJECT

public:
    HelpWidget(QHelpEngine *engine, MainWindow *parent = 0);

    inline void resetZoom()
        { setTextSizeMultiplier(1.0); }
    inline void zoomIn(qreal range = 1.0)
        { setTextSizeMultiplier(textSizeMultiplier()+0.1*range); }
    inline void zoomOut(qreal range = 1)
        { setTextSizeMultiplier(qMax(0.0, textSizeMultiplier()-0.1*range)); }

    inline bool isForwardAvailable() const
        { return pageAction(QWebPage::Forward)->isEnabled(); }
    inline bool isBackwardAvailable() const
        { return pageAction(QWebPage::Back)->isEnabled(); }
    inline qreal zoom() const
        { return textSizeMultiplier(); }

protected:
    virtual void wheelEvent(QWheelEvent*);

    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    QHelpEngine *helpEngine;

    bool eventFilter(QObject *object, QEvent *event);

public Q_SLOTS:
    inline void backward()
        { back(); }

private Q_SLOTS:
    void actionChanged();

signals:
    void forwardAvailable(bool available);
    void backwardAvailable(bool available);

    void mouseDoubleClick(QMouseEvent *event);
};

#endif
