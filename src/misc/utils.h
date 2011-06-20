#ifndef UTILS_H
#define UTILS_H

#include <QString>

class QCoreApplication;

namespace OpenCOR {

QString exec(const QString &pProg, const QString &pArgs);

QString getOsName();
QString getAppVersion(const QCoreApplication *pApp);
QString getAppCopyright(const bool &pHtml);

}

#endif
