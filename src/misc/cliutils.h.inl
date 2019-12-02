/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Core CLI utilities
//==============================================================================

QString CORE_EXPORT locale();

QString CORE_EXPORT rawLocale();
void CORE_EXPORT setRawLocale(const QString &pRawLocale);

QString CORE_EXPORT shortVersion();
QString CORE_EXPORT version();

QString CORE_EXPORT pluginCategoryName(const PluginInfo::Category &pCategory);
QString CORE_EXPORT pluginCategoryDescription(const PluginInfo::Category &pCategory);

QString CORE_EXPORT canonicalDirName(const QString &pDirName);
QString CORE_EXPORT canonicalFileName(const QString &pFileName);
QStringList CORE_EXPORT canonicalFileNames(const QStringList &pFileNames);

int CORE_EXPORT exec(const QString &pProgram, const QStringList &pArgs,
                     QString &pOutput);
int CORE_EXPORT exec(const QString &pProgram, const QStringList &pArgs);

bool CORE_EXPORT hasInternetConnection();
QString CORE_EXPORT noInternetConnectionMessage();

QString CORE_EXPORT copyright();

QString CORE_EXPORT formatMessage(const QString &pMessage,
                                  bool pLowerCase = true,
                                  bool pDotDotDot = false);

QByteArray CORE_EXPORT resource(const QString &pResource);

QString CORE_EXPORT temporaryDirName();
QString CORE_EXPORT temporaryFileName(const QString &pExtension = ".tmp");

void CORE_EXPORT checkFileNameOrUrl(const QString &pInFileNameOrUrl,
                                    bool &pOutIsLocalFile,
                                    QString &pOutFileNameOrUrl);

bool CORE_EXPORT readFile(const QString &pFileNameOrUrl,
                          QByteArray &pFileContents,
                          QString *pErrorMessage = nullptr);
bool CORE_EXPORT readFile(const QString &pFileNameOrUrl, QString &pFileContents,
                          QString *pErrorMessage = nullptr);

bool CORE_EXPORT writeFile(const QString &pFileName,
                           const QByteArray &pFileContents);
bool CORE_EXPORT writeFile(const QString &pFileName,
                           const QString &pFileContents);

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
