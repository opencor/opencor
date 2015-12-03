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

void Tests::initTestCase()
{
    // Create a simple COMBINE archive that contains various files

    mCombineArchive = new OpenCOR::COMBINESupport::CombineArchive(OpenCOR::Core::temporaryFileName());

    int counter = 0;

    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 3; ++j, ++counter) {
            mCombineArchive->addFile(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/dir0%1/file0%2.txt").arg(QString::number(i), QString::number(j)),
                                     QString("dir0%1/file0%2.txt").arg(QString::number(i), QString::number(j)),
                                     OpenCOR::COMBINESupport::CombineArchiveFile::Format(1+counter%5),
                                     !(counter%2));
        }
    }
}

//==============================================================================

void Tests::cleanupTestCase()
{
    // Clean up after ourselves

    QFile::remove(mCombineArchive->fileName());

    // Delete some internal objects

    delete mCombineArchive;
}

//==============================================================================

void Tests::doBasicTests(const QString &pFileName)
{
    // Save the given COMBINE archive to the given file

    QString fileName = pFileName.isEmpty()?mCombineArchive->fileName():pFileName;

    mCombineArchive->save(fileName);

    QVERIFY(QFile::exists(fileName));

    // Unzip our COMBINE archive and make sure that its manifest is correct and
    // that the various files are present

    OpenCOR::ZIPSupport::QZipReader zipReader(fileName);
    QTemporaryDir temporaryDir;

    QVERIFY(zipReader.extractAll(temporaryDir.path()));

    QCOMPARE(OpenCOR::fileContents(temporaryDir.path()+QDir::separator()+"manifest.xml"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/manifest.xml")));

    for (int i = 1; i <= 3; ++i) {
        for (int j = 1; j <= 3; ++j)
            QVERIFY(QFile::exists(temporaryDir.path()+QDir::separator()+QString("dir0%1/file0%2.txt").arg(QString::number(i), QString::number(j))));
    }
}

//==============================================================================

void Tests::basicTests()
{
    // Some basic tests to save our COMBINE archive either directly or to a
    // different file and checking that its contents is sound

    QString otherFileName = OpenCOR::Core::temporaryFileName();

    doBasicTests();
    doBasicTests(otherFileName);

    // Clean up after ourselves

    QFile::remove(otherFileName);

    // Check that we can load our COMBINE archive

    QVERIFY(mCombineArchive->load());
}

//==============================================================================

void Tests::loadingErrorTests()
{
    // Try to load a non-existent COMBINE archive

    OpenCOR::COMBINESupport::CombineArchive combineArchive(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/nonexistentarchive.omex"));

    QVERIFY(!combineArchive.load());
    QCOMPARE(combineArchive.issue(), QString("the archive does not exist"));

    // Try to load a non-extractable COMBINE archive

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/nomanifestarchive.omex"));

    QVERIFY(!combineArchive.reload());
    QCOMPARE(combineArchive.issue(), QString("the archive does not have a manifest"));

    // Try to load a COMBINE archive which manifest is not a valid OMEX file

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/notvalidomexarchive.omex"));

    QVERIFY(!combineArchive.reload());
    QCOMPARE(combineArchive.issue(), QString("the manifest is not a valid OMEX file"));

    // Try to load a COMBINE archive that contains a non-existent file

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/nonexistentfilearchive.omex"));

    QVERIFY(!combineArchive.reload());
    QCOMPARE(combineArchive.issue(), QString("<strong>nonexistentfile.txt</strong> could not be found"));

    // Try to load a COMBINE archive which manifest doesn't reference the
    // COMBINE archive itself

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/nocombinearchivereferencearchive.omex"));

    QVERIFY(!combineArchive.reload());
    QCOMPARE(combineArchive.issue(), QString("no reference to the COMBINE archive itself could be found"));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
