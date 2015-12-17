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

    static const QString sourceDir = OpenCOR::fileContents(":source_directory").first();

    return QDir::toNativeSeparators(sourceDir+QDir::separator()+QString(pDirOrFileName));
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

    return contents.split("\n");
}

//==============================================================================

QString fileSha1(const QString &pFileName)
{
    // Read and return the contents of the given file

    return QCryptographicHash::hash(rawFileContents(pFileName),
                                    QCryptographicHash::Sha1).toHex();
}

//==============================================================================

QStringList runCli(const QStringList pArguments)
{
    // Go to the directory where our tests are located
    // Note: see main()...

    static const QString buildDir = OpenCOR::fileContents(":build_directory").first();

#ifdef Q_OS_WIN
    QString crtPath = QDir::currentPath();

    QDir::setCurrent(buildDir+"/bin");
#endif

    // Execute the CLI version of OpenCOR (passing to it the given arguments)
    // and return the output it has generated, if any

#if defined(Q_OS_WIN)
    QString program = "OpenCOR.com";
#elif defined(Q_OS_LINUX)
    QString program = buildDir+"/bin/OpenCOR";
#elif defined(Q_OS_MAC)
    QString program = buildDir+"/OpenCOR.app/Contents/MacOS/OpenCOR";
#else
    #error Unsupported platform
#endif

    QProcess process;

    process.setProcessChannelMode(QProcess::MergedChannels);

    QStringList defaultArguments = QStringList();

    process.start(program, defaultArguments << pArguments);

    QString output = QString();

    while (process.waitForReadyRead())
        output += process.readAll();

    // Go back to our original directory

#ifdef Q_OS_WIN
    QDir::setCurrent(crtPath);
#endif

    return output.remove("\r").split("\n");
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
