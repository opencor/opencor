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
class QLabel;
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

void CORE_EXPORT setFocusTo(QWidget *pWidget);

QString CORE_EXPORT nativeCanonicalFileName(const QString &pFileName);

QFrame CORE_EXPORT * newLineWidget(QWidget *pParent,
                                   const bool &pHorizontal = true);

QLabel CORE_EXPORT * newLabel(QWidget *pParent, const QString &pText,
                              const double &pFontPercentage = 1.0,
                              const bool &pBold = false,
                              const bool &pItalic = false,
                              const Qt::Alignment &pAlignment = Qt::AlignLeft|Qt::AlignVCenter);
QLabel CORE_EXPORT * newLabelLink(QWidget *pParent, const QString &pText,
                                  const double &pFontPercentage = 1.0,
                                  const bool &pBold = false,
                                  const bool &pItalic = false,
                                  const Qt::Alignment &pAlignment = Qt::AlignLeft|Qt::AlignVCenter);

QString CORE_EXPORT copyright();

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
