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

#include "tests.h"

//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include <QZipReader>
#include <QZipWriter>

//==============================================================================

static const auto DirName = QStringLiteral("src/plugins/support/ZIPSupport/tests/");

//==============================================================================

static const auto ZipDirName  = QStringLiteral("tests");
static const auto ZipFileName = QStringLiteral("tests.zip");

//==============================================================================

static const auto CppFileName = QStringLiteral("tests.cpp");
static const auto HFileName   = QStringLiteral("tests.h");

//==============================================================================

void Tests::initTestCase()
{
    // Keep track of our original directory and go to our tests directory

    mOrigPath = QDir::currentPath();

    QDir::setCurrent(OpenCOR::dirName("src/plugins/support/ZIPSupport/tests/"));

    // Create the directory where we are going to uncompress our ZIP file

    QDir().mkdir(ZipDirName);
}

//==============================================================================

void Tests::cleanupTestCase()
{
    // Delete the files and directory we created

    QFile::remove(ZipFileName);

    QFile::remove(ZipDirName+QDir::separator()+CppFileName);
    QFile::remove(ZipDirName+QDir::separator()+HFileName);

    QDir().rmdir(ZipDirName);

    // Go back to our original directory

    QDir::setCurrent(mOrigPath);
}

//==============================================================================

void Tests::compressTests()
{
    // Compress ourselves and our header file

    QZipWriter zipWriter(ZipFileName);

    zipWriter.addFile(CppFileName, OpenCOR::rawFileContents(CppFileName));
    zipWriter.addFile(HFileName, OpenCOR::rawFileContents(HFileName));

    zipWriter.close();
}

//==============================================================================

void Tests::uncompressTests()
{
    // Uncompress our ZIP file

    QZipReader zipReader(ZipFileName);

    zipReader.extractAll(ZipDirName);

    zipReader.close();

    // Make sure that its contents is what we expect

    QCOMPARE(OpenCOR::fileSha1(ZipDirName+QDir::separator()+CppFileName),
             OpenCOR::fileSha1(CppFileName));
    QCOMPARE(OpenCOR::fileSha1(ZipDirName+QDir::separator()+HFileName),
             OpenCOR::fileSha1(HFileName));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
