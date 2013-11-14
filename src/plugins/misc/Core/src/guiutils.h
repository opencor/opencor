/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// GUI utilities
//==============================================================================

#ifndef GUIUTILS_H
#define GUIUTILS_H

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

#pragma GCC diagnostic ignored "-Wunused-variable"
// Note: the following constants are defined here but used elsewhere, hence we
//       temporarily ignore warnings about unused variables...

static const char *SettingsBorderColor    = "BorderColor";
static const char *SettingsBaseColor      = "BaseColor";
static const char *SettingsWindowColor    = "WindowColor";
static const char *SettingsHighlightColor = "HighlightColor";

#pragma GCC diagnostic warning "-Wunused-variable"

//==============================================================================

QStringList CORE_EXPORT getOpenFileNames(const QString &pCaption,
                                         const QString &pFilter);
QString CORE_EXPORT getSaveFileName(const QString &pCaption,
                                    const QString &pFileName,
                                    const QString &pFilter);

void CORE_EXPORT setFocusTo(QWidget *pWidget);

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

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
