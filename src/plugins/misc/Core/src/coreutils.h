#ifndef COREUTILS_H
#define COREUTILS_H

#include "coreglobal.h"

#include <QString>

namespace OpenCOR {

QByteArray CORE_EXPORT resourceAsByteArray(const QString &pResource);
void CORE_EXPORT saveResourceAs(const QString &pResource,
                                const QString &pFilename);

}

#endif
