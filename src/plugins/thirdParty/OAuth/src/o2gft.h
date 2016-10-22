#ifndef O2GFT_H
#define O2GFT_H

//---OPENCOR--- BEGIN
#include "oauthglobal.h"
//---OPENCOR--- END
#include "o2.h"

/// Google Fusion Tables' dialect of OAuth 2.0
/*---OPENCOR---
class O2Gft: public O2 {
*/
//---OPENCOR--- BEGIN
class OAUTH_EXPORT O2Gft: public O2 {
//---OPENCOR--- END
    Q_OBJECT

public:
    explicit O2Gft(QObject *parent = 0);
};

#endif // O2GFT_H
