#ifndef CONSOLE_H
#define CONSOLE_H

class QCoreApplication;
class QString;

void error(const QCoreApplication &pApp, const QString &pMsg);

void initApplication(QCoreApplication &pApp);

bool consoleApplication(const QCoreApplication &pApp, int &pRes);

#endif
