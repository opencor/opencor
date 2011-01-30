#ifndef CONSOLE_H
#define CONSOLE_H

class QCoreApplication;

void initApplication(QCoreApplication& pApp);

bool consoleApplication(QCoreApplication& pApp, int& pRes);

#endif
