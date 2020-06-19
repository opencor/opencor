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

QString locale()
{
    // Retrieve and return the locale

    QString res = rawLocale();

    if (res.isEmpty()) {
        return QLocale::system().name().left(2);
    }

    return res;
}

//==============================================================================

static const char *RawSettingsLocale = "RawLocale";

//==============================================================================

QString rawLocale()
{
    // Retrieve and return the raw locale

    return QSettings().value(RawSettingsLocale).toString();
}

//==============================================================================

void setRawLocale(const QString &pRawLocale)
{
    // Keep track of the raw locale

    QSettings().setValue(RawSettingsLocale, pRawLocale);
}

//==============================================================================

QString shortVersion()
{
    QString res;
    QString appVersion = qApp->applicationVersion();

    if (!appVersion.contains('-')) {
        res += "Version ";
    } else {
        res += "Snapshot ";
    }

    res += appVersion;

    return res;
}

//==============================================================================

QString version()
{
    return qAppName()+" "+shortVersion();
}

//==============================================================================

QString pluginCategoryName(const PluginInfo::Category &pCategory)
{
    // Return the name of the given category

    switch (pCategory) {
#ifdef ENABLE_SAMPLE_PLUGINS
    case PluginInfo::Category::Sample:
        return QObject::tr("Sample");
#endif
#ifdef ENABLE_TEST_PLUGINS
    case PluginInfo::Category::Test:
        return QObject::tr("Test");
#endif
    case PluginInfo::Category::Invalid:
        return QObject::tr("Invalid");
    case PluginInfo::Category::DataStore:
        return QObject::tr("Data store");
    case PluginInfo::Category::Editing:
        return QObject::tr("Editing");
    case PluginInfo::Category::Miscellaneous:
        return QObject::tr("Miscellaneous");
    case PluginInfo::Category::Organisation:
        return QObject::tr("Organisation");
    case PluginInfo::Category::Simulation:
        return QObject::tr("Simulation");
    case PluginInfo::Category::Solver:
        return QObject::tr("Solver");
    case PluginInfo::Category::Support:
        return QObject::tr("Support");
    case PluginInfo::Category::ThirdParty:
        return QObject::tr("Third-party");
    case PluginInfo::Category::Tools:
        return QObject::tr("Tools");
    case PluginInfo::Category::Widget:
        return QObject::tr("Widget");
    }

    return "???";
    // Note: we can't reach this point, but without it we may be told that not
    //       all control paths return a value...
}

//==============================================================================

QString pluginCategoryDescription(const PluginInfo::Category &pCategory)
{
    // Return the description of the given category

    switch (pCategory) {
#ifdef ENABLE_SAMPLE_PLUGINS
    case PluginInfo::Category::Sample:
        return QObject::tr("Plugins that illustrate various plugin-related aspects.");
#endif
#ifdef ENABLE_TEST_PLUGINS
    case PluginInfo::Category::Test:
        return QObject::tr("Plugins to test things.");
#endif
    case PluginInfo::Category::Invalid:
        return QObject::tr("Plugins that are not valid.");
    case PluginInfo::Category::DataStore:
        return QObject::tr("Plugins to store and manipulate data.");
    case PluginInfo::Category::Editing:
        return QObject::tr("Plugins to edit files.");
    case PluginInfo::Category::Miscellaneous:
        return QObject::tr("Plugins that do not fit in any other category.");
    case PluginInfo::Category::Organisation:
        return QObject::tr("Plugins to organise files.");
    case PluginInfo::Category::Simulation:
        return QObject::tr("Plugins to simulate files.");
    case PluginInfo::Category::Solver:
        return QObject::tr("Plugins to access various solvers.");
    case PluginInfo::Category::Support:
        return QObject::tr("Plugins to support various third-party libraries.");
    case PluginInfo::Category::ThirdParty:
        return QObject::tr("Plugins to access various third-party libraries.");
    case PluginInfo::Category::Tools:
        return QObject::tr("Plugins to access various tools.");
    case PluginInfo::Category::Widget:
        return QObject::tr("Plugins to access various <em>ad hoc</em> widgets.");
    }

    return "???";
    // Note: we can't reach this point, but without it we may be told that not
    //       all control paths return a value...
}

//==============================================================================

QString canonicalDirName(const QString &pDirName)
{
    // Return a canonical version of the given directory name or a native version, if the native and canonical version is empty (i.e. the
    // directory doesn't exist (anymore?))

    QString res = QDir(pDirName).canonicalPath();

    return res.isEmpty()?pDirName:res;
}

