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

    QString help = OpenCOR::fileContents("../src/plugins/misc/CellMLTools/tests/data/help.out");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools"),
             help);

    // Ask for the plugin's help

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::help"),
             help);

    // Try an unknown command, resulting in the help being shown

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::unknown"),
             help);
}

//==============================================================================

void Tests::cliCellmlExportTests()
{
    // Export a CellML 1.1 file to CellML 1.0

    QString inFileName = "../src/plugins/misc/CellMLTools/tests/data/experiments/periodic-stimulus.xml";
    QString outFileName = "actual.out";

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << outFileName << "cellml_1_0"),
             QString());
    QCOMPARE(OpenCOR::fileContents(outFileName),
             OpenCOR::fileContents("../src/plugins/misc/CellMLTools/tests/data/cellml_1_0_export.out"));

    // Try to export a non-existing CellML file to CellML 1.0

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << "non_existing_input_file" << outFileName << "cellml_1_0"),
             OpenCOR::fileContents("../src/plugins/misc/CellMLTools/tests/data/input_file_not_found.out"));

    // Try to export to a user format, which file description doesn't exist

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << outFileName << "non_existing_format_file"),
             OpenCOR::fileContents("../src/plugins/misc/CellMLTools/tests/data/format_file_not_found.out"));
}

//==============================================================================

QTEST_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
