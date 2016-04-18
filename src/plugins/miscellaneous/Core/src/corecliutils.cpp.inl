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
// Core CLI utilities
//==============================================================================

QString locale()
{
    // Retrieve and return the locale

    QString res = rawLocale();

    if (res.isEmpty())
        return QLocale::system().name().left(2);
    else
        return res;
}

//==============================================================================

static const auto RawSettingsLocale = QStringLiteral("RawLocale");

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
    QString res = QString();
    QString appVersion = qApp->applicationVersion();

    if (!appVersion.contains("-"))
        res += "Version ";
    else
        res += "Snapshot ";

    res += appVersion;

    return res;
}

//==============================================================================

QString version()
{
    return qAppName()+" "+shortVersion();
}

//==============================================================================

QString nativeCanonicalDirName(const QString &pDirName)
{
    // Return a native and canonical version of the given directory name or a
    // native version, if the native and canonical version is empty (i.e. the
    // directory doesn't exist (anymore?))

    QString res = QDir::toNativeSeparators(QDir(pDirName).canonicalPath());

    return res.isEmpty()?QDir::toNativeSeparators(pDirName):res;
}

//==============================================================================

QString nativeCanonicalFileName(const QString &pFileName)
{
    // Return a native and canonical version of the given file name or a native
    // version, if the native and canonical version is empty (i.e. the file
    // doesn't exist (anymore?))

    QString res = QDir::toNativeSeparators(QFileInfo(pFileName).canonicalFilePath());

    return res.isEmpty()?QDir::toNativeSeparators(pFileName):res;
}

//==============================================================================

QStringList nativeCanonicalFileNames(const QStringList &pFileNames)
{
    // Return a native and canonical version of the given file names or a native
    // version, if the native and canonical version of a given file name is
    // empty (i.e. the file doesn't exist (anymore?))

    QStringList res = QStringList();

    foreach (const QString &fileName, pFileNames)
        res << nativeCanonicalFileName(fileName);

    return res;
}

//==============================================================================

bool SynchronousFileDownloader::download(const QString &pUrl,
                                         QByteArray &pContents,
                                         QString *pErrorMessage) const
{
    // Try to read a remote file as text, but only if we are connected to the
    // Internet

    if (internetConnectionAvailable()) {
        // Create a network access manager so that we can retrieve the contents
        // of the remote file

        QNetworkAccessManager networkAccessManager;

        // Make sure that we get told if there are SSL errors (which would happen
        // if a website's certificate is invalid, e.g. it has expired)

        connect(&networkAccessManager, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
                this, SLOT(networkAccessManagerSslErrors(QNetworkReply *, const QList<QSslError> &)));

        // Download the contents of the remote file

        QNetworkReply *networkReply = networkAccessManager.get(QNetworkRequest(pUrl));
        QEventLoop eventLoop;

        connect(networkReply, SIGNAL(finished()),
                &eventLoop, SLOT(quit()));

        eventLoop.exec();

        // Check whether we were able to retrieve the contents of the file

        bool res = networkReply->error() == QNetworkReply::NoError;

        if (res) {
            // Before accepting the contents as is, make sure that we are not
            // dealing with a redirection

            QUrl redirectedUrl = networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

            if (!redirectedUrl.isEmpty())
                return download(redirectedUrl.toString(), pContents, pErrorMessage);

            pContents = networkReply->readAll();

            if (pErrorMessage)
                *pErrorMessage = QString();
        } else {
            pContents = QByteArray();

            if (pErrorMessage)
                *pErrorMessage = networkReply->errorString();
        }

        // Delete (later) the network reply

        networkReply->deleteLater();

        return res;
    } else {
        if (pErrorMessage)
            *pErrorMessage = QObject::tr("No Internet connection available.");

        return false;
    }
}

//==============================================================================

void SynchronousFileDownloader::networkAccessManagerSslErrors(QNetworkReply *pNetworkReply,
                                                              const QList<QSslError> &pSslErrors)
{
    // Ignore the SSL errors since we assume the user knows what s/he is doing

    pNetworkReply->ignoreSslErrors(pSslErrors);
}

//==============================================================================

QString exec(const QString &pProgram, const QStringList &pArgs)
{
    // Execute and return the output of a program given its arguments

    QProcess process;

    process.start(pProgram, pArgs);
    process.waitForFinished();

    return process.readAll().trimmed();
}

//==============================================================================

bool internetConnectionAvailable()
{
    // Check whether an Internet connection is available, this by going through
    // all of our network interfaces and checking whether one of them is both
    // active and is a loopback, and has at least one IP address

    QList<QNetworkInterface> networkInterfaces = QNetworkInterface::allInterfaces();

    for (int i = 0, iMax = networkInterfaces.count(); i < iMax; ++i) {
        QNetworkInterface networkInterface = networkInterfaces[i];

        if (    networkInterface.flags().testFlag(QNetworkInterface::IsUp)
            && !networkInterface.flags().testFlag(QNetworkInterface::IsLoopBack)
            &&  networkInterface.addressEntries().count()) {
            return true;
        }
    }

    return false;
}

//==============================================================================

QString noInternetConnectionAvailableMessage()
{
    // Return a typical message about no Internet connection being available

    return QObject::tr("No Internet connection available.");
}

//==============================================================================

QString copyright()
{
    return QObject::tr("Copyright")+" 2011-"+QString::number(QDate::currentDate().year());
}

//==============================================================================

