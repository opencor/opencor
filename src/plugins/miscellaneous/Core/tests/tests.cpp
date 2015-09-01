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
// Core tests
//==============================================================================

#include "corecliutils.h"
#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::initTestCase()
{
    // Initialise some constants

    mString = "1\n11\n121\n1331\n14641";
    mEol = "\n";
}

//==============================================================================

void Tests::qSameStringListsTests()
{
    // Compare two identical string lists

    QVERIFY(qSameStringLists(QStringList() << "QWE" << "ASD" << "XZC",
                             QStringList() << "QWE" << "ASD" << "XZC"));

    // Compare two string lists of different sizes

    QVERIFY(!qSameStringLists(QStringList() << "QWE" << "ASD" << "XZC",
                              QStringList() << "QWE"));

    // Compare two string lists of the same size, but different contents

    QVERIFY(!qSameStringLists(QStringList() << "QWE" << "ASD" << "XZC",
                              QStringList() << "ZXC" << "ASD" << "QWE"));

    // Compare two string lists of the same size and partially identical
    // contents

    QVERIFY(!qSameStringLists(QStringList() << "QWE" << "ASD" << "XZC",
                              QStringList() << "QWE" << "asd" << "XZC"));
}

//==============================================================================

void Tests::sizeAsStringTests()
{
    // Test the sizeAsString() method

    QCOMPARE(OpenCOR::Core::sizeAsString(256*qPow(1024, 0)), QString("256 B"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*qPow(1024, 1)), QString("256 KB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*qPow(1024, 2)), QString("256 MB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*qPow(1024, 3)), QString("256 GB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*qPow(1024, 4)), QString("256 TB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*qPow(1024, 5)), QString("256 PB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*qPow(1024, 6)), QString("256 EB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*qPow(1024, 7)), QString("256 ZB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*qPow(1024, 8)), QString("256 YB"));

    QCOMPARE(OpenCOR::Core::sizeAsString(256.256*qPow(1024, 2)), QString("256.3 MB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256.256*qPow(1024, 2), 3), QString("256.256 MB"));
}

//==============================================================================

void Tests::sha1Tests()
{
    // Test the sha1() method

    QCOMPARE(OpenCOR::Core::sha1("This is just for testing..."),
             QString("5f1f59774939bacbd8da99fa2c68ff1b78d4cd01"));
    QCOMPARE(OpenCOR::Core::sha1("5f1f59774939bacbd8da99fa2c68ff1b78d4cd01"),
             QString("3b673e33930f46151cbc58c04226eb3d66571cc1"));
    QCOMPARE(OpenCOR::Core::sha1("3b673e33930f46151cbc58c04226eb3d66571cc1"),
             QString("0ca7a40ecd1bae83526e70b40b5614986b7a0b61"));
}

//==============================================================================

void Tests::stringPositionAsLineColumnTests()
{
    // Test the stringPositionAsLineColumn() method

    int line;
    int column;

    OpenCOR::Core::stringPositionAsLineColumn(mString, mEol, -1, line, column);

    QCOMPARE(line, -1);
    QCOMPARE(column, -1);

    OpenCOR::Core::stringPositionAsLineColumn(mString, mEol, mString.length(), line, column);

    QCOMPARE(line, -1);
    QCOMPARE(column, -1);

    OpenCOR::Core::stringPositionAsLineColumn(mString, mEol, 0, line, column);

    QCOMPARE(line, 1);
    QCOMPARE(column, 1);

    OpenCOR::Core::stringPositionAsLineColumn(mString, mEol, mString.length()-1, line, column);

    QCOMPARE(line, 5);
    QCOMPARE(column, 5);

    OpenCOR::Core::stringPositionAsLineColumn(mString, mEol, 7, line, column);

    QCOMPARE(line, 3);
    QCOMPARE(column, 3);
}

//==============================================================================

void Tests::stringLineColumnAsPositionTests()
{
    // Test the stringLineColumnAsPosition() method

    int position;

    OpenCOR::Core::stringLineColumnAsPosition(mString, mEol, 0, 0, position);

    QCOMPARE(position, -1);

    OpenCOR::Core::stringLineColumnAsPosition(mString, mEol, 1, 0, position);

    QCOMPARE(position, -1);

    OpenCOR::Core::stringLineColumnAsPosition(mString, mEol, 0, 1, position);

    QCOMPARE(position, -1);

    OpenCOR::Core::stringLineColumnAsPosition(mString, mEol, 13, 1, position);

    QCOMPARE(position, -1);

    OpenCOR::Core::stringLineColumnAsPosition(mString, mEol, 1, 13, position);

    QCOMPARE(position, -1);

    OpenCOR::Core::stringLineColumnAsPosition(mString, mEol, 1, 1, position);

    QCOMPARE(position, 0);

    OpenCOR::Core::stringLineColumnAsPosition(mString, mEol, 5, 5, position);

    QCOMPARE(position, mString.length()-1);

    OpenCOR::Core::stringLineColumnAsPosition(mString, mEol, 3, 3, position);

    QCOMPARE(position, 7);
}

//==============================================================================

void Tests::newFileNameTests()
{
    // Test the newFileName() method

    QCOMPARE(OpenCOR::Core::newFileName("My file.txt"),
             QString("My file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My file.txt", QString(), "dat"),
             QString("My file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("My file.txt", "Extra"),
             QString("My file - Extra.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My file.txt", "Extra", "dat"),
             QString("My file - Extra.dat"));

    QCOMPARE(OpenCOR::Core::newFileName("my file.txt"),
             QString("my file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my file.txt", QString(), "dat"),
             QString("my file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("my file.txt", "Extra"),
             QString("my file - extra.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my file.txt", "Extra", "dat"),
             QString("my file - extra.dat"));

    QCOMPARE(OpenCOR::Core::newFileName("My_file.txt"),
             QString("My_file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My_file.txt", QString(), "dat"),
             QString("My_file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("My_file.txt", "Extra"),
             QString("My_file_Extra.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My_file.txt", "Extra", "dat"),
             QString("My_file_Extra.dat"));

    QCOMPARE(OpenCOR::Core::newFileName("my_file.txt"),
             QString("my_file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my_file.txt", QString(), "dat"),
             QString("my_file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("my_file.txt", "Extra"),
             QString("my_file_extra.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my_file.txt", "Extra", "dat"),
             QString("my_file_extra.dat"));

    QCOMPARE(OpenCOR::Core::newFileName("My-file.txt"),
             QString("My-file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My-file.txt", QString(), "dat"),
             QString("My-file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("My-file.txt", "Extra"),
             QString("My-file-Extra.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My-file.txt", "Extra", "dat"),
             QString("My-file-Extra.dat"));

    QCOMPARE(OpenCOR::Core::newFileName("my-file.txt"),
             QString("my-file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my-file.txt", QString(), "dat"),
             QString("my-file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("my-file.txt", "Extra"),
             QString("my-file-extra.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my-file.txt", "Extra", "dat"),
             QString("my-file-extra.dat"));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
