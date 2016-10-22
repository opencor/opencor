#ifndef O1FRESHBOOKS_H
#define O1FRESHBOOKS_H

//---OPENCOR--- BEGIN
#include "oauthglobal.h"
//---OPENCOR--- END
#include "o1.h"

/// FreshBooks authenticator.
/*---OPENCOR---
class O1Freshbooks: public O1 {
*/
//---OPENCOR--- BEGIN
class OAUTH_EXPORT O1Freshbooks: public O1 {
//---OPENCOR--- END
    Q_OBJECT

public:
    explicit O1Freshbooks(QObject *parent = 0): O1(parent) {
    }

    void setClientId(const QString &value) {
        O1::setClientId(value);
        setRequestTokenUrl(QUrl("https://" + clientId() + ".freshbooks.com/oauth/oauth_request.php"));
        setAuthorizeUrl(QUrl("https://" + clientId() + ".freshbooks.com/oauth/oauth_authorize.php"));
        setAccessTokenUrl(QUrl("https://" + clientId() + ".freshbooks.com/oauth/oauth_access.php"));
    }
};

#endif // O1FRESHBOOKS_H
