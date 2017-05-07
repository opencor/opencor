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
// ZIP support tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "corecliutils.h"
#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include <QZipReader>
#include <QZipWriter>

//==============================================================================

static const auto CppFileName = QStringLiteral("tests.cpp");
static const auto HFileName   = QStringLiteral("tests.h");
static const auto TxtFileName = QStringLiteral("data/data.txt");

//==============================================================================

void Tests::initTestCase()
{
    // Keep track of our original directory and go to our tests directory

    mOrigPath = QDir::currentPath();

    QDir::setCurrent(OpenCOR::dirName("src/plugins/support/ZIPSupport/tests/"));

    // Get a temporary file name for our ZIP file

    mFileName = OpenCOR::Core::temporaryFileName();
}

//==============================================================================

void Tests::cleanupTestCase()
{
    // Delete our ZIP file

    QFile::remove(mFileName);

    // Go back to our original directory

    QDir::setCurrent(mOrigPath);
}

//==============================================================================

void Tests::compressTests()
{
    // Compress ourselves and our header file

    OpenCOR::ZIPSupport::QZipWriter zipWriter(mFileName);

    zipWriter.addFile(CppFileName, OpenCOR::rawFileContents(CppFileName));
    zipWriter.addFile(HFileName, OpenCOR::rawFileContents(HFileName));
    zipWriter.addFile(TxtFileName, OpenCOR::rawFileContents(TxtFileName));
}

//==============================================================================

void Tests::uncompressTests()
{
    // Uncompress our ZIP file

    OpenCOR::ZIPSupport::QZipReader zipReader(mFileName);
    QTemporaryDir temporaryDir;

    QVERIFY(zipReader.extractAll(temporaryDir.path()));

    // Make sure that its contents is what we expect

    QCOMPARE(OpenCOR::fileContents(temporaryDir.path()+QDir::separator()+CppFileName),
             OpenCOR::fileContents(CppFileName));
    QCOMPARE(OpenCOR::fileContents(temporaryDir.path()+QDir::separator()+HFileName),
             OpenCOR::fileContents(HFileName));
    QCOMPARE(OpenCOR::fileContents(temporaryDir.path()+QDir::separator()+TxtFileName),
             OpenCOR::fileContents(TxtFileName));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
