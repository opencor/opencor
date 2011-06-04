#ifndef UTILS_H
#define UTILS_H

#include <QString>

class QCoreApplication;

namespace OpenCOR {

QString exec(const QString &pProg, const QString &pArgs);

QString getOsName();
QString getAppVersion(const QCoreApplication *pApp);
QString getAppCopyright(const bool &pHtml);

QByteArray resourceAsByteArray(const QString &pResource);
void saveResourceAs(const QString &pResource, const QString &pFilename);

#ifdef QT_GUI_LIB
void notYetImplemented(const QString &method);
#endif

}

#endif
