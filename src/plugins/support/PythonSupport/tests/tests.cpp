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
// Python support tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::basicTests()
{
    // Some basic tests to make sure that we can use our Python wrappers

    QStringList output;

    QVERIFY(!OpenCOR::runCli(QStringList() << "-c" << "PythonShell" << OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/basictests.py"), output));
    QCOMPARE(output, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/basictests.out")));
}

//==============================================================================

void Tests::coverageTests()
{
    // Some coverage tests to make sure that we test all of our Python wrappers
    // and that they can be safely used

    QStringList output;

    QVERIFY(!OpenCOR::runCli(QStringList() << "-c" << "PythonShell" << OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/coveragetests.py"), output));
    QCOMPARE(output, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/coveragetests.out")));
}

//==============================================================================

void Tests::hodgkinHuxley1952Tests()
{
    // Some tests to make sure that the Hodgkin–Huxley 1952 model works fine

    QStringList output;

    QVERIFY(!OpenCOR::runCli(QStringList() << "-c" << "PythonShell" << OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/hodgkinhuxley1952tests.py"), output));
    QCOMPARE(output, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/hodgkinhuxley1952tests.out")));
}

//==============================================================================

void Tests::noble1962Tests()
{
    // Some tests to make sure that the Noble 1962 model works fine

    QStringList output;

    QVERIFY(!OpenCOR::runCli(QStringList() << "-c" << "PythonShell" << OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/noble1962tests.py"), output));
    QCOMPARE(output, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/noble1962tests.out")));
}

//==============================================================================

void Tests::vanDerPol1928Tests()
{
    // Some tests to make sure that the van der Pol 1928 model works fine

    QStringList output;

    QVERIFY(!OpenCOR::runCli(QStringList() << "-c" << "PythonShell" << OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/vanderpol1928tests.py"), output));
    QCOMPARE(output, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/vanderpol1928tests.out")));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
