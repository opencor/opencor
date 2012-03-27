//==============================================================================
// Core utilities
//==============================================================================

#ifndef COREUTILS_H
#define COREUTILS_H

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QString>

//==============================================================================

class QFrame;
class QWidget;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

QByteArray CORE_EXPORT resourceAsByteArray(const QString &pResource);
void CORE_EXPORT saveResourceAs(const QString &pResource,
                                const QString &pFilename);

void CORE_EXPORT * instance(const QString &pClassName,
                            void *pDefaultGlobalInstance);

void CORE_EXPORT setFocusTo(QWidget *pWindow, QWidget *pWidget,
                            const bool &pForceFocus = true);

QString CORE_EXPORT nativeCanonicalFileName(const QString &pFileName);

QFrame CORE_EXPORT * newLineWidget(QWidget* pParent,
                                   const bool &pHorizontal = true);

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
