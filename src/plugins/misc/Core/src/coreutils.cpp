//==============================================================================
// Core utilities
//==============================================================================

#include "coreutils.h"

//==============================================================================

#include <QApplication>
#include <QFile>
#include <QResource>
#include <QSettings>

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

QByteArray resourceAsByteArray(const QString &pResource)
{
    // Retrieve a resource as a QByteArray

    QResource resource(pResource);

    if (resource.isValid()) {
        if (resource.isCompressed())
            // The resource is compressed, so uncompress it before returning it
            // Note: qUncompress returns a QByteArray, so we are fine...

            return qUncompress(reinterpret_cast<const uchar *>(resource.data()),
                               resource.size());
        else
            // The resource is not compressed, so just return it after doing the
            // right conversions

            return QByteArray(reinterpret_cast<const char *>(resource.data()),
                              resource.size());
    }
    else {
        // The resource doesn't exist, so...

        return QByteArray();
    }
}

//==============================================================================

void saveResourceAs(const QString &pResource, const QString &pFilename)
{
    if (QResource(pResource).isValid()) {
        // The resource exists, so write a file with name pFilename and which
        // contents is that of the resource which is retrieved as a QByteArray

        QFile file(pFilename);

        file.open(QIODevice::ReadWrite);
        file.write(resourceAsByteArray(pResource));
        file.close();
    }
}

//==============================================================================

void * singleton(const QString &pClassName, void *pClassDefaultInstance)
{
    // Retrieve the singleton associated with a given class

    QSettings settings(qApp->applicationName());
    qlonglong singleton;

    settings.beginGroup("Singletons");
        singleton = settings.value(pClassName, 0).toLongLong();

        if (!singleton) {
            // There is no singleton associated with the given class, so use
            // the class's default instance we were given

            singleton = (qlonglong) pClassDefaultInstance;

            settings.setValue(pClassName, singleton);
        }
    settings.endGroup();

    // Return the class's singleton

    return (void *) singleton;
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
