#ifndef UTILS_H
#define UTILS_H

#include <QString>

QString exec(const QString& prog, const QString& args);

QString getOsName();

void saveResourceAs(const QString& resourceName, const QString& fileName);

#endif // UTILS_H
