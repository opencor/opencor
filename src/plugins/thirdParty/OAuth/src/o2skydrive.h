#ifndef O2SKYDRIVE_H
#define O2SKYDRIVE_H

//---OPENCOR--- BEGIN
#include "oauthglobal.h"
//---OPENCOR--- END
#include "o2.h"

/// Skydrive's dialect of OAuth 2.0
/*---OPENCOR---
class O2Skydrive: public O2 {
*/
//---OPENCOR--- BEGIN
class OAUTH_EXPORT O2Skydrive: public O2 {
//---OPENCOR--- END
    Q_OBJECT

public:
    explicit O2Skydrive(QObject *parent = 0);

public Q_SLOTS:
    Q_INVOKABLE void link();
    Q_INVOKABLE virtual void redirected(const QUrl &url);
};

#endif // O2SKYDRIVE_H
