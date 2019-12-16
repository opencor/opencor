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
// Python support Hodgkin-Huxley 1952 tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "hodgkinhuxley1952tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void HodgkinHuxley1952Tests::tests()
{
    // Some tests to make sure that the Hodgkin–Huxley 1952 model works fine

    QStringList output;

    QVERIFY(!OpenCOR::runCli(QStringList() << "-c" << "PythonShell" << OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/hodgkinhuxley1952tests.py"), output));
    QCOMPARE(output, OpenCOR::fileContents(OpenCOR::fileName(QString("src/plugins/support/PythonSupport/tests/data/%1/hodgkinhuxley1952tests.out").arg(OpenCOR::targetPlatformDir()))));
}

//==============================================================================

QTEST_GUILESS_MAIN(HodgkinHuxley1952Tests)

//==============================================================================
// End of file
//==============================================================================
