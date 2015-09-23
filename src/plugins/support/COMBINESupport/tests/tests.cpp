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

#include "combinearchive.h"
#include "tests.h"

//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::basicTests()
{
    // Create (and delete afterwards) a simple COMBINE archive that contains
    // various files

    QString fileName = OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/tests.omex");

    OpenCOR::COMBINESupport::CombineArchive combineArchive(fileName);

    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 3; ++j)
            combineArchive.addFile(QDir::currentPath()+QDir::separator()+OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/dir0%1/file0%2.txt").arg(QString::number(i), QString::number(j)),
                                   QString("dir0%1/file0%2.txt").arg(QString::number(i), QString::number(j)),
                                   OpenCOR::COMBINESupport::CombineArchiveFile::Cellml);
    }

    combineArchive.save();

    QVERIFY(QFile::exists(fileName));

    QFile::remove(fileName);
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
