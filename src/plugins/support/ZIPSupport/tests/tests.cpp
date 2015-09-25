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
// ZIP support tests
//==============================================================================

#include "corecliutils.h"
#include "tests.h"

//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include <QZipReader>
#include <QZipWriter>

//==============================================================================

static const auto CppFileName = QStringLiteral("tests.cpp");
static const auto HFileName   = QStringLiteral("tests.h");

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

    zipWriter.close();
}

//==============================================================================

void Tests::uncompressTests()
{
    // Uncompress our ZIP file

    OpenCOR::ZIPSupport::QZipReader zipReader(mFileName);
    QTemporaryDir dirName;

    QVERIFY(zipReader.extractAll(dirName.path()));

    zipReader.close();

    // Make sure that its contents is what we expect

    QCOMPARE(OpenCOR::fileContents(dirName.path()+QDir::separator()+CppFileName),
             OpenCOR::fileContents(CppFileName));
    QCOMPARE(OpenCOR::fileContents(dirName.path()+QDir::separator()+HFileName),
             OpenCOR::fileContents(HFileName));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