QString formatMessage(const QString &pMessage, const bool &pLowerCase,
                      const bool &pDotDotDot)
{
    // Trim the message and make sure that we don't end up with an empty string

    static const QString DotDotDot = "...";

    QString message = pMessage.trimmed();

    if (message.isEmpty())
        return pDotDotDot?DotDotDot:QString();

    // Upper/lower the case of the first character, unless the message is one
    // character long (!!) or unless its second character is in lower case

    if (    (message.size() <= 1)
        || ((message.size() > 1) && message[1].isLower())) {
        if (pLowerCase)
            message[0] = message[0].toLower();
        else
            message[0] = message[0].toUpper();
    }

    // Return the message after making sure that it ends with "...", if
    // requested

    int subsize = message.size();

    while (subsize && (message[subsize-1] == '.'))
        --subsize;

    return message.left(subsize)+(pDotDotDot?DotDotDot:QString());
}

//==============================================================================

QByteArray resource(const QString &pResource)
{
    // Retrieve a resource as a QByteArray

    QResource resource(pResource);

    if (resource.isValid()) {
        if (resource.isCompressed()) {
            // The resource is compressed, so uncompress it before returning it

            return qUncompress(resource.data(), resource.size());
        } else {
            // The resource is not compressed, so just return it after doing the
            // right conversion

            return QByteArray(reinterpret_cast<const char *>(resource.data()),
                              resource.size());
        }
    } else {
        return QByteArray();
    }
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

    QTemporaryFile file(QDir::tempPath()+QDir::separator()+"XXXXXX"+pExtension);

    file.open();

    file.setAutoRemove(false);
    // Note: by default, a temporary file is to autoremove itself, but we
    //       clearly don't want that here...

    file.close();

    return file.fileName();
}

//==============================================================================

bool readFileContentsFromFile(const QString &pFileName,
                              QByteArray &pFileContents)
{
    // Read the contents of the file, which file name is given

    QFile file(pFileName);

    if (file.open(QIODevice::ReadOnly)) {
        pFileContents = file.readAll();

        file.close();

        return true;
    } else {
        pFileContents = QByteArray();

        return false;
    }
}

//==============================================================================

bool readFileContentsFromUrl(const QString &pUrl, QByteArray &pFileContents,
                             QString *pErrorMessage)
{
    // Read the contents of the file, which URL is given

    static SynchronousFileDownloader synchronousFileDownloader;

    return synchronousFileDownloader.download(pUrl, pFileContents, pErrorMessage);
}

//==============================================================================

bool writeFileContentsToFile(const QString &pFileName,
                             const QByteArray &pFileContents)
{
    // Write the given file contents to a temporary file and rename it to the
    // given file name, if successful

    QFile file(temporaryFileName());

    if (file.open(QIODevice::WriteOnly)) {
        bool res = file.write(pFileContents) != -1;

        file.close();

        // Rename the temporary file name to the given file name, if everything
        // went fine

        if (res) {
            if (QFile::exists(pFileName))
                QFile::remove(pFileName);

            res = file.rename(pFileName);
        }

        // Remove the temporary file, if either we couldn't rename it or the
        // initial saving didn't work

        if (!res)
            file.remove();

        return res;
    } else {
        return false;
    }
}

//==============================================================================

bool writeResourceToFile(const QString &pFileName, const QString &pResource)
{
    // Write the given resource to the given file, if possible

    if (QResource(pResource).isValid())
        return writeFileContentsToFile(pFileName, resource(pResource));
    else
        return false;
}

//==============================================================================

bool isTextFile(const QString &pFileName)
{
    // Return whether the given file is a text file

    QByteArray fileContents;

    readFileContentsFromFile(pFileName, fileContents);

    return fileContents == QString(fileContents).toUtf8();
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

QString nonDiacriticString(const QString &pString)
{
    // Remove and return a non-accentuated version of the given string
    // Note: this code is based on the one found at
    //       http://stackoverflow.com/questions/14009522/how-to-remove-accents-diacritic-marks-from-a-string-in-qt

    static QString diacriticLetters = QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");
    static QStringList nonDiacriticLetters = QStringList() << "S" << "OE" << "Z" << "s" << "oe" << "z" << "Y" << "Y" << "u" << "A" << "A" << "A" << "A" << "A" << "A" << "AE" << "C" << "E" << "E" << "E" << "E" << "I" << "I" << "I" << "I" << "D" << "N" << "O" << "O" << "O" << "O" << "O" << "O" << "U" << "U" << "U" << "U" << "Y" << "s" << "a" << "a" << "a" << "a" << "a" << "a" << "ae" << "c" << "e" << "e" << "e" << "e" << "i" << "i" << "i" << "i" << "o" << "n" << "o" << "o" << "o" << "o" << "o" << "o" << "u" << "u" << "u" << "u" << "y" << "y";

    QString res = QString();

    for (int i = 0, iMax = pString.length(); i < iMax; ++i) {
        QChar letter = pString[i];
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
    QString res = QString();

    while (!string.atEnd()) {
        if (string.readNext() == QXmlStreamReader::Characters)
            res += string.text();
    }

    return res;
}

//==============================================================================

QString urlArguments(const QUrl &pUrl)
{
    // Return the arguments (path) of the given URL
    // Note #1: we would normally retrieve the path using pUrl.path(), but this
    //          doesn't work when there are spaces, so instead we adjust the URL
    //          by removing its scheme and authority, and return it as a
    //          string...
    // Note #2: we use "|" to separate arguments, but they get converted to
    //          "%7C" and there doesn't seem to be a way to convert them back,
    //          so we do it ourselves...

    return pUrl.adjusted(QUrl::RemoveScheme|QUrl::RemoveAuthority).toString().remove(0, 1).replace("%7C", "|");
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
