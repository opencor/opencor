#ifndef O2HUBIC_H
#define O2HUBIC_H

//---OPENCOR--- BEGIN
#include "oauthglobal.h"
//---OPENCOR--- END
#include "o2.h"

/// Hubic's dialect of OAuth 2.0
/*---OPENCOR---
class O2Hubic: public O2 {
*/
//---OPENCOR--- BEGIN
class OAUTH_EXPORT O2Hubic: public O2 {
//---OPENCOR--- END
    Q_OBJECT

public:
    /// Constructor.
    /// @param  parent  Parent object.
    explicit O2Hubic(QObject *parent = 0);

};

#endif // O2_HUBIC
