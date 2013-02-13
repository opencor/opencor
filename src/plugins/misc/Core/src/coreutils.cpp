//==============================================================================
// Core utilities
//==============================================================================

#include "commonwidget.h"
#include "coreutils.h"

//==============================================================================

#include <QApplication>
#include <QDate>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>
#include <QProcess>
#include <QResource>
#include <QSettings>
#include <QTextEdit>
#include <QVariant>
#include <QWidget>

//==============================================================================

#include <qmath.h>

//==============================================================================

#if defined(Q_OS_WIN)
    #include <Windows.h>
#elif defined(Q_OS_MAC)
    #include <mach/host_info.h>
    #include <mach/mach_host.h>
    #include <sys/sysctl.h>
#endif

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

QString sizeAsString(const double &pSize, const int &pPrecision)
{
    // Note: pSize is a double rather than a size_t (as is the case when we want
    //       to retrieve the total/free amount of memory available; see below)
    //       in case we need to convert an insane size...

    static const double OneKiloByte = qPow(2.0, 10.0);
    static const double OneOverOneKiloByte = 1.0/OneKiloByte;

    static const double OneMegaByte = qPow(2.0, 20.0);
    static const double OneOverOneMegaByte = 1.0/qPow(2.0, 20.0);

    static const double OneGigaByte = qPow(2.0, 30.0);
    static const double OneOverOneGigaByte = 1.0/qPow(2.0, 30.0);

    static const double OneTeraByte = qPow(2.0, 40.0);
    static const double OneOverOneTeraByte = 1.0/qPow(2.0, 40.0);

    static const double OnePetaByte = qPow(2.0, 50.0);
    static const double OneOverOnePetaByte = 1.0/qPow(2.0, 50.0);

    static const double OneExaByte = qPow(2.0, 60.0);
    static const double OneOverOneExaByte = 1.0/qPow(2.0, 60.0);

    double scaling = qPow(10.0, pPrecision);

    if (pSize < OneKiloByte)
        return QString::number(pSize)+" "+QObject::tr("B");
    else if (pSize < OneMegaByte)
        return QString::number(ceil(scaling*pSize*OneOverOneKiloByte)/scaling)+" "+QObject::tr("KB");
    else if (pSize < OneGigaByte)
        return QString::number(ceil(scaling*pSize*OneOverOneMegaByte)/scaling)+" "+QObject::tr("MB");
    else if (pSize < OneTeraByte)
        return QString::number(ceil(scaling*pSize*OneOverOneGigaByte)/scaling)+" "+QObject::tr("GB");
    else if (pSize < OnePetaByte)
        return QString::number(ceil(scaling*pSize*OneOverOneTeraByte)/scaling)+" "+QObject::tr("TB");
    else if (pSize < OneExaByte)
        return QString::number(ceil(scaling*pSize*OneOverOnePetaByte)/scaling)+" "+QObject::tr("PB");
    else
        return QString::number(ceil(scaling*pSize*OneOverOneExaByte)/scaling)+" "+QObject::tr("EB");
}

//==============================================================================

