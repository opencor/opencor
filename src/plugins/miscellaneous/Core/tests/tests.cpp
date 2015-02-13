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

static const auto String = QStringLiteral("1\n11\n121\n1331\n14641");
static const auto Eol    = QStringLiteral("\n");

//==============================================================================

void Tests::stringPositionAsLineColumnTests()
{
    // Test the stringPositionAsLineColumn() method

    int line;
    int column;

    OpenCOR::Core::stringPositionAsLineColumn(String, Eol, -1, line, column);

    QCOMPARE(line, -1);
    QCOMPARE(column, -1);

    OpenCOR::Core::stringPositionAsLineColumn(String, Eol, String.length(), line, column);

    QCOMPARE(line, -1);
    QCOMPARE(column, -1);

    OpenCOR::Core::stringPositionAsLineColumn(String, Eol, 0, line, column);

    QCOMPARE(line, 1);
    QCOMPARE(column, 1);

    OpenCOR::Core::stringPositionAsLineColumn(String, Eol, String.length()-1, line, column);

    QCOMPARE(line, 5);
    QCOMPARE(column, 5);

    OpenCOR::Core::stringPositionAsLineColumn(String, Eol, 7, line, column);

    QCOMPARE(line, 3);
    QCOMPARE(column, 3);
}

//==============================================================================

void Tests::stringLineColumnAsPositionTests()
{
    // Test the stringLineColumnAsPosition() method

    int position;

    OpenCOR::Core::stringLineColumnAsPosition(String, Eol, 0, 0, position);

    QCOMPARE(position, -1);

    OpenCOR::Core::stringLineColumnAsPosition(String, Eol, 1, 0, position);

    QCOMPARE(position, -1);

    OpenCOR::Core::stringLineColumnAsPosition(String, Eol, 0, 1, position);

    QCOMPARE(position, -1);

    OpenCOR::Core::stringLineColumnAsPosition(String, Eol, 13, 1, position);

    QCOMPARE(position, -1);

    OpenCOR::Core::stringLineColumnAsPosition(String, Eol, 1, 13, position);

    QCOMPARE(position, -1);

    OpenCOR::Core::stringLineColumnAsPosition(String, Eol, 1, 1, position);

    QCOMPARE(position, 0);

    OpenCOR::Core::stringLineColumnAsPosition(String, Eol, 5, 5, position);

    QCOMPARE(position, String.length()-1);

    OpenCOR::Core::stringLineColumnAsPosition(String, Eol, 3, 3, position);

    QCOMPARE(position, 7);
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
