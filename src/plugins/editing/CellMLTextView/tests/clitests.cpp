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
// CellML Text view CLI tests
//==============================================================================

#include "../../../../tests/src/testsutils.h"

//==============================================================================

#include "clitests.h"
#include "corecliutils.h"

//==============================================================================

#include <QtTest/QtTest>

//==============================================================================

void CliTests::helpTests()
{
    // Ask for the plugin's help

    QStringList help = OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/cli/help.out"));

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTextView" }, mOutput));
    QCOMPARE(mOutput, help);
    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTextView::help" }, mOutput));
    QCOMPARE(mOutput, help);

    // Try a known command, but with the wrong number of arguments

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTextView::export", "argument", "argument" }, mOutput));
    QCOMPARE(mOutput, help);
    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTextView::import", "argument", "argument" }, mOutput));
    QCOMPARE(mOutput, help);

    // Try an unknown command, resulting in the help being shown

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTextView::unknown" }, mOutput));
    QCOMPARE(mOutput, help);
}

//==============================================================================

void CliTests::importTests()
{
    // Try to import a non-existing file

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTextView::import", "non_existing_file" }, mOutput));
    QCOMPARE(mOutput, QStringList() << "The file could not be found." << QString());

    // Try to import a non-CellML 1.0/1.1 file

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTextView::import", OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/cli/sine_approximation_model.cellml") }, mOutput));
    QCOMPARE(mOutput, QStringList() << "Only CellML 1.0/1.1 files can be imported." << QString());

    // Try to import an invalid CellML file

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTextView::import", OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/cli/invalid_model.cellml") }, mOutput));
    QCOMPARE(mOutput, QStringList() << "The file could not be imported:" << " [13:26] A 'diff' element must have two siblings." << QString());

    // Import the Noble 1962 model to the CellML Text format

    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTextView::import", OpenCOR::fileName("models/noble_model_1962.cellml") }, mOutput));
    QCOMPARE(mOutput, OpenCOR::fileContents(OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/cli/noble_model_1962.out")));
}

//==============================================================================

void CliTests::exportTests()
{
    // Try to export a non-existing file

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTextView::export", "non_existing_file" }, mOutput));
    QCOMPARE(mOutput, QStringList() << "The file could not be found." << QString());

    // Try to export a non-CellML Text file

    QVERIFY(OpenCOR::runCli({ "-c", "CellMLTextView::export", OpenCOR::fileName("models/noble_model_1962.cellml") }, mOutput));
    QCOMPARE(mOutput, QStringList() << "The file could not be exported:" << " [1:1] 'def' is expected, but '<' was found instead." << QString());

    // Export the CellML Text version of the Noble 1962 model to CellML

    QString expectedOutput = OpenCOR::fileContents(OpenCOR::fileName("models/noble_model_1962.cellml")).join('\n');

    expectedOutput.replace("cellml/1.0#", "cellml/1.1#");

    QVERIFY(!OpenCOR::runCli({ "-c", "CellMLTextView::export", OpenCOR::fileName("src/plugins/editing/CellMLTextView/tests/data/cli/noble_model_1962.out") }, mOutput));
    QCOMPARE(mOutput, expectedOutput.split('\n'));
}

//==============================================================================

QTEST_GUILESS_MAIN(CliTests)

//==============================================================================
// End of file
//==============================================================================