//==============================================================================

QString canonicalFileName(const QString &pFileName)
{
    // Return a canonical version of the given file name or the given file name
    // itself, if the canonical version is empty (i.e. the file doesn't exist
    // (anymore?))

    QString res = QFileInfo(pFileName).canonicalFilePath();

    return res.isEmpty()?pFileName:res;
}

//==============================================================================

QStringList canonicalFileNames(const QStringList &pFileNames)
{
    // Return a canonical version of the given file names or the given file
    // names themselves, if the canonical version of the given file names is
    // empty (i.e. the files don't exist (anymore?))

    QStringList res;

    for (const auto &fileName : pFileNames) {
        res << canonicalFileName(fileName);
    }

    return res;
}

//==============================================================================

bool SynchronousFileDownloader::download(const QString &pUrl,
                                         QByteArray &pContents,
                                         QString *pErrorMessage) const
{
    // Try to read a remote file as text, but only if we are connected to the
    // Internet

    pContents = QByteArray();

    if (hasInternetConnection()) {
        // Create a network access manager so that we can retrieve the contents
        // of the remote file

        QNetworkAccessManager networkAccessManager;

        // Make sure that we get told if there are SSL errors (which would
        // happen if a website's certificate is invalid, e.g. it has expired)

        connect(&networkAccessManager, &QNetworkAccessManager::sslErrors,
                this, &SynchronousFileDownloader::networkAccessManagerSslErrors);

        // Download the contents of the remote file

        QEventLoop waitLoop;
        QNetworkReply *networkReply = networkAccessManager.get(QNetworkRequest(pUrl));

        connect(networkReply, &QNetworkReply::finished, [&]() {
            waitLoop.quit();
        });

        waitLoop.exec();

        // Check whether we were able to retrieve the contents of the file

        bool res = networkReply->error() == QNetworkReply::NoError;

        if (res) {
            // Before accepting the contents as is, make sure that we are not
            // dealing with a redirection

            QUrl redirectedUrl = networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

            if (!redirectedUrl.isEmpty()) {
                networkReply->deleteLater();

                return download(redirectedUrl.toString(), pContents, pErrorMessage);
            }

            pContents = networkReply->readAll();

            if (pErrorMessage != nullptr) {
                *pErrorMessage = QString();
            }
        } else if (pErrorMessage != nullptr) {
            *pErrorMessage = networkReply->errorString();
        }

        networkReply->deleteLater();

        return res;
    }

    if (pErrorMessage != nullptr) {
        *pErrorMessage = QObject::tr("No Internet connection available.");
    }

    return false;
}

//==============================================================================

void SynchronousFileDownloader::networkAccessManagerSslErrors(QNetworkReply *pNetworkReply,
                                                              const QList<QSslError> &pSslErrors)
{
    // Ignore the SSL errors since we assume the user knows what s/he is doing

    pNetworkReply->ignoreSslErrors(pSslErrors);
}

//==============================================================================

int exec(const QString &pProgram, const QStringList &pArgs, QString &pOutput)
{
    // Execute the given program with the given arguments and track its output

    QProcess process;

    process.start(pProgram, pArgs);
    process.waitForFinished();

    pOutput = process.readAll().trimmed();

    return process.exitCode();
}

//==============================================================================

int exec(const QString &pProgram, const QStringList &pArgs)
{
    // Execute the given program with the given arguments and track its output

    QString dummy;

    return exec(pProgram, pArgs, dummy);
}

//==============================================================================

bool hasInternetConnection()
{
    // Check whether an Internet connection is available, this by going through
    // all of our network interfaces and checking whether one of them is both
    // active and is not a loopback, and has at least one IPv4 IP address

    for (const auto &networkInterface : QNetworkInterface::allInterfaces()) {
        QNetworkInterface::InterfaceFlags interfaceFlags = networkInterface.flags();

        if (    interfaceFlags.testFlag(QNetworkInterface::IsUp)
            && !interfaceFlags.testFlag(QNetworkInterface::IsLoopBack)) {
            for (const auto &addressEntry : networkInterface.addressEntries()) {
                QAbstractSocket::NetworkLayerProtocol protocol = addressEntry.ip().protocol();

                if (   (protocol == QAbstractSocket::IPv4Protocol)
                    || (protocol == QAbstractSocket::AnyIPProtocol)) {
                    return true;
                }
            }
        }
    }

    return false;
}

//==============================================================================

