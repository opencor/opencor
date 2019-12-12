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
// Python support basic tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "basictests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void BasicTests::tests()
{
    // Some basic tests to make sure that we can use our Python wrappers

    QStringList output;

    QVERIFY(!OpenCOR::runCli(QStringList() << "-c" << "PythonShell" << OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/basictests.py"), output));
    QCOMPARE(output, OpenCOR::fileContents(OpenCOR::fileName(QString("src/plugins/support/PythonSupport/tests/data/%1/basictests.out").arg(OpenCOR::targetPlatformDir()))));
}

//==============================================================================

QTEST_GUILESS_MAIN(BasicTests)

//==============================================================================
// End of file
//==============================================================================
