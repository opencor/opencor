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

#include <QAbstractMessageHandler>
#include <QByteArray>
#include <QSourceLocation>
#include <QSslError>
#include <QUrl>

//==============================================================================

typedef QList<int> QIntList;

//==============================================================================

QIntList CORE_EXPORT qVariantListToIntList(const QVariantList &pVariantList);
QVariantList CORE_EXPORT qIntListToVariantList(const QIntList &pIntList);

//==============================================================================

class QCoreApplication;
class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

#ifndef OpenCOR_MAIN
class CORE_EXPORT DummyMessageHandler : public QAbstractMessageHandler
{
    Q_OBJECT

protected:
    virtual void handleMessage(QtMsgType pType, const QString &pDescription,
                               const QUrl &pIdentifier,
                               const QSourceLocation &pSourceLocation);
};
#endif

//==============================================================================

#ifndef OpenCOR_MAIN
class SynchronousTextFileDownloader : public QObject
{
    Q_OBJECT

public:
    bool readTextFromUrl(const QString &pUrl, QString &pText,
                         QString *pErrorMessage) const;

private Q_SLOTS:
    void networkAccessManagerSslErrors(QNetworkReply *pNetworkReply,
                                       const QList<QSslError> &pSslErrors);
};
#endif

//==============================================================================

QString CORE_EXPORT osName();

QString CORE_EXPORT copyright();

QString CORE_EXPORT locale();

qulonglong CORE_EXPORT totalMemory();
qulonglong CORE_EXPORT freeMemory();

QString CORE_EXPORT digitGroupNumber(const QString &pNumber);

QString CORE_EXPORT sizeAsString(const double &pSize,
                                 const int &pPrecision = 1);

QString CORE_EXPORT sha1(const QString &pText);

void CORE_EXPORT stringPositionAsLineColumn(const QString &pString,
                                            const QString &pEol,
                                            const int &pPosition, int &pLine,
                                            int &pColumn);
void CORE_EXPORT stringLineColumnAsPosition(const QString &pString,
                                            const QString &pEol,
                                            const int &pLine,
                                            const int &pColumn, int &pPosition);

QByteArray CORE_EXPORT resourceAsByteArray(const QString &pResource);
bool CORE_EXPORT writeResourceToFile(const QString &pFilename,
                                     const QString &pResource);

#ifndef OpenCOR_MAIN
bool CORE_EXPORT readTextFromUrl(const QString &pUrl, QString &pText,
                                 QString *pErrorMessage = 0);
#endif

bool CORE_EXPORT readTextFromFile(const QString &pFileName, QString &pText);
bool CORE_EXPORT writeTextToFile(const QString &pFilename,
                                 const QString &pText);

void CORE_EXPORT * globalInstance(const QString &pObjectName,
                                  void *pDefaultGlobalInstance);

void CORE_EXPORT setActiveDirectory(const QString &pDirName);
QString CORE_EXPORT activeDirectory();

QString CORE_EXPORT nativeCanonicalFileName(const QString &pFileName);

QString CORE_EXPORT formatErrorMessage(const QString &pErrorMessage,
                                       const bool &pLowerCase = true,
                                       const bool &pDotDotDot = false);

QString CORE_EXPORT nonDiacriticString(const QString &pString);

void CORE_EXPORT doNothing(const int &pMax);

void CORE_EXPORT checkFileNameOrUrl(const QString &pInFileNameOrUrl,
                                    bool &pOutIsLocalFile,
                                    QString &pOutFileNameOrUrl);

QString CORE_EXPORT stringToPercentEncoding(const QString &pString);
QString CORE_EXPORT stringFromPercentEncoding(const QString &pString);

QString CORE_EXPORT eolString();

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