QString noInternetConnectionMessage()
{
    // Return a typical message about no Internet connection being available

    return QObject::tr("No Internet connection available.");
}

//==============================================================================

QString copyright()
{
    // Return our copyright

    return QObject::tr("Copyright")+" 2011-"+QString::number(QDate::currentDate().year());
}

//==============================================================================

QString formatMessage(const QString &pMessage, bool pLowerCase, bool pDotDotDot)
{
    // Trim the message and make sure that we don't end up with an empty string

    static const QString DotDotDot = "...";

    QString message = pMessage.trimmed();

    if (message.isEmpty()) {
        return pDotDotDot?DotDotDot:QString();
    }

    // Upper/lower the case of the first character if:
    //  - the message is one character long (!!), or its second character is in
    //    lower case or of space type; and
    //  - it doesn't start with CellML

    if (    (    (message.size() == 1)
             || ((message.size() > 1) && (message[1].isLower() || message[1].isSpace())))
        && !message.startsWith("CellML")) {
        if (pLowerCase) {
            message[0] = message[0].toLower();
        } else {
            message[0] = message[0].toUpper();
        }
    }

    // Return the message after making sure that it ends with "...", if
    // requested

    int subSize = message.size();

    while ((subSize != 0) && (message[subSize-1] == '.')) {
        --subSize;
    }

    return message.left(subSize)+(pDotDotDot?DotDotDot:QString());
}

//==============================================================================

QByteArray resource(const QString &pResource)
{
    // Retrieve a resource as a QByteArray

    QResource resource(pResource);

    if (resource.isValid()) {
        if (resource.isCompressed()) {
            // The resource is compressed, so uncompress it before returning it

            return qUncompress(resource.data(), int(resource.size()));
        }

        // The resource is not compressed, so just return it after doing the
        // right conversion

        return QByteArray(reinterpret_cast<const char *>(resource.data()),
                          int(resource.size()));
    }

    return {};
}

//==============================================================================

QString temporaryDirName()
{
    // Get and return a temporary directory name

    QTemporaryDir dir;

    dir.setAutoRemove(false);
    // Note: by default, a temporary directory is to autoremove itself, but we
    //       clearly don't want that here...

    return dir.path();
}

//==============================================================================

QString temporaryFileName(const QString &pExtension)
{
    // Get and return a temporary file name

    QTemporaryFile file(QDir::tempPath()+"/XXXXXX"+pExtension);

    file.open();

    file.setAutoRemove(false);
    // Note: by default, a temporary file is to autoremove itself, but we
    //       clearly don't want that here...

    file.close();

    return file.fileName();
}

//==============================================================================

void checkFileNameOrUrl(const QString &pInFileNameOrUrl, bool &pOutIsLocalFile,
                        QString &pOutFileNameOrUrl)
{
    // Determine whether pInFileNameOrUrl refers to a local or a remote file,
    // and set pOutIsLocalFile and pOutFileNameOrUrl accordingly
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
    //          However, to use fileNameOrUrl.toLocalFile() to retrieve that
    //          file won't work with a path that contains spaces, hence we
    //          return pInFileNameOrUrl after having removed "file:///" or
    //          "file://" from it on Windows and Linux/macOS, respectively...

    static const QString File = "file";

    QUrl fileNameOrUrl = pInFileNameOrUrl;

    pOutIsLocalFile =    (fileNameOrUrl.scheme() == File)
                      || fileNameOrUrl.host().isEmpty();
    pOutFileNameOrUrl = pOutIsLocalFile?
#ifdef Q_OS_WIN
                            canonicalFileName(QString(pInFileNameOrUrl).remove("file:///")):
#else
                            canonicalFileName(QString(pInFileNameOrUrl).remove("file://")):
#endif
                            fileNameOrUrl.url();
}

//==============================================================================

bool readFile(const QString &pFileNameOrUrl, QByteArray &pFileContents,
              QString *pErrorMessage)
{
    // Determine whether we are dealing with a local or a remote file

    bool isLocalFile;
    QString fileNameOrUrl;

    checkFileNameOrUrl(pFileNameOrUrl, isLocalFile, fileNameOrUrl);

    // Read the contents of the file, which file name or URL is given

    pFileContents = QByteArray();

    if (isLocalFile) {
        QFile file(fileNameOrUrl);

        if (file.open(QIODevice::ReadOnly)) {
            pFileContents = file.readAll();

            file.close();

            return true;
        }

        return false;
    }

    static SynchronousFileDownloader synchronousFileDownloader;

    return synchronousFileDownloader.download(fileNameOrUrl, pFileContents, pErrorMessage);
}

