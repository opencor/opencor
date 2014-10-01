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

#include "coresettings.h"
#include "cliutils.h"
#include "settings.h"

//==============================================================================

#include <QtMath>

//==============================================================================

#include <QChar>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDate>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QIODevice>
#include <QLocale>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QResource>
#include <QSettings>
#include <QString>
#include <QStringList>

//==============================================================================

#if defined(Q_OS_WIN)
    #include <Windows.h>
#elif defined(Q_OS_LINUX)
    #include <unistd.h>
#elif defined(Q_OS_MAC)
    #include <mach/host_info.h>
    #include <mach/mach_host.h>
    #include <sys/sysctl.h>
#endif

//==============================================================================

QIntList qVariantListToIntList(const QVariantList &pVariantList)
{
    // Convert the given list of variants to a list of integers

    QIntList res = QIntList();

    foreach (const QVariant &variant, pVariantList)
        res << variant.toInt();

    return res;
}

//==============================================================================

QVariantList qIntListToVariantList(const QIntList &pIntList)
{
    // Convert the given list of integers to a list of variants

    QVariantList res = QVariantList();

    foreach (const int &nb, pIntList)
        res << nb;

    return res;
}

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

#ifndef OpenCOR_MAIN
void DummyMessageHandler::handleMessage(QtMsgType pType,
                                        const QString &pDescription,
                                        const QUrl &pIdentifier,
                                        const QSourceLocation &pSourceLocation)
{
    Q_UNUSED(pType);
    Q_UNUSED(pDescription);
    Q_UNUSED(pIdentifier);
    Q_UNUSED(pSourceLocation);

    // We ignore the message...
}
#endif

//==============================================================================

#ifndef OpenCOR_MAIN
bool SynchronousTextFileDownloader::readTextFromUrl(const QString &pUrl,
                                                    QString &pText,
                                                    QString *pErrorMessage) const
{
    // Create a network access manager so that we can then retrieve the contents
    // of the remote file

    QNetworkAccessManager networkAccessManager;

    // Make sure that we get told if there are SSL errors (which would happen if
    // a website's certificate is invalid, e.g. it has expired)

    connect(&networkAccessManager, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(networkAccessManagerSslErrors(QNetworkReply *, const QList<QSslError> &)) );

    // Download the contents of the remote file

    QNetworkReply *networkReply = networkAccessManager.get(QNetworkRequest(pUrl));
    QEventLoop eventLoop;

    connect(networkReply, SIGNAL(finished()),
            &eventLoop, SLOT(quit()));

    eventLoop.exec();

    // Check whether we were able to retrieve the contents of the file

    bool res = networkReply->error() == QNetworkReply::NoError;

    if (res) {
        pText = networkReply->readAll();

        if (pErrorMessage)
            *pErrorMessage = QString();
    } else {
        pText = QString();

        if (pErrorMessage)
            *pErrorMessage = networkReply->errorString();
    }

    // Delete (later) the network reply

    networkReply->deleteLater();

    return res;
}
#endif

//==============================================================================

#ifndef OpenCOR_MAIN
void SynchronousTextFileDownloader::networkAccessManagerSslErrors(QNetworkReply *pNetworkReply,
                                                                  const QList<QSslError> &pSslErrors)
{
    // Ignore the SSL errors since we assume the user knows what s/he is doing

    pNetworkReply->ignoreSslErrors(pSslErrors);
}
#endif

//==============================================================================

QString exec(const QString &pProgram, const QStringList &pArgs = QStringList())
{
    // Execute and return the output of a program given its arguments

    QProcess process;

    process.start(pProgram, pArgs);
    process.waitForFinished();

    return process.readAll().trimmed();
}

//==============================================================================

