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

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::cliHelpTests()
{
    // Ask for the plugin's help

    QStringList help = OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/CellMLTools/tests/data/help.out"));

    QCOMPARELIST(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::help"),
                 help);

    // Try an unknown command, resulting in the help being shown

    QCOMPARELIST(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::unknown"),
                 help);
}

//==============================================================================

void Tests::cliCellmlExportTests()
{
    // Try to export a CellML 1.0 file to CellML 1.0

    QString inFileName = OpenCOR::cliFileName("src/plugins/miscellaneous/CellMLTools/tests/data/noble_model_1962.cellml");
    QString outFileName = "export.out";
    QString predefined_format = "cellml_1_0";

    QCOMPARELIST(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << OpenCOR::cliFileName(outFileName) << predefined_format),
                 OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/CellMLTools/tests/data/input_file_already_cellml_1_0.out")));

    // Export a CellML 1.1 file to CellML 1.0

    inFileName = OpenCOR::cliFileName("src/plugins/miscellaneous/CellMLTools/tests/data/experiments/periodic-stimulus.xml");

    QCOMPARELIST(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << OpenCOR::cliFileName(outFileName) << predefined_format),
                 QStringList() << QString());
    QCOMPARELIST(OpenCOR::fileContents(OpenCOR::fileName(outFileName)),
                 OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/CellMLTools/tests/data/cellml_1_0_export.out")));

    // Try to export a non-existing CellML file to CellML 1.0

    QCOMPARELIST(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << "non_existing_input_file" << OpenCOR::cliFileName(outFileName) << predefined_format),
                 OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/CellMLTools/tests/data/input_file_not_found.out")));

    // Try to export to a user-defined format, which file description doesn't
    // exist

    QCOMPARELIST(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << OpenCOR::cliFileName(outFileName) << "non_existing_user_defined_format_file"),
                 OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/CellMLTools/tests/data/user_defined_format_file_not_found.out")));

    // Try to export a local file to a user-defined format, which file
    // description exists

    QString userDefinedFormatFileName = OpenCOR::cliFileName("src/plugins/miscellaneous/CellMLTools/tests/data/user_defined_format.xml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << inFileName << OpenCOR::cliFileName(outFileName) << userDefinedFormatFileName),
             QStringList() << QString());
#ifdef Q_OS_WIN
    QCOMPARELIST(OpenCOR::fileContents(OpenCOR::fileName(outFileName)),
                 OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/CellMLTools/tests/data/user_defined_format_export_on_windows.out")));
#else
    QCOMPARELIST(OpenCOR::fileContents(OpenCOR::fileName(outFileName)),
                 OpenCOR::fileContents(OpenCOR::fileName("src/plugins/miscellaneous/CellMLTools/tests/data/user_defined_format_export_on_non_windows.out")));
#endif

    // Delete the output file we generated

    QFile::remove(OpenCOR::fileName(outFileName));
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
