#ifndef O2FACEBOOK_H
#define O2FACEBOOK_H

//---OPENCOR--- BEGIN
#include "oauthglobal.h"
//---OPENCOR--- END
#include "o2.h"

/// Facebook's dialect of OAuth 2.0
/*---OPENCOR---
class O2Facebook: public O2 {
*/
//---OPENCOR--- BEGIN
class OAUTH_EXPORT O2Facebook: public O2 {
//---OPENCOR--- END
    Q_OBJECT

public:
    explicit O2Facebook(QObject *parent = 0);

protected Q_SLOTS:
    void onVerificationReceived(QMap<QString, QString>);
    virtual void onTokenReplyFinished();
};

#endif // O2FACEBOOK_H
