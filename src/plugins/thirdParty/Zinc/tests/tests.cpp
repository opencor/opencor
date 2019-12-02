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
// Zinc tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

#include <array>

//==============================================================================

#include "zincbegin.h"
    #include "opencmiss/zinc/context.hpp"
    #include "opencmiss/zinc/result.hpp"
#include "zincend.h"

//==============================================================================

void Tests::basicTests()
{
    // Some very basic tests to make sure that we have access to Zinc

    // Create a context and check a few things

    static cmzn_context * const Id = cmzn_context_create("test");

    OpenCMISS::Zinc::Context context(Id);

    QCOMPARE(context.isValid(), true);
    QCOMPARE(context.getId(), Id);

    std::array<int, 3> version = {};

    QCOMPARE(context.getVersion(&version[0]), int(OpenCMISS::Zinc::Result::RESULT_OK));
    QCOMPARE(version[0], 3);
    QCOMPARE(version[1], 1);
    QCOMPARE(version[2], 2);

    QVERIFY(QString(context.getVersionString()).startsWith("3.1.2"));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
