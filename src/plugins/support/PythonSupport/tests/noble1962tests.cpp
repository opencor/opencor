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
// Python support Noble 1962 tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "noble1962tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Noble1962Tests::tests()
{
    // Some tests to make sure that the Noble 1962 model works fine

    QStringList output;

    QVERIFY(!OpenCOR::runCli({ "-c", "PythonShell", OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/noble1962tests.py") }, output));
    QCOMPARE(output, OpenCOR::fileContents(OpenCOR::fileName(QString("src/plugins/support/PythonSupport/tests/data/%1/noble1962tests.out").arg(OpenCOR::targetPlatformDir()))));
}

//==============================================================================

QTEST_GUILESS_MAIN(Noble1962Tests)

//==============================================================================
// End of file
//==============================================================================
