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
// CLI utilities
//==============================================================================

#include "corecliutils.h"
#include "filemanager.h"

//==============================================================================

#include <QtMath>

//==============================================================================

#include <QChar>
#include <QCryptographicHash>
#include <QDir>
#include <QDropEvent>
#include <QIODevice>
#include <QLocale>
#include <QMap>
#include <QMimeData>
#include <QNetworkAccessManager>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QRegularExpression>
#include <QResource>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QTemporaryDir>
#include <QTemporaryFile>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QXmlStreamReader>

//==============================================================================

#include <array>

//==============================================================================

#if defined(Q_OS_WIN)
    #include <Windows.h>
#elif defined(Q_OS_LINUX)
    #include <unistd.h>
#else
    #include <mach/host_info.h>
    #include <mach/mach_host.h>
    #include <sys/sysctl.h>
#endif

//==============================================================================

#ifdef Q_OS_WIN
    extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

#include "corecliutils.cpp.inl"

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================

QBoolList qVariantListToBoolList(const QVariantList &pVariantList)
{
    // Convert the given list of variants to a list of booleans

    QBoolList res = QBoolList();

    for (const auto &variant : pVariantList) {
        res << variant.toBool();
    }

    return res;
}

//==============================================================================

QVariantList qBoolListToVariantList(const QBoolList &pBoolList)
{
    // Convert the given list of booleans to a list of variants

    QVariantList res = QVariantList();

    for (auto nb : pBoolList) {
        res << nb;
    }

    return res;
}

//==============================================================================

QIntList qVariantListToIntList(const QVariantList &pVariantList)
{
    // Convert the given list of variants to a list of integers

    QIntList res = QIntList();

    for (const auto &variant : pVariantList) {
        res << variant.toInt();
    }

    return res;
}

//==============================================================================

QVariantList qIntListToVariantList(const QIntList &pIntList)
{
    // Convert the given list of integers to a list of variants

    QVariantList res = QVariantList();

    for (auto nb : pIntList) {
        res << nb;
    }

    return res;
}

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

void DummyMessageHandler::handleMessage(QtMsgType pType,
                                        const QString &pDescription,
                                        const QUrl &pIdentifier,
                                        const QSourceLocation &pSourceLocation)
{
    Q_UNUSED(pType)
    Q_UNUSED(pDescription)
    Q_UNUSED(pIdentifier)
    Q_UNUSED(pSourceLocation)

    // We ignore the message...
}

//==============================================================================

quint64 totalMemory()
{
    // Retrieve and return in bytes the total amount of physical memory

    quint64 res = 0;

#if defined(Q_OS_WIN)
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(memoryStatus);

    GlobalMemoryStatusEx(&memoryStatus);

    res = quint64(memoryStatus.ullTotalPhys);
#elif defined(Q_OS_LINUX)
    res = quint64(sysconf(_SC_PHYS_PAGES))*quint64(sysconf(_SC_PAGESIZE));
#else
    std::array<int, 2> mib = { CTL_HW, HW_MEMSIZE };

    size_t len = sizeof(res);

    sysctl(mib.data(), 2, &res, &len, nullptr, 0);
#endif

    return res;
}

//==============================================================================

quint64 freeMemory()
{
    // Retrieve and return in bytes the available amount of physical memory

    quint64 res = 0;

#if defined(Q_OS_WIN)
    MEMORYSTATUSEX memoryStatus;

    memoryStatus.dwLength = sizeof(memoryStatus);

    GlobalMemoryStatusEx(&memoryStatus);

    res = quint64(memoryStatus.ullAvailPhys);
#elif defined(Q_OS_LINUX)
    res = quint64(sysconf(_SC_AVPHYS_PAGES))*quint64(sysconf(_SC_PAGESIZE));
#else
    vm_statistics_data_t vmStats;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
#pragma clang diagnostic pop

    host_statistics(mach_host_self(), HOST_VM_INFO,
                    host_info_t(&vmStats), &infoCount);

    res = (quint64(vmStats.free_count)+quint64(vmStats.inactive_count))*quint64(vm_page_size);
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

    if (validNumber) {
        res = QLocale().toString(number, 'g', 15);
    }

    return res;
}

