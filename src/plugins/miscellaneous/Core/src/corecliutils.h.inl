/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Core CLI utilities
//==============================================================================

QString CORE_EXPORT locale();

QString CORE_EXPORT rawLocale();
void CORE_EXPORT setRawLocale(const QString &pRawLocale);

QString CORE_EXPORT shortVersion();
QString CORE_EXPORT version();

QString CORE_EXPORT nativeCanonicalDirName(const QString &pDirName);
QString CORE_EXPORT nativeCanonicalFileName(const QString &pFileName);
QStringList CORE_EXPORT nativeCanonicalFileNames(const QStringList &pFileNames);

QString CORE_EXPORT exec(const QString &pProgram,
                         const QStringList &pArgs = QStringList());

bool CORE_EXPORT internetConnectionAvailable();
QString CORE_EXPORT noInternetConnectionAvailableMessage();

QString CORE_EXPORT copyright();

QString CORE_EXPORT formatMessage(const QString &pMessage,
                                  const bool &pLowerCase = true,
                                  const bool &pDotDotDot = false);

QByteArray CORE_EXPORT resource(const QString &pResource);

QString CORE_EXPORT temporaryDirName();
QString CORE_EXPORT temporaryFileName(const QString &pExtension = ".tmp");

bool CORE_EXPORT readFileContentsFromFile(const QString &pFileName,
                                          QByteArray &pFileContents);
bool CORE_EXPORT readFileContentsFromUrl(const QString &pUrl,
                                         QByteArray &pFileContents,
                                         QString *pErrorMessage = 0);

bool CORE_EXPORT writeFileContentsToFile(const QString &pFileName,
                                         const QByteArray &pFileContents);
bool CORE_EXPORT writeResourceToFile(const QString &pFileName,
                                     const QString &pResource);

bool CORE_EXPORT isTextFile(const QString &pFileName);

QString CORE_EXPORT eolString();

QString CORE_EXPORT nonDiacriticString(const QString &pString);

QString CORE_EXPORT plainString(const QString &pString);

QString CORE_EXPORT urlArguments(const QUrl &pUrl);

QString CORE_EXPORT stringToPercentEncoding(const QString &pString);
QString CORE_EXPORT stringFromPercentEncoding(const QString &pString);

//==============================================================================
// End of file
//==============================================================================
