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

QString fileName(const QString &pFileName)
{
    // Format and return the given file name, so that it can be used on all our
    // supported platforms

#if defined(Q_OS_WIN)
    return "..\\..\\..\\"+QString(pFileName).replace("/", "\\");
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    return pFileName;
#else
    #error Unsupported platform
#endif
}

//==============================================================================

QString cliFileName(const QString &pFileName)
{
    // Format and return the given file name, so that it can be used on all our
    // supported platforms through the CLI version of OpenCOR

#if defined(Q_OS_WIN)
    return "..\\..\\"+QString(pFileName).replace("/", "\\");
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    return pFileName;
#else
    #error Unsupported platform
#endif
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

QStringList runCli(const QStringList pArguments)
{
    // Go to the directory where our tests are located
    // Note: see main()...

#ifdef Q_OS_WIN
    QString currentPath = QDir::currentPath();

    QDir::setCurrent(currentPath+"/../../bin");
#endif

    // Execute the CLI version of OpenCOR (passing to it the given arguments)
    // and return the output it has generated, if any

#if defined(Q_OS_WIN)
    QString program = "OpenCOR.com";
#elif defined(Q_OS_LINUX)
    QString program = "build/bin/OpenCOR";
#elif defined(Q_OS_MAC)
    QString program = "build/OpenCOR.app/Contents/MacOS/OpenCOR";
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
    QDir::setCurrent(currentPath);
#endif

    return output.remove("\r").split("\n");
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