//==============================================================================

QString sizeAsString(quint64 pSize, int pPrecision)
{
    static const std::array<QString, 9> Units = {
                                                    QObject::tr("B"),
                                                    QObject::tr("KB"),
                                                    QObject::tr("MB"),
                                                    QObject::tr("GB"),
                                                    QObject::tr("TB"),
                                                    QObject::tr("PB")
                                                };

    auto i = ulong(qFloor(log(pSize)/log(1024.0)));
    double size = pSize/qPow(1024.0, i);
    double scaling = qPow(10.0, pPrecision);

    size = qRound(scaling*size)/scaling;

    return QLocale().toString(size)+" "+Units.at(i);
}

//==============================================================================

QString formatTime(qint64 pTime)
{
    // Convert the given time (in milliseconds) to a string and return it
    // Note: we must not use floating point divisions as it may yield some
    //       unexpected results (not sure whether it's on some machines, with
    //       some compilers and/or with a release/debug mode). For example, to
    //       compute the number of hours, we might want to do something like
    //          int h = int(pTime/3600000.0)%24;
    //       but in some cases this will give 0 for pTime=3600000 while it
    //       should clearly give 1, hence the approach used below...

    QString res = QString();
    qint64 time = pTime;
    qint64 ms = time%1000; time = (time-ms)/1000;
    qint64 s  = time%60;   time = (time-s)/60;
    qint64 m  = time%60;   time = (time-m)/60;
    qint64 h  = time%24;   time = (time-h)/24;

    if ((time != 0) || (((h != 0) || (m != 0) || (s != 0) || (ms != 0)) && !res.isEmpty())) {
        res += (res.isEmpty()?QString():" ")+QString::number(time)+QObject::tr("d");
    }

    if ((h != 0) || (((m != 0) || (s != 0) || (ms != 0)) && !res.isEmpty())) {
        res += (res.isEmpty()?QString():" ")+QString::number(h)+QObject::tr("h");
    }

    if ((m != 0) || (((s != 0) || (ms != 0)) && !res.isEmpty())) {
        res += (res.isEmpty()?QString():" ")+QString::number(m)+QObject::tr("m");
    }

    if ((s != 0) || ((ms != 0) && !res.isEmpty())) {
        res += (res.isEmpty()?QString():" ")+QString::number(s)+QObject::tr("s");
    }

    if ((ms != 0) || res.isEmpty()) {
        res += (res.isEmpty()?QString():" ")+QString::number(ms)+QObject::tr("ms");
    }

    return res;
}

//==============================================================================

QString sha1(const QByteArray &pByteArray)
{
    // Return the SHA-1 value of the given byte array

    return QCryptographicHash::hash(pByteArray,
                                    QCryptographicHash::Sha1).toHex();
}

//==============================================================================

QString sha1(const QString &pString)
{
    // Return the SHA-1 value of the given string

    return sha1(pString.toUtf8());
}

//==============================================================================

QString fileSha1(const QString &pFileName)
{
    // Return the SHA-1 value of the given file

    QString res = QString();
    QFile file(pFileName);

    if (file.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha1);

        if (hash.addData(&file)) {
            res = hash.result().toHex();
        }

        file.close();
    }

    return res;
}

//==============================================================================

void stringPositionAsLineColumn(const QString &pString, const QString &pEol,
                                int pPosition, int &pLine, int &pColumn)
{
    // Determine the line and column values of the given position within the
    // given string using the given end of line

    if ((pPosition < 0) || (pPosition >= pString.length())) {
        // Invalid position values

        pLine = -1;
        pColumn = -1;
    } else {
        pLine = pString.leftRef(pPosition).count(pEol)+1;
        pColumn = pPosition-((pPosition >= pEol.length())?pString.lastIndexOf(pEol, pPosition-pEol.length()):-1);
    }
}

