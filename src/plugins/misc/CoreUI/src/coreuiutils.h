#ifndef COREUIUTILS_H
#define COREUIUTILS_H

#include <QString>

namespace OpenCOR {

QByteArray resourceAsByteArray(const QString &pResource);
void saveResourceAs(const QString &pResource, const QString &pFilename);

}

#endif
