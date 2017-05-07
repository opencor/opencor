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
    // another file

    OpenCOR::COMBINESupport::CombineArchive otherCombineArchive(otherFileName);

    QString yetAnotherFileName = OpenCOR::Core::temporaryFileName();

    QVERIFY(otherCombineArchive.load());
    QVERIFY(otherCombineArchive.isValid(mIssues));
    QVERIFY(otherCombineArchive.save(yetAnotherFileName));

    // Unzip our two COMBINE archives and make sure that their contents is the
    // same
    // Note: the original plan was to check that the SHA-1 value of the two
    //       files was the same, but the fact is that the ZIP file format
    //       contains various date and time information, so the SHA-1 value of
    //       two files may be different...

    QTemporaryDir temporaryDir;
    QString otherDir = temporaryDir.path()+"/otherDir";
    QString yetAnotherDir = temporaryDir.path()+"/yetAnotherDir";
    OpenCOR::ZIPSupport::QZipReader otherZipReader(otherFileName);
    OpenCOR::ZIPSupport::QZipReader yetAnotherZipReader(yetAnotherFileName);

    QVERIFY(otherZipReader.extractAll(otherDir));
    QVERIFY(yetAnotherZipReader.extractAll(yetAnotherDir));

    QDirIterator dirIterator(otherDir, QStringList() << "*",
                             QDir::Files, QDirIterator::Subdirectories);

    while (dirIterator.hasNext()) {
        QString otherFileName = dirIterator.next();
        QString yetAnotherFileName = otherFileName.replace(otherDir, yetAnotherDir);
        QString otherFileContents;
        QString yetAnotherFileContents;

        QVERIFY(OpenCOR::Core::readFileContentsFromFile(otherFileName, otherFileContents));
        QVERIFY(OpenCOR::Core::readFileContentsFromFile(yetAnotherFileName, yetAnotherFileContents));

        QCOMPARE(OpenCOR::Core::sha1(otherFileContents),
                 OpenCOR::Core::sha1(yetAnotherFileContents));
    }

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
    QVERIFY(!combineArchive.isValid(mIssues));
    QVERIFY(mIssues.count() == 1);
    QCOMPARE(mIssues.first().message(), QString("the archive does not have a manifest"));

    // Try to load a COMBINE archive which manifest is not a valid OMEX file

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/notvalidomex.omex"));

    QVERIFY(combineArchive.reload());
    QVERIFY(!combineArchive.isValid(mIssues));
    QVERIFY(mIssues.count() == 1);
    QCOMPARE(mIssues.first().message(), QString("the manifest is not a valid OMEX file"));

    // Try to load a COMBINE archive that contains a non-existent file

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/nonexistentfile.omex"));

    QVERIFY(combineArchive.reload());
    QVERIFY(!combineArchive.isValid(mIssues));
    QVERIFY(mIssues.count() == 1);
    QCOMPARE(mIssues.first().message(), QString("<strong>nonexistentfile.txt</strong> could not be found"));

    // Try to load a COMBINE archive which manifest doesn't reference the
    // COMBINE archive itself

    combineArchive.setFileName(OpenCOR::fileName("src/plugins/support/COMBINESupport/tests/data/nocombinearchivereference.omex"));

    QVERIFY(combineArchive.reload());
    QVERIFY(!combineArchive.isValid(mIssues));
    QVERIFY(mIssues.count() == 1);
    QCOMPARE(mIssues.first().message(), QString("no reference to the COMBINE archive itself could be found"));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
