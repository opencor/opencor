#ifndef O2SURVEYMONKEY_H
#define O2SURVEYMONKEY_H

//---OPENCOR--- BEGIN
#include "oauthglobal.h"
//---OPENCOR--- END
#include "o2.h"

/// SurveyMonkey's dialect of OAuth 2.0
/*---OPENCOR---
class O2SurveyMonkey: public O2 {
*/
//---OPENCOR--- BEGIN
class OAUTH_EXPORT O2SurveyMonkey: public O2 {
//---OPENCOR--- END
    Q_OBJECT

public:
    explicit O2SurveyMonkey(QObject *parent = 0);
};

#endif // O2SURVEYMONKEY_H
