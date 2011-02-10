#ifndef UTILS_H
#define UTILS_H

#include <QString>

QString exec(const QString &pProg, const QString &pArgs);

QString getOsName();

void saveResourceAs(const QString &pResource, const QString &pFilename);

#endif
