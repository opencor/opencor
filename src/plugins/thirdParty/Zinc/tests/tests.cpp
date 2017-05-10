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
