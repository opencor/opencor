#ifndef O0REQUESTPARAMETER_H
#define O0REQUESTPARAMETER_H
//---OPENCOR--- BEGIN
#include "oauthglobal.h"
//---OPENCOR--- END

/// Request parameter (name-value pair) participating in authentication.
/*---OPENCOR---
struct O0RequestParameter {
*/
//---OPENCOR--- BEGIN
struct OAUTH_EXPORT O0RequestParameter {
//---OPENCOR--- END
    O0RequestParameter(const QByteArray &n, const QByteArray &v): name(n), value(v) {}
    bool operator <(const O0RequestParameter &other) const {
        return (name == other.name)? (value < other.value): (name < other.name);
    }
    QByteArray name;
    QByteArray value;
};

#endif // O0REQUESTPARAMETER_H
