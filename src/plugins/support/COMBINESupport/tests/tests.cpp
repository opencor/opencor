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
// COMBINE support tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "combinearchive.h"
#include "corecliutils.h"
#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include <QZipReader>

//==============================================================================

void Tests::basicTests()
{
    // Create a simple COMBINE archive that contains various files

    QString fileName = OpenCOR::Core::temporaryFileName();
    OpenCOR::COMBINESupport::CombineArchive combineArchive(fileName);
    int counter = 0;

    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 3; ++j, ++counter) {
            combineArchive.addFile(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/dir0%1/file0%2.txt").arg(QString::number(i), QString::number(j)),
                                   QString("dir0%1/file0%2.txt").arg(QString::number(i), QString::number(j)),
                                   OpenCOR::COMBINESupport::CombineArchiveFile::Format(1+counter%4),
                                   !(counter%2));
        }
    }

    combineArchive.save();

    QVERIFY(QFile::exists(fileName));

    // Unzip our COMBINE archive

    OpenCOR::ZIPSupport::QZipReader zipReader(fileName);
    QTemporaryDir temporaryDir;

    QVERIFY(zipReader.extractAll(temporaryDir.path()));

    zipReader.close();

    // Make sure that our COMBINE archive's manifest is correct

    QCOMPARE(OpenCOR::fileContents(temporaryDir.path()+QDir::separator()+"manifest.xml"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/manifest.xml")));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