size_t totalMemory()
{
    // Retrieve and return in bytes the total amount of physical memory

    size_t res = 0;

#if defined(Q_OS_WIN)
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(memoryStatus);

    GlobalMemoryStatusEx(&memoryStatus);

    res = memoryStatus.ullTotalPhys;
#elif defined(Q_OS_LINUX)
    res = sysconf(_SC_PHYS_PAGES)*sysconf(_SC_PAGESIZE);
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

size_t freeMemory()
{
    // Retrieve and return in bytes the available amount of physical memory

    size_t res = 0;

#if defined(Q_OS_WIN)
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(memoryStatus);

    GlobalMemoryStatusEx(&memoryStatus);

    res = memoryStatus.ullAvailPhys;
#elif defined(Q_OS_LINUX)
    res = sysconf(_SC_AVPHYS_PAGES)*sysconf(_SC_PAGESIZE);
#elif defined(Q_OS_MAC)
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;

    host_statistics(mach_host_self(), HOST_VM_INFO,
                    host_info_t(&vmStats), &infoCount);

    res = (vmStats.free_count+vmStats.inactive_count)*vm_page_size;
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
            // Note: for some reasons, on Windows and OS X, QSettings doesn't
            //       handle qulonglong values properly, so we do it through a
            //       QString value instead...
            //       (See https://bugreports.qt-project.org/browse/QTBUG-29681)
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

QStringList CORE_EXPORT getOpenFileNames(const QString &pCaption,
                                         const QString &pFilter)
{
    // Retrieve and return one or several open file names

    QStringList res = QFileDialog::getOpenFileNames(qApp->activeWindow(),
                                                    pCaption,
                                                    activeDirectory(),
                                                    pFilter);

    if (res.count())
        // We have retrieve at least one open file name, so we can keep track of
        // the folder in which it is
        // Note #1: we use the last open file name to determine the folder that
        //          is to be remembered since on Windows 7, at least, it's
        //          possible to search for files from within the file dialog
        //          box, the last open file name should be the one we are
        //          'interested' in...
        // Note #2: this doesn't, unfortunately, address the case where the user
        //          goes to a directory and then closes the file dialog box
        //          without selecting any open file name. There might be a way
        //          to get it to work, but that would involve using the exec()
        //          method rather than the static getOpenFilesNames() method,
        //          which would result in a non-native looking file dialog box
        //          (on Windows 7 at least), so it's not an option
        //          unfortunately...

        setActiveDirectory(QFileInfo(res[res.count()-1]).path());

    // Return the open file name(s)

    return res;
}

//==============================================================================

QString CORE_EXPORT getSaveFileName(const QString &pCaption,
                                    const QString &pFileName,
                                    const QString &pFilter)
{
    // Retrieve and return a save file name

    QString res = QDir::toNativeSeparators(QFileDialog::getSaveFileName(qApp->activeWindow(),
                                                                        pCaption,
                                                                        pFileName.isEmpty()?
                                                                            activeDirectory():
                                                                            QFileInfo(pFileName).canonicalPath(),
#if defined(Q_OS_WIN) || defined(Q_OS_LINUX)
                                                                        pFilter, 0,
#elif defined(Q_OS_MAC)
//---GRY--- FOR SOME REASONS, OS X / Qt DOESN'T LIKE US SPECIFYING SUPPORTED
//          FILE TYPES...!? (SEE https://github.com/opencor/opencor/issues/110)
                                                                        QString(), 0,
#else
    #error Unsupported platform
#endif
                                                                        QFileDialog::DontConfirmOverwrite));

    // Make sure that we got a save file name

    if (!res.isEmpty()) {
        // Update our active directory

        QFileInfo resInfo = res;

        setActiveDirectory(resInfo.path());

        // Check whether the save file already exists

        if (resInfo.exists())
            // The save file already exists, so ask whether we want to overwrite
            // it

            if (QMessageBox::question(qApp->activeWindow(),
                                      qApp->applicationName(),
                                      QObject::tr("<strong>%1</strong> already exists. Do you want to overwrite it?").arg(res),
                                      QMessageBox::Yes|QMessageBox::No,
                                      QMessageBox::Yes) == QMessageBox::No )
                // We don't want to overwrite the save file, so...

                return QString();
    }

    // Everything went fine,so return the save file name

    return res;
}

//==============================================================================

void setFocusTo(QWidget *pWidget)
{
    // Give the focus to pWidget, but then revert the focus back to whoever had
    // it before, if needed

    if (!pWidget)
        return;

    QWidget *focusedWidget = qApp->activeWindow()?
                                 qApp->activeWindow()->focusWidget():
                                 0;

    pWidget->setFocus();

    if (   focusedWidget
        && (pWidget->parentWidget() != focusedWidget->parentWidget()))
        // The current and previously focused widgets don't share the same
        // parent, so revert the focus back to the previously focused widget

        focusedWidget->setFocus();
}

//==============================================================================

QString nativeCanonicalFileName(const QString &pFileName)
{
    // Return a native and canonical version of the file's name

    return QDir::toNativeSeparators(QFileInfo(pFileName).canonicalFilePath());
}

//==============================================================================

QFrame * newLineWidget(const bool &pHorizontal, const QColor &pColor,
                       QWidget *pParent)
{
    // Create and return a 'real' line widget, i.e. one which is 1 pixel wide,
    // using a QFrame widget

    QFrame *res = new QFrame(pParent);

    res->setStyleSheet(QString("QFrame {"
                               "    border: 1px solid rgb(%1, %2, %3);"
                               "}").arg(QString::number(pColor.red()),
                                        QString::number(pColor.green()),
                                        QString::number(pColor.blue())));

    if (pHorizontal) {
        res->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        res->setFixedHeight(1);
    } else {
        res->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        res->setFixedWidth(1);
    }

    return res;
}

//==============================================================================

QFrame * newLineWidget(const bool &pHorizontal, QWidget *pParent)
{
    // Create and return a 'real' horizontal line widget

    return newLineWidget(pHorizontal, CommonWidget::borderColor(), pParent);
}

//==============================================================================

QFrame * newLineWidget(const QColor &pColor, QWidget *pParent)
{
    // Create and return a 'real' horizontal line widget

    return newLineWidget(true, pColor, pParent);
}

//==============================================================================

QFrame * newLineWidget(QWidget *pParent)
{
    // Create and return a 'real' horizontal line widget

    return newLineWidget(true, CommonWidget::borderColor(), pParent);
}

//==============================================================================

QLabel * newLabel(const QString &pText, const double &pFontPercentage,
                  const bool &pBold, const bool &pItalic,
                  const Qt::Alignment &pAlignment, QWidget *pParent)
{
    // Create and return a label, after having set some of its properties

    QLabel *res = new QLabel(pText, pParent);

    QFont font = res->font();

    font.setBold(pBold);
    font.setItalic(pItalic);
    font.setPointSize(pFontPercentage*font.pointSize());

    res->setAlignment(pAlignment);
    res->setFont(font);

    // Prevent Qt from associating a context menu with the label (something
    // which it does automatically when a label is a link...)

    res->setContextMenuPolicy(Qt::NoContextMenu);

    // Return our link

    return res;
}

//==============================================================================

QLabel * newLabel(const QString &pText, const double &pFontPercentage,
                  const bool &pBold, const bool &pItalic, QWidget *pParent)
{
    // Create and return a label

    return newLabel(pText, pFontPercentage, pBold, pItalic,
                    Qt::AlignLeft|Qt::AlignVCenter, pParent);
}

//==============================================================================

QLabel * newLabel(const QString &pText, const double &pFontPercentage,
                  const bool &pBold, QWidget *pParent)
{
    // Create and return a label

    return newLabel(pText, pFontPercentage, pBold, false,
                    Qt::AlignLeft|Qt::AlignVCenter, pParent);
}

//==============================================================================

QLabel * newLabel(const QString &pText, const double &pFontPercentage,
                  QWidget *pParent)
{
    // Create and return a label

    return newLabel(pText, pFontPercentage, false, false,
                    Qt::AlignLeft|Qt::AlignVCenter, pParent);
}

//==============================================================================

QLabel * newLabel(const QString &pText, QWidget *pParent)
{
    // Create and return a label

    return newLabel(pText, 1.0, false, false, Qt::AlignLeft|Qt::AlignVCenter,
                    pParent);
}

//==============================================================================

QString copyright()
{
    // Return a generic copyright

    return QObject::tr("Copyright")+" ©2011-"+QString::number(QDate::currentDate().year());
}

//==============================================================================

}   // namespace Core
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
