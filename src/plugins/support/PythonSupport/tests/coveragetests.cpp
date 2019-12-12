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
// Python support coverage tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "coveragetests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void CoverageTests::tests()
{
    // Some coverage tests to make sure that we test all of our Python wrappers
    // and that they can be safely used

    QStringList output;

    QVERIFY(!OpenCOR::runCli(QStringList() << "-c" << "PythonShell" << OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/coveragetests.py"), output));
    QCOMPARE(output, OpenCOR::fileContents(OpenCOR::fileName(QString("src/plugins/support/PythonSupport/tests/data/%1/coveragetests.out").arg(OpenCOR::targetPlatformDir()))));
}

//==============================================================================

QTEST_GUILESS_MAIN(CoverageTests)

//==============================================================================
// End of file
//==============================================================================