QString osName()
{
#if defined(Q_OS_WIN)
    switch (QSysInfo::WindowsVersion) {
    case QSysInfo::WV_NT:
        return "Microsoft Windows NT";
    case QSysInfo::WV_2000:
        return "Microsoft Windows 2000";
    case QSysInfo::WV_XP:
        return "Microsoft Windows XP";
    case QSysInfo::WV_2003:
        return "Microsoft Windows 2003";
    case QSysInfo::WV_VISTA:
        return "Microsoft Windows Vista";
    case QSysInfo::WV_WINDOWS7:
        return "Microsoft Windows 7";
    case QSysInfo::WV_WINDOWS8:
        return "Microsoft Windows 8";
    default:
        return "Microsoft Windows";
    }
#elif defined(Q_OS_LINUX)
    QString os = exec("uname", QStringList() << "-o");

    if (os.isEmpty())
        // We couldn't find uname or something went wrong, so simple return
        // "Linux" as the OS name

        return "Linux";
    else
        return os+" "+exec("uname", QStringList() << "-r");
#elif defined(Q_OS_MAC)
    // Note: from version 10.8, Apple officially uses OS X rather than Mac OS
    //       X...

    switch (QSysInfo::MacintoshVersion) {
    case QSysInfo::MV_9:
        return "Mac OS 9";
    case QSysInfo::MV_10_0:
        return "Mac OS X 10.0 (Cheetah)";
    case QSysInfo::MV_10_1:
        return "Mac OS X 10.1 (Puma)";
    case QSysInfo::MV_10_2:
        return "Mac OS X 10.2 (Jaguar)";
    case QSysInfo::MV_10_3:
        return "Mac OS X 10.3 (Panther)";
    case QSysInfo::MV_10_4:
        return "Mac OS X 10.4 (Tiger)";
    case QSysInfo::MV_10_5:
        return "Mac OS X 10.5 (Leopard)";
    case QSysInfo::MV_10_6:
        return "Mac OS X 10.6 (Snow Leopard)";
    case QSysInfo::MV_10_7:
        return "Mac OS X 10.7 (Lion)";
    case QSysInfo::MV_10_8:
        return "OS X 10.8 (Mountain Lion)";
    case QSysInfo::MV_10_9:
        return "OS X 10.9 (Mavericks)";
    default:
        return "Mac OS";
        // Note: we return Mac OS rather than Mac OS X or even OS X since only
        //       old versions are not handled...
    }
#else
    #error Unsupported platform
#endif
}

//==============================================================================

QString copyright()
{
    return QObject::tr("Copyright")+" 2011-"+QString::number(QDate::currentDate().year());
}

//==============================================================================

QString locale()
{
    // Return our current locale
    // Note: this gets set in MainWindow::setLocale()...

    QString res;
    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        res = settings.value(SettingsLocale).toString();
    settings.endGroup();

    return res;
}

//==============================================================================

qulonglong totalMemory()
{
    // Retrieve and return in bytes the total amount of physical memory

    qulonglong res = 0;

#if defined(Q_OS_WIN)
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(memoryStatus);

    GlobalMemoryStatusEx(&memoryStatus);

    res = qulonglong(memoryStatus.ullTotalPhys);
#elif defined(Q_OS_LINUX)
    res = qulonglong(sysconf(_SC_PHYS_PAGES))*qulonglong(sysconf(_SC_PAGESIZE));
#elif defined(Q_OS_MAC)
    int mib[2];

    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;

    size_t len = sizeof(res);

    sysctl(mib, 2, &res, &len, 0, 0);
#else
    #error Unsupported platform
#endif

    return res;
}

//==============================================================================

qulonglong freeMemory()
{
    // Retrieve and return in bytes the available amount of physical memory

    qulonglong res = 0;

#if defined(Q_OS_WIN)
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(memoryStatus);

    GlobalMemoryStatusEx(&memoryStatus);

    res = qulonglong(memoryStatus.ullAvailPhys);
#elif defined(Q_OS_LINUX)
    res = qulonglong(sysconf(_SC_AVPHYS_PAGES))*qulonglong(sysconf(_SC_PAGESIZE));
#elif defined(Q_OS_MAC)
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;

    host_statistics(mach_host_self(), HOST_VM_INFO,
                    host_info_t(&vmStats), &infoCount);

    res = (qulonglong(vmStats.free_count)+qulonglong(vmStats.inactive_count))*qulonglong(vm_page_size);
#else
    #error Unsupported platform
#endif

    return res;
}

//==============================================================================

QString digitGroupNumber(const QString &pNumber)
{
    // Digit group the given number (which we assume to be specified in the "C"
    // locale), if possible, and return the result of the digit grouping (which
    // is done keeping in mind the current locale)

    QString res = pNumber;
    bool validNumber;
    double number = res.toDouble(&validNumber);

    if (validNumber)
        res = QLocale().toString(number);

    return res;
}

