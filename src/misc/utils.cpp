#include "utils.h"

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QResource>

QString exec(const QString &pProg, const QString &pArgs)
{
    if (QFileInfo(pProg).exists()) {
        // The program exists, so we can execute it

        QProcess process;

        process.start(pProg,  QStringList() << pArgs);
        process.waitForFinished();

        return process.readAll().trimmed();
    } else {
        // The program doesn't exist, so...

        return QString();
    }
}

QString getOsName()
{
#ifdef Q_WS_WIN
    switch(QSysInfo::WindowsVersion)
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
        switch(QSysInfo::MacintoshVersion)
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

QByteArray resourceAsByteArray(const QString &pResource)
{
    // Retrieve a resource as a QByteArray

    QResource resource(pResource);

    if (resource.isValid()) {
        if (resource.isCompressed())
            // The resource is compressed, so uncompress it before returning it
            // Note: qUncompress returns a QByteArray, so we are fine...

            return qUncompress(reinterpret_cast<const uchar *>(resource.data()),
                               resource.size());
        else
            // The resource is not compressed, so just return it after doing the
            // right conversions

            return QByteArray(reinterpret_cast<const char *>(resource.data()),
                              resource.size());
    }
    else {
        // The resource doesn't exist, so...

        return QByteArray();
    }
}

void saveResourceAs(const QString &pResource, const QString &pFilename)
{
    if (QResource(pResource).isValid()) {
        // The resource exists, so write a file with name pFilename and which
        // contents is that of the resource which is retrieved as a QByteArray

        QFile file(pFilename);

        file.open(QIODevice::ReadWrite);
        file.write(resourceAsByteArray(pResource));
        file.close();
    }
}

#ifdef QT_GUI_LIB
#include <QApplication>
#include <QMessageBox>

void notYetImplemented(const QString &method)
{
    QMessageBox::information(0, qApp->applicationName()+" Information",
                             "Sorry, but the '"+method+"' method has not yet been implemented.");
}
#endif