//==============================================================================

void stringLineColumnAsPosition(const QString &pString, const QString &pEol,
                                int pLine, int pColumn, int &pPosition)
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
            }

            pPosition = pos+pEol.length();
        }

        pPosition += pColumn-1;

        // Make sure that the column value is actually valid by trying to
        // retrieve the line and column values from our computed position

        int testLine;
        int testColumn;

        stringPositionAsLineColumn(pString, pEol, pPosition, testLine, testColumn);

        if ((testLine != pLine) || (testColumn != pColumn)) {
            pPosition = -1;
        }
    }
}

//==============================================================================

void * globalInstance(const QString &pObjectName, void *pDefaultGlobalInstance)
{
    // Retrieve and return the 'global' instance of an object
    // Note: initially, the plan was to have a static instance of an object and
    //       return its address. However, this approach doesn't work on Windows
    //       and Linux (but does on macOS). Indeed, say that the Core plugin is
    //       required by two other plugins, then these two plugins won't get the
    //       same 'copy' of the Core plugin. (It seems like) each 'copy' gets
    //       its own address space. (This is not the case on macOS, (most
    //       likely) because of the way applications are bundled on that
    //       platform.) So, to address this issue, we keep track of the address
    //       of a 'global' instance as a qApp property...

    QByteArray objectName = pObjectName.toUtf8().constData();
    QVariant res = qApp->property(objectName);

    if (!res.isValid()) {
        // There is no 'global' instance associated with the given object, so
        // use the object's default 'global' instance we were given, if any

        res = quint64(pDefaultGlobalInstance);

        qApp->setProperty(objectName, res);
    }

    return reinterpret_cast<void *>(res.toULongLong());
}

//==============================================================================

static const char *SettingsActiveDirectory = "ActiveDirectory";

//==============================================================================

QString activeDirectory()
{
    // Retrieve and return the active directory

    return QSettings().value(SettingsActiveDirectory, QDir::homePath()).toString();
}

//==============================================================================

void setActiveDirectory(const QString &pDirName)
{
    // Keep track of the active directory

    QSettings().setValue(SettingsActiveDirectory, pDirName);
}

//==============================================================================

bool isDirectory(const QString &pDirName)
{
    // Return whether the given directory exists

    if (!pDirName.isEmpty() && QDir(pDirName).exists()) {
        // Check whether the directory is writable

#ifdef Q_OS_WIN
        ++qt_ntfs_permission_lookup;
#endif

        bool res = QFileInfo(pDirName).isWritable();

#ifdef Q_OS_WIN
        --qt_ntfs_permission_lookup;
#endif

        return res;
    }

    return false;
}

//==============================================================================

bool isEmptyDirectory(const QString &pDirName)
{
    // Return whether the given directory is really a directory and an empty one
    // at that

    return    isDirectory(pDirName)
           && QDir(pDirName).entryInfoList(QDir::AllEntries|QDir::System|QDir::Hidden|QDir::NoDotAndDotDot).isEmpty();
}

//==============================================================================

#ifdef Q_OS_WIN
    #pragma optimize("", off)
#endif

void doNothing(quint64 pMax)
{
    // A silly function, which aim is simply to do nothing
    // Note: this function came about because there is no way, on Windows, to
    //       pause a thread for less than a millisecond (and this is in the best
    //       of cases...

    for (quint64 i = 0; i < 1000*pMax; ++i) {
#ifdef Q_OS_WIN
        ;
#else
        asm("nop"); // NOLINT(hicpp-no-assembler)
#endif
    }
}

#ifdef Q_OS_WIN
    #pragma optimize("", on)
#endif

//==============================================================================

#ifdef Q_OS_WIN
    #pragma optimize("", off)
#endif

