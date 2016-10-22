#ifndef O0ABSTRACTSTORE_H
#define O0ABSTRACTSTORE_H

#include <QObject>
#include <QString>
//---OPENCOR--- BEGIN
#include "oauthglobal.h"
//---OPENCOR--- END

/// Storage for strings.
/*---OPENCOR---
class O0AbstractStore: public QObject {
*/
//---OPENCOR--- BEGIN
class OAUTH_EXPORT O0AbstractStore: public QObject {
//---OPENCOR--- END
    Q_OBJECT

public:
    explicit O0AbstractStore(QObject *parent = 0): QObject(parent) {
    }

    /// Retrieve a string value by key.
    virtual QString value(const QString &key, const QString &defaultValue = QString()) = 0;

    /// Set a string value for a key.
    virtual void setValue(const QString &key, const QString &value) = 0;
};

#endif // O0ABSTRACTSTORE_H
