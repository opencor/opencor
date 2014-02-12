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
// CLI utilities
//==============================================================================

#ifndef CLIUTILS_H
#define CLIUTILS_H

//==============================================================================

#include "coreglobal.h"
#include "plugin.h"

//==============================================================================

#include <QByteArray>

//==============================================================================

class QCoreApplication;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

static const auto SettingsGlobal          = QStringLiteral("Global");
static const auto SettingsLocale          = QStringLiteral("Locale");
static const auto SettingsActiveDirectory = QStringLiteral("ActiveDirectory");

//==============================================================================

QString CORE_EXPORT osName();

QString CORE_EXPORT shortVersion(QCoreApplication *pApp);
QString CORE_EXPORT version(QCoreApplication *pApp);

QString CORE_EXPORT copyright();

QString CORE_EXPORT locale();

qulonglong CORE_EXPORT totalMemory();
qulonglong CORE_EXPORT freeMemory();

QString CORE_EXPORT sizeAsString(const double &pSize,
                                 const int &pPrecision = 1);

QByteArray CORE_EXPORT resourceAsByteArray(const QString &pResource);
bool CORE_EXPORT writeResourceToFile(const QString &pFilename,
                                     const QString &pResource);

bool CORE_EXPORT readTextFromFile(const QString &pFileName, QString &pText);
bool CORE_EXPORT writeTextToFile(const QString &pFilename,
                                 const QString &pText);

void CORE_EXPORT * globalInstance(const QString &pObjectName,
                                  void *pDefaultGlobalInstance);

void CORE_EXPORT setActiveDirectory(const QString &pDirName);
QString CORE_EXPORT activeDirectory();

QString CORE_EXPORT nativeCanonicalFileName(const QString &pFileName);

QString CORE_EXPORT formatErrorMessage(const QString &pErrorMessage);

QString CORE_EXPORT nonDiacriticString(const QString &pString);

void CORE_EXPORT doNothing(const int &pMax);

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