void doNothing(const quint64 *pMax, const bool *pStopped)
{
    // A silly function, which aim is simply to do nothing
    // Note #1: this function came about because there is no way, on Windows, to
    //          pause a thread for less than a millisecond (and this is in the
    //          best of cases)...
    // Note #2: pMax is a pointer so that we can adapt in real-time to the a
    //          value we are passed (which is useful if, in our Simulation
    //          Experiment view, we set a big delay and then decide to reduce
    //          it)...
    // Note #3: pStopped is optional and is used to stop our loop, in case it
    //          takes forever...

    for (quint64 i = 0; i < 1000**pMax; ++i) {
        if ((pStopped != nullptr) && *pStopped) {
            break;
        }

#ifdef Q_OS_WIN
        ;
#else
        asm("nop"); // NOLINT(hicpp-no-assembler)
#endif
    }
}

#ifdef Q_OS_WIN
    #pragma optimize("", on)
#endif

//==============================================================================

QString openFile(const QString &pFileName, const File::Type &pType,
                 const QString &pUrl)
{
    // Register the file with our file manager and get its status

    FileManager::Status fileStatus = FileManager::instance()->manage(pFileName, pType, pUrl);

    if (fileStatus == FileManager::Status::DoesNotExist) {
        return QObject::tr("'%1' could not be opened.").arg(pUrl.isEmpty()?
                                                                QDir::toNativeSeparators(pFileName):
                                                                pFileName);
    }

    return {};
}

//==============================================================================

QString openRemoteFile(const QString &pUrl)
{
    // Note: this method is used by our Python wrapper and should be kept in
    //       sync with that of CentralWidget::openRemoteFile() in
    //       src/plugins/miscellaneous/Core/src/centralwidget.cpp...

    // Make sure that pUrl really refers to a remote file

    bool isLocalFile;
    QString fileNameOrUrl;

    checkFileNameOrUrl(pUrl, isLocalFile, fileNameOrUrl);

    if (isLocalFile) {
        // It looks like the user tried to open a local file using a URL, e.g.
        //     file:///home/me/mymodel.cellml
        // rather than a local file name, e.g.
        //     /home/me/mymodel.cellml
        // so open the file as a local file and leave

        return openFile(fileNameOrUrl);
    }

    // Check whether the remote file is already opened and if so select it,
    // otherwise retrieve its contents

    FileManager *fileManagerInstance = FileManager::instance();
    QString fileName = fileManagerInstance->fileName(fileNameOrUrl);

    if (fileName.isEmpty()) {
        // The remote file isn't already opened, so download its contents

        QByteArray fileContents;
        QString errorMessage;

        if (readFile(fileNameOrUrl, fileContents, &errorMessage)) {
            // We were able to retrieve the contents of the remote file, so save
            // it locally

            fileName = Core::temporaryFileName();

            if (!writeFile(fileName, fileContents)) {
#ifdef QT_DEBUG
                qFatal("FATAL ERROR | %s:%d: '%s' could not be created.", __FILE__, __LINE__, qPrintable(fileNameOrUrl)); // NOLINT(cppcoreguidelines-pro-bounds-array-to-pointer-decay, cppcoreguidelines-pro-type-vararg)
#else
                return QObject::tr("'%1' could not be created.").arg(fileNameOrUrl);
#endif
            }
        } else {
            // We were not able to retrieve the contents of the remote file, so let
            // the user know about it

            return QObject::tr("'%1' could not be opened (%2).").arg(fileNameOrUrl,
                                                                     formatMessage(errorMessage));
        }
    }

    return openFile(fileName, File::Type::Remote, fileNameOrUrl);
}

//==============================================================================

QString formatXml(const QString &pXml)
{
    // Format the given XML

    QDomDocument domDocument;

    if (domDocument.setContent(pXml)) {
        return serialiseDomDocument(domDocument);
    }

    return {};
}

//==============================================================================

