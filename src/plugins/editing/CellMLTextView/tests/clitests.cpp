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
// CellML Text view CLI tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "corecliutils.h"
#include "clitests.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void CliTests::helpTests()
{
    // Ask for the plugin's help

    QStringList help = OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/cli/help.out"));

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTextView::help"),
             help);

    // Try an unknown command, resulting in the help being shown

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTextView::unknown"),
             help);
}

//==============================================================================

void CliTests::importTests()
{
    // Try to import a non-existing file

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTextView::import" << "non_existing_file"),
             QStringList() << "The file could not be found." << QString());

    // Try to import a non CellML file

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTextView::import" << OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/cli/noble_model_1962.out")),
             QStringList() << "The file could not be imported:" << " [1:1] Error occurred while parsing element." << QString());

    // Import the Noble 1962 model to the CellML Text format

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTextView::import" << OpenCOR::fileName("models/noble_model_1962.cellml")),
             OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/cli/noble_model_1962.out")));
}

//==============================================================================

void CliTests::exportTests()
{
    // Try to export a non-existing file

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTextView::export" << "non_existing_file"),
             QStringList() << "The file could not be found." << QString());

    // Try to export a non CellML Text file

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTextView::export" << OpenCOR::fileName("models/noble_model_1962.cellml")),
             QStringList() << "The file could not be exported:" << " [1:1] 'def' is expected, but '<' was found instead." << QString());

    // Export the CellML Text version of the Noble 1962 model to CellML

    QString expectedOutput = OpenCOR::fileContents(OpenCOR::fileName("models/noble_model_1962.cellml")).join("\n");

    expectedOutput.replace("cellml/1.0#", "cellml/1.1#");

    QCOMPARE(OpenCOR::runCli(QStringList() << "-c" << "CellMLTextView::export" << OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/cli/noble_model_1962.out")),
             expectedOutput.split("\n"));
}

//==============================================================================

QTEST_GUILESS_MAIN(CliTests)

//==============================================================================
// End of file
//==============================================================================
