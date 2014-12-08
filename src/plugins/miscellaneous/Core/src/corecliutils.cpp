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

#include "corecliutils.h"
#include "coresettings.h"
#include "settings.h"

//==============================================================================

#include <Qt>
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
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QResource>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QSysInfo>
#include <QTextStream>

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

bool sortSerialisedAttributes(const QString &pSerialisedAttribute1,
                              const QString &pSerialisedAttribute2)
{
    // Determine which of the two serialised attributes should be first based on
    // the attribute name, i.e. ignoring the "=<AttributeValue>" bit

    return pSerialisedAttribute1.left(pSerialisedAttribute1.indexOf("=")).compare(pSerialisedAttribute2.left(pSerialisedAttribute2.indexOf("=")), Qt::CaseInsensitive) < 0;
}

//==============================================================================

void cleanDomElement(QDomElement &pDomElement,
                     QMap<QString, QString> &pElementsAttributes)
{
    // Serialise all the element's attributes and sort their serialised version
    // before removing them from the element and adding a new attribute that
    // will later on be used for string replacement

    static qulonglong attributeNumber = 0;
    static const int ULLONG_WIDTH = ceil(log(ULLONG_MAX));

    if (pDomElement.hasAttributes()) {
        QStringList serialisedAttributes = QStringList();
        QDomNamedNodeMap domElementAttributes = pDomElement.attributes();
        QDomNode attributeNode;
        QString serialisedAttribute;
        QTextStream textStream(&serialisedAttribute, QIODevice::WriteOnly);

        while (domElementAttributes.count()) {
            // Serialise the element's attribute

            attributeNode = domElementAttributes.item(0);

            serialisedAttribute = QString();

            attributeNode.save(textStream, 4);

            serialisedAttributes << serialisedAttribute;

            // Remove the attribute from the element

            domElementAttributes.removeNamedItem(attributeNode.nodeName());
        }

        // Sort the serialised attributes using the attributes' name

        std::sort(serialisedAttributes.begin(), serialisedAttributes.end(), sortSerialisedAttributes);

        // Keep track of the serialisation of the element's attribute

        QString elementAttributes = QString("Element%1Attributes").arg(++attributeNumber, ULLONG_WIDTH, 10, QChar('0'));

        pElementsAttributes.insert(elementAttributes, serialisedAttributes.join(" "));

        // Add a new attribute to the element
        // Note: this attribute, once serialised by QDomDocument::save(), will
        //       be used to do a string replacement (see
        //       qDomDocumentToString())...

        domElementAttributes.setNamedItem(pDomElement.ownerDocument().createAttribute(elementAttributes));
    }

    // Recursively clean ourselves

    for (QDomElement childElement = pDomElement.firstChildElement(); !childElement.isNull(); childElement = childElement.nextSiblingElement())
        cleanDomElement(childElement, pElementsAttributes);
}

//==============================================================================

QString qDomDocumentToString(const QDomDocument &pDomDocument)
{
    // Serialise the given DOM document
    // Note: normally, we would simply be using QDomDocument::save(), but we
    //       want elements' attributes to be sorted when serialised (so that it
    //       is easier to compare two different XML documents). Unfortunately,
    //       QDomDocument::save() doesn't provide such a functionality (since
    //       the order of attributes doesn't matter in XML). So, we make a call
    //       to QDomDocument::save(), but only after having removed all the
    //       elements' attributes, which we serialise manually afterwards...

    QString res = QString();

    // Make a deep copy of the given DOM document and remove all the elements'
    // attributes (but keep track of them, so that we can later on serialise
    // them manually)

    QDomDocument domDocument = pDomDocument.cloneNode().toDocument();
    QMap<QString, QString> elementsAttributes = QMap<QString, QString>();

    for (QDomElement childElement = domDocument.firstChildElement(); !childElement.isNull(); childElement = childElement.nextSiblingElement())
        cleanDomElement(childElement, elementsAttributes);

    // Serialise our 'reduced' DOM document

    QTextStream textStream(&res, QIODevice::WriteOnly);

    domDocument.save(textStream, 4);

    // Manually serialise the elements' attributes

    foreach (const QString &elementAttribute, elementsAttributes.keys())
        res.replace(elementAttribute+"=\"\"", elementsAttributes.value(elementAttribute));

    return res;
}

//==============================================================================

namespace OpenCOR {
namespace Core {

//==============================================================================

#include "corecliutils.cpp.inl"

//==============================================================================

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
