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
// Core CLI utilities
//==============================================================================

#ifndef CORECLIUTILS_H
#define CORECLIUTILS_H

//==============================================================================

#include "coreglobal.h"
#include "plugin.h"

//==============================================================================

#include <QAbstractMessageHandler>
#include <QByteArray>
#include <QDomDocument>
#include <QSourceLocation>
#include <QSslError>
#include <QUrl>

//==============================================================================

typedef QList<int> QIntList;

//==============================================================================

QIntList CORE_EXPORT qVariantListToIntList(const QVariantList &pVariantList);
QVariantList CORE_EXPORT qIntListToVariantList(const QIntList &pIntList);

QString CORE_EXPORT qDomDocumentToString(const QDomDocument &pDomDocument);

//==============================================================================

class QCoreApplication;
class QNetworkAccessManager;
class QNetworkReply;

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

#include "corecliutils.h.inl"

//==============================================================================

class CORE_EXPORT DummyMessageHandler : public QAbstractMessageHandler
{
    Q_OBJECT

protected:
    virtual void handleMessage(QtMsgType pType, const QString &pDescription,
                               const QUrl &pIdentifier,
                               const QSourceLocation &pSourceLocation);
};

//==============================================================================
// Note: both cliutils.h and corecliutils.h must specifically define
//       SynchronousTextFileDownloader. To have it in cliutils.h.inl is NOT good
//       enough since the MOC won't pick it up...

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

//==============================================================================

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

void CORE_EXPORT * globalInstance(const QString &pObjectName,
                                  void *pDefaultGlobalInstance);

void CORE_EXPORT setActiveDirectory(const QString &pDirName);
QString CORE_EXPORT activeDirectory();

QString CORE_EXPORT nativeCanonicalFileName(const QString &pFileName);

void CORE_EXPORT doNothing(const int &pMax);

void CORE_EXPORT checkFileNameOrUrl(const QString &pInFileNameOrUrl,
                                    bool &pOutIsLocalFile,
                                    QString &pOutFileNameOrUrl);

QString CORE_EXPORT stringToPercentEncoding(const QString &pString);
QString CORE_EXPORT stringFromPercentEncoding(const QString &pString);

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