//==============================================================================

bool readFile(const QString &pFileNameOrUrl, QString &pFileContents,
              QString *pErrorMessage)
{
    // Read the contents of the file, which file name or URL is given

    QByteArray fileContents;
    bool res = readFile(pFileNameOrUrl, fileContents, pErrorMessage);

    pFileContents = fileContents;

    return res;
}

//==============================================================================

bool writeFile(const QString &pFileName, const QByteArray &pFileContents)
{
    // Write the given file contents to a temporary file and rename it to the
    // given file name, if successful

    QFile file(temporaryFileName());

    if (file.open(QIODevice::WriteOnly)) {
        bool res = file.write(pFileContents) != -1;

        file.close();

        // Rename the temporary file name to the given file name, if everything
        // went fine and that the path exists or that it can be created, if
        // needed

        QDir dir(QFileInfo(pFileName).path());

        res = res && (dir.exists() || dir.mkpath(dir.dirName()));

        if (res) {
            if (QFile::exists(pFileName)) {
                QFile::remove(pFileName);
            }

            res = file.rename(pFileName);
        }

        // Remove the temporary file, if either we couldn't rename it or the
        // initial saving didn't work

        if (!res) {
            file.remove();
        }

        return res;
    }

    return false;
}

//==============================================================================

bool writeFile(const QString &pFileName, const QString &pFileContents)
{
    // Write the given file contents to the given file name

    return writeFile(pFileName, pFileContents.toUtf8());
}

//==============================================================================

bool writeResourceToFile(const QString &pFileName, const QString &pResource)
{
    // Write the given resource to the given file, if possible

    if (QResource(pResource).isValid()) {
        return writeFile(pFileName, resource(pResource));
    }

    return false;
}

//==============================================================================

bool isTextFile(const QString &pFileName)
{
    // Return whether the given file is a text file

    QByteArray fileContents;

    readFile(pFileName, fileContents);

    return fileContents == QString(fileContents).toUtf8();
}

//==============================================================================

QString eolString()
{
    // Return the end of line to use

#ifdef Q_OS_WIN
    return "\r\n";
#else
    // Note: before macOS, the EOL string would have been "\r", but since macOS
    //       it is the same as on Linux (i.e. "\n") and since we don't support
    //       versions prior to macOS...

    return "\n";
#endif
}

//==============================================================================

QString nonDiacriticString(const QString &pString)
{
    // Remove and return a non-accentuated version of the given string
    // Note: this code is based on the one found at
    //       http://stackoverflow.com/questions/14009522/how-to-remove-accents-diacritic-marks-from-a-string-in-qt

    static QString diacriticLetters = QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");
    static QStringList nonDiacriticLetters = { "S", "OE", "Z", "s", "oe", "z",
                                               "Y", "Y", "u", "A", "A", "A",
                                               "A", "A", "A", "AE", "C", "E",
                                               "E", "E", "E", "I", "I", "I",
                                               "I", "D", "N", "O", "O", "O",
                                               "O", "O", "O", "U", "U", "U",
                                               "U", "Y", "s", "a", "a", "a",
                                               "a", "a", "a", "ae", "c", "e",
                                               "e", "e", "e", "i", "i", "i",
                                               "i", "o", "n", "o", "o", "o",
                                               "o", "o", "o", "u", "u", "u",
                                               "u", "y", "y" };

    QString res;

    for (auto letter : pString) {
        int index = diacriticLetters.indexOf(letter);

        res.append((index < 0)?letter:nonDiacriticLetters[index]);
    }

    return res;
}

//==============================================================================

QString plainString(const QString &pString)
{
    // Return the given string after stripping out all its HTML code (should it
    // have some)
    // Note: we enclose the given string within an HTML tag so that its
    //       stripping out can proceed without any problem...

    QXmlStreamReader string("<html>"+pString+"</html>");
    QString res;

    while (!string.atEnd()) {
        if (string.readNext() == QXmlStreamReader::Characters) {
            res += string.text();
        }
    }

    return res;
}

//==============================================================================

QString urlArguments(const QUrl &pUrl)
{
    // Return the arguments (path) of the given URL
    // Note: we use "|" to separate arguments, but they get converted to "%7C"
    //       and there doesn't seem to be a way to convert them back, so we do
    //       it ourselves...

    return pUrl.path().remove(0, 1).replace("%7C", "|");
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
// End of file
//==============================================================================
