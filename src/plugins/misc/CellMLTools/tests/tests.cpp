/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// CellMLTools tests
//==============================================================================

#include "tests.h"

//==============================================================================

#include "../../../../tests/testsutils.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::cliHelpTests()
{
    // Ask for the plugin's help

    QStringList a = OpenCOR::runCli(QStringList() << "-c" << "CellMLTools");
    QStringList b = OpenCOR::fileContents("../src/plugins/misc/CellMLTools/tests/data/help.out");

    QCOMPARE(a, b);

    // Ask for the plugin's help

    a = OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::help");

    QCOMPARE(a, b);

    // Try an unknown command, resulting in the help being shown

    a = OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::unknown");

    QCOMPARE(a, b);
}

//==============================================================================

void Tests::cliCellmlExportTests()
{
    // Export a CellML 1.1 file to CellML 1.0

    QString inFileName = QFileInfo("../src/plugins/misc/CellMLTools/tests/data/experiments/periodic-stimulus.xml").canonicalFilePath();
    QString outFileName = "actual.out";

    QStringList a = OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << outFileName << "cellml_1_0");
    QStringList b = QStringList() << QString();

    QCOMPARE(a, b);

    a = OpenCOR::fileContents(outFileName);
    b = OpenCOR::fileContents("../src/plugins/misc/CellMLTools/tests/data/cellml_1_0_export.out");

    QCOMPARE(a, b);

    // Try to export a non-existing CellML file to CellML 1.0

    a = OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << "non_existing_file" << outFileName << "cellml_1_0");
    b = OpenCOR::fileContents("../src/plugins/misc/CellMLTools/tests/data/file_not_found.out");

    QCOMPARE(a, b);

    // Try to export an unknown format

    a = OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << outFileName << "unknown_format");
    b = OpenCOR::fileContents("../src/plugins/misc/CellMLTools/tests/data/help.out");

    QCOMPARE(a, b);
}

//==============================================================================

QTEST_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
