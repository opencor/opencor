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

#include "cliutils.h"

//==============================================================================

#include <QChar>
#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QFile>
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

namespace OpenCOR {
namespace Core {

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
        // We couldn't find uname or something went wrong, so...

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

QString version(QCoreApplication *pApp)
{
    QString appVersion = pApp->applicationVersion();
    QString bitVersion;
    enum {
        SizeOfPointer = sizeof(void *)
    };

    if (SizeOfPointer == 4)
        bitVersion = "32-bit";
    else if (SizeOfPointer == 8)
        bitVersion = "64-bit";
    else
        // Not a size that we could recognise, so...

        bitVersion = "";

    bool snapshot = appVersion.contains("-");
    QString res = pApp->applicationName()+" ";

    if (snapshot)
        res += "[";

    res += appVersion;

    if (snapshot)
        res += "]";

    if (!bitVersion.isEmpty())
        res += " ("+bitVersion+")";

    return res;
}

//==============================================================================

QString copyright()
{
    return QObject::tr("Copyright")+" ©2011-"+QString::number(QDate::currentDate().year());
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
        // The resource doesn't exist, so...

        return QByteArray();
    }
}

//==============================================================================

bool saveResourceAs(const QString &pResource, const QString &pFilename)
{
    if (QResource(pResource).isValid()) {
        // The resource exists, so write a file with name pFilename and which
        // contents is that of the resource which is retrieved as a QByteArray

        QFile file(pFilename);

        if (file.open(QIODevice::ReadWrite)) {
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

QString formatErrorMsg(const QString &pErrorMsg)
{
    static const auto DotDotDot = QStringLiteral("...");

    if (pErrorMsg.isEmpty())
        return DotDotDot;

    // Format and return the error message

    QString errorMsg = pErrorMsg;

    // Lower the case of the first character, but if the message is one
    // character long (!!) or if its second character is not in upper case

    if (    (errorMsg.size() <= 1)
        || ((errorMsg.size() > 1) && !errorMsg[1].isUpper())) {
        errorMsg = errorMsg[0].toLower()+errorMsg.right(errorMsg.size()-1);
    }

    // Return the error message after making sure that its end finishes with
    // "..."

    int subsize = errorMsg.size();

    while (subsize && (errorMsg[subsize-1] == '.'))
        --subsize;

    return errorMsg.left(subsize)+DotDotDot;
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

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
