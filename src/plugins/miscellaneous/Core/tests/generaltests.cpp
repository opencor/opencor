/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

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

void GeneralTests::formatMessage()
{
    // Test the formatMessage() method

    QCOMPARE(OpenCOR::Core::formatMessage(""), "");
    QCOMPARE(OpenCOR::Core::formatMessage("", true), "");
    QCOMPARE(OpenCOR::Core::formatMessage("", true, true), "...");
    QCOMPARE(OpenCOR::Core::formatMessage("", true, false), "");
    QCOMPARE(OpenCOR::Core::formatMessage("", false), "");
    QCOMPARE(OpenCOR::Core::formatMessage("", false, true), "...");
    QCOMPARE(OpenCOR::Core::formatMessage("", false, false), "");

    QCOMPARE(OpenCOR::Core::formatMessage(" "), "");
    QCOMPARE(OpenCOR::Core::formatMessage(" ", true), "");
    QCOMPARE(OpenCOR::Core::formatMessage(" ", true, true), "...");
    QCOMPARE(OpenCOR::Core::formatMessage(" ", true, false), "");
    QCOMPARE(OpenCOR::Core::formatMessage(" ", false), "");
    QCOMPARE(OpenCOR::Core::formatMessage(" ", false, true), "...");
    QCOMPARE(OpenCOR::Core::formatMessage(" ", false, false), "");

    QCOMPARE(OpenCOR::Core::formatMessage("a"), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("a", true), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("a", true, true), "a...");
    QCOMPARE(OpenCOR::Core::formatMessage("a", true, false), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("a", false), "A");
    QCOMPARE(OpenCOR::Core::formatMessage("a", false, true), "A...");
    QCOMPARE(OpenCOR::Core::formatMessage("a", false, false), "A");

    QCOMPARE(OpenCOR::Core::formatMessage("A"), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("A", true), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("A", true, true), "a...");
    QCOMPARE(OpenCOR::Core::formatMessage("A", true, false), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("A", false), "A");
    QCOMPARE(OpenCOR::Core::formatMessage("A", false, true), "A...");
    QCOMPARE(OpenCOR::Core::formatMessage("A", false, false), "A");

    QCOMPARE(OpenCOR::Core::formatMessage("a "), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("a ", true), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("a ", true, true), "a...");
    QCOMPARE(OpenCOR::Core::formatMessage("a ", true, false), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("a ", false), "A");
    QCOMPARE(OpenCOR::Core::formatMessage("a ", false, true), "A...");
    QCOMPARE(OpenCOR::Core::formatMessage("a ", false, false), "A");

    QCOMPARE(OpenCOR::Core::formatMessage("A "), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("A ", true), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("A ", true, true), "a...");
    QCOMPARE(OpenCOR::Core::formatMessage("A ", true, false), "a");
    QCOMPARE(OpenCOR::Core::formatMessage("A ", false), "A");
    QCOMPARE(OpenCOR::Core::formatMessage("A ", false, true), "A...");
    QCOMPARE(OpenCOR::Core::formatMessage("A ", false, false), "A");

    QCOMPARE(OpenCOR::Core::formatMessage(" a"), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" a", true), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" a", true, true), "a...");
    QCOMPARE(OpenCOR::Core::formatMessage(" a", true, false), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" a", false), "A");
    QCOMPARE(OpenCOR::Core::formatMessage(" a", false, true), "A...");
    QCOMPARE(OpenCOR::Core::formatMessage(" a", false, false), "A");

    QCOMPARE(OpenCOR::Core::formatMessage(" A"), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" A", true), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" A", true, true), "a...");
    QCOMPARE(OpenCOR::Core::formatMessage(" A", true, false), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" A", false), "A");
    QCOMPARE(OpenCOR::Core::formatMessage(" A", false, true), "A...");
    QCOMPARE(OpenCOR::Core::formatMessage(" A", false, false), "A");

    QCOMPARE(OpenCOR::Core::formatMessage(" a "), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" a ", true), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" a ", true, true), "a...");
    QCOMPARE(OpenCOR::Core::formatMessage(" a ", true, false), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" a ", false), "A");
    QCOMPARE(OpenCOR::Core::formatMessage(" a ", false, true), "A...");
    QCOMPARE(OpenCOR::Core::formatMessage(" a ", false, false), "A");

    QCOMPARE(OpenCOR::Core::formatMessage(" A "), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" A ", true), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" A ", true, true), "a...");
    QCOMPARE(OpenCOR::Core::formatMessage(" A ", true, false), "a");
    QCOMPARE(OpenCOR::Core::formatMessage(" A ", false), "A");
    QCOMPARE(OpenCOR::Core::formatMessage(" A ", false, true), "A...");
    QCOMPARE(OpenCOR::Core::formatMessage(" A ", false, false), "A");

    QCOMPARE(OpenCOR::Core::formatMessage("ab"), "ab");
    QCOMPARE(OpenCOR::Core::formatMessage("ab", true), "ab");
    QCOMPARE(OpenCOR::Core::formatMessage("ab", true, true), "ab...");
    QCOMPARE(OpenCOR::Core::formatMessage("ab", true, false), "ab");
    QCOMPARE(OpenCOR::Core::formatMessage("ab", false), "Ab");
    QCOMPARE(OpenCOR::Core::formatMessage("ab", false, true), "Ab...");
    QCOMPARE(OpenCOR::Core::formatMessage("ab", false, false), "Ab");

    QCOMPARE(OpenCOR::Core::formatMessage("Ab"), "ab");
    QCOMPARE(OpenCOR::Core::formatMessage("Ab", true), "ab");
    QCOMPARE(OpenCOR::Core::formatMessage("Ab", true, true), "ab...");
    QCOMPARE(OpenCOR::Core::formatMessage("Ab", true, false), "ab");
    QCOMPARE(OpenCOR::Core::formatMessage("Ab", false), "Ab");
    QCOMPARE(OpenCOR::Core::formatMessage("Ab", false, true), "Ab...");
    QCOMPARE(OpenCOR::Core::formatMessage("Ab", false, false), "Ab");

    QCOMPARE(OpenCOR::Core::formatMessage("aB"), "aB");
    QCOMPARE(OpenCOR::Core::formatMessage("aB", true), "aB");
    QCOMPARE(OpenCOR::Core::formatMessage("aB", true, true), "aB...");
    QCOMPARE(OpenCOR::Core::formatMessage("aB", true, false), "aB");
    QCOMPARE(OpenCOR::Core::formatMessage("aB", false), "aB");
    QCOMPARE(OpenCOR::Core::formatMessage("aB", false, true), "aB...");
    QCOMPARE(OpenCOR::Core::formatMessage("aB", false, false), "aB");

    QCOMPARE(OpenCOR::Core::formatMessage("AB"), "AB");
    QCOMPARE(OpenCOR::Core::formatMessage("AB", true), "AB");
    QCOMPARE(OpenCOR::Core::formatMessage("AB", true, true), "AB...");
    QCOMPARE(OpenCOR::Core::formatMessage("AB", true, false), "AB");
    QCOMPARE(OpenCOR::Core::formatMessage("AB", false), "AB");
    QCOMPARE(OpenCOR::Core::formatMessage("AB", false, true), "AB...");
    QCOMPARE(OpenCOR::Core::formatMessage("AB", false, false), "AB");

    QCOMPARE(OpenCOR::Core::formatMessage("a b"), "a b");
    QCOMPARE(OpenCOR::Core::formatMessage("a b", true), "a b");
    QCOMPARE(OpenCOR::Core::formatMessage("a b", true, true), "a b...");
    QCOMPARE(OpenCOR::Core::formatMessage("a b", true, false), "a b");
    QCOMPARE(OpenCOR::Core::formatMessage("a b", false), "A b");
    QCOMPARE(OpenCOR::Core::formatMessage("a b", false, true), "A b...");
    QCOMPARE(OpenCOR::Core::formatMessage("a b", false, false), "A b");

    QCOMPARE(OpenCOR::Core::formatMessage("A b"), "a b");
    QCOMPARE(OpenCOR::Core::formatMessage("A b", true), "a b");
    QCOMPARE(OpenCOR::Core::formatMessage("A b", true, true), "a b...");
    QCOMPARE(OpenCOR::Core::formatMessage("A b", true, false), "a b");
    QCOMPARE(OpenCOR::Core::formatMessage("A b", false), "A b");
    QCOMPARE(OpenCOR::Core::formatMessage("A b", false, true), "A b...");
    QCOMPARE(OpenCOR::Core::formatMessage("A b", false, false), "A b");

    QCOMPARE(OpenCOR::Core::formatMessage("a B"), "a B");
    QCOMPARE(OpenCOR::Core::formatMessage("a B", true), "a B");
    QCOMPARE(OpenCOR::Core::formatMessage("a B", true, true), "a B...");
    QCOMPARE(OpenCOR::Core::formatMessage("a B", true, false), "a B");
    QCOMPARE(OpenCOR::Core::formatMessage("a B", false), "A B");
    QCOMPARE(OpenCOR::Core::formatMessage("a B", false, true), "A B...");
    QCOMPARE(OpenCOR::Core::formatMessage("a B", false, false), "A B");

    QCOMPARE(OpenCOR::Core::formatMessage("A B"), "a B");
    QCOMPARE(OpenCOR::Core::formatMessage("A B", true), "a B");
    QCOMPARE(OpenCOR::Core::formatMessage("A B", true, true), "a B...");
    QCOMPARE(OpenCOR::Core::formatMessage("A B", true, false), "a B");
    QCOMPARE(OpenCOR::Core::formatMessage("A B", false), "A B");
    QCOMPARE(OpenCOR::Core::formatMessage("A B", false, true), "A B...");
    QCOMPARE(OpenCOR::Core::formatMessage("A B", false, false), "A B");

    QCOMPARE(OpenCOR::Core::formatMessage("CellML"), "CellML");
    QCOMPARE(OpenCOR::Core::formatMessage("CellML", true), "CellML");
    QCOMPARE(OpenCOR::Core::formatMessage("CellML", true, true), "CellML...");
    QCOMPARE(OpenCOR::Core::formatMessage("CellML", true, false), "CellML");
    QCOMPARE(OpenCOR::Core::formatMessage("CellML", false), "CellML");
    QCOMPARE(OpenCOR::Core::formatMessage("CellML", false, true), "CellML...");
    QCOMPARE(OpenCOR::Core::formatMessage("CellML", false, false), "CellML");
}

