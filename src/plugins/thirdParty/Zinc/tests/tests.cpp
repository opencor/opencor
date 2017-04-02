/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Zinc API tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include "opencmiss/zinc/context.hpp"
#include "opencmiss/zinc/result.hpp"

//==============================================================================

void Tests::basicTests()
{
    // Some very basic tests to make sure that we have access to the Zinc API

    // Create a context and check a few things

    static const cmzn_context_id Id = cmzn_context_create("test");

    OpenCMISS::Zinc::Context context(Id);

    QCOMPARE(context.isValid(), true);
    QCOMPARE(context.getId(), Id);

    int version[3];

    QCOMPARE(context.getVersion(&version[0]), int(OpenCMISS::Zinc::Result::RESULT_OK));
    QCOMPARE(version[0], 3);
    QCOMPARE(version[1], 1);
    QCOMPARE(version[2], 1);

    QVERIFY(QString(context.getVersionString()).startsWith("3.1.1"));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
