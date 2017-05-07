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
// Core general tests
//==============================================================================

#include "corecliutils.h"
#include "generaltests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void GeneralTests::initTestCase()
{
    // Initialise some constants

    mString = "1\n11\n121\n1331\n14641";
    mEol = "\n";
}

//==============================================================================

void GeneralTests::sizeAsStringTests()
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

    QCOMPARE(OpenCOR::Core::sizeAsString(256.256*qPow(1024, 2)), QLocale().toString(256.3)+" MB");
    QCOMPARE(OpenCOR::Core::sizeAsString(256.256*qPow(1024, 2), 3), QLocale().toString(256.256)+" MB");
}

//==============================================================================

void GeneralTests::sha1Tests()
{
    // Test the sha1() method

    QCOMPARE(OpenCOR::Core::sha1(QString("This is just for testing...")),
             QString("5f1f59774939bacbd8da99fa2c68ff1b78d4cd01"));
    QCOMPARE(OpenCOR::Core::sha1(QString("5f1f59774939bacbd8da99fa2c68ff1b78d4cd01")),
             QString("3b673e33930f46151cbc58c04226eb3d66571cc1"));
    QCOMPARE(OpenCOR::Core::sha1(QString("3b673e33930f46151cbc58c04226eb3d66571cc1")),
             QString("0ca7a40ecd1bae83526e70b40b5614986b7a0b61"));
}

//==============================================================================

void GeneralTests::stringPositionAsLineColumnTests()
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

void GeneralTests::stringLineColumnAsPositionTests()
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

void GeneralTests::newFileNameTests()
{
    // Test the newFileName() method

    QCOMPARE(OpenCOR::Core::newFileName("My file.txt", "dat"),
             QString("My file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("My file.txt", "Before", true),
             QString("Before - My file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My file.txt", "Before", true, "dat"),
             QString("Before - My file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("My file.txt", "After", false),
             QString("My file - After.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My file.txt", "After", false, "dat"),
             QString("My file - After.dat"));

    QCOMPARE(OpenCOR::Core::newFileName("my file.txt", "dat"),
             QString("my file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("my file.txt", "Before", true),
             QString("before - my file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my file.txt", "Before", true, "dat"),
             QString("before - my file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("my file.txt", "After", false),
             QString("my file - after.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my file.txt", "After", false, "dat"),
             QString("my file - after.dat"));

    QCOMPARE(OpenCOR::Core::newFileName("My_file.txt", "dat"),
             QString("My_file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("My_file.txt", "Before", true),
             QString("Before_My_file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My_file.txt", "Before", true, "dat"),
             QString("Before_My_file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("My_file.txt", "After", false),
             QString("My_file_After.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My_file.txt", "After", false, "dat"),
             QString("My_file_After.dat"));

    QCOMPARE(OpenCOR::Core::newFileName("my_file.txt", "dat"),
             QString("my_file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("my_file.txt", "Before", true),
             QString("before_my_file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my_file.txt", "Before", true, "dat"),
             QString("before_my_file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("my_file.txt", "After", false),
             QString("my_file_after.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my_file.txt", "After", false, "dat"),
             QString("my_file_after.dat"));

    QCOMPARE(OpenCOR::Core::newFileName("My-file.txt", "dat"),
             QString("My-file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("My-file.txt", "Before", true),
             QString("Before-My-file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My-file.txt", "Before", true, "dat"),
             QString("Before-My-file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("My-file.txt", "After", false),
             QString("My-file-After.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("My-file.txt", "After", false, "dat"),
             QString("My-file-After.dat"));

    QCOMPARE(OpenCOR::Core::newFileName("my-file.txt", "dat"),
             QString("my-file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("my-file.txt", "Before", true),
             QString("before-my-file.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my-file.txt", "Before", true, "dat"),
             QString("before-my-file.dat"));
    QCOMPARE(OpenCOR::Core::newFileName("my-file.txt", "After", false),
             QString("my-file-after.txt"));
    QCOMPARE(OpenCOR::Core::newFileName("my-file.txt", "After", false, "dat"),
             QString("my-file-after.dat"));
}

//==============================================================================

QTEST_GUILESS_MAIN(GeneralTests)

//==============================================================================
// End of file
//==============================================================================
