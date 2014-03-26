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

#include "cliutils.h"
#include "tests.h"

//==============================================================================

#include "../../../../tests/testsutils.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::initTestCase()
{
    // Load the Core plugin

    OpenCOR::loadPlugin("Core");
}

//==============================================================================

void Tests::sizeAsStringTests()
{
    // Test the sizeAsString() method

    QCOMPARE(OpenCOR::Core::sizeAsString(256*pow(1024, 0)), QString("256 B"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*pow(1024, 1)), QString("256 KB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*pow(1024, 2)), QString("256 MB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*pow(1024, 3)), QString("256 GB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*pow(1024, 4)), QString("256 TB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*pow(1024, 5)), QString("256 PB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*pow(1024, 6)), QString("256 EB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*pow(1024, 7)), QString("256 ZB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256*pow(1024, 8)), QString("256 YB"));

    QCOMPARE(OpenCOR::Core::sizeAsString(256.256*pow(1024, 2)), QString("256.3 MB"));
    QCOMPARE(OpenCOR::Core::sizeAsString(256.256*pow(1024, 2), 3), QString("256.256 MB"));
}

//==============================================================================

QTEST_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
