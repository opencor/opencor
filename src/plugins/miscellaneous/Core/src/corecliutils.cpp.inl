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

//==============================================================================

void SynchronousTextFileDownloader::networkAccessManagerSslErrors(QNetworkReply *pNetworkReply,
                                                                  const QList<QSslError> &pSslErrors)
{
    // Ignore the SSL errors since we assume the user knows what s/he is doing

    pNetworkReply->ignoreSslErrors(pSslErrors);
}

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
        // We couldn't find uname or something went wrong, so simply return
        // "Linux" as the OS name

        return "Linux";
    else
        return os+" "+exec("uname", QStringList() << "-r");
#elif defined(Q_OS_MAC)
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
//---GRY--- THE BELOW IS BECAUSE Qt DOESN'T CURRENTLY SUPPORT OS X 10.10
//          (Yosemite), SO ONCE IT DOES, WE SHOULD BE ABLE TO HAVE A CASE FOR
//          QSysInfo::MV_10_10 OR SOMETHING SIMILAR...
        if (QSysInfo::MacintoshVersion == 12)
            return "OS X 10.10 (Yosemite)";
        else
            return "Mac OS";
            // Note: we return "Mac OS" rather than "Mac OS X" or even "OS X"
            //       since only versions prior to (Mac) OS X are not
            //       recognised...
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

    // Return the error message after making sure that it ends with "...", if
    // requested

    int subsize = errorMessage.size();

    while (subsize && (errorMessage[subsize-1] == '.'))
        --subsize;

    return errorMessage.left(subsize)+(pDotDotDot?DotDotDot:QString());
}

//==============================================================================

bool readTextFromUrl(const QString &pUrl, QString &pText,
                     QString *pErrorMessage)
{
    // Read the contents of the file, which URL is given, as a string

    static SynchronousTextFileDownloader synchronousTextFileDownloader;

    return synchronousTextFileDownloader.readTextFromUrl(pUrl, pText, pErrorMessage);
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
// End of file
//==============================================================================
