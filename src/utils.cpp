#include "utils.h"

#include <QProcess>
#include <QSysInfo>

QString exec(const QString& prog, const QString& args)
{
    QProcess process;

    process.start(prog,  QStringList() << args);
    process.waitForFinished();

    return process.readAll().trimmed();
}

QString getOsName()
{
#ifdef Q_WS_WIN
    switch(QSysInfo::windowsVersion())
    {
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
        default:
            return "Microsoft Windows";
    }
#else
    #ifdef Q_WS_MAC
        switch(QSysInfo::windowsVersion())
        {
            case QSysInfo::MV_10_3:
                return "Mac OS X 10.3 (Jaguar)";
            case QSysInfo::MV_10_4:
                return "Mac OS X 10.4 (Panther)";
            case QSysInfo::MV_10_5:
                return "Mac OS X 10.5 (Leopard)";
            case QSysInfo::MV_10_6:
                return "Mac OS X 10.6 (Snow Leopard)";
            default:
                return "Mac OS X";
        }
    #else
        // Linux

        return exec("uname", "-o")+" "+exec("uname", "-r");
    #endif
#endif
}