void cleanContentMathml(QDomElement *pDomElement)
{
    // Clean up the current element
    // Note: the idea is to remove all the attributes that are not in the
    //       MathML namespace. Indeed, if we were to leave them in then the XSL
    //       transformation would either do nothing or, worst, crash OpenCOR...

    QDomNamedNodeMap attributes = pDomElement->attributes();
    QList<QDomNode> nonMathmlAttributes = QList<QDomNode>();

    for (int i = 0, iMax = attributes.count(); i < iMax; ++i) {
        QDomNode attribute = attributes.item(i);

        if (attribute.localName() != attribute.nodeName()) {
            nonMathmlAttributes << attribute;
        }
    }

    for (const auto &nonMathmlAttribute : nonMathmlAttributes) {
        pDomElement->removeAttributeNode(nonMathmlAttribute.toAttr());
    }

    // Go through the element's child elements, if any, and clean them up

    for (QDomElement childElement = pDomElement->firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        cleanContentMathml(&childElement);
    }
}

//==============================================================================

QString cleanContentMathml(const QString &pContentMathml)
{
    // Clean up and return the given MathML string

    QDomDocument domDocument;

    if (domDocument.setContent(pContentMathml, true)) {
        for (QDomElement childElement = domDocument.firstChildElement();
             !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
            cleanContentMathml(&childElement);
        }

        return domDocument.toString(-1);
    }

    return {};
}

//==============================================================================

void cleanPresentationMathml(QDomElement *pDomElement)
{
    // Merge successive child mrow elements, as long as their parent is not an
    // element that requires a specific number of arguments (which could become
    // wrong if we were to merge two successive mrow elements)
    // Note: see http://www.w3.org/TR/MathML2/chapter3.html#id.3.1.3.2 for the
    //       list of the elements to check...

    static const QString Mfrac      = "mfrac";
    static const QString Mroot      = "mroot";
    static const QString Msub       = "msub";
    static const QString Msup       = "msup";
    static const QString Msubsup    = "msubsup";
    static const QString Munder     = "munder";
    static const QString Mover      = "mover";
    static const QString Munderover = "munderover";
    static const QString Mrow       = "mrow";

    if (   (pDomElement->nodeName() != Mfrac)
        && (pDomElement->nodeName() != Mroot)
        && (pDomElement->nodeName() != Msub)
        && (pDomElement->nodeName() != Msup)
        && (pDomElement->nodeName() != Msubsup)
        && (pDomElement->nodeName() != Munder)
        && (pDomElement->nodeName() != Mover)
        && (pDomElement->nodeName() != Munderover)) {
        for (QDomElement childElement = pDomElement->firstChildElement();
             !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
            QDomElement nextChildElement = childElement.nextSiblingElement();

            if (   !nextChildElement.isNull()
                &&  (childElement.nodeName() == Mrow)
                &&  (childElement.nodeName() == nextChildElement.nodeName())) {
                // The current and next child elements are both mrow's, so merge
                // them together

                for (QDomElement nextChildChildElement = nextChildElement.firstChildElement();
                     !nextChildChildElement.isNull(); nextChildChildElement = nextChildElement.firstChildElement()) {
                    childElement.appendChild(nextChildChildElement);
                }

                pDomElement->removeChild(nextChildElement);
            }
        }
    }

    // Recursively clean ourselves

    for (QDomElement childElement = pDomElement->firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        cleanPresentationMathml(&childElement);
    }

    // Move the contents of child mrow elements to their parent, should it also
    // be an mrow element
    // Note: we do this after having recursively cleaned ourselves to make sure
    //       that we also take into account the root element, in case it's an
    //       mrow element and the contents of its mrow child elements have been
    //       moved to it...

    if (pDomElement->nodeName() == Mrow) {
        for (QDomElement childElement = pDomElement->firstChildElement();
             !childElement.isNull(); ) {
            QDomElement nextChildElement = childElement.nextSiblingElement();

            if (childElement.nodeName() == Mrow) {
                // The current child element is an mrow, so move its contents to
                // its parent

                for (QDomElement childChildElement = childElement.firstChildElement();
                     !childChildElement.isNull(); childChildElement = childElement.firstChildElement()) {
                    pDomElement->insertBefore(childChildElement, childElement);
                }

                pDomElement->removeChild(childElement);
            }

            childElement = nextChildElement;
        }
    }
}

