#ifndef CONSOLE_H
#define CONSOLE_H

#include <QString>

class QCoreApplication;

static const QString OpencorHomepageUrl = "http://www.opencor.ws/";

void error(const QCoreApplication &pApp, const QString &pMsg);

void initApplication(QCoreApplication &pApp);

bool consoleApplication(const QCoreApplication &pApp, int &pRes);

#endif