//==============================================================================

QString sizeAsString(const double &pSize, const int &pPrecision)
{
    // Note: pSize is a double rather than a qulonglong, in case we need to
    //       convert an insane size...

    QString units[9] = { QObject::tr("B"), QObject::tr("KB"), QObject::tr("MB"),
                         QObject::tr("GB"), QObject::tr("TB"), QObject::tr("PB"),
                         QObject::tr("EB"), QObject::tr("ZB"), QObject::tr("YB") };

    int i = qFloor(log(pSize)/log(1024.0));
    double size = pSize/qPow(1024.0, i);
    double scaling = qPow(10.0, pPrecision);

    size = qRound(scaling*size)/scaling;

    return QLocale().toString(size)+" "+units[i];
}

//==============================================================================

QString sha1(const QString &pText)
{
    // Return the SHA-1 value of the given text

    return QCryptographicHash::hash(pText.toUtf8(),
                                    QCryptographicHash::Sha1).toHex();
}

//==============================================================================

void stringPositionAsLineColumn(const QString &pString, const QString &pEol,
                                const int &pPosition, int &pLine, int &pColumn)
{
    // Determine the line and column values of the given position within the
    // given string using the given end of line

    if ((pPosition < 0) || (pPosition >= pString.length())) {
        // Invalid position values

        pLine = -1;
        pColumn = -1;
    } else {
        pLine = pString.left(pPosition).count(pEol)+1;
        pColumn = pPosition-((pPosition >= pEol.length())?pString.lastIndexOf(pEol, pPosition-pEol.length()):-1);
    }
}

//==============================================================================

void stringLineColumnAsPosition(const QString &pString, const QString &pEol,
                                const int &pLine, const int &pColumn,
                                int &pPosition)
{
    // Determine the position value of the given line and column within the
    // given string using the given end of line

    if ((pLine < 1) || (pColumn < 1)) {
        // Invalid line and/or colunn values

        pPosition = -1;
    } else {
        pPosition = 0;

        for (int i = 1; i < pLine; ++i) {
            int pos = pString.indexOf(pEol, pPosition);

            if (pos == -1) {
                // Invalid line value

                pPosition = -1;

                return;
            } else {
                pPosition = pos+pEol.length();
            }
        }

        pPosition += pColumn-1;

        // Make sure that the column value is actually valid by trying to
        // retrieve the line and column values from our computed position

        int testLine;
        int testColumn;

        stringPositionAsLineColumn(pString, pEol, pPosition, testLine, testColumn);

        if ((testLine != pLine) || (testColumn != pColumn))
            pPosition = -1;
    }
}

//==============================================================================

QByteArray resourceAsByteArray(const QString &pResource)
{
    // Retrieve a resource as a QByteArray

    QResource resource(pResource);

    if (resource.isValid()) {
        if (resource.isCompressed())
            // The resource is compressed, so uncompress it before returning it

            return qUncompress(resource.data(), resource.size());
        else
            // The resource is not compressed, so just return it after doing the
            // right conversion

            return QByteArray(reinterpret_cast<const char *>(resource.data()),
                              resource.size());
    }
    else {
        return QByteArray();
    }
}

//==============================================================================