//==============================================================================

QString cleanPresentationMathml(const QString &pPresentationMathml)
{
    // Clean the given Presentation MathML

    QDomDocument domDocument;

    if (domDocument.setContent(pPresentationMathml)) {
        for (QDomElement childElement = domDocument.firstChildElement();
             !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
            cleanPresentationMathml(&childElement);
        }

        return domDocument.toString(-1);
    }

    return {};
}

//==============================================================================

QString newFileName(const QString &pFileName, const QString &pExtra,
                    bool pBefore, const QString &pFileExtension)
{
    // Return the name of a 'new' file
    // Note: see Tests::newFileNameTests() for what we want to be able to get...

    static const QString Dot = ".";

    FileManager *fileManagerInstance = FileManager::instance();
    QString fileName = fileManagerInstance->isRemote(pFileName)?
                           fileManagerInstance->url(pFileName):
                           pFileName;
    QFileInfo fileInfo = fileName;
    QString fileCanonicalPath = fileInfo.canonicalPath();
    QString fileBaseName = fileInfo.baseName();
    QString fileCompleteSuffix = pFileExtension.isEmpty()?
                                     fileInfo.completeSuffix():
                                     pFileExtension;

    if (fileCanonicalPath == Dot) {
        fileCanonicalPath = QString();
    } else {
        fileCanonicalPath += "/";
    }

    if (!fileCompleteSuffix.isEmpty()) {
        fileCompleteSuffix.prepend('.');
    }

    static const QString Space = " ";
    static const QString Hyphen = "-";
    static const QString Underscore = "_";

    int nbOfSpaces = fileBaseName.count(Space);
    int nbOfHyphens = fileBaseName.count(Hyphen);
    int nbOfUnderscores = fileBaseName.count(Underscore);

    if (pExtra.isEmpty()) {
        return fileCanonicalPath+fileBaseName+fileCompleteSuffix;
    }

    static const QRegularExpression InitialCapitalLetterRegEx = QRegularExpression("^\\p{Lu}");

    QString separator = ((nbOfSpaces >= nbOfHyphens) && (nbOfSpaces >= nbOfUnderscores))?
                            Space+Hyphen+Space:
                            ((nbOfUnderscores >= nbOfSpaces) && (nbOfUnderscores >= nbOfHyphens))?
                                Underscore:
                                Hyphen;
    QString extra = pExtra;

    if (!InitialCapitalLetterRegEx.match(fileBaseName).hasMatch()) {
        extra[0] = extra[0].toLower();
    }

    if (pBefore) {
        return fileCanonicalPath+extra+separator+fileBaseName+fileCompleteSuffix;
    }

    return fileCanonicalPath+fileBaseName+separator+extra+fileCompleteSuffix;
}

//==============================================================================

QString newFileName(const QString &pFileName, const QString &pExtra,
                    bool pBefore)
{
    // Return the name of a 'new' file

    return newFileName(pFileName, pExtra, pBefore, QString());
}

//==============================================================================

QString newFileName(const QString &pFileName, const QString &pFileExtension)
{
    // Return the name of a 'new' file

    return newFileName(pFileName, QString(), true, pFileExtension);
}

//==============================================================================

bool validXml(const QByteArray &pXml, const QByteArray &pSchema)
{
    // Validate the given XML against the given schema

    QXmlSchema schema;
    DummyMessageHandler dummyMessageHandler;

    schema.setMessageHandler(&dummyMessageHandler);

    schema.load(pSchema);

    QXmlSchemaValidator validator(schema);

    return validator.validate(pXml);
}

//==============================================================================

bool validXmlFile(const QString &pXmlFileName, const QString &pSchemaFileName)
{
    // Validate the given XML file against the given schema file

    QByteArray xmlContents;
    QByteArray schemaContents;

    readFile(pXmlFileName, xmlContents);
    readFile(pSchemaFileName, schemaContents);

    return validXml(xmlContents, schemaContents);
}

//==============================================================================

