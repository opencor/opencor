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
// Python support imports tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "importstests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void ImportsTests::tests()
{
    // Some basic tests to make sure that we can use our Python wrappers

    QStringList output;

    QVERIFY(!OpenCOR::runCli(QStringList() << "-c" << "PythonShell" << OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/importstests.py"), output));
#if defined(Q_OS_WIN) && defined(QT_DEBUG)
    QCOMPARE(output, OpenCOR::fileContents(OpenCOR::fileName(QString("src/plugins/support/PythonSupport/tests/data/%1/importstests.out").arg(OpenCOR::targetPlatformDir()))));
#else
    QCOMPARE(output, QStringList() << QString());
#endif
}

//==============================================================================

QTEST_GUILESS_MAIN(ImportsTests)

//==============================================================================
// End of file
//==============================================================================