bool writeResourceToFile(const QString &pFilename, const QString &pResource)
{
    if (QResource(pResource).isValid()) {
        // The resource exists, so write a file with name pFilename and which
        // contents is that of the resource which is retrieved as a QByteArray

        QFile file(pFilename);

        if (file.open(QIODevice::WriteOnly)) {
            QByteArray resource = resourceAsByteArray(pResource);
            bool res;

            res = file.write(resource) == resource.size();

            file.close();

            // Remove the newly created file in case not all the resource was
            // written to it

            if (!res)
                file.remove();

            return res;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

//==============================================================================

bool readTextFromFile(const QString &pFileName, QString &pText)
{
    // Read the contents of the file, which file name is given, as a string

    QFile file(pFileName);

    if (file.open(QIODevice::ReadOnly)) {
        pText = file.readAll();

        file.close();

        return true;
    } else {
        pText = QString();

        return false;
    }
}

//==============================================================================

#ifndef OpenCOR_MAIN
bool readTextFromUrl(const QString &pUrl, QString &pText,
                     QString *pErrorMessage)
{
    // Read the contents of the file, which URL is given, as a string

    static SynchronousTextFileDownloader synchronousTextFileDownloader;

    return synchronousTextFileDownloader.readTextFromUrl(pUrl, pText, pErrorMessage);
}
#endif

//==============================================================================

bool writeTextToFile(const QString &pFilename, const QString &pText)
{
    // Write the given string to a file with the given file name

    QFile file(pFilename);

    if (file.open(QIODevice::WriteOnly)) {
        bool res = file.write(pText.toUtf8()) != -1;

        file.close();

        // Remove the newly created file in case the string couldn't be written
        // to it

        if (!res)
            file.remove();

        return res;
    } else {
        return false;
    }
}

//==============================================================================

void * globalInstance(const QString &pObjectName, void *pDefaultGlobalInstance)
{
    // Retrieve the 'global' instance of an object
    // Note: initially, the plan was to have a static instance of an object and
    //       return its address. However, this approach doesn't work on Windows
    //       and Linux (but does on OS X). Indeed, say that the Core plugin is
    //       required by two other plugins, then these two plugins won't get the
    //       same 'copy' of the Core plugin. (It seems like) each 'copy' gets
    //       its own address space. (This is not the case on OS X, (most likely)
    //       because of the way applications are bundled on that platform.) So,
    //       to address this issue, we keep track of the address of a 'global'
    //       instance using QSettings...

    QSettings settings(SettingsOrganization, SettingsApplication);
    qulonglong globalInstance;

    settings.beginGroup(SettingsGlobal);
        globalInstance = settings.value(pObjectName, 0).toULongLong();

        if (!globalInstance) {
            // There is no 'global' instance associated with the given object,
            // so use the object's default 'global' instance we were given

            globalInstance = qulonglong(pDefaultGlobalInstance);

            settings.setValue(pObjectName, QString::number(globalInstance));
            // Note #1: for some reasons, on OS X, QSettings doesn't handle
            //          qulonglong values properly, so we do it through a
            //          QString value instead...
            // Note #2: see
            //          https://bugreports.qt-project.org/browse/QTBUG-29681 for
            //          more information...
        }
    settings.endGroup();

    // Return the class's 'global' instance

    return (void *) globalInstance;
}

//==============================================================================

void CORE_EXPORT setActiveDirectory(const QString &pDirName)
{
    // Keep track of the active directory

    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        settings.setValue(SettingsActiveDirectory, pDirName);
    settings.endGroup();
}

//==============================================================================

QString CORE_EXPORT activeDirectory()
{
    // Retrieve and return the active directory

    QString res;
    QSettings settings(SettingsOrganization, SettingsApplication);

    settings.beginGroup(SettingsGlobal);
        res = settings.value(SettingsActiveDirectory).toString();
    settings.endGroup();

    return res;
}

//==============================================================================

QString nativeCanonicalFileName(const QString &pFileName)
{
    // Return a native and canonical version of the given file name or the given
    // file name, the native and canonical version is empty (i.e. the file
    // doesn't exist (anymore?))

    QString res = QDir::toNativeSeparators(QFileInfo(pFileName).canonicalFilePath());

    return res.isEmpty()?pFileName:res;
}

//==============================================================================

QString formatErrorMessage(const QString &pErrorMessage, const bool &pLowerCase,
                           const bool &pDotDotDot)
{
    static const QString DotDotDot = "...";

    if (pErrorMessage.isEmpty())
        return pDotDotDot?DotDotDot:QString();

    // Format and return the error message

    QString errorMessage = pErrorMessage;

    // Upper/lower the case of the first character, unless the message is one
    // character long (!!) or unless its second character is in lower case

    if (    (errorMessage.size() <= 1)
        || ((errorMessage.size() > 1) && errorMessage[1].isLower())) {
        if (pLowerCase)
            errorMessage[0] = errorMessage[0].toLower();
        else
            errorMessage[0] = errorMessage[0].toUpper();
    }

    // Return the error message after making sure that its end finishes with
    // "...", if requested

    int subsize = errorMessage.size();

    while (subsize && (errorMessage[subsize-1] == '.'))
        --subsize;

    return errorMessage.left(subsize)+(pDotDotDot?DotDotDot:QString());
}

//==============================================================================

QString nonDiacriticString(const QString &pString)
{
    // Remove and return a non-accentuated version of the given string
    // Note: this code is based on the one that can be found at
    //       http://stackoverflow.com/questions/14009522/how-to-remove-accents-diacritic-marks-from-a-string-in-qt

    static QString diacriticLetters = QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");
    static QStringList nonDiacriticLetters = QStringList() << "S" << "OE" << "Z" << "s" << "oe" << "z" << "Y" << "Y" << "u" << "A" << "A" << "A" << "A" << "A" << "A" << "AE" << "C" << "E" << "E" << "E" << "E" << "I" << "I" << "I" << "I" << "D" << "N" << "O" << "O" << "O" << "O" << "O" << "O" << "U" << "U" << "U" << "U" << "Y" << "s" << "a" << "a" << "a" << "a" << "a" << "a" << "ae" << "c" << "e" << "e" << "e" << "e" << "i" << "i" << "i" << "i" << "o" << "n" << "o" << "o" << "o" << "o" << "o" << "o" << "u" << "u" << "u" << "u" << "y" << "y";

    QString res = QString();

    for (int i = 0, iMax = pString.length(); i < iMax; ++i) {
        QChar letter = pString[i];
        int dIndex = diacriticLetters.indexOf(letter);

        res.append((dIndex < 0)?letter:nonDiacriticLetters[dIndex]);
    }

    return res;
}

//==============================================================================

#ifdef Q_OS_WIN
    #pragma optimize("", off)
#endif

void doNothing(const int &pMax)
{
    // A silly function, which aim is simply to do nothing
    // Note: this function came about because there is no way, on Windows, to
    //       pause a thread for less than a millisecond (and this is in the best
    //       of cases)...

    for (int i = 0; i < pMax; ++i)
#ifdef Q_OS_WIN
        ;
#else
        asm("nop");
#endif
}

#ifdef Q_OS_WIN
    #pragma optimize("", on)
#endif

//==============================================================================

void checkFileNameOrUrl(const QString &pInFileNameOrUrl, bool &pOutIsLocalFile,
                        QString &pOutFileNameOrUrl)
{
    // Determine whether pInFileNameOrUrl refers to a local file or a remote
    // one, and set pOutIsLocalFile and pOutFileNameOrUrl accordingly
    // Note #1: to use QUrl::isLocalFile() is not enough. Indeed, say that
    //          pInFileNameOrUrl is equal to
    //              /home/me/mymodel.cellml
    //          then QUrl(pInFileNameOrUrl).isLocalFile() will be false. For it
    //          to be true, we would have to initialise the QUrl object using
    //          QUrl::fromLocalFile(), but we can't do that since we don't know
    //          whether pInFileNameOrUrl refers to a local file or not. So,
    //          instead we test for the scheme and host of the QUrl object...
    // Note #2: a local file can be passed as a URL. For example,
    //              file:///home/me/mymodel.cellml
    //          is a URL, but effectively a local file, hence pOutIsLocalFile is
    //          to be true and pOutFileNameOrUrl is to be set to
    //              /home/me/mymodel.cellml

    QUrl fileNameOrUrl = pInFileNameOrUrl;

    pOutIsLocalFile =    !fileNameOrUrl.scheme().compare("file")
                      ||  fileNameOrUrl.host().isEmpty();
    pOutFileNameOrUrl = pOutIsLocalFile?
                            !fileNameOrUrl.scheme().compare("file")?
                                nativeCanonicalFileName(fileNameOrUrl.toLocalFile()):
                                nativeCanonicalFileName(pInFileNameOrUrl):
                            fileNameOrUrl.url();
}

//==============================================================================

QString stringToPercentEncoding(const QString &pString)
{
    // Convert the given string to one with percent encoding

    return QUrl::toPercentEncoding(pString);
}

//==============================================================================

QString stringFromPercentEncoding(const QString &pString)
{
    // Remove the percent encoding from the given string

    return QUrl::fromPercentEncoding(pString.toUtf8());
}

//==============================================================================

QString eolString()
{
    // Return the end of line to use

#ifdef Q_OS_WIN
    return "\r\n";
#else
    // Note: before OS X, the EOL string would have been "\r", but since OS X it
    //       is the same as on Linux (i.e. "\n") and since we don't support
    //       versions prior to OS X...

    return "\n";
#endif
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
