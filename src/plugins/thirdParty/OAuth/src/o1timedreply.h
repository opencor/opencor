#ifndef O1TIMEDREPLY_H
#define O1TIMEDREPLY_H

#include <QNetworkReply>
#include <QTimer>
//---OPENCOR--- BEGIN
#include "oauthglobal.h"
//---OPENCOR--- END

/// A timer connected to a network reply.
/*---OPENCOR---
class O1TimedReply: public QTimer {
*/
//---OPENCOR--- BEGIN
class OAUTH_EXPORT O1TimedReply: public QTimer {
//---OPENCOR--- END
    Q_OBJECT

public:
    explicit O1TimedReply(QNetworkReply *parent, int pTimeout=60*1000);

Q_SIGNALS:
    /// Emitted when we have timed out waiting for the network reply.
    void error(QNetworkReply::NetworkError);
    /// Emitted when the network reply has responded.
    void finished();

private Q_SLOTS:
    void onFinished();
    void onTimeout();
};

#endif