bool sortSerialisedAttributes(const QString &pSerialisedAttribute1,
                              const QString &pSerialisedAttribute2)
{
    // Determine which of the two serialised attributes should be first based on
    // the attribute name, i.e. ignoring the "=<AttributeValue>" bit

    return pSerialisedAttribute1.leftRef(pSerialisedAttribute1.indexOf('=')).compare(pSerialisedAttribute2.left(pSerialisedAttribute2.indexOf('=')), Qt::CaseInsensitive) < 0;
}

//==============================================================================

void cleanDomElement(QDomElement *pDomElement,
                     QMap<QString, QByteArray> *pElementsAttributes)
{
    // Serialise all the element's attributes and sort their serialised version
    // before removing them from the element and adding a new attribute that
    // will later on be used for string replacement

    static const int AttributeNumberWidth = int(ceil(log(ULLONG_MAX)));

    static int attributeNumber = 0;

    if (pDomElement->hasAttributes()) {
        QStringList serialisedAttributes = QStringList();
        QDomNamedNodeMap domElementAttributes = pDomElement->attributes();

        while (!domElementAttributes.isEmpty()) {
            // Serialise (ourselves) the element's attribute
            // Note: to rely on QDomNode::save() to do the serialisation isn't
            //       good enough. Indeed, if it is going to be fine for an
            //       attribute that doesn't have a prefix, e.g.
            //           name="my_name"
            //       it may not be fine for an attribute with a prefix, e.g.
            //           cmeta:id="my_cmeta_id"
            //       since depending on how that attribute has been created
            //       (i.e. using QDomDocument::createAttribute() or
            //       QDomDocument::createAttributeNS()), then it may or not have
            //       a namespace associated with it. If it does, then its
            //       serialisation will look something like
            //           cmeta:id="my_cmeta_id" xmlns:cmeta="http://www.cellml.org/metadata/1.0#"
            //       which is clearly not what we want since that's effectively
            //       two attributes in one. So, we need to separate them, which
            //       is what we do here, after making sure that the namespace
            //       for the attribute is not already defined for the given DOM
            //       element...

            QDomAttr attributeNode = domElementAttributes.item(0).toAttr();

            if (attributeNode.namespaceURI().isEmpty()) {
                serialisedAttributes << attributeNode.name()+R"(=")"+attributeNode.value().toHtmlEscaped()+R"(")";
            } else {
                serialisedAttributes << attributeNode.prefix()+":"+attributeNode.name()+R"(=")"+attributeNode.value().toHtmlEscaped()+R"(")";

                if (   (attributeNode.prefix() != pDomElement->prefix())
                    && (attributeNode.namespaceURI() != pDomElement->namespaceURI())) {
                    serialisedAttributes << "xmlns:"+attributeNode.prefix()+R"(=")"+attributeNode.namespaceURI()+R"(")";
                }
            }

            // Remove the attribute node from the element

            pDomElement->removeAttributeNode(attributeNode);
        }

        // Sort the serialised attributes, using the attributes' name, and
        // remove duplicates, if any

        std::sort(serialisedAttributes.begin(), serialisedAttributes.end(),
                  sortSerialisedAttributes);

        serialisedAttributes.removeDuplicates();

        // Keep track of the serialisation of the element's attribute

        QString elementAttributes = QString("Element%1Attributes").arg(++attributeNumber, AttributeNumberWidth, 10, QChar('0'));

        pElementsAttributes->insert(elementAttributes, serialisedAttributes.join(' ').toUtf8());

        // Add a new attribute to the element
        // Note: this attribute, once serialised by QDomDocument::save(), will
        //       be used to do a string replacement (see
        //       serialiseDomDocument())...

        domElementAttributes.setNamedItem(pDomElement->ownerDocument().createAttribute(elementAttributes));
    }

    // Recursively clean ourselves

    for (QDomElement childElement = pDomElement->firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        cleanDomElement(&childElement, pElementsAttributes);
    }
}

//==============================================================================

