#ifndef COREUTILS_H
#define COREUTILS_H

#include <QString>

namespace OpenCOR {

QByteArray resourceAsByteArray(const QString &pResource);
void saveResourceAs(const QString &pResource, const QString &pFilename);

}

#endif
