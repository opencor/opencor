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
// Some useful tests-related functions
//==============================================================================

#include "testsutils.h"

//==============================================================================

#include <QFile>
#include <QIODevice>
#include <QProcess>

//==============================================================================

namespace OpenCOR {

//==============================================================================

QString dirOrFileName(const QString &pDirOrFileName)
{
    // Format and return the given directory or file name, so that it can be
    // used on all our supported platforms

    static const QString SourceDir = OpenCOR::fileContents(":/source_directory").first();

    return QDir::toNativeSeparators(SourceDir+"/"+QString(pDirOrFileName));
}

//==============================================================================

QString dirName(const QString &pDirName)
{
    // Format and return the given file name, so that it can be used on all our
    // supported platforms

    return dirOrFileName(pDirName);
}

//==============================================================================

QString fileName(const QString &pFileName)
{
    // Format and return the given file name, so that it can be used on all our
    // supported platforms

    return dirOrFileName(pFileName);
}

//==============================================================================

QByteArray rawFileContents(const QString &pFileName)
{
    // Read and return the contents of the given file

    QFile file(pFileName);
    QByteArray contents = QByteArray();

    if (file.open(QIODevice::ReadOnly)) {
        contents = file.readAll();

        file.close();
    }

    return contents;
}

//==============================================================================

QStringList fileContents(const QString &pFileName)
{
    // Read and return the contents of the given file

    QFile file(pFileName);
    QString contents = QString();

    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        // Note: QIODevice::Text ensures that end-of-line terminators are
        //       converted to "\n", which is exactly what we want...

        contents = file.readAll();

        file.close();
    }

    return contents.split('\n');
}

//==============================================================================

QString fileSha1(const QString &pFileName)
{
    // Read and return the contents of the given file

    return QCryptographicHash::hash(rawFileContents(pFileName),
                                    QCryptographicHash::Sha1).toHex();
}

//==============================================================================

int runCli(const QStringList &pArguments, QStringList &pOutput)
{
    // Go to the directory where our tests are located
    // Note: see main()...

    static const QString BuildDir = OpenCOR::fileContents(":/build_directory").first();

#ifdef Q_OS_WIN
    QString origPath = QDir::currentPath();

    QDir::setCurrent(BuildDir+"/bin");
#endif

    // Execute the CLI version of OpenCOR (passing to it the given arguments)
    // and return the output it has generated, if any

#if defined(Q_OS_WIN)
    QString program = "OpenCOR.com";
#elif defined(Q_OS_LINUX)
    QString program = BuildDir+"/bin/OpenCOR";
#else
    QString program = BuildDir+"/OpenCOR.app/Contents/MacOS/OpenCOR";
#endif

    QProcess process;

    process.setProcessChannelMode(QProcess::MergedChannels);

    QStringList defaultArguments = QStringList();

    process.start(program, defaultArguments << pArguments);

    QString output = QString();

    while (process.waitForReadyRead()) {
        output += process.readAll();
    }

    // Go back to our original directory

#ifdef Q_OS_WIN
    QDir::setCurrent(origPath);
#endif

    pOutput = output.remove('\r').split('\n');

    return process.exitCode();
}

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