//==============================================================================

void GeneralTests::sizeAsStringTests()
{
    // Test the sizeAsString() method

    QCOMPARE(OpenCOR::Core::sizeAsString(quint64(256*qPow(1024, 0))), QString("256 B"));
    QCOMPARE(OpenCOR::Core::sizeAsString(quint64(256*qPow(1024, 1))), QString("256 KB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(quint64(256*qPow(1024, 2))), QString("256 MB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(quint64(256*qPow(1024, 3))), QString("256 GB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(quint64(256*qPow(1024, 4))), QString("256 TB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(quint64(256*qPow(1024, 5))), QString("256 PB"));

    QCOMPARE(OpenCOR::Core::sizeAsString(quint64(256.256*qPow(1024, 2))), QLocale().toString(256.3)+" MB");
    QCOMPARE(OpenCOR::Core::sizeAsString(quint64(256.256*qPow(1024, 2)), 3), QLocale().toString(256.256)+" MB");
}

//==============================================================================

void GeneralTests::formatTimeTests()
{
    // Test the formatTime() method

    QCOMPARE(OpenCOR::Core::formatTime(0), QString("0ms"));
    QCOMPARE(OpenCOR::Core::formatTime(999), QString("999ms"));
    QCOMPARE(OpenCOR::Core::formatTime(1000), QString("1s"));
    QCOMPARE(OpenCOR::Core::formatTime(59999), QString("59s 999ms"));
    QCOMPARE(OpenCOR::Core::formatTime(60000), QString("1m"));
    QCOMPARE(OpenCOR::Core::formatTime(60999), QString("1m 0s 999ms"));
    QCOMPARE(OpenCOR::Core::formatTime(61000), QString("1m 1s"));
    QCOMPARE(OpenCOR::Core::formatTime(3599999), QString("59m 59s 999ms"));
    QCOMPARE(OpenCOR::Core::formatTime(3600000), QString("1h"));
    QCOMPARE(OpenCOR::Core::formatTime(3600999), QString("1h 0m 0s 999ms"));
    QCOMPARE(OpenCOR::Core::formatTime(3601000), QString("1h 0m 1s"));
    QCOMPARE(OpenCOR::Core::formatTime(3660000), QString("1h 1m"));
    QCOMPARE(OpenCOR::Core::formatTime(86399999), QString("23h 59m 59s 999ms"));
    QCOMPARE(OpenCOR::Core::formatTime(86400000), QString("1d"));
    QCOMPARE(OpenCOR::Core::formatTime(86400999), QString("1d 0h 0m 0s 999ms"));
    QCOMPARE(OpenCOR::Core::formatTime(86401000), QString("1d 0h 0m 1s"));
    QCOMPARE(OpenCOR::Core::formatTime(86460000), QString("1d 0h 1m"));
    QCOMPARE(OpenCOR::Core::formatTime(90000000), QString("1d 1h"));
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
