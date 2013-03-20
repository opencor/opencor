//==============================================================================
// Some useful functions
//==============================================================================

#include "utils.h"

//==============================================================================

#include <QCoreApplication>
#include <QDate>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QResource>

//==============================================================================

namespace OpenCOR {

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

QString getOsName()
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
    // Note: from version 10.7, Apple uses OS X rather than Mac OS X...

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
        return "OS X 10.7 (Lion)";
    case QSysInfo::MV_10_8:
        return "OS X 10.8 (Mountain Lion)";
    default:
        return "Mac OS";
        // Note: we return Mac OS rather than Mac OS X or even OS X since we
        //       handle all the recent versions...
    }
#else
    #error Unsupported platform
#endif
}

//==============================================================================

QString getAppVersion(QCoreApplication *pApp)
{
    QString appVersion = pApp->applicationVersion();
    QString bitVersion;
    static const int SizeOfPointer = sizeof(void *);

    switch (SizeOfPointer) {
    case 4:
        bitVersion = "32-bit";

        break;
    case 8:
        bitVersion = "64-bit";

        break;
    default:
        // Not a size that we could recognise, so...

        bitVersion = "";
    }

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

QString getAppCopyright(const bool &pHtml)
{
    return QObject::tr("Copyright")+" "+QString(pHtml?"&copy;":"")+"2011-"+QString::number(QDate().currentDate().year());
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