QByteArray serialiseDomDocument(const QDomDocument &pDomDocument)
{
    // Serialise the given DOM document
    // Note: normally, we would simply be using QDomDocument::save(), but we
    //       want elements' attributes to be sorted when serialised (so that it
    //       is easier to compare two different XML documents). Unfortunately,
    //       QDomDocument::save() doesn't provide such a functionality (since
    //       the order of attributes doesn't matter in XML). So, we make a call
    //       to QDomDocument::save(), but only after having removed all the
    //       elements' attributes, which we serialise manually afterwards...

    QByteArray res = QByteArray();

    // Make a deep copy of the given DOM document and remove all the elements'
    // attributes (but keep track of them, so that we can later on serialise
    // them manually)

    QDomDocument domDocument = pDomDocument.cloneNode().toDocument();
    QMap<QString, QByteArray> elementsAttributes = QMap<QString, QByteArray>();

    for (QDomElement childElement = domDocument.firstChildElement();
         !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
        cleanDomElement(&childElement, &elementsAttributes);
    }

    // Serialise our 'reduced' DOM document

    res = domDocument.toByteArray(4);

    // Replace <?xml version='1.0'?> with <?xml version='1.0' encoding='UTF-8'?>
    // Note: indeed, by default QDomDocument uses UTF-8 and when it gets
    //       serialised it doesn't specify it while we want it specified (so as
    //       to be consistent with SED-ML for example)...

    res.replace("<?xml version='1.0'?>", "<?xml version='1.0' encoding='UTF-8'?>");

    // Manually serialise the elements' attributes

    QStringList elementAttributeKeys = elementsAttributes.keys();

    for (const auto &elementAttribute : elementAttributeKeys) {
        res.replace(elementAttribute+R"(="")", elementsAttributes.value(elementAttribute));
    }

    return res;
}

//==============================================================================

QStringList filters(const FileTypeInterfaces &pFileTypeInterfaces,
                    bool pCheckMimeTypes, const QString &pMimeType)
{
    // Convert and return as a list of strings the filters corresponding to the
    // given file type interfaces, using the given MIME types, if any

    QStringList res = QStringList();

    for (auto fileTypeInterface : pFileTypeInterfaces) {
        if (!pCheckMimeTypes || (pMimeType == fileTypeInterface->mimeType())) {
            res << fileTypeInterface->fileTypeDescription()+" (*."+fileTypeInterface->fileExtension()+")";
        }
    }

    return res;
}

//==============================================================================

QStringList filters(const FileTypeInterfaces &pFileTypeInterfaces)
{
    // Convert and return as a list of strings the filters corresponding to the
    // given file type interfaces

    return filters(pFileTypeInterfaces, false, QString());
}

//==============================================================================

QStringList filters(const FileTypeInterfaces &pFileTypeInterfaces,
                    const QString &pMimeType)
{
    // Convert and return as a list of strings the filters corresponding to the
    // given file type interfaces, using the given MIME types

    return filters(pFileTypeInterfaces, true, pMimeType);
}

//==============================================================================

QStringList droppedFileNames(QDropEvent *pEvent)
{
    // Retrieve the name of the various files that have been dropped

    QStringList res = QStringList();
    QList<QUrl> urls = pEvent->mimeData()->urls();

    for (int i = 0, iMax = urls.count(); i < iMax; ++i) {
        QString fileName = urls[i].toLocalFile();
        QFileInfo fileInfo = fileName;

        if (fileInfo.isFile()) {
            if (fileInfo.isSymLink()) {
                // We are dropping a symbolic link, so retrieve its target and
                // check that it exists, and if it does then add it

                fileName = fileInfo.symLinkTarget();

                if (QFile::exists(fileName)) {
                    res << fileName;
                }
            } else {
                // We are dropping a file, so we can just add it

                res << fileName;
            }
        }
    }

    // There may be duplicates (in case we dropped some symbolic links), so
    // remove them

    res.removeDuplicates();

    return res;
}

//==============================================================================

} // namespace Core
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
