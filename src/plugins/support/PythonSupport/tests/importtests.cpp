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
// Python support import tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "importtests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void ImportTests::tests()
{
    // Some basic tests to make sure that we can import the Python modules that
    // we are supposed to ship

    QStringList output;

    QVERIFY(!OpenCOR::runCli({ "-c", "PythonShell", OpenCOR::fileName("src/plugins/support/PythonSupport/tests/data/importtests.py") }, output));
#if defined(Q_OS_WIN) && defined(QT_DEBUG)
    QCOMPARE(output, OpenCOR::fileContents(OpenCOR::fileName(QString("src/plugins/support/PythonSupport/tests/data/%1/importtests.out").arg(OpenCOR::targetPlatformDir()))));
#else
    QCOMPARE(output, QStringList() << QString());
#endif
}

//==============================================================================

QTEST_GUILESS_MAIN(ImportTests)

//==============================================================================
// End of file
//==============================================================================
