#ifndef UTILS_H
#define UTILS_H

#include <QString>

namespace OpenCOR {

QString exec(const QString &pProg, const QString &pArgs);

enum OsType
{
    Windows,
    Linux,
    MacOsX
};

OsType getOsType();
QString getOsName();

enum BitVersion
{
    x86,
    x86_64,
    xUnknown
};

BitVersion getAppBitVersion();

QByteArray resourceAsByteArray(const QString &pResource);
void saveResourceAs(const QString &pResource, const QString &pFilename);

#ifdef QT_GUI_LIB
void notYetImplemented(const QString &method);
#endif

}

#endif
