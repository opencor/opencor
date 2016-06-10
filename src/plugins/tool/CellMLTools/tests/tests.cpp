/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// CellML tools tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "corecliutils.h"
#include "tests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void Tests::helpTests()
{
    // Ask for the plugin's help

    QStringList help = OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/help.out"));

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::help"),
             help);

    // Try an unknown command, resulting in the help being shown

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::unknown"),
             help);
}

//==============================================================================

void Tests::exportTests()
{
    // Try to export a CellML 1.0 file to CellML 1.0

    QString fileName = OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/noble_model_1962.cellml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << fileName << "cellml_1_0"),
             QStringList() << "The file is already a CellML 1.0 file." << QString());

    // Export a CellML 1.1 file to CellML 1.0

    fileName = OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/experiments/periodic-stimulus.xml");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << fileName << "cellml_1_0"),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/cellml_1_0_export.out")) << QString());

    // Try to export a non-existing CellML file to CellML 1.0

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << "non_existing_file" << "cellml_1_0"),
             QStringList() << "The file could not be found." << QString());

    // Try to export to a user-defined format, which file description doesn't
    // exist

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << fileName << "non_existing_user_defined_format_file"),
             QStringList() << "The user-defined format file could not be found." << QString());

    // Try to export a local file to a user-defined format, which file
    // description exists

    QString userDefinedFormatFileName = OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/user_defined_format.xml");

#ifdef Q_OS_WIN
    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << fileName << userDefinedFormatFileName),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/user_defined_format_export_on_windows.out")));
#else
    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTools::export" << fileName << userDefinedFormatFileName),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/tool/CellMLTools/tests/data/user_defined_format_export_on_non_windows.out")));
#endif
}

//==============================================================================

QTEST_GUILESS_MAIN(Tests)

//==============================================================================
// End of file
//==============================================================================
