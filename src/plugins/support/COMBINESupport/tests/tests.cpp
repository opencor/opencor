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

#include <QXmlSchema>
#include <QZipReader>

//==============================================================================

void dummyMessageHandler(QtMsgType pType, const QMessageLogContext &pContext,
                         const QString &pMessage)
{
    Q_UNUSED(pType);
    Q_UNUSED(pContext);
    Q_UNUSED(pMessage);
}

//==============================================================================

void Tests::initTestCase()
{
    // Quick trick to prevent warnings from Qt
    // Note: indeed, to call CombineArchive::load() calls Core::validXml(),
    //       which creates a QXmlSchema object. The first time a QXmlSchema
    //       object is created, Qt generates some warnings about the current
    //       thread not being the object's thread and therefore not being able
    //       to move to the target thread. We clearly have nothing to do with
    //       those warnings and, if anything, they just pollute our test output,
    //       so...

QtMessageHandler oldMessageHandler = qInstallMessageHandler(dummyMessageHandler);
    QXmlSchema schema;
qInstallMessageHandler(oldMessageHandler);

    Q_UNUSED(schema);

    // Create a simple COMBINE archive that contains various files

    mCombineArchive = new OpenCOR::COMBINESupport::CombineArchive(OpenCOR::Core::temporaryFileName(), true);

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

    // Check that we can load our other COMBINE archive and save it in yet
    // another file, and make sure that the SHA-1 of the two files is the same

    OpenCOR::COMBINESupport::CombineArchive otherCombineArchive(otherFileName);

    QString yetAnotherFileName = OpenCOR::Core::temporaryFileName();

    QVERIFY(otherCombineArchive.load());
QtMessageHandler oldMessageHandler = qInstallMessageHandler(dummyMessageHandler);
    QVERIFY(otherCombineArchive.isValid());
qInstallMessageHandler(oldMessageHandler);
    QVERIFY(otherCombineArchive.save(yetAnotherFileName));

    QByteArray otherFileContents;
    QByteArray yetAnotherFileContents;

    QVERIFY(OpenCOR::Core::readFileContentsFromFile(otherFileName, otherFileContents));
    QVERIFY(OpenCOR::Core::readFileContentsFromFile(yetAnotherFileName, yetAnotherFileContents));

    QCOMPARE(OpenCOR::Core::sha1(otherFileContents),
             OpenCOR::Core::sha1(yetAnotherFileContents));

    // Clean up after ourselves

    QFile::remove(otherFileName);
    QFile::remove(yetAnotherFileName);
}

//==============================================================================

void Tests::loadingErrorTests()
{
    // Try to load a non-existent COMBINE archive

    OpenCOR::COMBINESupport::CombineArchive combineArchive(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/nonexistent.omex"));

    QVERIFY(!combineArchive.load());
    QVERIFY(combineArchive.issues().count() == 1);
    QCOMPARE(combineArchive.issues().first().message(), QString("the archive does not exist"));

    // Try to load a non-signed COMBINE archive

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/notsigned.omex"));

    QVERIFY(!combineArchive.reload());
    QVERIFY(combineArchive.issues().count() == 1);
    QCOMPARE(combineArchive.issues().first().message(), QString("the archive is not signed"));

    // Try to load a badly signed COMBINE archive

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/badlysigned.omex"));

    QVERIFY(!combineArchive.reload());
    QVERIFY(combineArchive.issues().count() == 1);
    QCOMPARE(combineArchive.issues().first().message(), QString("the archive does not have the correct signature"));

    // Try to load a COMBINE archive with no manifest

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/nomanifest.omex"));

    QVERIFY(combineArchive.reload());
    QVERIFY(!combineArchive.isValid());
    QVERIFY(combineArchive.issues().count() == 1);
    QCOMPARE(combineArchive.issues().first().message(), QString("the archive does not have a manifest"));

    // Try to load a COMBINE archive which manifest is not a valid OMEX file

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/notvalidomex.omex"));

    QVERIFY(combineArchive.reload());
QtMessageHandler oldMessageHandler = qInstallMessageHandler(dummyMessageHandler);
    QVERIFY(!combineArchive.isValid());
qInstallMessageHandler(oldMessageHandler);
    QVERIFY(combineArchive.issues().count() == 1);
    QCOMPARE(combineArchive.issues().first().message(), QString("the manifest is not a valid OMEX file"));

    // Try to load a COMBINE archive that contains a non-existent file

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/nonexistentfile.omex"));

    QVERIFY(combineArchive.reload());
qInstallMessageHandler(dummyMessageHandler);
    QVERIFY(!combineArchive.isValid());
qInstallMessageHandler(oldMessageHandler);
    QVERIFY(combineArchive.issues().count() == 1);
    QCOMPARE(combineArchive.issues().first().message(), QString("<strong>nonexistentfile.txt</strong> could not be found"));

    // Try to load a COMBINE archive which manifest doesn't reference the
    // COMBINE archive itself

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/nocombinearchivereference.omex"));

    QVERIFY(combineArchive.reload());
qInstallMessageHandler(dummyMessageHandler);
    QVERIFY(!combineArchive.isValid());
qInstallMessageHandler(oldMessageHandler);
    QVERIFY(combineArchive.issues().count() == 1);
    QCOMPARE(combineArchive.issues().first().message(), QString("no reference to the COMBINE archive itself could be found"));
}

//==============================================================================

QTEST_APPLESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
