/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

QString CORE_EXPORT nativeCanonicalDirName(const QString &pDirName);
QString CORE_EXPORT nativeCanonicalFileName(const QString &pFileName);
QStringList CORE_EXPORT nativeCanonicalFileNames(const QStringList &pFileNames);

int CORE_EXPORT exec(const QString &pProgram, const QStringList &pArgs,
                     QString &pOutput);
int CORE_EXPORT exec(const QString &pProgram, const QStringList &pArgs);

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
bool CORE_EXPORT readFileContentsFromFile(const QString &pFileName,
                                          QString &pFileContents);

bool CORE_EXPORT readFileContentsFromUrl(const QString &pUrl,
                                         QByteArray &pFileContents,
                                         QString *pErrorMessage = 0);
bool CORE_EXPORT readFileContentsFromUrl(const QString &pUrl,
                                         QString &pFileContents,
                                         QString *pErrorMessage = 0);

bool CORE_EXPORT writeFileContentsToFile(const QString &pFileName,
                                         const QByteArray &pFileContents);
bool CORE_EXPORT writeFileContentsToFile(const QString &pFileName,
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

template <typename T> void resetList(QList<T> &pList)
{
    // Reset the contents of the list
    // Note: to have the implementation in corecliutils.cpp.inl results in the
    //       linker telling us that reset() cannot be resolved (why?!), so have
    //       it here instead...

    for (int i = 0, iMax = pList.size(); i < iMax; ++i)
        delete pList[i];

    pList.clear();
};

//==============================================================================
// End of file
//==============================================================================
