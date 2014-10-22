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

QString fileContents(const QString &pFileName)
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

    return contents;
}

//==============================================================================

QString runCli(const QStringList pArguments)
{
    // Execute the CLI version of OpenCOR (passing to it the given arguments)
    // and return the output it has generated, if any

#if defined(Q_OS_WIN)
    QString program = "bin/OpenCOR.com";
#elif defined(Q_OS_LINUX)
    QString program = "bin/OpenCOR";
#elif defined(Q_OS_MAC)
    QString program = "OpenCOR.app/Contents/MacOS/OpenCOR";
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

    return output.replace("\r", "");
}

//==============================================================================

}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
