//==============================================================================
// Core utilities
//==============================================================================

#ifndef COREUTILS_H
#define COREUTILS_H

//==============================================================================

#include "coreglobal.h"

//==============================================================================

#include <QColor>
#include <QString>

//==============================================================================

class QFrame;
class QLabel;
class QWidget;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

static const QString SettingsGlobal = "Global";

static const QString SettingsActiveDirectory = "ActiveDirectory";

static const QString SettingsBorderColor    = "BorderColor";
static const QString SettingsBaseColor      = "BaseColor";
static const QString SettingsWindowColor    = "WindowColor";
static const QString SettingsHighlightColor = "HighlightColor";

//==============================================================================

size_t CORE_EXPORT totalPhysicalMemory();

QByteArray CORE_EXPORT resourceAsByteArray(const QString &pResource);
bool CORE_EXPORT saveResourceAs(const QString &pResource,
                                const QString &pFilename);

void CORE_EXPORT * globalInstance(const QString &pObjectName,
                                  void *pDefaultGlobalInstance);

void CORE_EXPORT setActiveDirectory(const QString &pDirName);
QString CORE_EXPORT activeDirectory();

QStringList CORE_EXPORT getOpenFileNames(const QString &pCaption,
                                         const QString &pFilter);
QString CORE_EXPORT getSaveFileName(const QString &pCaption,
                                    const QString &pFileName,
                                    const QString &pFilter);

void CORE_EXPORT setFocusTo(QWidget *pWidget);

QString CORE_EXPORT nativeCanonicalFileName(const QString &pFileName);

QFrame CORE_EXPORT * newLineWidget(const bool &pHorizontal,
                                   const QColor &pColor,
                                   QWidget *pParent = 0);
QFrame CORE_EXPORT * newLineWidget(const bool &pHorizontal,
                                   QWidget *pParent = 0);
QFrame CORE_EXPORT * newLineWidget(const QColor &pColor,
                                   QWidget *pParent = 0);
QFrame CORE_EXPORT * newLineWidget(QWidget *pParent = 0);

QLabel CORE_EXPORT * newLabel(const QString &pText,
                              const double &pFontPercentage,
                              const bool &pBold, const bool &pItalic,
                              const Qt::Alignment &pAlignment,
                              QWidget *pParent = 0);
QLabel CORE_EXPORT * newLabel(const QString &pText,
                              const double &pFontPercentage,
                              const bool &pBold, const bool &pItalic,
                              QWidget *pParent = 0);
QLabel CORE_EXPORT * newLabel(const QString &pText,
                              const double &pFontPercentage,
                              const bool &pBold, QWidget *pParent = 0);
QLabel CORE_EXPORT * newLabel(const QString &pText,
                              const double &pFontPercentage,
                              QWidget *pParent = 0);
QLabel CORE_EXPORT * newLabel(const QString &pText, QWidget *pParent = 0);

QString CORE_EXPORT copyright();

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
